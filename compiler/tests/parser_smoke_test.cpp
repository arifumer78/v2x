// Smoke test for AsnEtsiItsLexer/Parser (compiler/grammar/): confirms the
// grammar parses real ETSI ASN.1 modules (not just hand-crafted snippets)
// with zero lexer/parser errors. This is a syntax-level check only — it does
// not validate that the resulting parse tree is a sound basis for IR
// generation; that needs a Visitor-based extraction test once IR work
// starts (see compiler/README.md).
#include <fstream>
#include <iostream>
#include <sstream>

#include "antlr4-runtime.h"

#include "AsnEtsiItsLexer.h"
#include "AsnEtsiItsParser.h"

using namespace antlr4;

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

bool tryParse(const std::string &path) {
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

    std::cout << "--- " << path << " (" << tokens.size() << " tokens) ---\n";
    tree::ParseTree *tree = parser.modules();
    (void)tree;

    std::cout << "lexer errors: " << lexerErrors.count << "\n";
    std::cout << "parser errors: " << parserErrors.count << "\n";
    return lexerErrors.count == 0 && parserErrors.count == 0;
}

}  // namespace

int main() {
    bool camOk = tryParse("fixtures/CAM-PDU-Descriptions.asn");
    bool cddOk = tryParse("fixtures/ETSI-ITS-CDD.asn");
    return (camOk && cddOk) ? 0 : 1;
}
