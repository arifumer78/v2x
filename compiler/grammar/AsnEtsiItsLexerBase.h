#ifndef ASN_3GPPLEXERBASE_H
#define ASN_3GPPLEXERBASE_H

#include "antlr4-runtime.h"

class AsnEtsiItsLexerBase : public antlr4::Lexer
{
public:
    AsnEtsiItsLexerBase(antlr4::CharStream *input) : Lexer(input), self(*this) { }

public:
    AsnEtsiItsLexerBase & self;

public:
    bool IsColumnZero()
    {
        return this->getCharPositionInLine() == 0;
    }
};

#endif