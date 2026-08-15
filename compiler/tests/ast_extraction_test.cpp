// Visitor-based AST extraction proof for AsnEtsiItsLexer/Parser (compiler/grammar/):
// confirms the parse tree AsnParserSmokeTest already proves error-free is actually a
// workable basis for pulling structured type/field/constraint data back out, per
// docs/design/compiler-frontend-design.md §6.
//
// Two checks:
//   1. Every real .asn fixture (same set AsnParserSmokeTest walks) is run through
//      AstBuilder end to end with no crash/exception — the broad "sound basis across
//      the real message set" claim.
//   2. Deep structural assertions against CamParameters (the design doc's named proof
//      case: optional fields, extensibility, cross-module-referenced containers) and
//      HighFrequencyContainer from the real CAM module, plus a constraint-extraction
//      check against two real CDD types (one SIZE, one plain value range).
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "antlr4-runtime.h"

#include "AsnEtsiItsLexer.h"
#include "AsnEtsiItsParser.h"
#include "ast_builder.h"
#include "v2x/asn/ast.hpp"

using namespace antlr4;
namespace fs = std::filesystem;
using v2x::asn::ConstraintKind;
using v2x::asn::Module;
using v2x::asn::TypeAssignment;
using v2x::asn::TypeKind;

namespace {

int failureCount = 0;

void check(bool condition, const std::string &what) {
    if (!condition) {
        std::cerr << "  FAIL: " << what << "\n";
        ++failureCount;
    }
}

class SilentErrorListener : public BaseErrorListener {
public:
    size_t count = 0;
    void syntaxError(Recognizer *, Token *, size_t, size_t, const std::string &, std::exception_ptr) override {
        ++count;
    }
};

// Parses a file and returns its AST modules; returns an empty vector (and reports a
// failure) if the file doesn't parse clean or AstBuilder throws.
std::vector<Module> parseToAst(const fs::path &path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        check(false, "could not open " + path.string());
        return {};
    }
    std::stringstream ss;
    ss << stream.rdbuf();
    std::string content = ss.str();

    ANTLRInputStream input(content);
    AsnEtsiItsLexer lexer(&input);
    SilentErrorListener lexerErrors;
    lexer.removeErrorListeners();
    lexer.addErrorListener(&lexerErrors);

    CommonTokenStream tokens(&lexer);
    tokens.fill();

    AsnEtsiItsParser parser(&tokens);
    SilentErrorListener parserErrors;
    parser.removeErrorListeners();
    parser.addErrorListener(&parserErrors);

    AsnEtsiItsParser::ModulesContext *tree = parser.modules();
    if (lexerErrors.count != 0 || parserErrors.count != 0) {
        check(false, path.string() + " did not parse clean (already covered by AsnParserSmokeTest)");
        return {};
    }

    try {
        AstBuilder builder;
        return builder.build(tree);
    } catch (const std::exception &e) {
        check(false, path.string() + ": AstBuilder threw: " + e.what());
        return {};
    }
}

const TypeAssignment *findTypeAssignment(const std::vector<Module> &modules, const std::string &name) {
    for (const auto &m : modules) {
        for (const auto &ta : m.typeAssignments) {
            if (ta.name == name) {
                return &ta;
            }
        }
    }
    return nullptr;
}

void checkCamParameters(const std::vector<Module> &camModules) {
    const TypeAssignment *cam = findTypeAssignment(camModules, "CamParameters");
    check(cam != nullptr, "CamParameters type assignment found in CAM-PDU-Descriptions.asn");
    if (cam == nullptr) {
        return;
    }
    const auto &type = cam->type;
    check(type.kind == TypeKind::Sequence, "CamParameters is a SEQUENCE");
    check(type.extensible, "CamParameters is extensible (has '...')");
    check(type.fields.size() == 5, "CamParameters has 5 fields (4 root + 1 extension addition), got " +
                                        std::to_string(type.fields.size()));
    if (type.fields.size() != 5) {
        return;
    }

    const auto &basic = type.fields[0];
    check(basic.name == "basicContainer", "field[0] is basicContainer");
    check(!basic.optional, "basicContainer is mandatory");
    check(!basic.isExtensionAddition, "basicContainer is a root field");
    check(basic.type.kind == TypeKind::Reference && basic.type.referencedName == "BasicContainer",
          "basicContainer references BasicContainer");

    const auto &highFreq = type.fields[1];
    check(highFreq.name == "highFrequencyContainer", "field[1] is highFrequencyContainer");
    check(!highFreq.optional, "highFrequencyContainer is mandatory");
    check(highFreq.type.kind == TypeKind::Reference && highFreq.type.referencedName == "HighFrequencyContainer",
          "highFrequencyContainer references HighFrequencyContainer");

    const auto &lowFreq = type.fields[2];
    check(lowFreq.name == "lowFrequencyContainer", "field[2] is lowFrequencyContainer");
    check(lowFreq.optional, "lowFrequencyContainer is OPTIONAL");
    check(!lowFreq.isExtensionAddition, "lowFrequencyContainer is a root field");

    const auto &special = type.fields[3];
    check(special.name == "specialVehicleContainer", "field[3] is specialVehicleContainer");
    check(special.optional, "specialVehicleContainer is OPTIONAL");
    check(!special.isExtensionAddition, "specialVehicleContainer is a root field");

    const auto &ext = type.fields[4];
    check(ext.name == "extensionContainers", "field[4] is extensionContainers");
    check(ext.optional, "extensionContainers is OPTIONAL");
    check(ext.isExtensionAddition, "extensionContainers is an extension addition (beyond '...')");
    check(ext.type.kind == TypeKind::Reference && ext.type.referencedName == "WrappedExtensionContainers",
          "extensionContainers references WrappedExtensionContainers");
}

void checkHighFrequencyContainer(const std::vector<Module> &camModules) {
    const TypeAssignment *hfc = findTypeAssignment(camModules, "HighFrequencyContainer");
    check(hfc != nullptr, "HighFrequencyContainer type assignment found");
    if (hfc == nullptr) {
        return;
    }
    const auto &type = hfc->type;
    check(type.kind == TypeKind::Choice, "HighFrequencyContainer is a CHOICE");
    check(type.fields.size() == 2, "HighFrequencyContainer has 2 root alternatives, got " +
                                        std::to_string(type.fields.size()));
    if (type.fields.size() == 2) {
        check(type.fields[0].name == "basicVehicleContainerHighFrequency",
              "alternative[0] is basicVehicleContainerHighFrequency");
        check(type.fields[1].name == "rsuContainerHighFrequency", "alternative[1] is rsuContainerHighFrequency");
    }
}

void checkCddConstraints(const std::vector<Module> &cddModules) {
    // CountryCode ::= BIT STRING(SIZE(10)) — a fixed-size SIZE constraint.
    const TypeAssignment *countryCode = findTypeAssignment(cddModules, "CountryCode");
    check(countryCode != nullptr, "CountryCode type assignment found in ETSI-ITS-CDD.asn");
    if (countryCode != nullptr) {
        check(countryCode->type.kind == TypeKind::BitString, "CountryCode is a BIT STRING");
        check(countryCode->type.constraints.size() == 1, "CountryCode has exactly one constraint");
        if (countryCode->type.constraints.size() == 1) {
            const auto &c = countryCode->type.constraints[0];
            check(c.kind == ConstraintKind::SizeRange, "CountryCode's constraint is structurally a SizeRange");
            check(c.rootRanges.size() == 1, "CountryCode's SizeRange has one root range");
            if (c.rootRanges.size() == 1) {
                check(c.rootRanges[0].lower.raw == "10" && c.rootRanges[0].upper.raw == "10",
                      "CountryCode's SIZE resolves to the fixed value 10");
            }
            check(!c.extensible, "CountryCode's SIZE constraint is not extensible");
        }
    }

    // DeltaTimeMilliSecondPositive ::= INTEGER (1..10000) — a plain value range.
    const TypeAssignment *delta = findTypeAssignment(cddModules, "DeltaTimeMilliSecondPositive");
    check(delta != nullptr, "DeltaTimeMilliSecondPositive type assignment found in ETSI-ITS-CDD.asn");
    if (delta != nullptr) {
        check(delta->type.kind == TypeKind::Integer, "DeltaTimeMilliSecondPositive is an INTEGER");
        check(delta->type.constraints.size() == 1, "DeltaTimeMilliSecondPositive has exactly one constraint");
        if (delta->type.constraints.size() == 1) {
            const auto &c = delta->type.constraints[0];
            check(c.kind == ConstraintKind::ValueRange,
                  "DeltaTimeMilliSecondPositive's constraint is structurally a ValueRange");
            check(c.rootRanges.size() == 1, "DeltaTimeMilliSecondPositive's ValueRange has one root range");
            if (c.rootRanges.size() == 1) {
                check(c.rootRanges[0].lower.raw == "1" && c.rootRanges[0].upper.raw == "10000",
                      "DeltaTimeMilliSecondPositive resolves to bounds [1, 10000]");
            }
            check(!c.extensible, "DeltaTimeMilliSecondPositive's constraint is not extensible");
        }
    }
}

}  // namespace

int main() {
    std::vector<fs::path> files;
    for (auto &entry : fs::recursive_directory_iterator("fixtures")) {
        if (entry.is_regular_file() && entry.path().extension() == ".asn") {
            files.push_back(entry.path());
        }
    }
    std::sort(files.begin(), files.end());

    if (files.empty()) {
        std::cerr << "no .asn fixtures found under fixtures/\n";
        return 1;
    }

    std::cout << "--- walking all " << files.size() << " fixtures through AstBuilder ---\n";
    int walked = 0;
    for (auto &f : files) {
        auto modules = parseToAst(f);
        if (!modules.empty()) {
            ++walked;
        }
    }
    std::cout << walked << "/" << files.size() << " fixtures produced a non-empty AST with no crash\n\n";

    std::cout << "--- CamParameters / HighFrequencyContainer (CAM-PDU-Descriptions.asn) ---\n";
    auto camModules = parseToAst("fixtures/CAM-PDU-Descriptions.asn");
    checkCamParameters(camModules);
    checkHighFrequencyContainer(camModules);

    std::cout << "\n--- constraint extraction (ETSI-ITS-CDD.asn) ---\n";
    auto cddModules = parseToAst("fixtures/ETSI-ITS-CDD.asn");
    checkCddConstraints(cddModules);

    std::cout << "\n" << failureCount << " failure(s)\n";
    return failureCount == 0 ? 0 : 1;
}
