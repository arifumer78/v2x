
// Generated from AsnEtsiItsParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "AsnEtsiItsParserListener.h"


/**
 * This class provides an empty implementation of AsnEtsiItsParserListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  AsnEtsiItsParserBaseListener : public AsnEtsiItsParserListener {
public:

  virtual void enterModules(AsnEtsiItsParser::ModulesContext * /*ctx*/) override { }
  virtual void exitModules(AsnEtsiItsParser::ModulesContext * /*ctx*/) override { }

  virtual void enterModuleDefinition(AsnEtsiItsParser::ModuleDefinitionContext * /*ctx*/) override { }
  virtual void exitModuleDefinition(AsnEtsiItsParser::ModuleDefinitionContext * /*ctx*/) override { }

  virtual void enterTagDefault(AsnEtsiItsParser::TagDefaultContext * /*ctx*/) override { }
  virtual void exitTagDefault(AsnEtsiItsParser::TagDefaultContext * /*ctx*/) override { }

  virtual void enterExtensionDefault(AsnEtsiItsParser::ExtensionDefaultContext * /*ctx*/) override { }
  virtual void exitExtensionDefault(AsnEtsiItsParser::ExtensionDefaultContext * /*ctx*/) override { }

  virtual void enterModuleBody(AsnEtsiItsParser::ModuleBodyContext * /*ctx*/) override { }
  virtual void exitModuleBody(AsnEtsiItsParser::ModuleBodyContext * /*ctx*/) override { }

  virtual void enterExports(AsnEtsiItsParser::ExportsContext * /*ctx*/) override { }
  virtual void exitExports(AsnEtsiItsParser::ExportsContext * /*ctx*/) override { }

  virtual void enterSymbolsExported(AsnEtsiItsParser::SymbolsExportedContext * /*ctx*/) override { }
  virtual void exitSymbolsExported(AsnEtsiItsParser::SymbolsExportedContext * /*ctx*/) override { }

  virtual void enterImports(AsnEtsiItsParser::ImportsContext * /*ctx*/) override { }
  virtual void exitImports(AsnEtsiItsParser::ImportsContext * /*ctx*/) override { }

  virtual void enterSymbolsImported(AsnEtsiItsParser::SymbolsImportedContext * /*ctx*/) override { }
  virtual void exitSymbolsImported(AsnEtsiItsParser::SymbolsImportedContext * /*ctx*/) override { }

  virtual void enterSymbolsFromModuleList(AsnEtsiItsParser::SymbolsFromModuleListContext * /*ctx*/) override { }
  virtual void exitSymbolsFromModuleList(AsnEtsiItsParser::SymbolsFromModuleListContext * /*ctx*/) override { }

  virtual void enterSymbolsFromModule(AsnEtsiItsParser::SymbolsFromModuleContext * /*ctx*/) override { }
  virtual void exitSymbolsFromModule(AsnEtsiItsParser::SymbolsFromModuleContext * /*ctx*/) override { }

  virtual void enterGlobalModuleReference(AsnEtsiItsParser::GlobalModuleReferenceContext * /*ctx*/) override { }
  virtual void exitGlobalModuleReference(AsnEtsiItsParser::GlobalModuleReferenceContext * /*ctx*/) override { }

  virtual void enterAssignedIdentifier(AsnEtsiItsParser::AssignedIdentifierContext * /*ctx*/) override { }
  virtual void exitAssignedIdentifier(AsnEtsiItsParser::AssignedIdentifierContext * /*ctx*/) override { }

  virtual void enterSymbolList(AsnEtsiItsParser::SymbolListContext * /*ctx*/) override { }
  virtual void exitSymbolList(AsnEtsiItsParser::SymbolListContext * /*ctx*/) override { }

  virtual void enterSymbol(AsnEtsiItsParser::SymbolContext * /*ctx*/) override { }
  virtual void exitSymbol(AsnEtsiItsParser::SymbolContext * /*ctx*/) override { }

  virtual void enterAssignmentList(AsnEtsiItsParser::AssignmentListContext * /*ctx*/) override { }
  virtual void exitAssignmentList(AsnEtsiItsParser::AssignmentListContext * /*ctx*/) override { }

  virtual void enterAssignment(AsnEtsiItsParser::AssignmentContext * /*ctx*/) override { }
  virtual void exitAssignment(AsnEtsiItsParser::AssignmentContext * /*ctx*/) override { }

  virtual void enterSequenceType(AsnEtsiItsParser::SequenceTypeContext * /*ctx*/) override { }
  virtual void exitSequenceType(AsnEtsiItsParser::SequenceTypeContext * /*ctx*/) override { }

  virtual void enterExtensionAndException(AsnEtsiItsParser::ExtensionAndExceptionContext * /*ctx*/) override { }
  virtual void exitExtensionAndException(AsnEtsiItsParser::ExtensionAndExceptionContext * /*ctx*/) override { }

  virtual void enterOptionalExtensionMarker(AsnEtsiItsParser::OptionalExtensionMarkerContext * /*ctx*/) override { }
  virtual void exitOptionalExtensionMarker(AsnEtsiItsParser::OptionalExtensionMarkerContext * /*ctx*/) override { }

  virtual void enterComponentTypeLists(AsnEtsiItsParser::ComponentTypeListsContext * /*ctx*/) override { }
  virtual void exitComponentTypeLists(AsnEtsiItsParser::ComponentTypeListsContext * /*ctx*/) override { }

  virtual void enterRootComponentTypeList(AsnEtsiItsParser::RootComponentTypeListContext * /*ctx*/) override { }
  virtual void exitRootComponentTypeList(AsnEtsiItsParser::RootComponentTypeListContext * /*ctx*/) override { }

  virtual void enterComponentTypeList(AsnEtsiItsParser::ComponentTypeListContext * /*ctx*/) override { }
  virtual void exitComponentTypeList(AsnEtsiItsParser::ComponentTypeListContext * /*ctx*/) override { }

  virtual void enterComponentType(AsnEtsiItsParser::ComponentTypeContext * /*ctx*/) override { }
  virtual void exitComponentType(AsnEtsiItsParser::ComponentTypeContext * /*ctx*/) override { }

  virtual void enterTag(AsnEtsiItsParser::TagContext * /*ctx*/) override { }
  virtual void exitTag(AsnEtsiItsParser::TagContext * /*ctx*/) override { }

  virtual void enterNeedTag(AsnEtsiItsParser::NeedTagContext * /*ctx*/) override { }
  virtual void exitNeedTag(AsnEtsiItsParser::NeedTagContext * /*ctx*/) override { }

  virtual void enterCondTag(AsnEtsiItsParser::CondTagContext * /*ctx*/) override { }
  virtual void exitCondTag(AsnEtsiItsParser::CondTagContext * /*ctx*/) override { }

  virtual void enterExtensionAdditions(AsnEtsiItsParser::ExtensionAdditionsContext * /*ctx*/) override { }
  virtual void exitExtensionAdditions(AsnEtsiItsParser::ExtensionAdditionsContext * /*ctx*/) override { }

  virtual void enterExtensionAdditionList(AsnEtsiItsParser::ExtensionAdditionListContext * /*ctx*/) override { }
  virtual void exitExtensionAdditionList(AsnEtsiItsParser::ExtensionAdditionListContext * /*ctx*/) override { }

  virtual void enterExtensionAddition(AsnEtsiItsParser::ExtensionAdditionContext * /*ctx*/) override { }
  virtual void exitExtensionAddition(AsnEtsiItsParser::ExtensionAdditionContext * /*ctx*/) override { }

  virtual void enterExtensionAdditionGroup(AsnEtsiItsParser::ExtensionAdditionGroupContext * /*ctx*/) override { }
  virtual void exitExtensionAdditionGroup(AsnEtsiItsParser::ExtensionAdditionGroupContext * /*ctx*/) override { }

  virtual void enterVersionNumber(AsnEtsiItsParser::VersionNumberContext * /*ctx*/) override { }
  virtual void exitVersionNumber(AsnEtsiItsParser::VersionNumberContext * /*ctx*/) override { }

  virtual void enterSequenceOfType(AsnEtsiItsParser::SequenceOfTypeContext * /*ctx*/) override { }
  virtual void exitSequenceOfType(AsnEtsiItsParser::SequenceOfTypeContext * /*ctx*/) override { }

  virtual void enterSizeConstraint(AsnEtsiItsParser::SizeConstraintContext * /*ctx*/) override { }
  virtual void exitSizeConstraint(AsnEtsiItsParser::SizeConstraintContext * /*ctx*/) override { }

  virtual void enterParameterizedAssignment(AsnEtsiItsParser::ParameterizedAssignmentContext * /*ctx*/) override { }
  virtual void exitParameterizedAssignment(AsnEtsiItsParser::ParameterizedAssignmentContext * /*ctx*/) override { }

  virtual void enterParameterList(AsnEtsiItsParser::ParameterListContext * /*ctx*/) override { }
  virtual void exitParameterList(AsnEtsiItsParser::ParameterListContext * /*ctx*/) override { }

  virtual void enterParameter(AsnEtsiItsParser::ParameterContext * /*ctx*/) override { }
  virtual void exitParameter(AsnEtsiItsParser::ParameterContext * /*ctx*/) override { }

  virtual void enterParamGovernor(AsnEtsiItsParser::ParamGovernorContext * /*ctx*/) override { }
  virtual void exitParamGovernor(AsnEtsiItsParser::ParamGovernorContext * /*ctx*/) override { }

  virtual void enterGovernor(AsnEtsiItsParser::GovernorContext * /*ctx*/) override { }
  virtual void exitGovernor(AsnEtsiItsParser::GovernorContext * /*ctx*/) override { }

  virtual void enterObjectClassAssignment(AsnEtsiItsParser::ObjectClassAssignmentContext * /*ctx*/) override { }
  virtual void exitObjectClassAssignment(AsnEtsiItsParser::ObjectClassAssignmentContext * /*ctx*/) override { }

  virtual void enterObjectClass(AsnEtsiItsParser::ObjectClassContext * /*ctx*/) override { }
  virtual void exitObjectClass(AsnEtsiItsParser::ObjectClassContext * /*ctx*/) override { }

  virtual void enterDefinedObjectClass(AsnEtsiItsParser::DefinedObjectClassContext * /*ctx*/) override { }
  virtual void exitDefinedObjectClass(AsnEtsiItsParser::DefinedObjectClassContext * /*ctx*/) override { }

  virtual void enterUsefulObjectClassReference(AsnEtsiItsParser::UsefulObjectClassReferenceContext * /*ctx*/) override { }
  virtual void exitUsefulObjectClassReference(AsnEtsiItsParser::UsefulObjectClassReferenceContext * /*ctx*/) override { }

  virtual void enterExternalObjectClassReference(AsnEtsiItsParser::ExternalObjectClassReferenceContext * /*ctx*/) override { }
  virtual void exitExternalObjectClassReference(AsnEtsiItsParser::ExternalObjectClassReferenceContext * /*ctx*/) override { }

  virtual void enterObjectClassDefn(AsnEtsiItsParser::ObjectClassDefnContext * /*ctx*/) override { }
  virtual void exitObjectClassDefn(AsnEtsiItsParser::ObjectClassDefnContext * /*ctx*/) override { }

  virtual void enterWithSyntaxSpec(AsnEtsiItsParser::WithSyntaxSpecContext * /*ctx*/) override { }
  virtual void exitWithSyntaxSpec(AsnEtsiItsParser::WithSyntaxSpecContext * /*ctx*/) override { }

  virtual void enterSyntaxList(AsnEtsiItsParser::SyntaxListContext * /*ctx*/) override { }
  virtual void exitSyntaxList(AsnEtsiItsParser::SyntaxListContext * /*ctx*/) override { }

  virtual void enterTokenOrGroupSpec(AsnEtsiItsParser::TokenOrGroupSpecContext * /*ctx*/) override { }
  virtual void exitTokenOrGroupSpec(AsnEtsiItsParser::TokenOrGroupSpecContext * /*ctx*/) override { }

  virtual void enterOptionalGroup(AsnEtsiItsParser::OptionalGroupContext * /*ctx*/) override { }
  virtual void exitOptionalGroup(AsnEtsiItsParser::OptionalGroupContext * /*ctx*/) override { }

  virtual void enterRequiredToken(AsnEtsiItsParser::RequiredTokenContext * /*ctx*/) override { }
  virtual void exitRequiredToken(AsnEtsiItsParser::RequiredTokenContext * /*ctx*/) override { }

  virtual void enterLiteral(AsnEtsiItsParser::LiteralContext * /*ctx*/) override { }
  virtual void exitLiteral(AsnEtsiItsParser::LiteralContext * /*ctx*/) override { }

  virtual void enterPrimitiveFieldName(AsnEtsiItsParser::PrimitiveFieldNameContext * /*ctx*/) override { }
  virtual void exitPrimitiveFieldName(AsnEtsiItsParser::PrimitiveFieldNameContext * /*ctx*/) override { }

  virtual void enterFieldSpec(AsnEtsiItsParser::FieldSpecContext * /*ctx*/) override { }
  virtual void exitFieldSpec(AsnEtsiItsParser::FieldSpecContext * /*ctx*/) override { }

  virtual void enterTypeFieldSpec(AsnEtsiItsParser::TypeFieldSpecContext * /*ctx*/) override { }
  virtual void exitTypeFieldSpec(AsnEtsiItsParser::TypeFieldSpecContext * /*ctx*/) override { }

  virtual void enterTypeOptionalitySpec(AsnEtsiItsParser::TypeOptionalitySpecContext * /*ctx*/) override { }
  virtual void exitTypeOptionalitySpec(AsnEtsiItsParser::TypeOptionalitySpecContext * /*ctx*/) override { }

  virtual void enterFixedTypeValueFieldSpec(AsnEtsiItsParser::FixedTypeValueFieldSpecContext * /*ctx*/) override { }
  virtual void exitFixedTypeValueFieldSpec(AsnEtsiItsParser::FixedTypeValueFieldSpecContext * /*ctx*/) override { }

  virtual void enterValueOptionalitySpec(AsnEtsiItsParser::ValueOptionalitySpecContext * /*ctx*/) override { }
  virtual void exitValueOptionalitySpec(AsnEtsiItsParser::ValueOptionalitySpecContext * /*ctx*/) override { }

  virtual void enterVariableTypeValueFieldSpec(AsnEtsiItsParser::VariableTypeValueFieldSpecContext * /*ctx*/) override { }
  virtual void exitVariableTypeValueFieldSpec(AsnEtsiItsParser::VariableTypeValueFieldSpecContext * /*ctx*/) override { }

  virtual void enterFixedTypeValueSetFieldSpec(AsnEtsiItsParser::FixedTypeValueSetFieldSpecContext * /*ctx*/) override { }
  virtual void exitFixedTypeValueSetFieldSpec(AsnEtsiItsParser::FixedTypeValueSetFieldSpecContext * /*ctx*/) override { }

  virtual void enterValueSetOptionalitySpec(AsnEtsiItsParser::ValueSetOptionalitySpecContext * /*ctx*/) override { }
  virtual void exitValueSetOptionalitySpec(AsnEtsiItsParser::ValueSetOptionalitySpecContext * /*ctx*/) override { }

  virtual void enterObject_(AsnEtsiItsParser::Object_Context * /*ctx*/) override { }
  virtual void exitObject_(AsnEtsiItsParser::Object_Context * /*ctx*/) override { }

  virtual void enterObjectDefn(AsnEtsiItsParser::ObjectDefnContext * /*ctx*/) override { }
  virtual void exitObjectDefn(AsnEtsiItsParser::ObjectDefnContext * /*ctx*/) override { }

  virtual void enterParameterizedObject(AsnEtsiItsParser::ParameterizedObjectContext * /*ctx*/) override { }
  virtual void exitParameterizedObject(AsnEtsiItsParser::ParameterizedObjectContext * /*ctx*/) override { }

  virtual void enterDefinedObject(AsnEtsiItsParser::DefinedObjectContext * /*ctx*/) override { }
  virtual void exitDefinedObject(AsnEtsiItsParser::DefinedObjectContext * /*ctx*/) override { }

  virtual void enterObjectSet(AsnEtsiItsParser::ObjectSetContext * /*ctx*/) override { }
  virtual void exitObjectSet(AsnEtsiItsParser::ObjectSetContext * /*ctx*/) override { }

  virtual void enterObjectSetSpec(AsnEtsiItsParser::ObjectSetSpecContext * /*ctx*/) override { }
  virtual void exitObjectSetSpec(AsnEtsiItsParser::ObjectSetSpecContext * /*ctx*/) override { }

  virtual void enterFieldName(AsnEtsiItsParser::FieldNameContext * /*ctx*/) override { }
  virtual void exitFieldName(AsnEtsiItsParser::FieldNameContext * /*ctx*/) override { }

  virtual void enterValueSet(AsnEtsiItsParser::ValueSetContext * /*ctx*/) override { }
  virtual void exitValueSet(AsnEtsiItsParser::ValueSetContext * /*ctx*/) override { }

  virtual void enterElementSetSpecs(AsnEtsiItsParser::ElementSetSpecsContext * /*ctx*/) override { }
  virtual void exitElementSetSpecs(AsnEtsiItsParser::ElementSetSpecsContext * /*ctx*/) override { }

  virtual void enterRootElementSetSpec(AsnEtsiItsParser::RootElementSetSpecContext * /*ctx*/) override { }
  virtual void exitRootElementSetSpec(AsnEtsiItsParser::RootElementSetSpecContext * /*ctx*/) override { }

  virtual void enterAdditionalElementSetSpec(AsnEtsiItsParser::AdditionalElementSetSpecContext * /*ctx*/) override { }
  virtual void exitAdditionalElementSetSpec(AsnEtsiItsParser::AdditionalElementSetSpecContext * /*ctx*/) override { }

  virtual void enterElementSetSpec(AsnEtsiItsParser::ElementSetSpecContext * /*ctx*/) override { }
  virtual void exitElementSetSpec(AsnEtsiItsParser::ElementSetSpecContext * /*ctx*/) override { }

  virtual void enterUnions(AsnEtsiItsParser::UnionsContext * /*ctx*/) override { }
  virtual void exitUnions(AsnEtsiItsParser::UnionsContext * /*ctx*/) override { }

  virtual void enterExclusions(AsnEtsiItsParser::ExclusionsContext * /*ctx*/) override { }
  virtual void exitExclusions(AsnEtsiItsParser::ExclusionsContext * /*ctx*/) override { }

  virtual void enterIntersections(AsnEtsiItsParser::IntersectionsContext * /*ctx*/) override { }
  virtual void exitIntersections(AsnEtsiItsParser::IntersectionsContext * /*ctx*/) override { }

  virtual void enterUnionMark(AsnEtsiItsParser::UnionMarkContext * /*ctx*/) override { }
  virtual void exitUnionMark(AsnEtsiItsParser::UnionMarkContext * /*ctx*/) override { }

  virtual void enterIntersectionMark(AsnEtsiItsParser::IntersectionMarkContext * /*ctx*/) override { }
  virtual void exitIntersectionMark(AsnEtsiItsParser::IntersectionMarkContext * /*ctx*/) override { }

  virtual void enterElements(AsnEtsiItsParser::ElementsContext * /*ctx*/) override { }
  virtual void exitElements(AsnEtsiItsParser::ElementsContext * /*ctx*/) override { }

  virtual void enterObjectSetElements(AsnEtsiItsParser::ObjectSetElementsContext * /*ctx*/) override { }
  virtual void exitObjectSetElements(AsnEtsiItsParser::ObjectSetElementsContext * /*ctx*/) override { }

  virtual void enterIntersectionElements(AsnEtsiItsParser::IntersectionElementsContext * /*ctx*/) override { }
  virtual void exitIntersectionElements(AsnEtsiItsParser::IntersectionElementsContext * /*ctx*/) override { }

  virtual void enterSubtypeElements(AsnEtsiItsParser::SubtypeElementsContext * /*ctx*/) override { }
  virtual void exitSubtypeElements(AsnEtsiItsParser::SubtypeElementsContext * /*ctx*/) override { }

  virtual void enterVariableTypeValueSetFieldSpec(AsnEtsiItsParser::VariableTypeValueSetFieldSpecContext * /*ctx*/) override { }
  virtual void exitVariableTypeValueSetFieldSpec(AsnEtsiItsParser::VariableTypeValueSetFieldSpecContext * /*ctx*/) override { }

  virtual void enterObjectFieldSpec(AsnEtsiItsParser::ObjectFieldSpecContext * /*ctx*/) override { }
  virtual void exitObjectFieldSpec(AsnEtsiItsParser::ObjectFieldSpecContext * /*ctx*/) override { }

  virtual void enterObjectOptionalitySpec(AsnEtsiItsParser::ObjectOptionalitySpecContext * /*ctx*/) override { }
  virtual void exitObjectOptionalitySpec(AsnEtsiItsParser::ObjectOptionalitySpecContext * /*ctx*/) override { }

  virtual void enterObjectSetFieldSpec(AsnEtsiItsParser::ObjectSetFieldSpecContext * /*ctx*/) override { }
  virtual void exitObjectSetFieldSpec(AsnEtsiItsParser::ObjectSetFieldSpecContext * /*ctx*/) override { }

  virtual void enterObjectSetOptionalitySpec(AsnEtsiItsParser::ObjectSetOptionalitySpecContext * /*ctx*/) override { }
  virtual void exitObjectSetOptionalitySpec(AsnEtsiItsParser::ObjectSetOptionalitySpecContext * /*ctx*/) override { }

  virtual void enterTypeAssignment(AsnEtsiItsParser::TypeAssignmentContext * /*ctx*/) override { }
  virtual void exitTypeAssignment(AsnEtsiItsParser::TypeAssignmentContext * /*ctx*/) override { }

  virtual void enterValueAssignment(AsnEtsiItsParser::ValueAssignmentContext * /*ctx*/) override { }
  virtual void exitValueAssignment(AsnEtsiItsParser::ValueAssignmentContext * /*ctx*/) override { }

  virtual void enterAsnType(AsnEtsiItsParser::AsnTypeContext * /*ctx*/) override { }
  virtual void exitAsnType(AsnEtsiItsParser::AsnTypeContext * /*ctx*/) override { }

  virtual void enterBuiltinType(AsnEtsiItsParser::BuiltinTypeContext * /*ctx*/) override { }
  virtual void exitBuiltinType(AsnEtsiItsParser::BuiltinTypeContext * /*ctx*/) override { }

  virtual void enterTaggedType(AsnEtsiItsParser::TaggedTypeContext * /*ctx*/) override { }
  virtual void exitTaggedType(AsnEtsiItsParser::TaggedTypeContext * /*ctx*/) override { }

  virtual void enterClassTag(AsnEtsiItsParser::ClassTagContext * /*ctx*/) override { }
  virtual void exitClassTag(AsnEtsiItsParser::ClassTagContext * /*ctx*/) override { }

  virtual void enterObjectClassFieldType(AsnEtsiItsParser::ObjectClassFieldTypeContext * /*ctx*/) override { }
  virtual void exitObjectClassFieldType(AsnEtsiItsParser::ObjectClassFieldTypeContext * /*ctx*/) override { }

  virtual void enterSetType(AsnEtsiItsParser::SetTypeContext * /*ctx*/) override { }
  virtual void exitSetType(AsnEtsiItsParser::SetTypeContext * /*ctx*/) override { }

  virtual void enterSetOfType(AsnEtsiItsParser::SetOfTypeContext * /*ctx*/) override { }
  virtual void exitSetOfType(AsnEtsiItsParser::SetOfTypeContext * /*ctx*/) override { }

  virtual void enterReferencedType(AsnEtsiItsParser::ReferencedTypeContext * /*ctx*/) override { }
  virtual void exitReferencedType(AsnEtsiItsParser::ReferencedTypeContext * /*ctx*/) override { }

  virtual void enterDefinedType(AsnEtsiItsParser::DefinedTypeContext * /*ctx*/) override { }
  virtual void exitDefinedType(AsnEtsiItsParser::DefinedTypeContext * /*ctx*/) override { }

  virtual void enterConstraint(AsnEtsiItsParser::ConstraintContext * /*ctx*/) override { }
  virtual void exitConstraint(AsnEtsiItsParser::ConstraintContext * /*ctx*/) override { }

  virtual void enterConstraintSpec(AsnEtsiItsParser::ConstraintSpecContext * /*ctx*/) override { }
  virtual void exitConstraintSpec(AsnEtsiItsParser::ConstraintSpecContext * /*ctx*/) override { }

  virtual void enterUserDefinedConstraint(AsnEtsiItsParser::UserDefinedConstraintContext * /*ctx*/) override { }
  virtual void exitUserDefinedConstraint(AsnEtsiItsParser::UserDefinedConstraintContext * /*ctx*/) override { }

  virtual void enterGeneralConstraint(AsnEtsiItsParser::GeneralConstraintContext * /*ctx*/) override { }
  virtual void exitGeneralConstraint(AsnEtsiItsParser::GeneralConstraintContext * /*ctx*/) override { }

  virtual void enterUserDefinedConstraintParameter(AsnEtsiItsParser::UserDefinedConstraintParameterContext * /*ctx*/) override { }
  virtual void exitUserDefinedConstraintParameter(AsnEtsiItsParser::UserDefinedConstraintParameterContext * /*ctx*/) override { }

  virtual void enterTableConstraint(AsnEtsiItsParser::TableConstraintContext * /*ctx*/) override { }
  virtual void exitTableConstraint(AsnEtsiItsParser::TableConstraintContext * /*ctx*/) override { }

  virtual void enterSimpleTableConstraint(AsnEtsiItsParser::SimpleTableConstraintContext * /*ctx*/) override { }
  virtual void exitSimpleTableConstraint(AsnEtsiItsParser::SimpleTableConstraintContext * /*ctx*/) override { }

  virtual void enterContentsConstraint(AsnEtsiItsParser::ContentsConstraintContext * /*ctx*/) override { }
  virtual void exitContentsConstraint(AsnEtsiItsParser::ContentsConstraintContext * /*ctx*/) override { }

  virtual void enterComponentPresenceLists(AsnEtsiItsParser::ComponentPresenceListsContext * /*ctx*/) override { }
  virtual void exitComponentPresenceLists(AsnEtsiItsParser::ComponentPresenceListsContext * /*ctx*/) override { }

  virtual void enterComponentPresenceList(AsnEtsiItsParser::ComponentPresenceListContext * /*ctx*/) override { }
  virtual void exitComponentPresenceList(AsnEtsiItsParser::ComponentPresenceListContext * /*ctx*/) override { }

  virtual void enterComponentPresence(AsnEtsiItsParser::ComponentPresenceContext * /*ctx*/) override { }
  virtual void exitComponentPresence(AsnEtsiItsParser::ComponentPresenceContext * /*ctx*/) override { }

  virtual void enterSubtypeConstraint(AsnEtsiItsParser::SubtypeConstraintContext * /*ctx*/) override { }
  virtual void exitSubtypeConstraint(AsnEtsiItsParser::SubtypeConstraintContext * /*ctx*/) override { }

  virtual void enterValue(AsnEtsiItsParser::ValueContext * /*ctx*/) override { }
  virtual void exitValue(AsnEtsiItsParser::ValueContext * /*ctx*/) override { }

  virtual void enterBuiltinValue(AsnEtsiItsParser::BuiltinValueContext * /*ctx*/) override { }
  virtual void exitBuiltinValue(AsnEtsiItsParser::BuiltinValueContext * /*ctx*/) override { }

  virtual void enterObjectIdentifierValue(AsnEtsiItsParser::ObjectIdentifierValueContext * /*ctx*/) override { }
  virtual void exitObjectIdentifierValue(AsnEtsiItsParser::ObjectIdentifierValueContext * /*ctx*/) override { }

  virtual void enterObjIdComponentsList(AsnEtsiItsParser::ObjIdComponentsListContext * /*ctx*/) override { }
  virtual void exitObjIdComponentsList(AsnEtsiItsParser::ObjIdComponentsListContext * /*ctx*/) override { }

  virtual void enterObjIdComponents(AsnEtsiItsParser::ObjIdComponentsContext * /*ctx*/) override { }
  virtual void exitObjIdComponents(AsnEtsiItsParser::ObjIdComponentsContext * /*ctx*/) override { }

  virtual void enterIntegerValue(AsnEtsiItsParser::IntegerValueContext * /*ctx*/) override { }
  virtual void exitIntegerValue(AsnEtsiItsParser::IntegerValueContext * /*ctx*/) override { }

  virtual void enterChoiceValue(AsnEtsiItsParser::ChoiceValueContext * /*ctx*/) override { }
  virtual void exitChoiceValue(AsnEtsiItsParser::ChoiceValueContext * /*ctx*/) override { }

  virtual void enterEnumeratedValue(AsnEtsiItsParser::EnumeratedValueContext * /*ctx*/) override { }
  virtual void exitEnumeratedValue(AsnEtsiItsParser::EnumeratedValueContext * /*ctx*/) override { }

  virtual void enterSignedNumber(AsnEtsiItsParser::SignedNumberContext * /*ctx*/) override { }
  virtual void exitSignedNumber(AsnEtsiItsParser::SignedNumberContext * /*ctx*/) override { }

  virtual void enterChoiceType(AsnEtsiItsParser::ChoiceTypeContext * /*ctx*/) override { }
  virtual void exitChoiceType(AsnEtsiItsParser::ChoiceTypeContext * /*ctx*/) override { }

  virtual void enterAlternativeTypeLists(AsnEtsiItsParser::AlternativeTypeListsContext * /*ctx*/) override { }
  virtual void exitAlternativeTypeLists(AsnEtsiItsParser::AlternativeTypeListsContext * /*ctx*/) override { }

  virtual void enterExtensionAdditionAlternatives(AsnEtsiItsParser::ExtensionAdditionAlternativesContext * /*ctx*/) override { }
  virtual void exitExtensionAdditionAlternatives(AsnEtsiItsParser::ExtensionAdditionAlternativesContext * /*ctx*/) override { }

  virtual void enterExtensionAdditionAlternativesList(AsnEtsiItsParser::ExtensionAdditionAlternativesListContext * /*ctx*/) override { }
  virtual void exitExtensionAdditionAlternativesList(AsnEtsiItsParser::ExtensionAdditionAlternativesListContext * /*ctx*/) override { }

  virtual void enterExtensionAdditionAlternative(AsnEtsiItsParser::ExtensionAdditionAlternativeContext * /*ctx*/) override { }
  virtual void exitExtensionAdditionAlternative(AsnEtsiItsParser::ExtensionAdditionAlternativeContext * /*ctx*/) override { }

  virtual void enterExtensionAdditionAlternativesGroup(AsnEtsiItsParser::ExtensionAdditionAlternativesGroupContext * /*ctx*/) override { }
  virtual void exitExtensionAdditionAlternativesGroup(AsnEtsiItsParser::ExtensionAdditionAlternativesGroupContext * /*ctx*/) override { }

  virtual void enterRootAlternativeTypeList(AsnEtsiItsParser::RootAlternativeTypeListContext * /*ctx*/) override { }
  virtual void exitRootAlternativeTypeList(AsnEtsiItsParser::RootAlternativeTypeListContext * /*ctx*/) override { }

  virtual void enterAlternativeTypeList(AsnEtsiItsParser::AlternativeTypeListContext * /*ctx*/) override { }
  virtual void exitAlternativeTypeList(AsnEtsiItsParser::AlternativeTypeListContext * /*ctx*/) override { }

  virtual void enterNamedType(AsnEtsiItsParser::NamedTypeContext * /*ctx*/) override { }
  virtual void exitNamedType(AsnEtsiItsParser::NamedTypeContext * /*ctx*/) override { }

  virtual void enterEnumeratedType(AsnEtsiItsParser::EnumeratedTypeContext * /*ctx*/) override { }
  virtual void exitEnumeratedType(AsnEtsiItsParser::EnumeratedTypeContext * /*ctx*/) override { }

  virtual void enterEnumerations(AsnEtsiItsParser::EnumerationsContext * /*ctx*/) override { }
  virtual void exitEnumerations(AsnEtsiItsParser::EnumerationsContext * /*ctx*/) override { }

  virtual void enterRootEnumeration(AsnEtsiItsParser::RootEnumerationContext * /*ctx*/) override { }
  virtual void exitRootEnumeration(AsnEtsiItsParser::RootEnumerationContext * /*ctx*/) override { }

  virtual void enterEnumeration(AsnEtsiItsParser::EnumerationContext * /*ctx*/) override { }
  virtual void exitEnumeration(AsnEtsiItsParser::EnumerationContext * /*ctx*/) override { }

  virtual void enterEnumerationItem(AsnEtsiItsParser::EnumerationItemContext * /*ctx*/) override { }
  virtual void exitEnumerationItem(AsnEtsiItsParser::EnumerationItemContext * /*ctx*/) override { }

  virtual void enterNamedNumber(AsnEtsiItsParser::NamedNumberContext * /*ctx*/) override { }
  virtual void exitNamedNumber(AsnEtsiItsParser::NamedNumberContext * /*ctx*/) override { }

  virtual void enterDefinedValue(AsnEtsiItsParser::DefinedValueContext * /*ctx*/) override { }
  virtual void exitDefinedValue(AsnEtsiItsParser::DefinedValueContext * /*ctx*/) override { }

  virtual void enterParameterizedValue(AsnEtsiItsParser::ParameterizedValueContext * /*ctx*/) override { }
  virtual void exitParameterizedValue(AsnEtsiItsParser::ParameterizedValueContext * /*ctx*/) override { }

  virtual void enterSimpleDefinedValue(AsnEtsiItsParser::SimpleDefinedValueContext * /*ctx*/) override { }
  virtual void exitSimpleDefinedValue(AsnEtsiItsParser::SimpleDefinedValueContext * /*ctx*/) override { }

  virtual void enterActualParameterList(AsnEtsiItsParser::ActualParameterListContext * /*ctx*/) override { }
  virtual void exitActualParameterList(AsnEtsiItsParser::ActualParameterListContext * /*ctx*/) override { }

  virtual void enterActualParameter(AsnEtsiItsParser::ActualParameterContext * /*ctx*/) override { }
  virtual void exitActualParameter(AsnEtsiItsParser::ActualParameterContext * /*ctx*/) override { }

  virtual void enterExceptionSpec(AsnEtsiItsParser::ExceptionSpecContext * /*ctx*/) override { }
  virtual void exitExceptionSpec(AsnEtsiItsParser::ExceptionSpecContext * /*ctx*/) override { }

  virtual void enterExceptionIdentification(AsnEtsiItsParser::ExceptionIdentificationContext * /*ctx*/) override { }
  virtual void exitExceptionIdentification(AsnEtsiItsParser::ExceptionIdentificationContext * /*ctx*/) override { }

  virtual void enterAdditionalEnumeration(AsnEtsiItsParser::AdditionalEnumerationContext * /*ctx*/) override { }
  virtual void exitAdditionalEnumeration(AsnEtsiItsParser::AdditionalEnumerationContext * /*ctx*/) override { }

  virtual void enterIntegerType(AsnEtsiItsParser::IntegerTypeContext * /*ctx*/) override { }
  virtual void exitIntegerType(AsnEtsiItsParser::IntegerTypeContext * /*ctx*/) override { }

  virtual void enterNamedNumberList(AsnEtsiItsParser::NamedNumberListContext * /*ctx*/) override { }
  virtual void exitNamedNumberList(AsnEtsiItsParser::NamedNumberListContext * /*ctx*/) override { }

  virtual void enterObjectidentifiertype(AsnEtsiItsParser::ObjectidentifiertypeContext * /*ctx*/) override { }
  virtual void exitObjectidentifiertype(AsnEtsiItsParser::ObjectidentifiertypeContext * /*ctx*/) override { }

  virtual void enterComponentRelationConstraint(AsnEtsiItsParser::ComponentRelationConstraintContext * /*ctx*/) override { }
  virtual void exitComponentRelationConstraint(AsnEtsiItsParser::ComponentRelationConstraintContext * /*ctx*/) override { }

  virtual void enterAtNotation(AsnEtsiItsParser::AtNotationContext * /*ctx*/) override { }
  virtual void exitAtNotation(AsnEtsiItsParser::AtNotationContext * /*ctx*/) override { }

  virtual void enterLevel(AsnEtsiItsParser::LevelContext * /*ctx*/) override { }
  virtual void exitLevel(AsnEtsiItsParser::LevelContext * /*ctx*/) override { }

  virtual void enterComponentIdList(AsnEtsiItsParser::ComponentIdListContext * /*ctx*/) override { }
  virtual void exitComponentIdList(AsnEtsiItsParser::ComponentIdListContext * /*ctx*/) override { }

  virtual void enterOctetStringType(AsnEtsiItsParser::OctetStringTypeContext * /*ctx*/) override { }
  virtual void exitOctetStringType(AsnEtsiItsParser::OctetStringTypeContext * /*ctx*/) override { }

  virtual void enterBitStringType(AsnEtsiItsParser::BitStringTypeContext * /*ctx*/) override { }
  virtual void exitBitStringType(AsnEtsiItsParser::BitStringTypeContext * /*ctx*/) override { }

  virtual void enterNamedBitList(AsnEtsiItsParser::NamedBitListContext * /*ctx*/) override { }
  virtual void exitNamedBitList(AsnEtsiItsParser::NamedBitListContext * /*ctx*/) override { }

  virtual void enterNamedBit(AsnEtsiItsParser::NamedBitContext * /*ctx*/) override { }
  virtual void exitNamedBit(AsnEtsiItsParser::NamedBitContext * /*ctx*/) override { }

  virtual void enterBooleanValue(AsnEtsiItsParser::BooleanValueContext * /*ctx*/) override { }
  virtual void exitBooleanValue(AsnEtsiItsParser::BooleanValueContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

