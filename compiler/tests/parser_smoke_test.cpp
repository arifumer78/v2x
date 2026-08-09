// Smoke test for AsnEtsiItsLexer/Parser (compiler/grammar/): confirms the
// grammar parses every real ETSI ASN.1 module vendored under fixtures/ (not
// just hand-crafted snippets) with zero lexer/parser errors. Walks the
// fixtures/ tree recursively, so adding a new message family is just
// dropping its .asn files in — no code change needed here.
//
// This is a syntax-level check only — it does not validate that the
// resulting parse tree is a sound basis for IR generation; that needs a
// Visitor-based extraction test once IR work starts (see
// docs/design/compiler-frontend-design.md).
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "antlr4-runtime.h"

#include "AsnEtsiItsLexer.h"
#include "AsnEtsiItsParser.h"

using namespace antlr4;
namespace fs = std::filesystem;

namespace {

class CountingErrorListener : public BaseErrorListener {
public:
    size_t count = 0;
    void syntaxError(Recognizer *, Token *, size_t line, size_t charPositionInLine,
                      const std::string &msg, std::exception_ptr) override {
        std::cerr << "  line " << line << ":" << charPositionInLine << " " << msg << "\n";
        count++;
    }
};

bool tryParse(const fs::path &path) {
    std::ifstream stream(path, std::ios::binary);
    if (!stream) {
        std::cerr << "could not open " << path << "\n";
        return false;
    }
    std::stringstream ss;
    ss << stream.rdbuf();
    std::string content = ss.str();

    ANTLRInputStream input(content);
    AsnEtsiItsLexer lexer(&input);
    CountingErrorListener lexerErrors;
    lexer.removeErrorListeners();
    lexer.addErrorListener(&lexerErrors);

    CommonTokenStream tokens(&lexer);
    tokens.fill();

    AsnEtsiItsParser parser(&tokens);
    CountingErrorListener parserErrors;
    parser.removeErrorListeners();
    parser.addErrorListener(&parserErrors);

    std::cout << "--- " << path.string() << " (" << tokens.size() << " tokens) ---\n";
    tree::ParseTree *tree = parser.modules();
    (void)tree;

    std::cout << "lexer errors: " << lexerErrors.count << "\n";
    std::cout << "parser errors: " << parserErrors.count << "\n";
    return lexerErrors.count == 0 && parserErrors.count == 0;
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

    int passCount = 0;
    for (auto &f : files) {
        if (tryParse(f)) {
            passCount++;
        }
    }
    std::cout << "\n" << passCount << "/" << files.size() << " fixtures parsed clean\n";
    return (passCount == static_cast<int>(files.size())) ? 0 : 1;
}
