
// Generated from AsnEtsiItsParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "AsnEtsiItsParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by AsnEtsiItsParser.
 */
class  AsnEtsiItsParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterModules(AsnEtsiItsParser::ModulesContext *ctx) = 0;
  virtual void exitModules(AsnEtsiItsParser::ModulesContext *ctx) = 0;

  virtual void enterModuleDefinition(AsnEtsiItsParser::ModuleDefinitionContext *ctx) = 0;
  virtual void exitModuleDefinition(AsnEtsiItsParser::ModuleDefinitionContext *ctx) = 0;

  virtual void enterTagDefault(AsnEtsiItsParser::TagDefaultContext *ctx) = 0;
  virtual void exitTagDefault(AsnEtsiItsParser::TagDefaultContext *ctx) = 0;

  virtual void enterExtensionDefault(AsnEtsiItsParser::ExtensionDefaultContext *ctx) = 0;
  virtual void exitExtensionDefault(AsnEtsiItsParser::ExtensionDefaultContext *ctx) = 0;

  virtual void enterModuleBody(AsnEtsiItsParser::ModuleBodyContext *ctx) = 0;
  virtual void exitModuleBody(AsnEtsiItsParser::ModuleBodyContext *ctx) = 0;

  virtual void enterExports(AsnEtsiItsParser::ExportsContext *ctx) = 0;
  virtual void exitExports(AsnEtsiItsParser::ExportsContext *ctx) = 0;

  virtual void enterSymbolsExported(AsnEtsiItsParser::SymbolsExportedContext *ctx) = 0;
  virtual void exitSymbolsExported(AsnEtsiItsParser::SymbolsExportedContext *ctx) = 0;

  virtual void enterImports(AsnEtsiItsParser::ImportsContext *ctx) = 0;
  virtual void exitImports(AsnEtsiItsParser::ImportsContext *ctx) = 0;

  virtual void enterSymbolsImported(AsnEtsiItsParser::SymbolsImportedContext *ctx) = 0;
  virtual void exitSymbolsImported(AsnEtsiItsParser::SymbolsImportedContext *ctx) = 0;

  virtual void enterSymbolsFromModuleList(AsnEtsiItsParser::SymbolsFromModuleListContext *ctx) = 0;
  virtual void exitSymbolsFromModuleList(AsnEtsiItsParser::SymbolsFromModuleListContext *ctx) = 0;

  virtual void enterSymbolsFromModule(AsnEtsiItsParser::SymbolsFromModuleContext *ctx) = 0;
  virtual void exitSymbolsFromModule(AsnEtsiItsParser::SymbolsFromModuleContext *ctx) = 0;

  virtual void enterGlobalModuleReference(AsnEtsiItsParser::GlobalModuleReferenceContext *ctx) = 0;
  virtual void exitGlobalModuleReference(AsnEtsiItsParser::GlobalModuleReferenceContext *ctx) = 0;

  virtual void enterAssignedIdentifier(AsnEtsiItsParser::AssignedIdentifierContext *ctx) = 0;
  virtual void exitAssignedIdentifier(AsnEtsiItsParser::AssignedIdentifierContext *ctx) = 0;

  virtual void enterSymbolList(AsnEtsiItsParser::SymbolListContext *ctx) = 0;
  virtual void exitSymbolList(AsnEtsiItsParser::SymbolListContext *ctx) = 0;

  virtual void enterSymbol(AsnEtsiItsParser::SymbolContext *ctx) = 0;
  virtual void exitSymbol(AsnEtsiItsParser::SymbolContext *ctx) = 0;

  virtual void enterAssignmentList(AsnEtsiItsParser::AssignmentListContext *ctx) = 0;
  virtual void exitAssignmentList(AsnEtsiItsParser::AssignmentListContext *ctx) = 0;

  virtual void enterAssignment(AsnEtsiItsParser::AssignmentContext *ctx) = 0;
  virtual void exitAssignment(AsnEtsiItsParser::AssignmentContext *ctx) = 0;

  virtual void enterSequenceType(AsnEtsiItsParser::SequenceTypeContext *ctx) = 0;
  virtual void exitSequenceType(AsnEtsiItsParser::SequenceTypeContext *ctx) = 0;

  virtual void enterExtensionAndException(AsnEtsiItsParser::ExtensionAndExceptionContext *ctx) = 0;
  virtual void exitExtensionAndException(AsnEtsiItsParser::ExtensionAndExceptionContext *ctx) = 0;

  virtual void enterOptionalExtensionMarker(AsnEtsiItsParser::OptionalExtensionMarkerContext *ctx) = 0;
  virtual void exitOptionalExtensionMarker(AsnEtsiItsParser::OptionalExtensionMarkerContext *ctx) = 0;

  virtual void enterComponentTypeLists(AsnEtsiItsParser::ComponentTypeListsContext *ctx) = 0;
  virtual void exitComponentTypeLists(AsnEtsiItsParser::ComponentTypeListsContext *ctx) = 0;

  virtual void enterRootComponentTypeList(AsnEtsiItsParser::RootComponentTypeListContext *ctx) = 0;
  virtual void exitRootComponentTypeList(AsnEtsiItsParser::RootComponentTypeListContext *ctx) = 0;

  virtual void enterComponentTypeList(AsnEtsiItsParser::ComponentTypeListContext *ctx) = 0;
  virtual void exitComponentTypeList(AsnEtsiItsParser::ComponentTypeListContext *ctx) = 0;

  virtual void enterComponentType(AsnEtsiItsParser::ComponentTypeContext *ctx) = 0;
  virtual void exitComponentType(AsnEtsiItsParser::ComponentTypeContext *ctx) = 0;

  virtual void enterTag(AsnEtsiItsParser::TagContext *ctx) = 0;
  virtual void exitTag(AsnEtsiItsParser::TagContext *ctx) = 0;

  virtual void enterNeedTag(AsnEtsiItsParser::NeedTagContext *ctx) = 0;
  virtual void exitNeedTag(AsnEtsiItsParser::NeedTagContext *ctx) = 0;

  virtual void enterCondTag(AsnEtsiItsParser::CondTagContext *ctx) = 0;
  virtual void exitCondTag(AsnEtsiItsParser::CondTagContext *ctx) = 0;

  virtual void enterExtensionAdditions(AsnEtsiItsParser::ExtensionAdditionsContext *ctx) = 0;
  virtual void exitExtensionAdditions(AsnEtsiItsParser::ExtensionAdditionsContext *ctx) = 0;

  virtual void enterExtensionAdditionList(AsnEtsiItsParser::ExtensionAdditionListContext *ctx) = 0;
  virtual void exitExtensionAdditionList(AsnEtsiItsParser::ExtensionAdditionListContext *ctx) = 0;

  virtual void enterExtensionAddition(AsnEtsiItsParser::ExtensionAdditionContext *ctx) = 0;
  virtual void exitExtensionAddition(AsnEtsiItsParser::ExtensionAdditionContext *ctx) = 0;

  virtual void enterExtensionAdditionGroup(AsnEtsiItsParser::ExtensionAdditionGroupContext *ctx) = 0;
  virtual void exitExtensionAdditionGroup(AsnEtsiItsParser::ExtensionAdditionGroupContext *ctx) = 0;

  virtual void enterVersionNumber(AsnEtsiItsParser::VersionNumberContext *ctx) = 0;
  virtual void exitVersionNumber(AsnEtsiItsParser::VersionNumberContext *ctx) = 0;

  virtual void enterSequenceOfType(AsnEtsiItsParser::SequenceOfTypeContext *ctx) = 0;
  virtual void exitSequenceOfType(AsnEtsiItsParser::SequenceOfTypeContext *ctx) = 0;

  virtual void enterSizeConstraint(AsnEtsiItsParser::SizeConstraintContext *ctx) = 0;
  virtual void exitSizeConstraint(AsnEtsiItsParser::SizeConstraintContext *ctx) = 0;

  virtual void enterParameterizedAssignment(AsnEtsiItsParser::ParameterizedAssignmentContext *ctx) = 0;
  virtual void exitParameterizedAssignment(AsnEtsiItsParser::ParameterizedAssignmentContext *ctx) = 0;

  virtual void enterParameterList(AsnEtsiItsParser::ParameterListContext *ctx) = 0;
  virtual void exitParameterList(AsnEtsiItsParser::ParameterListContext *ctx) = 0;

  virtual void enterParameter(AsnEtsiItsParser::ParameterContext *ctx) = 0;
  virtual void exitParameter(AsnEtsiItsParser::ParameterContext *ctx) = 0;

  virtual void enterParamGovernor(AsnEtsiItsParser::ParamGovernorContext *ctx) = 0;
  virtual void exitParamGovernor(AsnEtsiItsParser::ParamGovernorContext *ctx) = 0;

  virtual void enterGovernor(AsnEtsiItsParser::GovernorContext *ctx) = 0;
  virtual void exitGovernor(AsnEtsiItsParser::GovernorContext *ctx) = 0;

  virtual void enterObjectClassAssignment(AsnEtsiItsParser::ObjectClassAssignmentContext *ctx) = 0;
  virtual void exitObjectClassAssignment(AsnEtsiItsParser::ObjectClassAssignmentContext *ctx) = 0;

  virtual void enterObjectClass(AsnEtsiItsParser::ObjectClassContext *ctx) = 0;
  virtual void exitObjectClass(AsnEtsiItsParser::ObjectClassContext *ctx) = 0;

  virtual void enterDefinedObjectClass(AsnEtsiItsParser::DefinedObjectClassContext *ctx) = 0;
  virtual void exitDefinedObjectClass(AsnEtsiItsParser::DefinedObjectClassContext *ctx) = 0;

  virtual void enterUsefulObjectClassReference(AsnEtsiItsParser::UsefulObjectClassReferenceContext *ctx) = 0;
  virtual void exitUsefulObjectClassReference(AsnEtsiItsParser::UsefulObjectClassReferenceContext *ctx) = 0;

  virtual void enterExternalObjectClassReference(AsnEtsiItsParser::ExternalObjectClassReferenceContext *ctx) = 0;
  virtual void exitExternalObjectClassReference(AsnEtsiItsParser::ExternalObjectClassReferenceContext *ctx) = 0;

  virtual void enterObjectClassDefn(AsnEtsiItsParser::ObjectClassDefnContext *ctx) = 0;
  virtual void exitObjectClassDefn(AsnEtsiItsParser::ObjectClassDefnContext *ctx) = 0;

  virtual void enterWithSyntaxSpec(AsnEtsiItsParser::WithSyntaxSpecContext *ctx) = 0;
  virtual void exitWithSyntaxSpec(AsnEtsiItsParser::WithSyntaxSpecContext *ctx) = 0;

  virtual void enterSyntaxList(AsnEtsiItsParser::SyntaxListContext *ctx) = 0;
  virtual void exitSyntaxList(AsnEtsiItsParser::SyntaxListContext *ctx) = 0;

  virtual void enterTokenOrGroupSpec(AsnEtsiItsParser::TokenOrGroupSpecContext *ctx) = 0;
  virtual void exitTokenOrGroupSpec(AsnEtsiItsParser::TokenOrGroupSpecContext *ctx) = 0;

  virtual void enterOptionalGroup(AsnEtsiItsParser::OptionalGroupContext *ctx) = 0;
  virtual void exitOptionalGroup(AsnEtsiItsParser::OptionalGroupContext *ctx) = 0;

  virtual void enterRequiredToken(AsnEtsiItsParser::RequiredTokenContext *ctx) = 0;
  virtual void exitRequiredToken(AsnEtsiItsParser::RequiredTokenContext *ctx) = 0;

  virtual void enterLiteral(AsnEtsiItsParser::LiteralContext *ctx) = 0;
  virtual void exitLiteral(AsnEtsiItsParser::LiteralContext *ctx) = 0;

  virtual void enterPrimitiveFieldName(AsnEtsiItsParser::PrimitiveFieldNameContext *ctx) = 0;
  virtual void exitPrimitiveFieldName(AsnEtsiItsParser::PrimitiveFieldNameContext *ctx) = 0;

  virtual void enterFieldSpec(AsnEtsiItsParser::FieldSpecContext *ctx) = 0;
  virtual void exitFieldSpec(AsnEtsiItsParser::FieldSpecContext *ctx) = 0;

  virtual void enterTypeFieldSpec(AsnEtsiItsParser::TypeFieldSpecContext *ctx) = 0;
  virtual void exitTypeFieldSpec(AsnEtsiItsParser::TypeFieldSpecContext *ctx) = 0;

  virtual void enterTypeOptionalitySpec(AsnEtsiItsParser::TypeOptionalitySpecContext *ctx) = 0;
  virtual void exitTypeOptionalitySpec(AsnEtsiItsParser::TypeOptionalitySpecContext *ctx) = 0;

  virtual void enterFixedTypeValueFieldSpec(AsnEtsiItsParser::FixedTypeValueFieldSpecContext *ctx) = 0;
  virtual void exitFixedTypeValueFieldSpec(AsnEtsiItsParser::FixedTypeValueFieldSpecContext *ctx) = 0;

  virtual void enterValueOptionalitySpec(AsnEtsiItsParser::ValueOptionalitySpecContext *ctx) = 0;
  virtual void exitValueOptionalitySpec(AsnEtsiItsParser::ValueOptionalitySpecContext *ctx) = 0;

  virtual void enterVariableTypeValueFieldSpec(AsnEtsiItsParser::VariableTypeValueFieldSpecContext *ctx) = 0;
  virtual void exitVariableTypeValueFieldSpec(AsnEtsiItsParser::VariableTypeValueFieldSpecContext *ctx) = 0;

  virtual void enterFixedTypeValueSetFieldSpec(AsnEtsiItsParser::FixedTypeValueSetFieldSpecContext *ctx) = 0;
  virtual void exitFixedTypeValueSetFieldSpec(AsnEtsiItsParser::FixedTypeValueSetFieldSpecContext *ctx) = 0;

  virtual void enterValueSetOptionalitySpec(AsnEtsiItsParser::ValueSetOptionalitySpecContext *ctx) = 0;
  virtual void exitValueSetOptionalitySpec(AsnEtsiItsParser::ValueSetOptionalitySpecContext *ctx) = 0;

  virtual void enterObject_(AsnEtsiItsParser::Object_Context *ctx) = 0;
  virtual void exitObject_(AsnEtsiItsParser::Object_Context *ctx) = 0;

  virtual void enterObjectDefn(AsnEtsiItsParser::ObjectDefnContext *ctx) = 0;
  virtual void exitObjectDefn(AsnEtsiItsParser::ObjectDefnContext *ctx) = 0;

  virtual void enterParameterizedObject(AsnEtsiItsParser::ParameterizedObjectContext *ctx) = 0;
  virtual void exitParameterizedObject(AsnEtsiItsParser::ParameterizedObjectContext *ctx) = 0;

  virtual void enterDefinedObject(AsnEtsiItsParser::DefinedObjectContext *ctx) = 0;
  virtual void exitDefinedObject(AsnEtsiItsParser::DefinedObjectContext *ctx) = 0;

  virtual void enterObjectSet(AsnEtsiItsParser::ObjectSetContext *ctx) = 0;
  virtual void exitObjectSet(AsnEtsiItsParser::ObjectSetContext *ctx) = 0;

  virtual void enterObjectSetSpec(AsnEtsiItsParser::ObjectSetSpecContext *ctx) = 0;
  virtual void exitObjectSetSpec(AsnEtsiItsParser::ObjectSetSpecContext *ctx) = 0;

  virtual void enterFieldName(AsnEtsiItsParser::FieldNameContext *ctx) = 0;
  virtual void exitFieldName(AsnEtsiItsParser::FieldNameContext *ctx) = 0;

  virtual void enterValueSet(AsnEtsiItsParser::ValueSetContext *ctx) = 0;
  virtual void exitValueSet(AsnEtsiItsParser::ValueSetContext *ctx) = 0;

  virtual void enterElementSetSpecs(AsnEtsiItsParser::ElementSetSpecsContext *ctx) = 0;
  virtual void exitElementSetSpecs(AsnEtsiItsParser::ElementSetSpecsContext *ctx) = 0;

  virtual void enterRootElementSetSpec(AsnEtsiItsParser::RootElementSetSpecContext *ctx) = 0;
  virtual void exitRootElementSetSpec(AsnEtsiItsParser::RootElementSetSpecContext *ctx) = 0;

  virtual void enterAdditionalElementSetSpec(AsnEtsiItsParser::AdditionalElementSetSpecContext *ctx) = 0;
  virtual void exitAdditionalElementSetSpec(AsnEtsiItsParser::AdditionalElementSetSpecContext *ctx) = 0;

  virtual void enterElementSetSpec(AsnEtsiItsParser::ElementSetSpecContext *ctx) = 0;
  virtual void exitElementSetSpec(AsnEtsiItsParser::ElementSetSpecContext *ctx) = 0;

  virtual void enterUnions(AsnEtsiItsParser::UnionsContext *ctx) = 0;
  virtual void exitUnions(AsnEtsiItsParser::UnionsContext *ctx) = 0;

  virtual void enterExclusions(AsnEtsiItsParser::ExclusionsContext *ctx) = 0;
  virtual void exitExclusions(AsnEtsiItsParser::ExclusionsContext *ctx) = 0;

  virtual void enterIntersections(AsnEtsiItsParser::IntersectionsContext *ctx) = 0;
  virtual void exitIntersections(AsnEtsiItsParser::IntersectionsContext *ctx) = 0;

  virtual void enterUnionMark(AsnEtsiItsParser::UnionMarkContext *ctx) = 0;
  virtual void exitUnionMark(AsnEtsiItsParser::UnionMarkContext *ctx) = 0;

  virtual void enterIntersectionMark(AsnEtsiItsParser::IntersectionMarkContext *ctx) = 0;
  virtual void exitIntersectionMark(AsnEtsiItsParser::IntersectionMarkContext *ctx) = 0;

  virtual void enterElements(AsnEtsiItsParser::ElementsContext *ctx) = 0;
  virtual void exitElements(AsnEtsiItsParser::ElementsContext *ctx) = 0;

  virtual void enterObjectSetElements(AsnEtsiItsParser::ObjectSetElementsContext *ctx) = 0;
  virtual void exitObjectSetElements(AsnEtsiItsParser::ObjectSetElementsContext *ctx) = 0;

  virtual void enterIntersectionElements(AsnEtsiItsParser::IntersectionElementsContext *ctx) = 0;
  virtual void exitIntersectionElements(AsnEtsiItsParser::IntersectionElementsContext *ctx) = 0;

  virtual void enterSubtypeElements(AsnEtsiItsParser::SubtypeElementsContext *ctx) = 0;
  virtual void exitSubtypeElements(AsnEtsiItsParser::SubtypeElementsContext *ctx) = 0;

  virtual void enterVariableTypeValueSetFieldSpec(AsnEtsiItsParser::VariableTypeValueSetFieldSpecContext *ctx) = 0;
  virtual void exitVariableTypeValueSetFieldSpec(AsnEtsiItsParser::VariableTypeValueSetFieldSpecContext *ctx) = 0;

  virtual void enterObjectFieldSpec(AsnEtsiItsParser::ObjectFieldSpecContext *ctx) = 0;
  virtual void exitObjectFieldSpec(AsnEtsiItsParser::ObjectFieldSpecContext *ctx) = 0;

  virtual void enterObjectOptionalitySpec(AsnEtsiItsParser::ObjectOptionalitySpecContext *ctx) = 0;
  virtual void exitObjectOptionalitySpec(AsnEtsiItsParser::ObjectOptionalitySpecContext *ctx) = 0;

  virtual void enterObjectSetFieldSpec(AsnEtsiItsParser::ObjectSetFieldSpecContext *ctx) = 0;
  virtual void exitObjectSetFieldSpec(AsnEtsiItsParser::ObjectSetFieldSpecContext *ctx) = 0;

  virtual void enterObjectSetOptionalitySpec(AsnEtsiItsParser::ObjectSetOptionalitySpecContext *ctx) = 0;
  virtual void exitObjectSetOptionalitySpec(AsnEtsiItsParser::ObjectSetOptionalitySpecContext *ctx) = 0;

  virtual void enterTypeAssignment(AsnEtsiItsParser::TypeAssignmentContext *ctx) = 0;
  virtual void exitTypeAssignment(AsnEtsiItsParser::TypeAssignmentContext *ctx) = 0;

  virtual void enterValueAssignment(AsnEtsiItsParser::ValueAssignmentContext *ctx) = 0;
  virtual void exitValueAssignment(AsnEtsiItsParser::ValueAssignmentContext *ctx) = 0;

  virtual void enterAsnType(AsnEtsiItsParser::AsnTypeContext *ctx) = 0;
  virtual void exitAsnType(AsnEtsiItsParser::AsnTypeContext *ctx) = 0;

  virtual void enterBuiltinType(AsnEtsiItsParser::BuiltinTypeContext *ctx) = 0;
  virtual void exitBuiltinType(AsnEtsiItsParser::BuiltinTypeContext *ctx) = 0;

  virtual void enterTaggedType(AsnEtsiItsParser::TaggedTypeContext *ctx) = 0;
  virtual void exitTaggedType(AsnEtsiItsParser::TaggedTypeContext *ctx) = 0;

  virtual void enterClassTag(AsnEtsiItsParser::ClassTagContext *ctx) = 0;
  virtual void exitClassTag(AsnEtsiItsParser::ClassTagContext *ctx) = 0;

  virtual void enterObjectClassFieldType(AsnEtsiItsParser::ObjectClassFieldTypeContext *ctx) = 0;
  virtual void exitObjectClassFieldType(AsnEtsiItsParser::ObjectClassFieldTypeContext *ctx) = 0;

  virtual void enterSetType(AsnEtsiItsParser::SetTypeContext *ctx) = 0;
  virtual void exitSetType(AsnEtsiItsParser::SetTypeContext *ctx) = 0;

  virtual void enterSetOfType(AsnEtsiItsParser::SetOfTypeContext *ctx) = 0;
  virtual void exitSetOfType(AsnEtsiItsParser::SetOfTypeContext *ctx) = 0;

  virtual void enterReferencedType(AsnEtsiItsParser::ReferencedTypeContext *ctx) = 0;
  virtual void exitReferencedType(AsnEtsiItsParser::ReferencedTypeContext *ctx) = 0;

  virtual void enterDefinedType(AsnEtsiItsParser::DefinedTypeContext *ctx) = 0;
  virtual void exitDefinedType(AsnEtsiItsParser::DefinedTypeContext *ctx) = 0;

  virtual void enterConstraint(AsnEtsiItsParser::ConstraintContext *ctx) = 0;
  virtual void exitConstraint(AsnEtsiItsParser::ConstraintContext *ctx) = 0;

  virtual void enterConstraintSpec(AsnEtsiItsParser::ConstraintSpecContext *ctx) = 0;
  virtual void exitConstraintSpec(AsnEtsiItsParser::ConstraintSpecContext *ctx) = 0;

  virtual void enterUserDefinedConstraint(AsnEtsiItsParser::UserDefinedConstraintContext *ctx) = 0;
  virtual void exitUserDefinedConstraint(AsnEtsiItsParser::UserDefinedConstraintContext *ctx) = 0;

  virtual void enterGeneralConstraint(AsnEtsiItsParser::GeneralConstraintContext *ctx) = 0;
  virtual void exitGeneralConstraint(AsnEtsiItsParser::GeneralConstraintContext *ctx) = 0;

  virtual void enterUserDefinedConstraintParameter(AsnEtsiItsParser::UserDefinedConstraintParameterContext *ctx) = 0;
  virtual void exitUserDefinedConstraintParameter(AsnEtsiItsParser::UserDefinedConstraintParameterContext *ctx) = 0;

  virtual void enterTableConstraint(AsnEtsiItsParser::TableConstraintContext *ctx) = 0;
  virtual void exitTableConstraint(AsnEtsiItsParser::TableConstraintContext *ctx) = 0;

  virtual void enterSimpleTableConstraint(AsnEtsiItsParser::SimpleTableConstraintContext *ctx) = 0;
  virtual void exitSimpleTableConstraint(AsnEtsiItsParser::SimpleTableConstraintContext *ctx) = 0;

  virtual void enterContentsConstraint(AsnEtsiItsParser::ContentsConstraintContext *ctx) = 0;
  virtual void exitContentsConstraint(AsnEtsiItsParser::ContentsConstraintContext *ctx) = 0;

  virtual void enterComponentPresenceLists(AsnEtsiItsParser::ComponentPresenceListsContext *ctx) = 0;
  virtual void exitComponentPresenceLists(AsnEtsiItsParser::ComponentPresenceListsContext *ctx) = 0;

  virtual void enterComponentPresenceList(AsnEtsiItsParser::ComponentPresenceListContext *ctx) = 0;
  virtual void exitComponentPresenceList(AsnEtsiItsParser::ComponentPresenceListContext *ctx) = 0;

  virtual void enterComponentPresence(AsnEtsiItsParser::ComponentPresenceContext *ctx) = 0;
  virtual void exitComponentPresence(AsnEtsiItsParser::ComponentPresenceContext *ctx) = 0;

  virtual void enterSubtypeConstraint(AsnEtsiItsParser::SubtypeConstraintContext *ctx) = 0;
  virtual void exitSubtypeConstraint(AsnEtsiItsParser::SubtypeConstraintContext *ctx) = 0;

  virtual void enterValue(AsnEtsiItsParser::ValueContext *ctx) = 0;
  virtual void exitValue(AsnEtsiItsParser::ValueContext *ctx) = 0;

  virtual void enterBuiltinValue(AsnEtsiItsParser::BuiltinValueContext *ctx) = 0;
  virtual void exitBuiltinValue(AsnEtsiItsParser::BuiltinValueContext *ctx) = 0;

  virtual void enterObjectIdentifierValue(AsnEtsiItsParser::ObjectIdentifierValueContext *ctx) = 0;
  virtual void exitObjectIdentifierValue(AsnEtsiItsParser::ObjectIdentifierValueContext *ctx) = 0;

  virtual void enterObjIdComponentsList(AsnEtsiItsParser::ObjIdComponentsListContext *ctx) = 0;
  virtual void exitObjIdComponentsList(AsnEtsiItsParser::ObjIdComponentsListContext *ctx) = 0;

  virtual void enterObjIdComponents(AsnEtsiItsParser::ObjIdComponentsContext *ctx) = 0;
  virtual void exitObjIdComponents(AsnEtsiItsParser::ObjIdComponentsContext *ctx) = 0;

  virtual void enterIntegerValue(AsnEtsiItsParser::IntegerValueContext *ctx) = 0;
  virtual void exitIntegerValue(AsnEtsiItsParser::IntegerValueContext *ctx) = 0;

  virtual void enterChoiceValue(AsnEtsiItsParser::ChoiceValueContext *ctx) = 0;
  virtual void exitChoiceValue(AsnEtsiItsParser::ChoiceValueContext *ctx) = 0;

  virtual void enterEnumeratedValue(AsnEtsiItsParser::EnumeratedValueContext *ctx) = 0;
  virtual void exitEnumeratedValue(AsnEtsiItsParser::EnumeratedValueContext *ctx) = 0;

  virtual void enterSignedNumber(AsnEtsiItsParser::SignedNumberContext *ctx) = 0;
  virtual void exitSignedNumber(AsnEtsiItsParser::SignedNumberContext *ctx) = 0;

  virtual void enterChoiceType(AsnEtsiItsParser::ChoiceTypeContext *ctx) = 0;
  virtual void exitChoiceType(AsnEtsiItsParser::ChoiceTypeContext *ctx) = 0;

  virtual void enterAlternativeTypeLists(AsnEtsiItsParser::AlternativeTypeListsContext *ctx) = 0;
  virtual void exitAlternativeTypeLists(AsnEtsiItsParser::AlternativeTypeListsContext *ctx) = 0;

  virtual void enterExtensionAdditionAlternatives(AsnEtsiItsParser::ExtensionAdditionAlternativesContext *ctx) = 0;
  virtual void exitExtensionAdditionAlternatives(AsnEtsiItsParser::ExtensionAdditionAlternativesContext *ctx) = 0;

  virtual void enterExtensionAdditionAlternativesList(AsnEtsiItsParser::ExtensionAdditionAlternativesListContext *ctx) = 0;
  virtual void exitExtensionAdditionAlternativesList(AsnEtsiItsParser::ExtensionAdditionAlternativesListContext *ctx) = 0;

  virtual void enterExtensionAdditionAlternative(AsnEtsiItsParser::ExtensionAdditionAlternativeContext *ctx) = 0;
  virtual void exitExtensionAdditionAlternative(AsnEtsiItsParser::ExtensionAdditionAlternativeContext *ctx) = 0;

  virtual void enterExtensionAdditionAlternativesGroup(AsnEtsiItsParser::ExtensionAdditionAlternativesGroupContext *ctx) = 0;
  virtual void exitExtensionAdditionAlternativesGroup(AsnEtsiItsParser::ExtensionAdditionAlternativesGroupContext *ctx) = 0;

  virtual void enterRootAlternativeTypeList(AsnEtsiItsParser::RootAlternativeTypeListContext *ctx) = 0;
  virtual void exitRootAlternativeTypeList(AsnEtsiItsParser::RootAlternativeTypeListContext *ctx) = 0;

  virtual void enterAlternativeTypeList(AsnEtsiItsParser::AlternativeTypeListContext *ctx) = 0;
  virtual void exitAlternativeTypeList(AsnEtsiItsParser::AlternativeTypeListContext *ctx) = 0;

  virtual void enterNamedType(AsnEtsiItsParser::NamedTypeContext *ctx) = 0;
  virtual void exitNamedType(AsnEtsiItsParser::NamedTypeContext *ctx) = 0;

  virtual void enterEnumeratedType(AsnEtsiItsParser::EnumeratedTypeContext *ctx) = 0;
  virtual void exitEnumeratedType(AsnEtsiItsParser::EnumeratedTypeContext *ctx) = 0;

  virtual void enterEnumerations(AsnEtsiItsParser::EnumerationsContext *ctx) = 0;
  virtual void exitEnumerations(AsnEtsiItsParser::EnumerationsContext *ctx) = 0;

  virtual void enterRootEnumeration(AsnEtsiItsParser::RootEnumerationContext *ctx) = 0;
  virtual void exitRootEnumeration(AsnEtsiItsParser::RootEnumerationContext *ctx) = 0;

  virtual void enterEnumeration(AsnEtsiItsParser::EnumerationContext *ctx) = 0;
  virtual void exitEnumeration(AsnEtsiItsParser::EnumerationContext *ctx) = 0;

  virtual void enterEnumerationItem(AsnEtsiItsParser::EnumerationItemContext *ctx) = 0;
  virtual void exitEnumerationItem(AsnEtsiItsParser::EnumerationItemContext *ctx) = 0;

  virtual void enterNamedNumber(AsnEtsiItsParser::NamedNumberContext *ctx) = 0;
  virtual void exitNamedNumber(AsnEtsiItsParser::NamedNumberContext *ctx) = 0;

  virtual void enterDefinedValue(AsnEtsiItsParser::DefinedValueContext *ctx) = 0;
  virtual void exitDefinedValue(AsnEtsiItsParser::DefinedValueContext *ctx) = 0;

  virtual void enterParameterizedValue(AsnEtsiItsParser::ParameterizedValueContext *ctx) = 0;
  virtual void exitParameterizedValue(AsnEtsiItsParser::ParameterizedValueContext *ctx) = 0;

  virtual void enterSimpleDefinedValue(AsnEtsiItsParser::SimpleDefinedValueContext *ctx) = 0;
  virtual void exitSimpleDefinedValue(AsnEtsiItsParser::SimpleDefinedValueContext *ctx) = 0;

  virtual void enterActualParameterList(AsnEtsiItsParser::ActualParameterListContext *ctx) = 0;
  virtual void exitActualParameterList(AsnEtsiItsParser::ActualParameterListContext *ctx) = 0;

  virtual void enterActualParameter(AsnEtsiItsParser::ActualParameterContext *ctx) = 0;
  virtual void exitActualParameter(AsnEtsiItsParser::ActualParameterContext *ctx) = 0;

  virtual void enterExceptionSpec(AsnEtsiItsParser::ExceptionSpecContext *ctx) = 0;
  virtual void exitExceptionSpec(AsnEtsiItsParser::ExceptionSpecContext *ctx) = 0;

  virtual void enterExceptionIdentification(AsnEtsiItsParser::ExceptionIdentificationContext *ctx) = 0;
  virtual void exitExceptionIdentification(AsnEtsiItsParser::ExceptionIdentificationContext *ctx) = 0;

  virtual void enterAdditionalEnumeration(AsnEtsiItsParser::AdditionalEnumerationContext *ctx) = 0;
  virtual void exitAdditionalEnumeration(AsnEtsiItsParser::AdditionalEnumerationContext *ctx) = 0;

  virtual void enterIntegerType(AsnEtsiItsParser::IntegerTypeContext *ctx) = 0;
  virtual void exitIntegerType(AsnEtsiItsParser::IntegerTypeContext *ctx) = 0;

  virtual void enterNamedNumberList(AsnEtsiItsParser::NamedNumberListContext *ctx) = 0;
  virtual void exitNamedNumberList(AsnEtsiItsParser::NamedNumberListContext *ctx) = 0;

  virtual void enterObjectidentifiertype(AsnEtsiItsParser::ObjectidentifiertypeContext *ctx) = 0;
  virtual void exitObjectidentifiertype(AsnEtsiItsParser::ObjectidentifiertypeContext *ctx) = 0;

  virtual void enterComponentRelationConstraint(AsnEtsiItsParser::ComponentRelationConstraintContext *ctx) = 0;
  virtual void exitComponentRelationConstraint(AsnEtsiItsParser::ComponentRelationConstraintContext *ctx) = 0;

  virtual void enterAtNotation(AsnEtsiItsParser::AtNotationContext *ctx) = 0;
  virtual void exitAtNotation(AsnEtsiItsParser::AtNotationContext *ctx) = 0;

  virtual void enterLevel(AsnEtsiItsParser::LevelContext *ctx) = 0;
  virtual void exitLevel(AsnEtsiItsParser::LevelContext *ctx) = 0;

  virtual void enterComponentIdList(AsnEtsiItsParser::ComponentIdListContext *ctx) = 0;
  virtual void exitComponentIdList(AsnEtsiItsParser::ComponentIdListContext *ctx) = 0;

  virtual void enterOctetStringType(AsnEtsiItsParser::OctetStringTypeContext *ctx) = 0;
  virtual void exitOctetStringType(AsnEtsiItsParser::OctetStringTypeContext *ctx) = 0;

  virtual void enterBitStringType(AsnEtsiItsParser::BitStringTypeContext *ctx) = 0;
  virtual void exitBitStringType(AsnEtsiItsParser::BitStringTypeContext *ctx) = 0;

  virtual void enterNamedBitList(AsnEtsiItsParser::NamedBitListContext *ctx) = 0;
  virtual void exitNamedBitList(AsnEtsiItsParser::NamedBitListContext *ctx) = 0;

  virtual void enterNamedBit(AsnEtsiItsParser::NamedBitContext *ctx) = 0;
  virtual void exitNamedBit(AsnEtsiItsParser::NamedBitContext *ctx) = 0;

  virtual void enterBooleanValue(AsnEtsiItsParser::BooleanValueContext *ctx) = 0;
  virtual void exitBooleanValue(AsnEtsiItsParser::BooleanValueContext *ctx) = 0;


};

