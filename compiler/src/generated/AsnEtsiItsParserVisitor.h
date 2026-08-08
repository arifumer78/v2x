
// Generated from AsnEtsiItsParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "AsnEtsiItsParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by AsnEtsiItsParser.
 */
class  AsnEtsiItsParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by AsnEtsiItsParser.
   */
    virtual std::any visitModules(AsnEtsiItsParser::ModulesContext *context) = 0;

    virtual std::any visitModuleDefinition(AsnEtsiItsParser::ModuleDefinitionContext *context) = 0;

    virtual std::any visitTagDefault(AsnEtsiItsParser::TagDefaultContext *context) = 0;

    virtual std::any visitExtensionDefault(AsnEtsiItsParser::ExtensionDefaultContext *context) = 0;

    virtual std::any visitModuleBody(AsnEtsiItsParser::ModuleBodyContext *context) = 0;

    virtual std::any visitExports(AsnEtsiItsParser::ExportsContext *context) = 0;

    virtual std::any visitSymbolsExported(AsnEtsiItsParser::SymbolsExportedContext *context) = 0;

    virtual std::any visitImports(AsnEtsiItsParser::ImportsContext *context) = 0;

    virtual std::any visitSymbolsImported(AsnEtsiItsParser::SymbolsImportedContext *context) = 0;

    virtual std::any visitSymbolsFromModuleList(AsnEtsiItsParser::SymbolsFromModuleListContext *context) = 0;

    virtual std::any visitSymbolsFromModule(AsnEtsiItsParser::SymbolsFromModuleContext *context) = 0;

    virtual std::any visitGlobalModuleReference(AsnEtsiItsParser::GlobalModuleReferenceContext *context) = 0;

    virtual std::any visitAssignedIdentifier(AsnEtsiItsParser::AssignedIdentifierContext *context) = 0;

    virtual std::any visitSymbolList(AsnEtsiItsParser::SymbolListContext *context) = 0;

    virtual std::any visitSymbol(AsnEtsiItsParser::SymbolContext *context) = 0;

    virtual std::any visitAssignmentList(AsnEtsiItsParser::AssignmentListContext *context) = 0;

    virtual std::any visitAssignment(AsnEtsiItsParser::AssignmentContext *context) = 0;

    virtual std::any visitSequenceType(AsnEtsiItsParser::SequenceTypeContext *context) = 0;

    virtual std::any visitExtensionAndException(AsnEtsiItsParser::ExtensionAndExceptionContext *context) = 0;

    virtual std::any visitOptionalExtensionMarker(AsnEtsiItsParser::OptionalExtensionMarkerContext *context) = 0;

    virtual std::any visitComponentTypeLists(AsnEtsiItsParser::ComponentTypeListsContext *context) = 0;

    virtual std::any visitRootComponentTypeList(AsnEtsiItsParser::RootComponentTypeListContext *context) = 0;

    virtual std::any visitComponentTypeList(AsnEtsiItsParser::ComponentTypeListContext *context) = 0;

    virtual std::any visitComponentType(AsnEtsiItsParser::ComponentTypeContext *context) = 0;

    virtual std::any visitTag(AsnEtsiItsParser::TagContext *context) = 0;

    virtual std::any visitNeedTag(AsnEtsiItsParser::NeedTagContext *context) = 0;

    virtual std::any visitCondTag(AsnEtsiItsParser::CondTagContext *context) = 0;

    virtual std::any visitExtensionAdditions(AsnEtsiItsParser::ExtensionAdditionsContext *context) = 0;

    virtual std::any visitExtensionAdditionList(AsnEtsiItsParser::ExtensionAdditionListContext *context) = 0;

    virtual std::any visitExtensionAddition(AsnEtsiItsParser::ExtensionAdditionContext *context) = 0;

    virtual std::any visitExtensionAdditionGroup(AsnEtsiItsParser::ExtensionAdditionGroupContext *context) = 0;

    virtual std::any visitVersionNumber(AsnEtsiItsParser::VersionNumberContext *context) = 0;

    virtual std::any visitSequenceOfType(AsnEtsiItsParser::SequenceOfTypeContext *context) = 0;

    virtual std::any visitSizeConstraint(AsnEtsiItsParser::SizeConstraintContext *context) = 0;

    virtual std::any visitParameterizedAssignment(AsnEtsiItsParser::ParameterizedAssignmentContext *context) = 0;

    virtual std::any visitParameterList(AsnEtsiItsParser::ParameterListContext *context) = 0;

    virtual std::any visitParameter(AsnEtsiItsParser::ParameterContext *context) = 0;

    virtual std::any visitParamGovernor(AsnEtsiItsParser::ParamGovernorContext *context) = 0;

    virtual std::any visitGovernor(AsnEtsiItsParser::GovernorContext *context) = 0;

    virtual std::any visitObjectClassAssignment(AsnEtsiItsParser::ObjectClassAssignmentContext *context) = 0;

    virtual std::any visitObjectClass(AsnEtsiItsParser::ObjectClassContext *context) = 0;

    virtual std::any visitDefinedObjectClass(AsnEtsiItsParser::DefinedObjectClassContext *context) = 0;

    virtual std::any visitUsefulObjectClassReference(AsnEtsiItsParser::UsefulObjectClassReferenceContext *context) = 0;

    virtual std::any visitExternalObjectClassReference(AsnEtsiItsParser::ExternalObjectClassReferenceContext *context) = 0;

    virtual std::any visitObjectClassDefn(AsnEtsiItsParser::ObjectClassDefnContext *context) = 0;

    virtual std::any visitWithSyntaxSpec(AsnEtsiItsParser::WithSyntaxSpecContext *context) = 0;

    virtual std::any visitSyntaxList(AsnEtsiItsParser::SyntaxListContext *context) = 0;

    virtual std::any visitTokenOrGroupSpec(AsnEtsiItsParser::TokenOrGroupSpecContext *context) = 0;

    virtual std::any visitOptionalGroup(AsnEtsiItsParser::OptionalGroupContext *context) = 0;

    virtual std::any visitRequiredToken(AsnEtsiItsParser::RequiredTokenContext *context) = 0;

    virtual std::any visitLiteral(AsnEtsiItsParser::LiteralContext *context) = 0;

    virtual std::any visitPrimitiveFieldName(AsnEtsiItsParser::PrimitiveFieldNameContext *context) = 0;

    virtual std::any visitFieldSpec(AsnEtsiItsParser::FieldSpecContext *context) = 0;

    virtual std::any visitTypeFieldSpec(AsnEtsiItsParser::TypeFieldSpecContext *context) = 0;

    virtual std::any visitTypeOptionalitySpec(AsnEtsiItsParser::TypeOptionalitySpecContext *context) = 0;

    virtual std::any visitFixedTypeValueFieldSpec(AsnEtsiItsParser::FixedTypeValueFieldSpecContext *context) = 0;

    virtual std::any visitValueOptionalitySpec(AsnEtsiItsParser::ValueOptionalitySpecContext *context) = 0;

    virtual std::any visitVariableTypeValueFieldSpec(AsnEtsiItsParser::VariableTypeValueFieldSpecContext *context) = 0;

    virtual std::any visitFixedTypeValueSetFieldSpec(AsnEtsiItsParser::FixedTypeValueSetFieldSpecContext *context) = 0;

    virtual std::any visitValueSetOptionalitySpec(AsnEtsiItsParser::ValueSetOptionalitySpecContext *context) = 0;

    virtual std::any visitObject_(AsnEtsiItsParser::Object_Context *context) = 0;

    virtual std::any visitObjectDefn(AsnEtsiItsParser::ObjectDefnContext *context) = 0;

    virtual std::any visitParameterizedObject(AsnEtsiItsParser::ParameterizedObjectContext *context) = 0;

    virtual std::any visitDefinedObject(AsnEtsiItsParser::DefinedObjectContext *context) = 0;

    virtual std::any visitObjectSet(AsnEtsiItsParser::ObjectSetContext *context) = 0;

    virtual std::any visitObjectSetSpec(AsnEtsiItsParser::ObjectSetSpecContext *context) = 0;

    virtual std::any visitFieldName(AsnEtsiItsParser::FieldNameContext *context) = 0;

    virtual std::any visitValueSet(AsnEtsiItsParser::ValueSetContext *context) = 0;

    virtual std::any visitElementSetSpecs(AsnEtsiItsParser::ElementSetSpecsContext *context) = 0;

    virtual std::any visitRootElementSetSpec(AsnEtsiItsParser::RootElementSetSpecContext *context) = 0;

    virtual std::any visitAdditionalElementSetSpec(AsnEtsiItsParser::AdditionalElementSetSpecContext *context) = 0;

    virtual std::any visitElementSetSpec(AsnEtsiItsParser::ElementSetSpecContext *context) = 0;

    virtual std::any visitUnions(AsnEtsiItsParser::UnionsContext *context) = 0;

    virtual std::any visitExclusions(AsnEtsiItsParser::ExclusionsContext *context) = 0;

    virtual std::any visitIntersections(AsnEtsiItsParser::IntersectionsContext *context) = 0;

    virtual std::any visitUnionMark(AsnEtsiItsParser::UnionMarkContext *context) = 0;

    virtual std::any visitIntersectionMark(AsnEtsiItsParser::IntersectionMarkContext *context) = 0;

    virtual std::any visitElements(AsnEtsiItsParser::ElementsContext *context) = 0;

    virtual std::any visitObjectSetElements(AsnEtsiItsParser::ObjectSetElementsContext *context) = 0;

    virtual std::any visitIntersectionElements(AsnEtsiItsParser::IntersectionElementsContext *context) = 0;

    virtual std::any visitSubtypeElements(AsnEtsiItsParser::SubtypeElementsContext *context) = 0;

    virtual std::any visitVariableTypeValueSetFieldSpec(AsnEtsiItsParser::VariableTypeValueSetFieldSpecContext *context) = 0;

    virtual std::any visitObjectFieldSpec(AsnEtsiItsParser::ObjectFieldSpecContext *context) = 0;

    virtual std::any visitObjectOptionalitySpec(AsnEtsiItsParser::ObjectOptionalitySpecContext *context) = 0;

    virtual std::any visitObjectSetFieldSpec(AsnEtsiItsParser::ObjectSetFieldSpecContext *context) = 0;

    virtual std::any visitObjectSetOptionalitySpec(AsnEtsiItsParser::ObjectSetOptionalitySpecContext *context) = 0;

    virtual std::any visitTypeAssignment(AsnEtsiItsParser::TypeAssignmentContext *context) = 0;

    virtual std::any visitValueAssignment(AsnEtsiItsParser::ValueAssignmentContext *context) = 0;

    virtual std::any visitAsnType(AsnEtsiItsParser::AsnTypeContext *context) = 0;

    virtual std::any visitBuiltinType(AsnEtsiItsParser::BuiltinTypeContext *context) = 0;

    virtual std::any visitTaggedType(AsnEtsiItsParser::TaggedTypeContext *context) = 0;

    virtual std::any visitClassTag(AsnEtsiItsParser::ClassTagContext *context) = 0;

    virtual std::any visitObjectClassFieldType(AsnEtsiItsParser::ObjectClassFieldTypeContext *context) = 0;

    virtual std::any visitSetType(AsnEtsiItsParser::SetTypeContext *context) = 0;

    virtual std::any visitSetOfType(AsnEtsiItsParser::SetOfTypeContext *context) = 0;

    virtual std::any visitReferencedType(AsnEtsiItsParser::ReferencedTypeContext *context) = 0;

    virtual std::any visitDefinedType(AsnEtsiItsParser::DefinedTypeContext *context) = 0;

    virtual std::any visitConstraint(AsnEtsiItsParser::ConstraintContext *context) = 0;

    virtual std::any visitConstraintSpec(AsnEtsiItsParser::ConstraintSpecContext *context) = 0;

    virtual std::any visitUserDefinedConstraint(AsnEtsiItsParser::UserDefinedConstraintContext *context) = 0;

    virtual std::any visitGeneralConstraint(AsnEtsiItsParser::GeneralConstraintContext *context) = 0;

    virtual std::any visitUserDefinedConstraintParameter(AsnEtsiItsParser::UserDefinedConstraintParameterContext *context) = 0;

    virtual std::any visitTableConstraint(AsnEtsiItsParser::TableConstraintContext *context) = 0;

    virtual std::any visitSimpleTableConstraint(AsnEtsiItsParser::SimpleTableConstraintContext *context) = 0;

    virtual std::any visitContentsConstraint(AsnEtsiItsParser::ContentsConstraintContext *context) = 0;

    virtual std::any visitComponentPresenceLists(AsnEtsiItsParser::ComponentPresenceListsContext *context) = 0;

    virtual std::any visitComponentPresenceList(AsnEtsiItsParser::ComponentPresenceListContext *context) = 0;

    virtual std::any visitComponentPresence(AsnEtsiItsParser::ComponentPresenceContext *context) = 0;

    virtual std::any visitSubtypeConstraint(AsnEtsiItsParser::SubtypeConstraintContext *context) = 0;

    virtual std::any visitValue(AsnEtsiItsParser::ValueContext *context) = 0;

    virtual std::any visitBuiltinValue(AsnEtsiItsParser::BuiltinValueContext *context) = 0;

    virtual std::any visitObjectIdentifierValue(AsnEtsiItsParser::ObjectIdentifierValueContext *context) = 0;

    virtual std::any visitObjIdComponentsList(AsnEtsiItsParser::ObjIdComponentsListContext *context) = 0;

    virtual std::any visitObjIdComponents(AsnEtsiItsParser::ObjIdComponentsContext *context) = 0;

    virtual std::any visitIntegerValue(AsnEtsiItsParser::IntegerValueContext *context) = 0;

    virtual std::any visitChoiceValue(AsnEtsiItsParser::ChoiceValueContext *context) = 0;

    virtual std::any visitEnumeratedValue(AsnEtsiItsParser::EnumeratedValueContext *context) = 0;

    virtual std::any visitSignedNumber(AsnEtsiItsParser::SignedNumberContext *context) = 0;

    virtual std::any visitChoiceType(AsnEtsiItsParser::ChoiceTypeContext *context) = 0;

    virtual std::any visitAlternativeTypeLists(AsnEtsiItsParser::AlternativeTypeListsContext *context) = 0;

    virtual std::any visitExtensionAdditionAlternatives(AsnEtsiItsParser::ExtensionAdditionAlternativesContext *context) = 0;

    virtual std::any visitExtensionAdditionAlternativesList(AsnEtsiItsParser::ExtensionAdditionAlternativesListContext *context) = 0;

    virtual std::any visitExtensionAdditionAlternative(AsnEtsiItsParser::ExtensionAdditionAlternativeContext *context) = 0;

    virtual std::any visitExtensionAdditionAlternativesGroup(AsnEtsiItsParser::ExtensionAdditionAlternativesGroupContext *context) = 0;

    virtual std::any visitRootAlternativeTypeList(AsnEtsiItsParser::RootAlternativeTypeListContext *context) = 0;

    virtual std::any visitAlternativeTypeList(AsnEtsiItsParser::AlternativeTypeListContext *context) = 0;

    virtual std::any visitNamedType(AsnEtsiItsParser::NamedTypeContext *context) = 0;

    virtual std::any visitEnumeratedType(AsnEtsiItsParser::EnumeratedTypeContext *context) = 0;

    virtual std::any visitEnumerations(AsnEtsiItsParser::EnumerationsContext *context) = 0;

    virtual std::any visitRootEnumeration(AsnEtsiItsParser::RootEnumerationContext *context) = 0;

    virtual std::any visitEnumeration(AsnEtsiItsParser::EnumerationContext *context) = 0;

    virtual std::any visitEnumerationItem(AsnEtsiItsParser::EnumerationItemContext *context) = 0;

    virtual std::any visitNamedNumber(AsnEtsiItsParser::NamedNumberContext *context) = 0;

    virtual std::any visitDefinedValue(AsnEtsiItsParser::DefinedValueContext *context) = 0;

    virtual std::any visitParameterizedValue(AsnEtsiItsParser::ParameterizedValueContext *context) = 0;

    virtual std::any visitSimpleDefinedValue(AsnEtsiItsParser::SimpleDefinedValueContext *context) = 0;

    virtual std::any visitActualParameterList(AsnEtsiItsParser::ActualParameterListContext *context) = 0;

    virtual std::any visitActualParameter(AsnEtsiItsParser::ActualParameterContext *context) = 0;

    virtual std::any visitExceptionSpec(AsnEtsiItsParser::ExceptionSpecContext *context) = 0;

    virtual std::any visitExceptionIdentification(AsnEtsiItsParser::ExceptionIdentificationContext *context) = 0;

    virtual std::any visitAdditionalEnumeration(AsnEtsiItsParser::AdditionalEnumerationContext *context) = 0;

    virtual std::any visitIntegerType(AsnEtsiItsParser::IntegerTypeContext *context) = 0;

    virtual std::any visitNamedNumberList(AsnEtsiItsParser::NamedNumberListContext *context) = 0;

    virtual std::any visitObjectidentifiertype(AsnEtsiItsParser::ObjectidentifiertypeContext *context) = 0;

    virtual std::any visitComponentRelationConstraint(AsnEtsiItsParser::ComponentRelationConstraintContext *context) = 0;

    virtual std::any visitAtNotation(AsnEtsiItsParser::AtNotationContext *context) = 0;

    virtual std::any visitLevel(AsnEtsiItsParser::LevelContext *context) = 0;

    virtual std::any visitComponentIdList(AsnEtsiItsParser::ComponentIdListContext *context) = 0;

    virtual std::any visitOctetStringType(AsnEtsiItsParser::OctetStringTypeContext *context) = 0;

    virtual std::any visitBitStringType(AsnEtsiItsParser::BitStringTypeContext *context) = 0;

    virtual std::any visitNamedBitList(AsnEtsiItsParser::NamedBitListContext *context) = 0;

    virtual std::any visitNamedBit(AsnEtsiItsParser::NamedBitContext *context) = 0;

    virtual std::any visitBooleanValue(AsnEtsiItsParser::BooleanValueContext *context) = 0;


};

