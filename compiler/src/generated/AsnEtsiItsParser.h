
// Generated from AsnEtsiItsParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  AsnEtsiItsParser : public antlr4::Parser {
public:
  enum {
    NEED_LITERAL = 1, COND_LITERAL = 2, INVALID_TAG = 3, A_ROND = 4, STAR = 5, 
    ASSIGN_OP = 6, BOOLEAN_LITERAL = 7, TRUE_LITERAL = 8, FALSE_LITERAL = 9, 
    DOT = 10, DOUBLE_DOT = 11, ELLIPSIS = 12, APOSTROPHE = 13, AMPERSAND = 14, 
    LESS_THAN = 15, GREATER_THAN = 16, LESS_THAN_SLASH = 17, SLASH_GREATER_THAN = 18, 
    TRUE_SMALL_LITERAL = 19, FALSE_SMALL_LITERAL = 20, INTEGER_LITERAL = 21, 
    L_BRACE = 22, R_BRACE = 23, COMMA = 24, L_PARAN = 25, R_PARAN = 26, 
    MINUS = 27, ENUMERATED_LITERAL = 28, REAL_LITERAL = 29, PLUS_INFINITY_LITERAL = 30, 
    MINUS_INFINITY_LITERAL = 31, BIT_LITERAL = 32, STRING_LITERAL = 33, 
    CONTAINING_LITERAL = 34, OCTET_LITERAL = 35, NULL_LITERAL = 36, SEQUENCE_LITERAL = 37, 
    OPTIONAL_LITERAL = 38, DEFAULT_LITERAL = 39, COMPONENTS_LITERAL = 40, 
    OF_LITERAL = 41, SET_LITERAL = 42, EXCLAM = 43, ALL_LITERAL = 44, EXCEPT_LITERAL = 45, 
    POWER = 46, PIPE = 47, UNION_LITERAL = 48, INTERSECTION_LITERAL = 49, 
    INCLUDES_LITERAL = 50, MIN_LITERAL = 51, MAX_LITERAL = 52, SIZE_LITERAL = 53, 
    FROM_LITERAL = 54, WITH_LITERAL = 55, COMPONENT_LITERAL = 56, PRESENT_LITERAL = 57, 
    ABSENT_LITERAL = 58, PATTERN_LITERAL = 59, TYPE_IDENTIFIER_LITERAL = 60, 
    ABSTRACT_SYNTAX_LITERAL = 61, CLASS_LITERAL = 62, UNIQUE_LITERAL = 63, 
    SYNTAX_LITERAL = 64, L_BRACKET = 65, R_BRACKET = 66, INSTANCE_LITERAL = 67, 
    SEMI_COLON = 68, IMPORTS_LITERAL = 69, EXPORTS_LITERAL = 70, EXTENSIBILITY_LITERAL = 71, 
    IMPLIED_LITERAL = 72, EXPLICIT_LITERAL = 73, TAGS_LITERAL = 74, IMPLICIT_LITERAL = 75, 
    AUTOMATIC_LITERAL = 76, DEFINITIONS_LITERAL = 77, BEGIN_LITERAL = 78, 
    END_LITERAL = 79, DOUBLE_L_BRACKET = 80, DOUBLE_R_BRACKET = 81, COLON = 82, 
    CHOICE_LITERAL = 83, UNIVERSAL_LITERAL = 84, APPLICATION_LITERAL = 85, 
    PRIVATE_LITERAL = 86, EMBEDDED_LITERAL = 87, PDV_LITERAL = 88, EXTERNAL_LITERAL = 89, 
    OBJECT_LITERAL = 90, IDENTIFIER_LITERAL = 91, RELATIVE_OID_LITERAL = 92, 
    CHARACTER_LITERAL = 93, CONSTRAINED_LITERAL = 94, BY_LITERAL = 95, A_ROND_DOT = 96, 
    ENCODED_LITERAL = 97, UNRESTRICTEDCHARACTERSTRINGTYPE = 98, EXTENSTIONENDMARKER = 99, 
    NUMBER = 100, WS = 101, LINE_COMMENT = 102, BLOCK_COMMENT = 103, BSTRING = 104, 
    HSTRING = 105, CSTRING = 106, IDENTIFIER = 107
  };

  enum {
    RuleModules = 0, RuleModuleDefinition = 1, RuleTagDefault = 2, RuleExtensionDefault = 3, 
    RuleModuleBody = 4, RuleExports = 5, RuleSymbolsExported = 6, RuleImports = 7, 
    RuleSymbolsImported = 8, RuleSymbolsFromModuleList = 9, RuleSymbolsFromModule = 10, 
    RuleGlobalModuleReference = 11, RuleAssignedIdentifier = 12, RuleSymbolList = 13, 
    RuleSymbol = 14, RuleAssignmentList = 15, RuleAssignment = 16, RuleSequenceType = 17, 
    RuleExtensionAndException = 18, RuleOptionalExtensionMarker = 19, RuleComponentTypeLists = 20, 
    RuleRootComponentTypeList = 21, RuleComponentTypeList = 22, RuleComponentType = 23, 
    RuleTag = 24, RuleNeedTag = 25, RuleCondTag = 26, RuleExtensionAdditions = 27, 
    RuleExtensionAdditionList = 28, RuleExtensionAddition = 29, RuleExtensionAdditionGroup = 30, 
    RuleVersionNumber = 31, RuleSequenceOfType = 32, RuleSizeConstraint = 33, 
    RuleParameterizedAssignment = 34, RuleParameterList = 35, RuleParameter = 36, 
    RuleParamGovernor = 37, RuleGovernor = 38, RuleObjectClassAssignment = 39, 
    RuleObjectClass = 40, RuleDefinedObjectClass = 41, RuleUsefulObjectClassReference = 42, 
    RuleExternalObjectClassReference = 43, RuleObjectClassDefn = 44, RuleWithSyntaxSpec = 45, 
    RuleSyntaxList = 46, RuleTokenOrGroupSpec = 47, RuleOptionalGroup = 48, 
    RuleRequiredToken = 49, RuleLiteral = 50, RulePrimitiveFieldName = 51, 
    RuleFieldSpec = 52, RuleTypeFieldSpec = 53, RuleTypeOptionalitySpec = 54, 
    RuleFixedTypeValueFieldSpec = 55, RuleValueOptionalitySpec = 56, RuleVariableTypeValueFieldSpec = 57, 
    RuleFixedTypeValueSetFieldSpec = 58, RuleValueSetOptionalitySpec = 59, 
    RuleObject_ = 60, RuleObjectDefn = 61, RuleParameterizedObject = 62, 
    RuleDefinedObject = 63, RuleObjectSet = 64, RuleObjectSetSpec = 65, 
    RuleFieldName = 66, RuleValueSet = 67, RuleElementSetSpecs = 68, RuleRootElementSetSpec = 69, 
    RuleAdditionalElementSetSpec = 70, RuleElementSetSpec = 71, RuleUnions = 72, 
    RuleExclusions = 73, RuleIntersections = 74, RuleUnionMark = 75, RuleIntersectionMark = 76, 
    RuleElements = 77, RuleObjectSetElements = 78, RuleIntersectionElements = 79, 
    RuleSubtypeElements = 80, RuleVariableTypeValueSetFieldSpec = 81, RuleObjectFieldSpec = 82, 
    RuleObjectOptionalitySpec = 83, RuleObjectSetFieldSpec = 84, RuleObjectSetOptionalitySpec = 85, 
    RuleTypeAssignment = 86, RuleValueAssignment = 87, RuleAsnType = 88, 
    RuleBuiltinType = 89, RuleTaggedType = 90, RuleClassTag = 91, RuleObjectClassFieldType = 92, 
    RuleSetType = 93, RuleSetOfType = 94, RuleReferencedType = 95, RuleDefinedType = 96, 
    RuleConstraint = 97, RuleConstraintSpec = 98, RuleUserDefinedConstraint = 99, 
    RuleGeneralConstraint = 100, RuleUserDefinedConstraintParameter = 101, 
    RuleTableConstraint = 102, RuleSimpleTableConstraint = 103, RuleContentsConstraint = 104, 
    RuleComponentPresenceLists = 105, RuleComponentPresenceList = 106, RuleComponentPresence = 107, 
    RuleSubtypeConstraint = 108, RuleValue = 109, RuleBuiltinValue = 110, 
    RuleObjectIdentifierValue = 111, RuleObjIdComponentsList = 112, RuleObjIdComponents = 113, 
    RuleIntegerValue = 114, RuleChoiceValue = 115, RuleEnumeratedValue = 116, 
    RuleSignedNumber = 117, RuleChoiceType = 118, RuleAlternativeTypeLists = 119, 
    RuleExtensionAdditionAlternatives = 120, RuleExtensionAdditionAlternativesList = 121, 
    RuleExtensionAdditionAlternative = 122, RuleExtensionAdditionAlternativesGroup = 123, 
    RuleRootAlternativeTypeList = 124, RuleAlternativeTypeList = 125, RuleNamedType = 126, 
    RuleEnumeratedType = 127, RuleEnumerations = 128, RuleRootEnumeration = 129, 
    RuleEnumeration = 130, RuleEnumerationItem = 131, RuleNamedNumber = 132, 
    RuleDefinedValue = 133, RuleParameterizedValue = 134, RuleSimpleDefinedValue = 135, 
    RuleActualParameterList = 136, RuleActualParameter = 137, RuleExceptionSpec = 138, 
    RuleExceptionIdentification = 139, RuleAdditionalEnumeration = 140, 
    RuleIntegerType = 141, RuleNamedNumberList = 142, RuleObjectidentifiertype = 143, 
    RuleComponentRelationConstraint = 144, RuleAtNotation = 145, RuleLevel = 146, 
    RuleComponentIdList = 147, RuleOctetStringType = 148, RuleBitStringType = 149, 
    RuleNamedBitList = 150, RuleNamedBit = 151, RuleBooleanValue = 152
  };

  explicit AsnEtsiItsParser(antlr4::TokenStream *input);

  AsnEtsiItsParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~AsnEtsiItsParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ModulesContext;
  class ModuleDefinitionContext;
  class TagDefaultContext;
  class ExtensionDefaultContext;
  class ModuleBodyContext;
  class ExportsContext;
  class SymbolsExportedContext;
  class ImportsContext;
  class SymbolsImportedContext;
  class SymbolsFromModuleListContext;
  class SymbolsFromModuleContext;
  class GlobalModuleReferenceContext;
  class AssignedIdentifierContext;
  class SymbolListContext;
  class SymbolContext;
  class AssignmentListContext;
  class AssignmentContext;
  class SequenceTypeContext;
  class ExtensionAndExceptionContext;
  class OptionalExtensionMarkerContext;
  class ComponentTypeListsContext;
  class RootComponentTypeListContext;
  class ComponentTypeListContext;
  class ComponentTypeContext;
  class TagContext;
  class NeedTagContext;
  class CondTagContext;
  class ExtensionAdditionsContext;
  class ExtensionAdditionListContext;
  class ExtensionAdditionContext;
  class ExtensionAdditionGroupContext;
  class VersionNumberContext;
  class SequenceOfTypeContext;
  class SizeConstraintContext;
  class ParameterizedAssignmentContext;
  class ParameterListContext;
  class ParameterContext;
  class ParamGovernorContext;
  class GovernorContext;
  class ObjectClassAssignmentContext;
  class ObjectClassContext;
  class DefinedObjectClassContext;
  class UsefulObjectClassReferenceContext;
  class ExternalObjectClassReferenceContext;
  class ObjectClassDefnContext;
  class WithSyntaxSpecContext;
  class SyntaxListContext;
  class TokenOrGroupSpecContext;
  class OptionalGroupContext;
  class RequiredTokenContext;
  class LiteralContext;
  class PrimitiveFieldNameContext;
  class FieldSpecContext;
  class TypeFieldSpecContext;
  class TypeOptionalitySpecContext;
  class FixedTypeValueFieldSpecContext;
  class ValueOptionalitySpecContext;
  class VariableTypeValueFieldSpecContext;
  class FixedTypeValueSetFieldSpecContext;
  class ValueSetOptionalitySpecContext;
  class Object_Context;
  class ObjectDefnContext;
  class ParameterizedObjectContext;
  class DefinedObjectContext;
  class ObjectSetContext;
  class ObjectSetSpecContext;
  class FieldNameContext;
  class ValueSetContext;
  class ElementSetSpecsContext;
  class RootElementSetSpecContext;
  class AdditionalElementSetSpecContext;
  class ElementSetSpecContext;
  class UnionsContext;
  class ExclusionsContext;
  class IntersectionsContext;
  class UnionMarkContext;
  class IntersectionMarkContext;
  class ElementsContext;
  class ObjectSetElementsContext;
  class IntersectionElementsContext;
  class SubtypeElementsContext;
  class VariableTypeValueSetFieldSpecContext;
  class ObjectFieldSpecContext;
  class ObjectOptionalitySpecContext;
  class ObjectSetFieldSpecContext;
  class ObjectSetOptionalitySpecContext;
  class TypeAssignmentContext;
  class ValueAssignmentContext;
  class AsnTypeContext;
  class BuiltinTypeContext;
  class TaggedTypeContext;
  class ClassTagContext;
  class ObjectClassFieldTypeContext;
  class SetTypeContext;
  class SetOfTypeContext;
  class ReferencedTypeContext;
  class DefinedTypeContext;
  class ConstraintContext;
  class ConstraintSpecContext;
  class UserDefinedConstraintContext;
  class GeneralConstraintContext;
  class UserDefinedConstraintParameterContext;
  class TableConstraintContext;
  class SimpleTableConstraintContext;
  class ContentsConstraintContext;
  class ComponentPresenceListsContext;
  class ComponentPresenceListContext;
  class ComponentPresenceContext;
  class SubtypeConstraintContext;
  class ValueContext;
  class BuiltinValueContext;
  class ObjectIdentifierValueContext;
  class ObjIdComponentsListContext;
  class ObjIdComponentsContext;
  class IntegerValueContext;
  class ChoiceValueContext;
  class EnumeratedValueContext;
  class SignedNumberContext;
  class ChoiceTypeContext;
  class AlternativeTypeListsContext;
  class ExtensionAdditionAlternativesContext;
  class ExtensionAdditionAlternativesListContext;
  class ExtensionAdditionAlternativeContext;
  class ExtensionAdditionAlternativesGroupContext;
  class RootAlternativeTypeListContext;
  class AlternativeTypeListContext;
  class NamedTypeContext;
  class EnumeratedTypeContext;
  class EnumerationsContext;
  class RootEnumerationContext;
  class EnumerationContext;
  class EnumerationItemContext;
  class NamedNumberContext;
  class DefinedValueContext;
  class ParameterizedValueContext;
  class SimpleDefinedValueContext;
  class ActualParameterListContext;
  class ActualParameterContext;
  class ExceptionSpecContext;
  class ExceptionIdentificationContext;
  class AdditionalEnumerationContext;
  class IntegerTypeContext;
  class NamedNumberListContext;
  class ObjectidentifiertypeContext;
  class ComponentRelationConstraintContext;
  class AtNotationContext;
  class LevelContext;
  class ComponentIdListContext;
  class OctetStringTypeContext;
  class BitStringTypeContext;
  class NamedBitListContext;
  class NamedBitContext;
  class BooleanValueContext; 

  class  ModulesContext : public antlr4::ParserRuleContext {
  public:
    ModulesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<ModuleDefinitionContext *> moduleDefinition();
    ModuleDefinitionContext* moduleDefinition(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ModulesContext* modules();

  class  ModuleDefinitionContext : public antlr4::ParserRuleContext {
  public:
    ModuleDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *DEFINITIONS_LITERAL();
    TagDefaultContext *tagDefault();
    ExtensionDefaultContext *extensionDefault();
    antlr4::tree::TerminalNode *ASSIGN_OP();
    antlr4::tree::TerminalNode *BEGIN_LITERAL();
    ModuleBodyContext *moduleBody();
    antlr4::tree::TerminalNode *END_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    ObjIdComponentsListContext *objIdComponentsList();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ModuleDefinitionContext* moduleDefinition();

  class  TagDefaultContext : public antlr4::ParserRuleContext {
  public:
    TagDefaultContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TAGS_LITERAL();
    antlr4::tree::TerminalNode *EXPLICIT_LITERAL();
    antlr4::tree::TerminalNode *IMPLICIT_LITERAL();
    antlr4::tree::TerminalNode *AUTOMATIC_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TagDefaultContext* tagDefault();

  class  ExtensionDefaultContext : public antlr4::ParserRuleContext {
  public:
    ExtensionDefaultContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXTENSIBILITY_LITERAL();
    antlr4::tree::TerminalNode *IMPLIED_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionDefaultContext* extensionDefault();

  class  ModuleBodyContext : public antlr4::ParserRuleContext {
  public:
    ModuleBodyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExportsContext *exports();
    ImportsContext *imports();
    AssignmentListContext *assignmentList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ModuleBodyContext* moduleBody();

  class  ExportsContext : public antlr4::ParserRuleContext {
  public:
    ExportsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXPORTS_LITERAL();
    SymbolsExportedContext *symbolsExported();
    antlr4::tree::TerminalNode *SEMI_COLON();
    antlr4::tree::TerminalNode *ALL_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExportsContext* exports();

  class  SymbolsExportedContext : public antlr4::ParserRuleContext {
  public:
    SymbolsExportedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SymbolListContext *symbolList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolsExportedContext* symbolsExported();

  class  ImportsContext : public antlr4::ParserRuleContext {
  public:
    ImportsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IMPORTS_LITERAL();
    SymbolsImportedContext *symbolsImported();
    antlr4::tree::TerminalNode *SEMI_COLON();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ImportsContext* imports();

  class  SymbolsImportedContext : public antlr4::ParserRuleContext {
  public:
    SymbolsImportedContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SymbolsFromModuleListContext *symbolsFromModuleList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolsImportedContext* symbolsImported();

  class  SymbolsFromModuleListContext : public antlr4::ParserRuleContext {
  public:
    SymbolsFromModuleListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SymbolsFromModuleContext *> symbolsFromModule();
    SymbolsFromModuleContext* symbolsFromModule(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolsFromModuleListContext* symbolsFromModuleList();

  class  SymbolsFromModuleContext : public antlr4::ParserRuleContext {
  public:
    SymbolsFromModuleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SymbolListContext *symbolList();
    antlr4::tree::TerminalNode *FROM_LITERAL();
    GlobalModuleReferenceContext *globalModuleReference();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolsFromModuleContext* symbolsFromModule();

  class  GlobalModuleReferenceContext : public antlr4::ParserRuleContext {
  public:
    GlobalModuleReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    AssignedIdentifierContext *assignedIdentifier();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GlobalModuleReferenceContext* globalModuleReference();

  class  AssignedIdentifierContext : public antlr4::ParserRuleContext {
  public:
    AssignedIdentifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    ObjIdComponentsListContext *objIdComponentsList();
    antlr4::tree::TerminalNode *R_BRACE();
    antlr4::tree::TerminalNode *WITH_LITERAL();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignedIdentifierContext* assignedIdentifier();

  class  SymbolListContext : public antlr4::ParserRuleContext {
  public:
    SymbolListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<SymbolContext *> symbol();
    SymbolContext* symbol(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolListContext* symbolList();

  class  SymbolContext : public antlr4::ParserRuleContext {
  public:
    SymbolContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *L_BRACE();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SymbolContext* symbol();

  class  AssignmentListContext : public antlr4::ParserRuleContext {
  public:
    AssignmentListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AssignmentContext *> assignment();
    AssignmentContext* assignment(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentListContext* assignmentList();

  class  AssignmentContext : public antlr4::ParserRuleContext {
  public:
    AssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    ValueAssignmentContext *valueAssignment();
    TypeAssignmentContext *typeAssignment();
    ParameterizedAssignmentContext *parameterizedAssignment();
    ObjectClassAssignmentContext *objectClassAssignment();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentContext* assignment();

  class  SequenceTypeContext : public antlr4::ParserRuleContext {
  public:
    SequenceTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SEQUENCE_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    antlr4::tree::TerminalNode *R_BRACE();
    ExtensionAndExceptionContext *extensionAndException();
    OptionalExtensionMarkerContext *optionalExtensionMarker();
    ComponentTypeListsContext *componentTypeLists();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SequenceTypeContext* sequenceType();

  class  ExtensionAndExceptionContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAndExceptionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ELLIPSIS();
    ExceptionSpecContext *exceptionSpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAndExceptionContext* extensionAndException();

  class  OptionalExtensionMarkerContext : public antlr4::ParserRuleContext {
  public:
    OptionalExtensionMarkerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *ELLIPSIS();
    antlr4::tree::TerminalNode *EXTENSTIONENDMARKER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptionalExtensionMarkerContext* optionalExtensionMarker();

  class  ComponentTypeListsContext : public antlr4::ParserRuleContext {
  public:
    ComponentTypeListsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<RootComponentTypeListContext *> rootComponentTypeList();
    RootComponentTypeListContext* rootComponentTypeList(size_t i);
    std::vector<TagContext *> tag();
    TagContext* tag(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    ExtensionAndExceptionContext *extensionAndException();
    ExtensionAdditionsContext *extensionAdditions();
    OptionalExtensionMarkerContext *optionalExtensionMarker();
    antlr4::tree::TerminalNode *EXTENSTIONENDMARKER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentTypeListsContext* componentTypeLists();

  class  RootComponentTypeListContext : public antlr4::ParserRuleContext {
  public:
    RootComponentTypeListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComponentTypeListContext *componentTypeList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootComponentTypeListContext* rootComponentTypeList();

  class  ComponentTypeListContext : public antlr4::ParserRuleContext {
  public:
    ComponentTypeListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ComponentTypeContext *> componentType();
    ComponentTypeContext* componentType(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<TagContext *> tag();
    TagContext* tag(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentTypeListContext* componentTypeList();

  class  ComponentTypeContext : public antlr4::ParserRuleContext {
  public:
    ComponentTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NamedTypeContext *namedType();
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    ValueContext *value();
    antlr4::tree::TerminalNode *COMPONENTS_LITERAL();
    antlr4::tree::TerminalNode *OF_LITERAL();
    AsnTypeContext *asnType();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentTypeContext* componentType();

  class  TagContext : public antlr4::ParserRuleContext {
  public:
    TagContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NeedTagContext *needTag();
    CondTagContext *condTag();
    antlr4::tree::TerminalNode *INVALID_TAG();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TagContext* tag();

  class  NeedTagContext : public antlr4::ParserRuleContext {
  public:
    NeedTagContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NEED_LITERAL();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NeedTagContext* needTag();

  class  CondTagContext : public antlr4::ParserRuleContext {
  public:
    CondTagContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COND_LITERAL();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CondTagContext* condTag();

  class  ExtensionAdditionsContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMA();
    ExtensionAdditionListContext *extensionAdditionList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionsContext* extensionAdditions();

  class  ExtensionAdditionListContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExtensionAdditionContext *> extensionAddition();
    ExtensionAdditionContext* extensionAddition(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionListContext* extensionAdditionList();

  class  ExtensionAdditionContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComponentTypeContext *componentType();
    ExtensionAdditionGroupContext *extensionAdditionGroup();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionContext* extensionAddition();

  class  ExtensionAdditionGroupContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionGroupContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOUBLE_L_BRACKET();
    VersionNumberContext *versionNumber();
    ComponentTypeListContext *componentTypeList();
    antlr4::tree::TerminalNode *DOUBLE_R_BRACKET();
    TagContext *tag();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionGroupContext* extensionAdditionGroup();

  class  VersionNumberContext : public antlr4::ParserRuleContext {
  public:
    VersionNumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *COLON();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VersionNumberContext* versionNumber();

  class  SequenceOfTypeContext : public antlr4::ParserRuleContext {
  public:
    SequenceOfTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SEQUENCE_LITERAL();
    antlr4::tree::TerminalNode *OF_LITERAL();
    AsnTypeContext *asnType();
    NamedTypeContext *namedType();
    ConstraintContext *constraint();
    SizeConstraintContext *sizeConstraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SequenceOfTypeContext* sequenceOfType();

  class  SizeConstraintContext : public antlr4::ParserRuleContext {
  public:
    SizeConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SIZE_LITERAL();
    ConstraintContext *constraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SizeConstraintContext* sizeConstraint();

  class  ParameterizedAssignmentContext : public antlr4::ParserRuleContext {
  public:
    ParameterizedAssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParameterListContext *parameterList();
    antlr4::tree::TerminalNode *ASSIGN_OP();
    AsnTypeContext *asnType();
    ValueContext *value();
    ValueSetContext *valueSet();
    DefinedObjectClassContext *definedObjectClass();
    Object_Context *object_();
    ObjectClassContext *objectClass();
    ObjectSetContext *objectSet();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterizedAssignmentContext* parameterizedAssignment();

  class  ParameterListContext : public antlr4::ParserRuleContext {
  public:
    ParameterListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    std::vector<ParameterContext *> parameter();
    ParameterContext* parameter(size_t i);
    antlr4::tree::TerminalNode *R_BRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterListContext* parameterList();

  class  ParameterContext : public antlr4::ParserRuleContext {
  public:
    ParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    ParamGovernorContext *paramGovernor();
    antlr4::tree::TerminalNode *COLON();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterContext* parameter();

  class  ParamGovernorContext : public antlr4::ParserRuleContext {
  public:
    ParamGovernorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GovernorContext *governor();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParamGovernorContext* paramGovernor();

  class  GovernorContext : public antlr4::ParserRuleContext {
  public:
    GovernorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AsnTypeContext *asnType();
    DefinedObjectClassContext *definedObjectClass();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GovernorContext* governor();

  class  ObjectClassAssignmentContext : public antlr4::ParserRuleContext {
  public:
    ObjectClassAssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASSIGN_OP();
    ObjectClassContext *objectClass();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectClassAssignmentContext* objectClassAssignment();

  class  ObjectClassContext : public antlr4::ParserRuleContext {
  public:
    ObjectClassContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DefinedObjectClassContext *definedObjectClass();
    ObjectClassDefnContext *objectClassDefn();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectClassContext* objectClass();

  class  DefinedObjectClassContext : public antlr4::ParserRuleContext {
  public:
    DefinedObjectClassContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *TYPE_IDENTIFIER_LITERAL();
    antlr4::tree::TerminalNode *ABSTRACT_SYNTAX_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DefinedObjectClassContext* definedObjectClass();

  class  UsefulObjectClassReferenceContext : public antlr4::ParserRuleContext {
  public:
    UsefulObjectClassReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TYPE_IDENTIFIER_LITERAL();
    antlr4::tree::TerminalNode *ABSTRACT_SYNTAX_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UsefulObjectClassReferenceContext* usefulObjectClassReference();

  class  ExternalObjectClassReferenceContext : public antlr4::ParserRuleContext {
  public:
    ExternalObjectClassReferenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExternalObjectClassReferenceContext* externalObjectClassReference();

  class  ObjectClassDefnContext : public antlr4::ParserRuleContext {
  public:
    ObjectClassDefnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CLASS_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    std::vector<FieldSpecContext *> fieldSpec();
    FieldSpecContext* fieldSpec(size_t i);
    antlr4::tree::TerminalNode *R_BRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    WithSyntaxSpecContext *withSyntaxSpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectClassDefnContext* objectClassDefn();

  class  WithSyntaxSpecContext : public antlr4::ParserRuleContext {
  public:
    WithSyntaxSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *WITH_LITERAL();
    antlr4::tree::TerminalNode *SYNTAX_LITERAL();
    SyntaxListContext *syntaxList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  WithSyntaxSpecContext* withSyntaxSpec();

  class  SyntaxListContext : public antlr4::ParserRuleContext {
  public:
    SyntaxListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    antlr4::tree::TerminalNode *R_BRACE();
    std::vector<TokenOrGroupSpecContext *> tokenOrGroupSpec();
    TokenOrGroupSpecContext* tokenOrGroupSpec(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SyntaxListContext* syntaxList();

  class  TokenOrGroupSpecContext : public antlr4::ParserRuleContext {
  public:
    TokenOrGroupSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RequiredTokenContext *requiredToken();
    OptionalGroupContext *optionalGroup();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TokenOrGroupSpecContext* tokenOrGroupSpec();

  class  OptionalGroupContext : public antlr4::ParserRuleContext {
  public:
    OptionalGroupContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACKET();
    antlr4::tree::TerminalNode *R_BRACKET();
    std::vector<TokenOrGroupSpecContext *> tokenOrGroupSpec();
    TokenOrGroupSpecContext* tokenOrGroupSpec(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OptionalGroupContext* optionalGroup();

  class  RequiredTokenContext : public antlr4::ParserRuleContext {
  public:
    RequiredTokenContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LiteralContext *literal();
    PrimitiveFieldNameContext *primitiveFieldName();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RequiredTokenContext* requiredToken();

  class  LiteralContext : public antlr4::ParserRuleContext {
  public:
    LiteralContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *BY_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LiteralContext* literal();

  class  PrimitiveFieldNameContext : public antlr4::ParserRuleContext {
  public:
    PrimitiveFieldNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimitiveFieldNameContext* primitiveFieldName();

  class  FieldSpecContext : public antlr4::ParserRuleContext {
  public:
    FieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    AsnTypeContext *asnType();
    FieldNameContext *fieldName();
    DefinedObjectClassContext *definedObjectClass();
    TypeOptionalitySpecContext *typeOptionalitySpec();
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    ValueSetOptionalitySpecContext *valueSetOptionalitySpec();
    antlr4::tree::TerminalNode *UNIQUE_LITERAL();
    ValueOptionalitySpecContext *valueOptionalitySpec();
    ValueSetContext *valueSet();
    ValueContext *value();
    ObjectSetContext *objectSet();
    Object_Context *object_();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldSpecContext* fieldSpec();

  class  TypeFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    TypeFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    TypeOptionalitySpecContext *typeOptionalitySpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeFieldSpecContext* typeFieldSpec();

  class  TypeOptionalitySpecContext : public antlr4::ParserRuleContext {
  public:
    TypeOptionalitySpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    AsnTypeContext *asnType();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeOptionalitySpecContext* typeOptionalitySpec();

  class  FixedTypeValueFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    FixedTypeValueFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    AsnTypeContext *asnType();
    antlr4::tree::TerminalNode *UNIQUE_LITERAL();
    ValueOptionalitySpecContext *valueOptionalitySpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedTypeValueFieldSpecContext* fixedTypeValueFieldSpec();

  class  ValueOptionalitySpecContext : public antlr4::ParserRuleContext {
  public:
    ValueOptionalitySpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    ValueContext *value();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueOptionalitySpecContext* valueOptionalitySpec();

  class  VariableTypeValueFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    VariableTypeValueFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    FieldNameContext *fieldName();
    ValueOptionalitySpecContext *valueOptionalitySpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VariableTypeValueFieldSpecContext* variableTypeValueFieldSpec();

  class  FixedTypeValueSetFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    FixedTypeValueSetFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    AsnTypeContext *asnType();
    ValueSetOptionalitySpecContext *valueSetOptionalitySpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedTypeValueSetFieldSpecContext* fixedTypeValueSetFieldSpec();

  class  ValueSetOptionalitySpecContext : public antlr4::ParserRuleContext {
  public:
    ValueSetOptionalitySpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    ValueSetContext *valueSet();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueSetOptionalitySpecContext* valueSetOptionalitySpec();

  class  Object_Context : public antlr4::ParserRuleContext {
  public:
    Object_Context(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DefinedObjectContext *definedObject();
    ObjectDefnContext *objectDefn();
    ParameterizedObjectContext *parameterizedObject();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Object_Context* object_();

  class  ObjectDefnContext : public antlr4::ParserRuleContext {
  public:
    ObjectDefnContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    DefinedTypeContext *definedType();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *BY_LITERAL();
    ValueContext *value();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectDefnContext* objectDefn();

  class  ParameterizedObjectContext : public antlr4::ParserRuleContext {
  public:
    ParameterizedObjectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DefinedObjectContext *definedObject();
    ActualParameterListContext *actualParameterList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterizedObjectContext* parameterizedObject();

  class  DefinedObjectContext : public antlr4::ParserRuleContext {
  public:
    DefinedObjectContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DefinedObjectContext* definedObject();

  class  ObjectSetContext : public antlr4::ParserRuleContext {
  public:
    ObjectSetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    ObjectSetSpecContext *objectSetSpec();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectSetContext* objectSet();

  class  ObjectSetSpecContext : public antlr4::ParserRuleContext {
  public:
    ObjectSetSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RootElementSetSpecContext *rootElementSetSpec();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *ELLIPSIS();
    antlr4::tree::TerminalNode *EXTENSTIONENDMARKER();
    AdditionalElementSetSpecContext *additionalElementSetSpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectSetSpecContext* objectSetSpec();

  class  FieldNameContext : public antlr4::ParserRuleContext {
  public:
    FieldNameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> AMPERSAND();
    antlr4::tree::TerminalNode* AMPERSAND(size_t i);
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FieldNameContext* fieldName();

  class  ValueSetContext : public antlr4::ParserRuleContext {
  public:
    ValueSetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    ElementSetSpecsContext *elementSetSpecs();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueSetContext* valueSet();

  class  ElementSetSpecsContext : public antlr4::ParserRuleContext {
  public:
    ElementSetSpecsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RootElementSetSpecContext *rootElementSetSpec();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *ELLIPSIS();
    antlr4::tree::TerminalNode *EXTENSTIONENDMARKER();
    AdditionalElementSetSpecContext *additionalElementSetSpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementSetSpecsContext* elementSetSpecs();

  class  RootElementSetSpecContext : public antlr4::ParserRuleContext {
  public:
    RootElementSetSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementSetSpecContext *elementSetSpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootElementSetSpecContext* rootElementSetSpec();

  class  AdditionalElementSetSpecContext : public antlr4::ParserRuleContext {
  public:
    AdditionalElementSetSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementSetSpecContext *elementSetSpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AdditionalElementSetSpecContext* additionalElementSetSpec();

  class  ElementSetSpecContext : public antlr4::ParserRuleContext {
  public:
    ElementSetSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnionsContext *unions();
    antlr4::tree::TerminalNode *ALL_LITERAL();
    ExclusionsContext *exclusions();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementSetSpecContext* elementSetSpec();

  class  UnionsContext : public antlr4::ParserRuleContext {
  public:
    UnionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IntersectionsContext *> intersections();
    IntersectionsContext* intersections(size_t i);
    std::vector<UnionMarkContext *> unionMark();
    UnionMarkContext* unionMark(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnionsContext* unions();

  class  ExclusionsContext : public antlr4::ParserRuleContext {
  public:
    ExclusionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXCEPT_LITERAL();
    ElementsContext *elements();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExclusionsContext* exclusions();

  class  IntersectionsContext : public antlr4::ParserRuleContext {
  public:
    IntersectionsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<IntersectionElementsContext *> intersectionElements();
    IntersectionElementsContext* intersectionElements(size_t i);
    std::vector<IntersectionMarkContext *> intersectionMark();
    IntersectionMarkContext* intersectionMark(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntersectionsContext* intersections();

  class  UnionMarkContext : public antlr4::ParserRuleContext {
  public:
    UnionMarkContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PIPE();
    antlr4::tree::TerminalNode *UNION_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnionMarkContext* unionMark();

  class  IntersectionMarkContext : public antlr4::ParserRuleContext {
  public:
    IntersectionMarkContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *POWER();
    antlr4::tree::TerminalNode *INTERSECTION_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntersectionMarkContext* intersectionMark();

  class  ElementsContext : public antlr4::ParserRuleContext {
  public:
    ElementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SubtypeElementsContext *subtypeElements();
    ObjectSetElementsContext *objectSetElements();
    ConstraintContext *constraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElementsContext* elements();

  class  ObjectSetElementsContext : public antlr4::ParserRuleContext {
  public:
    ObjectSetElementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Object_Context *object_();
    DefinedObjectContext *definedObject();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectSetElementsContext* objectSetElements();

  class  IntersectionElementsContext : public antlr4::ParserRuleContext {
  public:
    IntersectionElementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementsContext *elements();
    ExclusionsContext *exclusions();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntersectionElementsContext* intersectionElements();

  class  SubtypeElementsContext : public antlr4::ParserRuleContext {
  public:
    SubtypeElementsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOUBLE_DOT();
    std::vector<ValueContext *> value();
    ValueContext* value(size_t i);
    antlr4::tree::TerminalNode *MIN_LITERAL();
    antlr4::tree::TerminalNode *MAX_LITERAL();
    std::vector<antlr4::tree::TerminalNode *> LESS_THAN();
    antlr4::tree::TerminalNode* LESS_THAN(size_t i);
    SizeConstraintContext *sizeConstraint();
    antlr4::tree::TerminalNode *PATTERN_LITERAL();
    antlr4::tree::TerminalNode *WITH_LITERAL();
    antlr4::tree::TerminalNode *COMPONENT_LITERAL();
    ConstraintContext *constraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SubtypeElementsContext* subtypeElements();

  class  VariableTypeValueSetFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    VariableTypeValueSetFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    FieldNameContext *fieldName();
    ValueSetOptionalitySpecContext *valueSetOptionalitySpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VariableTypeValueSetFieldSpecContext* variableTypeValueSetFieldSpec();

  class  ObjectFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    ObjectFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    DefinedObjectClassContext *definedObjectClass();
    ObjectOptionalitySpecContext *objectOptionalitySpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectFieldSpecContext* objectFieldSpec();

  class  ObjectOptionalitySpecContext : public antlr4::ParserRuleContext {
  public:
    ObjectOptionalitySpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    Object_Context *object_();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectOptionalitySpecContext* objectOptionalitySpec();

  class  ObjectSetFieldSpecContext : public antlr4::ParserRuleContext {
  public:
    ObjectSetFieldSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *AMPERSAND();
    antlr4::tree::TerminalNode *IDENTIFIER();
    DefinedObjectClassContext *definedObjectClass();
    ObjectSetOptionalitySpecContext *objectSetOptionalitySpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectSetFieldSpecContext* objectSetFieldSpec();

  class  ObjectSetOptionalitySpecContext : public antlr4::ParserRuleContext {
  public:
    ObjectSetOptionalitySpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();
    antlr4::tree::TerminalNode *DEFAULT_LITERAL();
    ObjectSetContext *objectSet();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectSetOptionalitySpecContext* objectSetOptionalitySpec();

  class  TypeAssignmentContext : public antlr4::ParserRuleContext {
  public:
    TypeAssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ASSIGN_OP();
    AsnTypeContext *asnType();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeAssignmentContext* typeAssignment();

  class  ValueAssignmentContext : public antlr4::ParserRuleContext {
  public:
    ValueAssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AsnTypeContext *asnType();
    antlr4::tree::TerminalNode *ASSIGN_OP();
    ValueContext *value();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueAssignmentContext* valueAssignment();

  class  AsnTypeContext : public antlr4::ParserRuleContext {
  public:
    AsnTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BuiltinTypeContext *builtinType();
    ReferencedTypeContext *referencedType();
    std::vector<ConstraintContext *> constraint();
    ConstraintContext* constraint(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AsnTypeContext* asnType();

  class  BuiltinTypeContext : public antlr4::ParserRuleContext {
  public:
    BuiltinTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OctetStringTypeContext *octetStringType();
    BitStringTypeContext *bitStringType();
    ChoiceTypeContext *choiceType();
    EnumeratedTypeContext *enumeratedType();
    IntegerTypeContext *integerType();
    SequenceTypeContext *sequenceType();
    SequenceOfTypeContext *sequenceOfType();
    SetTypeContext *setType();
    SetOfTypeContext *setOfType();
    ObjectidentifiertypeContext *objectidentifiertype();
    ObjectClassFieldTypeContext *objectClassFieldType();
    TaggedTypeContext *taggedType();
    antlr4::tree::TerminalNode *BOOLEAN_LITERAL();
    antlr4::tree::TerminalNode *NULL_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BuiltinTypeContext* builtinType();

  class  TaggedTypeContext : public antlr4::ParserRuleContext {
  public:
    TaggedTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ClassTagContext *classTag();
    AsnTypeContext *asnType();
    antlr4::tree::TerminalNode *IMPLICIT_LITERAL();
    antlr4::tree::TerminalNode *EXPLICIT_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TaggedTypeContext* taggedType();

  class  ClassTagContext : public antlr4::ParserRuleContext {
  public:
    ClassTagContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACKET();
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *R_BRACKET();
    antlr4::tree::TerminalNode *UNIVERSAL_LITERAL();
    antlr4::tree::TerminalNode *APPLICATION_LITERAL();
    antlr4::tree::TerminalNode *PRIVATE_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ClassTagContext* classTag();

  class  ObjectClassFieldTypeContext : public antlr4::ParserRuleContext {
  public:
    ObjectClassFieldTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DefinedObjectClassContext *definedObjectClass();
    antlr4::tree::TerminalNode *DOT();
    FieldNameContext *fieldName();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectClassFieldTypeContext* objectClassFieldType();

  class  SetTypeContext : public antlr4::ParserRuleContext {
  public:
    SetTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SET_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    antlr4::tree::TerminalNode *R_BRACE();
    ExtensionAndExceptionContext *extensionAndException();
    OptionalExtensionMarkerContext *optionalExtensionMarker();
    ComponentTypeListsContext *componentTypeLists();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetTypeContext* setType();

  class  SetOfTypeContext : public antlr4::ParserRuleContext {
  public:
    SetOfTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SET_LITERAL();
    antlr4::tree::TerminalNode *OF_LITERAL();
    AsnTypeContext *asnType();
    NamedTypeContext *namedType();
    ConstraintContext *constraint();
    SizeConstraintContext *sizeConstraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SetOfTypeContext* setOfType();

  class  ReferencedTypeContext : public antlr4::ParserRuleContext {
  public:
    ReferencedTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DefinedTypeContext *definedType();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ReferencedTypeContext* referencedType();

  class  DefinedTypeContext : public antlr4::ParserRuleContext {
  public:
    DefinedTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *DOT();
    ActualParameterListContext *actualParameterList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DefinedTypeContext* definedType();

  class  ConstraintContext : public antlr4::ParserRuleContext {
  public:
    ConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_PARAN();
    ConstraintSpecContext *constraintSpec();
    antlr4::tree::TerminalNode *R_PARAN();
    ExceptionSpecContext *exceptionSpec();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstraintContext* constraint();

  class  ConstraintSpecContext : public antlr4::ParserRuleContext {
  public:
    ConstraintSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GeneralConstraintContext *generalConstraint();
    SubtypeConstraintContext *subtypeConstraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstraintSpecContext* constraintSpec();

  class  UserDefinedConstraintContext : public antlr4::ParserRuleContext {
  public:
    UserDefinedConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CONSTRAINED_LITERAL();
    antlr4::tree::TerminalNode *BY_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    std::vector<UserDefinedConstraintParameterContext *> userDefinedConstraintParameter();
    UserDefinedConstraintParameterContext* userDefinedConstraintParameter(size_t i);
    antlr4::tree::TerminalNode *R_BRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UserDefinedConstraintContext* userDefinedConstraint();

  class  GeneralConstraintContext : public antlr4::ParserRuleContext {
  public:
    GeneralConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UserDefinedConstraintContext *userDefinedConstraint();
    TableConstraintContext *tableConstraint();
    ContentsConstraintContext *contentsConstraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GeneralConstraintContext* generalConstraint();

  class  UserDefinedConstraintParameterContext : public antlr4::ParserRuleContext {
  public:
    UserDefinedConstraintParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    GovernorContext *governor();
    antlr4::tree::TerminalNode *COLON();
    ValueContext *value();
    ValueSetContext *valueSet();
    Object_Context *object_();
    ObjectSetContext *objectSet();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UserDefinedConstraintParameterContext* userDefinedConstraintParameter();

  class  TableConstraintContext : public antlr4::ParserRuleContext {
  public:
    TableConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComponentRelationConstraintContext *componentRelationConstraint();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TableConstraintContext* tableConstraint();

  class  SimpleTableConstraintContext : public antlr4::ParserRuleContext {
  public:
    SimpleTableConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ObjectSetContext *objectSet();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleTableConstraintContext* simpleTableConstraint();

  class  ContentsConstraintContext : public antlr4::ParserRuleContext {
  public:
    ContentsConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CONTAINING_LITERAL();
    AsnTypeContext *asnType();
    antlr4::tree::TerminalNode *ENCODED_LITERAL();
    antlr4::tree::TerminalNode *BY_LITERAL();
    ValueContext *value();
    antlr4::tree::TerminalNode *WITH_LITERAL();
    antlr4::tree::TerminalNode *COMPONENTS_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    ComponentPresenceListsContext *componentPresenceLists();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ContentsConstraintContext* contentsConstraint();

  class  ComponentPresenceListsContext : public antlr4::ParserRuleContext {
  public:
    ComponentPresenceListsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ComponentPresenceListContext *> componentPresenceList();
    ComponentPresenceListContext* componentPresenceList(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *ELLIPSIS();
    antlr4::tree::TerminalNode *EXTENSTIONENDMARKER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentPresenceListsContext* componentPresenceLists();

  class  ComponentPresenceListContext : public antlr4::ParserRuleContext {
  public:
    ComponentPresenceListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ComponentPresenceContext *> componentPresence();
    ComponentPresenceContext* componentPresence(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentPresenceListContext* componentPresenceList();

  class  ComponentPresenceContext : public antlr4::ParserRuleContext {
  public:
    ComponentPresenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    ConstraintContext *constraint();
    antlr4::tree::TerminalNode *ABSENT_LITERAL();
    antlr4::tree::TerminalNode *PRESENT_LITERAL();
    antlr4::tree::TerminalNode *OPTIONAL_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentPresenceContext* componentPresence();

  class  SubtypeConstraintContext : public antlr4::ParserRuleContext {
  public:
    SubtypeConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ElementSetSpecsContext *elementSetSpecs();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SubtypeConstraintContext* subtypeConstraint();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BuiltinValueContext *builtinValue();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueContext* value();

  class  BuiltinValueContext : public antlr4::ParserRuleContext {
  public:
    BuiltinValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EnumeratedValueContext *enumeratedValue();
    IntegerValueContext *integerValue();
    ChoiceValueContext *choiceValue();
    ObjectIdentifierValueContext *objectIdentifierValue();
    BooleanValueContext *booleanValue();
    antlr4::tree::TerminalNode *CSTRING();
    antlr4::tree::TerminalNode *BSTRING();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BuiltinValueContext* builtinValue();

  class  ObjectIdentifierValueContext : public antlr4::ParserRuleContext {
  public:
    ObjectIdentifierValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    ObjIdComponentsListContext *objIdComponentsList();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectIdentifierValueContext* objectIdentifierValue();

  class  ObjIdComponentsListContext : public antlr4::ParserRuleContext {
  public:
    ObjIdComponentsListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ObjIdComponentsContext *> objIdComponents();
    ObjIdComponentsContext* objIdComponents(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjIdComponentsListContext* objIdComponentsList();

  class  ObjIdComponentsContext : public antlr4::ParserRuleContext {
  public:
    ObjIdComponentsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *L_PARAN();
    antlr4::tree::TerminalNode *R_PARAN();
    DefinedValueContext *definedValue();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjIdComponentsContext* objIdComponents();

  class  IntegerValueContext : public antlr4::ParserRuleContext {
  public:
    IntegerValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SignedNumberContext *signedNumber();
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerValueContext* integerValue();

  class  ChoiceValueContext : public antlr4::ParserRuleContext {
  public:
    ChoiceValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *COLON();
    ValueContext *value();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ChoiceValueContext* choiceValue();

  class  EnumeratedValueContext : public antlr4::ParserRuleContext {
  public:
    EnumeratedValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumeratedValueContext* enumeratedValue();

  class  SignedNumberContext : public antlr4::ParserRuleContext {
  public:
    SignedNumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *MINUS();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SignedNumberContext* signedNumber();

  class  ChoiceTypeContext : public antlr4::ParserRuleContext {
  public:
    ChoiceTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CHOICE_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    AlternativeTypeListsContext *alternativeTypeLists();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ChoiceTypeContext* choiceType();

  class  AlternativeTypeListsContext : public antlr4::ParserRuleContext {
  public:
    AlternativeTypeListsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RootAlternativeTypeListContext *rootAlternativeTypeList();
    antlr4::tree::TerminalNode *COMMA();
    ExtensionAndExceptionContext *extensionAndException();
    ExtensionAdditionAlternativesContext *extensionAdditionAlternatives();
    OptionalExtensionMarkerContext *optionalExtensionMarker();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AlternativeTypeListsContext* alternativeTypeLists();

  class  ExtensionAdditionAlternativesContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionAlternativesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMA();
    ExtensionAdditionAlternativesListContext *extensionAdditionAlternativesList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionAlternativesContext* extensionAdditionAlternatives();

  class  ExtensionAdditionAlternativesListContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionAlternativesListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExtensionAdditionAlternativeContext *> extensionAdditionAlternative();
    ExtensionAdditionAlternativeContext* extensionAdditionAlternative(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionAlternativesListContext* extensionAdditionAlternativesList();

  class  ExtensionAdditionAlternativeContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionAlternativeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExtensionAdditionAlternativesGroupContext *extensionAdditionAlternativesGroup();
    NamedTypeContext *namedType();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionAlternativeContext* extensionAdditionAlternative();

  class  ExtensionAdditionAlternativesGroupContext : public antlr4::ParserRuleContext {
  public:
    ExtensionAdditionAlternativesGroupContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOUBLE_L_BRACKET();
    VersionNumberContext *versionNumber();
    AlternativeTypeListContext *alternativeTypeList();
    antlr4::tree::TerminalNode *DOUBLE_R_BRACKET();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExtensionAdditionAlternativesGroupContext* extensionAdditionAlternativesGroup();

  class  RootAlternativeTypeListContext : public antlr4::ParserRuleContext {
  public:
    RootAlternativeTypeListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AlternativeTypeListContext *alternativeTypeList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootAlternativeTypeListContext* rootAlternativeTypeList();

  class  AlternativeTypeListContext : public antlr4::ParserRuleContext {
  public:
    AlternativeTypeListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<NamedTypeContext *> namedType();
    NamedTypeContext* namedType(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AlternativeTypeListContext* alternativeTypeList();

  class  NamedTypeContext : public antlr4::ParserRuleContext {
  public:
    NamedTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    AsnTypeContext *asnType();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamedTypeContext* namedType();

  class  EnumeratedTypeContext : public antlr4::ParserRuleContext {
  public:
    EnumeratedTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ENUMERATED_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    EnumerationsContext *enumerations();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumeratedTypeContext* enumeratedType();

  class  EnumerationsContext : public antlr4::ParserRuleContext {
  public:
    EnumerationsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    RootEnumerationContext *rootEnumeration();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    antlr4::tree::TerminalNode *ELLIPSIS();
    antlr4::tree::TerminalNode *EXTENSTIONENDMARKER();
    ExceptionSpecContext *exceptionSpec();
    AdditionalEnumerationContext *additionalEnumeration();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumerationsContext* enumerations();

  class  RootEnumerationContext : public antlr4::ParserRuleContext {
  public:
    RootEnumerationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EnumerationContext *enumeration();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RootEnumerationContext* rootEnumeration();

  class  EnumerationContext : public antlr4::ParserRuleContext {
  public:
    EnumerationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<EnumerationItemContext *> enumerationItem();
    EnumerationItemContext* enumerationItem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumerationContext* enumeration();

  class  EnumerationItemContext : public antlr4::ParserRuleContext {
  public:
    EnumerationItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    NamedNumberContext *namedNumber();
    ValueContext *value();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumerationItemContext* enumerationItem();

  class  NamedNumberContext : public antlr4::ParserRuleContext {
  public:
    NamedNumberContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *L_PARAN();
    antlr4::tree::TerminalNode *R_PARAN();
    SignedNumberContext *signedNumber();
    DefinedValueContext *definedValue();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamedNumberContext* namedNumber();

  class  DefinedValueContext : public antlr4::ParserRuleContext {
  public:
    DefinedValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParameterizedValueContext *parameterizedValue();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DefinedValueContext* definedValue();

  class  ParameterizedValueContext : public antlr4::ParserRuleContext {
  public:
    ParameterizedValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SimpleDefinedValueContext *simpleDefinedValue();
    ActualParameterListContext *actualParameterList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterizedValueContext* parameterizedValue();

  class  SimpleDefinedValueContext : public antlr4::ParserRuleContext {
  public:
    SimpleDefinedValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    antlr4::tree::TerminalNode *DOT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SimpleDefinedValueContext* simpleDefinedValue();

  class  ActualParameterListContext : public antlr4::ParserRuleContext {
  public:
    ActualParameterListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *L_BRACE();
    std::vector<ActualParameterContext *> actualParameter();
    ActualParameterContext* actualParameter(size_t i);
    antlr4::tree::TerminalNode *R_BRACE();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ActualParameterListContext* actualParameterList();

  class  ActualParameterContext : public antlr4::ParserRuleContext {
  public:
    ActualParameterContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AsnTypeContext *asnType();
    ValueContext *value();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ActualParameterContext* actualParameter();

  class  ExceptionSpecContext : public antlr4::ParserRuleContext {
  public:
    ExceptionSpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXCLAM();
    ExceptionIdentificationContext *exceptionIdentification();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExceptionSpecContext* exceptionSpec();

  class  ExceptionIdentificationContext : public antlr4::ParserRuleContext {
  public:
    ExceptionIdentificationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    SignedNumberContext *signedNumber();
    DefinedValueContext *definedValue();
    AsnTypeContext *asnType();
    antlr4::tree::TerminalNode *COLON();
    ValueContext *value();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExceptionIdentificationContext* exceptionIdentification();

  class  AdditionalEnumerationContext : public antlr4::ParserRuleContext {
  public:
    AdditionalEnumerationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    EnumerationContext *enumeration();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AdditionalEnumerationContext* additionalEnumeration();

  class  IntegerTypeContext : public antlr4::ParserRuleContext {
  public:
    IntegerTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *INTEGER_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    NamedNumberListContext *namedNumberList();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntegerTypeContext* integerType();

  class  NamedNumberListContext : public antlr4::ParserRuleContext {
  public:
    NamedNumberListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<NamedNumberContext *> namedNumber();
    NamedNumberContext* namedNumber(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamedNumberListContext* namedNumberList();

  class  ObjectidentifiertypeContext : public antlr4::ParserRuleContext {
  public:
    ObjectidentifiertypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OBJECT_LITERAL();
    antlr4::tree::TerminalNode *IDENTIFIER_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ObjectidentifiertypeContext* objectidentifiertype();

  class  ComponentRelationConstraintContext : public antlr4::ParserRuleContext {
  public:
    ComponentRelationConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> L_BRACE();
    antlr4::tree::TerminalNode* L_BRACE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> R_BRACE();
    antlr4::tree::TerminalNode* R_BRACE(size_t i);
    antlr4::tree::TerminalNode *DOT();
    std::vector<AtNotationContext *> atNotation();
    AtNotationContext* atNotation(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentRelationConstraintContext* componentRelationConstraint();

  class  AtNotationContext : public antlr4::ParserRuleContext {
  public:
    AtNotationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ComponentIdListContext *componentIdList();
    antlr4::tree::TerminalNode *A_ROND();
    antlr4::tree::TerminalNode *A_ROND_DOT();
    LevelContext *level();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AtNotationContext* atNotation();

  class  LevelContext : public antlr4::ParserRuleContext {
  public:
    LevelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    LevelContext *level();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LevelContext* level();

  class  ComponentIdListContext : public antlr4::ParserRuleContext {
  public:
    ComponentIdListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> IDENTIFIER();
    antlr4::tree::TerminalNode* IDENTIFIER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> DOT();
    antlr4::tree::TerminalNode* DOT(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ComponentIdListContext* componentIdList();

  class  OctetStringTypeContext : public antlr4::ParserRuleContext {
  public:
    OctetStringTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *OCTET_LITERAL();
    antlr4::tree::TerminalNode *STRING_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OctetStringTypeContext* octetStringType();

  class  BitStringTypeContext : public antlr4::ParserRuleContext {
  public:
    BitStringTypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *BIT_LITERAL();
    antlr4::tree::TerminalNode *STRING_LITERAL();
    antlr4::tree::TerminalNode *L_BRACE();
    NamedBitListContext *namedBitList();
    antlr4::tree::TerminalNode *R_BRACE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BitStringTypeContext* bitStringType();

  class  NamedBitListContext : public antlr4::ParserRuleContext {
  public:
    NamedBitListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<NamedBitContext *> namedBit();
    NamedBitContext* namedBit(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamedBitListContext* namedBitList();

  class  NamedBitContext : public antlr4::ParserRuleContext {
  public:
    NamedBitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IDENTIFIER();
    antlr4::tree::TerminalNode *L_PARAN();
    antlr4::tree::TerminalNode *R_PARAN();
    antlr4::tree::TerminalNode *NUMBER();
    DefinedValueContext *definedValue();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NamedBitContext* namedBit();

  class  BooleanValueContext : public antlr4::ParserRuleContext {
  public:
    BooleanValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TRUE_LITERAL();
    antlr4::tree::TerminalNode *FALSE_LITERAL();
    antlr4::tree::TerminalNode *TRUE_SMALL_LITERAL();
    antlr4::tree::TerminalNode *FALSE_SMALL_LITERAL();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BooleanValueContext* booleanValue();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

