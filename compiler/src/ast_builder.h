#pragma once

// Visitor-based extraction of v2x::asn::Module ASTs (compiler/include/v2x/asn/ast.hpp)
// from the ANTLR concrete parse tree produced by AsnEtsiItsParser. Subclasses the
// generated AsnEtsiItsParserBaseVisitor per the design decision in
// docs/design/compiler-frontend-design.md §6 (already codegen'd with -visitor).
//
// Most of the methods below are not overrides of the generic visitX(ParseTree*)
// dispatch — they take the concrete Context type the grammar already guarantees at
// each call site and return the concrete AST type directly, rather than routing
// through std::any. The virtual visitX overrides that do exist are only the ones
// needed to participate in the base class's ParseTree::accept() mechanism; internally
// this class calls its own sibling methods directly wherever the next rule is
// statically known.

#include <optional>
#include <vector>

#include "v2x/asn/ast.hpp"

#include "AsnEtsiItsParserBaseVisitor.h"

class AstBuilder : public AsnEtsiItsParserBaseVisitor {
public:
    std::vector<v2x::asn::Module> build(AsnEtsiItsParser::ModulesContext *ctx);

private:
    v2x::asn::Module buildModule(AsnEtsiItsParser::ModuleDefinitionContext *ctx);
    std::vector<v2x::asn::Import> buildImports(AsnEtsiItsParser::ImportsContext *ctx);

    // assignment: dispatches on which alternative is present.
    void addAssignment(AsnEtsiItsParser::AssignmentContext *ctx, v2x::asn::Module &module);

    v2x::asn::TypeNode buildAsnType(AsnEtsiItsParser::AsnTypeContext *ctx);
    v2x::asn::TypeNode buildBuiltinType(AsnEtsiItsParser::BuiltinTypeContext *ctx);
    v2x::asn::TypeNode buildReferencedType(AsnEtsiItsParser::ReferencedTypeContext *ctx);

    v2x::asn::TypeNode buildSequenceOrSetType(AsnEtsiItsParser::ComponentTypeListsContext *listsCtx, bool isSet);
    void collectComponentTypeList(AsnEtsiItsParser::ComponentTypeListContext *ctx, bool isExtensionAddition,
                                   std::vector<v2x::asn::Field> &out);
    v2x::asn::Field buildField(AsnEtsiItsParser::ComponentTypeContext *ctx, bool isExtensionAddition);

    v2x::asn::TypeNode buildSequenceOfOrSetOfType(AsnEtsiItsParser::AsnTypeContext *innerAsnType,
                                                   AsnEtsiItsParser::NamedTypeContext *innerNamedType,
                                                   AsnEtsiItsParser::ConstraintContext *constraintCtx,
                                                   AsnEtsiItsParser::SizeConstraintContext *sizeConstraintCtx,
                                                   bool isSet);

    v2x::asn::TypeNode buildChoiceType(AsnEtsiItsParser::ChoiceTypeContext *ctx);
    v2x::asn::TypeNode buildEnumeratedType(AsnEtsiItsParser::EnumeratedTypeContext *ctx);
    v2x::asn::TypeNode buildBitStringType(AsnEtsiItsParser::BitStringTypeContext *ctx);
    v2x::asn::TypeNode buildTaggedType(AsnEtsiItsParser::TaggedTypeContext *ctx);
    v2x::asn::Tag buildTag(AsnEtsiItsParser::ClassTagContext *ctx);

    v2x::asn::Constraint buildConstraint(AsnEtsiItsParser::ConstraintContext *ctx);
};
