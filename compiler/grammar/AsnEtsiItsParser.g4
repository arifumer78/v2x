/*
 [The "BSD licence"]
 Copyright (c) 2007-2008 Terence Parr
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
 3. The name of the author may not be used to endorse or promote products
    derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
author: Stefan Taranu
mail: stefan.taranu@gmail.com
Built with : java org.antlr.Tool ASN.g
antlr version: 3.1.1

The grammar is by far not complete. I have no experience in ANTLR, still
it was not so difficult to write this grammar.

In broad lines it is copied  from the ASN specification files (from the Annex):
X.680, X.681, X.682, X.683 and compiled it into one file. I removed some
of the predicates since it was too much ambiguity.

If you have some comments/improvements, send me an e-mail.
*/

// $antlr-format alignTrailingComments true, columnLimit 150, minEmptyLines 1, maxEmptyLinesToKeep 1, reflowComments false, useTab false
// $antlr-format allowShortRulesOnASingleLine false, allowShortBlocksOnASingleLine true, alignSemicolons hanging, alignColons hanging

/*
Forked from antlr/grammars-v4's asn_3gpp grammar (ASN_3gppParser.g4,
https://github.com/antlr/grammars-v4/tree/master/asn/asn_3gpp) to fix real
gaps found parsing actual ETSI ITS modules (CAM TS 103 900, CDD TS 102 894-2)
that the upstream grammar could not handle. Renamed on fork since this is no
longer the unmodified upstream grammar. Changes from upstream:
  - assignedIdentifier: was an empty stub; now parses the import OID and the
    optional "WITH SUCCESSORS"/"WITH DESCRIPTIONS" suffix.
  - moduleDefinition: reused the existing objIdComponentsList rule (which
    already supports bare NumberForm OID arcs) instead of a narrower
    NameAndNumberForm-only inline pattern.
  - componentPresence: added an optional value constraint before the
    PRESENT/ABSENT/OPTIONAL keyword, for "WITH COMPONENTS {..., field (val)}".
  - Fixed a token-fusion bug across 5 rules (optionalExtensionMarker,
    elementSetSpecs x2, componentPresenceLists, enumerations): the lexer's
    EXTENSTIONENDMARKER rule greedily fuses "," and "..." into one token when
    written without whitespace (",..."), which is normal ASN.1 style, so
    plain "COMMA ELLIPSIS" alternatives could never match. Each site now also
    accepts the fused EXTENSTIONENDMARKER token.
  - elements: enabled the previously-commented-out nested constraint
    grouping alternative (needed for unions of parenthesized constraints,
    e.g. "((WITH COMPONENTS {...}) | (WITH COMPONENTS {...}))"), and wired
    up objectSetElements (also previously commented out).
  - sequenceOfType: removed an erroneous extra pair of parentheses around
    the constraint/sizeConstraint that the sibling setOfType rule correctly
    did not require.
  - subtypeElements: added the "WITH COMPONENT Constraint" (singular) form,
    used to constrain every element of a SEQUENCE OF/SET OF.
  - builtinType: added taggedType, for explicit "[N] Type" tagging, which
    had no support at all.
  - literal (WITH SYNTAX custom syntax words): reserved-word tokens like BY
    are valid literal syntax words per X.681 but were rejected; added
    BY_LITERAL as an accepted literal.
  - object_/objectDefn: added inline Information Object literal support for
    the common "{Type WORD BY value}" idiom (e.g. "{Foo IDENTIFIED BY bar}"),
    not full generic X.681 WITH-SYNTAX-driven object parsing.
*/

parser grammar AsnEtsiItsParser;

options { tokenVocab = AsnEtsiItsLexer; }

modules
    : moduleDefinition+ EOF
    ;

moduleDefinition
    : IDENTIFIER (L_BRACE objIdComponentsList R_BRACE)? DEFINITIONS_LITERAL tagDefault extensionDefault ASSIGN_OP BEGIN_LITERAL
        moduleBody END_LITERAL
    ;

tagDefault
    : ((EXPLICIT_LITERAL | IMPLICIT_LITERAL | AUTOMATIC_LITERAL) TAGS_LITERAL)?
    ;

extensionDefault
    : (EXTENSIBILITY_LITERAL IMPLIED_LITERAL)?
    ;

moduleBody
    : (exports imports assignmentList)?
    ;

exports
    : (EXPORTS_LITERAL symbolsExported SEMI_COLON | EXPORTS_LITERAL ALL_LITERAL SEMI_COLON)?
    ;

symbolsExported
    : symbolList?
    ;

imports
    : (IMPORTS_LITERAL symbolsImported SEMI_COLON)?
    ;

symbolsImported
    : symbolsFromModuleList?
    ;

symbolsFromModuleList
    : symbolsFromModule symbolsFromModule*
    ;

symbolsFromModule
    : symbolList FROM_LITERAL globalModuleReference
    ;

globalModuleReference
    : IDENTIFIER assignedIdentifier
    ;

assignedIdentifier
    : (L_BRACE objIdComponentsList R_BRACE)? (WITH_LITERAL IDENTIFIER)?
    ;

symbolList
    : symbol (COMMA symbol)*
    ;

symbol
    : IDENTIFIER (L_BRACE R_BRACE)?
    ;

//parameterizedReference :
//  reference (L_BRACE  R_BRACE)?
//;

//reference :
// IDENTIFIER |
//  identifier
//;

assignmentList
    : assignment assignment*
    ;

assignment
    : IDENTIFIER (
        valueAssignment
        | typeAssignment
        | parameterizedAssignment
        | objectClassAssignment
    )
    ;

sequenceType
    : SEQUENCE_LITERAL L_BRACE (extensionAndException optionalExtensionMarker | componentTypeLists)? R_BRACE
    ;

extensionAndException
    : ELLIPSIS exceptionSpec?
    ;

optionalExtensionMarker
    : ((COMMA ELLIPSIS | EXTENSTIONENDMARKER))?
    ;

componentTypeLists
    : rootComponentTypeList (
        tag
        | COMMA tag? extensionAndException extensionAdditions (
            optionalExtensionMarker
            | EXTENSTIONENDMARKER COMMA rootComponentTypeList tag?
        )
    )?
    //  |  rootComponentTypeList  COMMA  extensionAndException  extensionAdditions    optionalExtensionMarker
    //  |  rootComponentTypeList  COMMA  extensionAndException  extensionAdditions     EXTENSTIONENDMARKER  COMMA  rootComponentTypeList
    | extensionAndException extensionAdditions (
        optionalExtensionMarker
        | EXTENSTIONENDMARKER COMMA rootComponentTypeList tag?
    )
    //  |  extensionAndException  extensionAdditions  optionalExtensionMarker
    ;

rootComponentTypeList
    : componentTypeList
    ;

componentTypeList
    : componentType (COMMA tag? componentType)*
    ;

componentType
    : namedType (OPTIONAL_LITERAL | DEFAULT_LITERAL value)?
    | COMPONENTS_LITERAL OF_LITERAL asnType
    ;

tag
    : needTag
    | condTag
    | INVALID_TAG
    ;

needTag
    : NEED_LITERAL IDENTIFIER
    ;

condTag
    : COND_LITERAL IDENTIFIER
    ;

extensionAdditions
    : (COMMA extensionAdditionList)?
    ;

extensionAdditionList
    : extensionAddition (COMMA extensionAddition)*
    ;

extensionAddition
    : componentType
    | extensionAdditionGroup
    ;

extensionAdditionGroup
    : DOUBLE_L_BRACKET versionNumber componentTypeList tag? DOUBLE_R_BRACKET
    ;

versionNumber
    : (NUMBER COLON)?
    ;

sequenceOfType
    : SEQUENCE_LITERAL (constraint | sizeConstraint)? OF_LITERAL (
        asnType
        | namedType
    )
    ;

sizeConstraint
    : SIZE_LITERAL constraint
    ;

parameterizedAssignment
    : parameterList ASSIGN_OP (asnType | value | valueSet)
    | definedObjectClass ASSIGN_OP (object_ | objectClass | objectSet)

    // parameterizedTypeAssignment
    //| parameterizedValueAssignment
    //| parameterizedValueSetTypeAssignment
    //| parameterizedObjectClassAssignment
    //| parameterizedObjectAssignment
    //| parameterizedObjectSetAssignment
    ;

parameterList
    : L_BRACE parameter (COMMA parameter)* R_BRACE
    ;

parameter
    : (paramGovernor COLON)? IDENTIFIER
    ;

paramGovernor
    : governor
    | IDENTIFIER
    ;

//dummyGovernor : dummyReference
//;

governor
    : asnType
    | definedObjectClass
    ;

objectClassAssignment
    : /*IDENTIFIER*/ ASSIGN_OP objectClass
    ;

objectClass
    : definedObjectClass
    | objectClassDefn /*| parameterizedObjectClass */
    ;

definedObjectClass
    : (IDENTIFIER DOT)? IDENTIFIER
    | TYPE_IDENTIFIER_LITERAL
    | ABSTRACT_SYNTAX_LITERAL
    ;

usefulObjectClassReference
    : TYPE_IDENTIFIER_LITERAL
    | ABSTRACT_SYNTAX_LITERAL
    ;

externalObjectClassReference
    : IDENTIFIER DOT IDENTIFIER
    ;

objectClassDefn
    : CLASS_LITERAL L_BRACE fieldSpec (COMMA fieldSpec)* R_BRACE withSyntaxSpec?
    ;

withSyntaxSpec
    : WITH_LITERAL SYNTAX_LITERAL syntaxList
    ;

syntaxList
    : L_BRACE tokenOrGroupSpec+ R_BRACE
    ;

tokenOrGroupSpec
    : requiredToken
    | optionalGroup
    ;

optionalGroup
    : L_BRACKET tokenOrGroupSpec+ R_BRACKET
    ;

requiredToken
    : literal
    | primitiveFieldName
    ;

literal
    : IDENTIFIER
    | COMMA
    | BY_LITERAL
    ;

primitiveFieldName
    : AMPERSAND IDENTIFIER
    ;

fieldSpec
    : AMPERSAND IDENTIFIER (
        typeOptionalitySpec?
        | asnType (valueSetOptionalitySpec? | UNIQUE_LITERAL? valueOptionalitySpec?)
        | fieldName (OPTIONAL_LITERAL | DEFAULT_LITERAL (valueSet | value))?
        | definedObjectClass (OPTIONAL_LITERAL | DEFAULT_LITERAL (objectSet | object_))?
    )

    //   typeFieldSpec
    //  | fixedTypeValueFieldSpec
    //  | variableTypeValueFieldSpec
    //  | fixedTypeValueSetFieldSpec
    //  | variableTypeValueSetFieldSpec
    //  | objectFieldSpec
    //  | objectSetFieldSpec
    ;

typeFieldSpec
    : AMPERSAND IDENTIFIER typeOptionalitySpec?
    ;

typeOptionalitySpec
    : OPTIONAL_LITERAL
    | DEFAULT_LITERAL asnType
    ;

fixedTypeValueFieldSpec
    : AMPERSAND IDENTIFIER asnType UNIQUE_LITERAL? valueOptionalitySpec?
    ;

valueOptionalitySpec
    : OPTIONAL_LITERAL
    | DEFAULT_LITERAL value
    ;

variableTypeValueFieldSpec
    : AMPERSAND IDENTIFIER fieldName valueOptionalitySpec?
    ;

fixedTypeValueSetFieldSpec
    : AMPERSAND IDENTIFIER asnType valueSetOptionalitySpec?
    ;

valueSetOptionalitySpec
    : OPTIONAL_LITERAL
    | DEFAULT_LITERAL valueSet
    ;

object_
    : definedObject
    | objectDefn /*| objectFromObject */
    | parameterizedObject
    ;

objectDefn
    : L_BRACE definedType IDENTIFIER BY_LITERAL value R_BRACE
    ;

parameterizedObject
    : definedObject actualParameterList
    ;

definedObject
    : IDENTIFIER DOT?
    ;

objectSet
    : L_BRACE objectSetSpec R_BRACE
    ;

objectSetSpec
    : rootElementSetSpec ((COMMA ELLIPSIS | EXTENSTIONENDMARKER) (COMMA additionalElementSetSpec)?)?
    | ELLIPSIS (COMMA additionalElementSetSpec)?
    ;

fieldName
    : AMPERSAND IDENTIFIER (AMPERSAND IDENTIFIER DOT)*
    ;

valueSet
    : L_BRACE elementSetSpecs R_BRACE
    ;

elementSetSpecs
    : rootElementSetSpec ((COMMA ELLIPSIS | EXTENSTIONENDMARKER) (COMMA additionalElementSetSpec)?)?
    ;

rootElementSetSpec
    : elementSetSpec
    ;

additionalElementSetSpec
    : elementSetSpec
    ;

elementSetSpec
    : unions
    | ALL_LITERAL exclusions
    ;

unions
    : intersections (unionMark intersections)*
    ;

exclusions
    : EXCEPT_LITERAL elements
    ;

intersections
    : intersectionElements (intersectionMark intersectionElements)*
    ;

unionMark
    : PIPE
    | UNION_LITERAL
    ;

intersectionMark
    : POWER
    | INTERSECTION_LITERAL
    ;

elements
    : subtypeElements
    | objectSetElements
    | constraint
    ;

objectSetElements
    : object_
    | definedObject /*| objectSetFromObjects | parameterizedObjectSet      */
    ;

intersectionElements
    : elements exclusions?
    ;

subtypeElements
    : (value | MIN_LITERAL) LESS_THAN? DOUBLE_DOT LESS_THAN? (value | MAX_LITERAL)
    | sizeConstraint
    | PATTERN_LITERAL value
    | WITH_LITERAL COMPONENT_LITERAL constraint
    | value
    ;

variableTypeValueSetFieldSpec
    : AMPERSAND IDENTIFIER fieldName valueSetOptionalitySpec?
    ;

objectFieldSpec
    : AMPERSAND IDENTIFIER definedObjectClass objectOptionalitySpec?
    ;

objectOptionalitySpec
    : OPTIONAL_LITERAL
    | DEFAULT_LITERAL object_
    ;

objectSetFieldSpec
    : AMPERSAND IDENTIFIER definedObjectClass objectSetOptionalitySpec?
    ;

objectSetOptionalitySpec
    : OPTIONAL_LITERAL
    | DEFAULT_LITERAL objectSet
    ;

typeAssignment
    : ASSIGN_OP asnType
    ;

valueAssignment
    : asnType ASSIGN_OP value
    ;

asnType
    : (builtinType | referencedType) constraint*
    ;

builtinType
    : octetStringType
    | bitStringType
    | choiceType
    | enumeratedType
    | integerType
    | sequenceType
    | sequenceOfType
    | setType
    | setOfType
    | objectidentifiertype
    | objectClassFieldType
    | taggedType
    | BOOLEAN_LITERAL
    | NULL_LITERAL
    ;

taggedType
    : classTag (IMPLICIT_LITERAL | EXPLICIT_LITERAL)? asnType
    ;

classTag
    : L_BRACKET (UNIVERSAL_LITERAL | APPLICATION_LITERAL | PRIVATE_LITERAL)? NUMBER R_BRACKET
    ;

objectClassFieldType
    : definedObjectClass DOT fieldName
    ;

setType
    : SET_LITERAL L_BRACE (extensionAndException optionalExtensionMarker | componentTypeLists)? R_BRACE
    ;

setOfType
    : SET_LITERAL (constraint | sizeConstraint)? OF_LITERAL (asnType | namedType)
    ;

referencedType
    : definedType
    // | selectionType
    // | typeFromObject
    // | valueSetFromObjects
    ;

definedType
    : IDENTIFIER (DOT IDENTIFIER)? actualParameterList?
    ;

constraint
    : L_PARAN constraintSpec exceptionSpec? R_PARAN
    //L_PARAN value DOT_DOT value R_PARAN
    ;

constraintSpec
    : generalConstraint
    | subtypeConstraint
    ;

userDefinedConstraint
    : CONSTRAINED_LITERAL BY_LITERAL L_BRACE userDefinedConstraintParameter (
        COMMA userDefinedConstraintParameter
    )* R_BRACE
    ;

generalConstraint
    : userDefinedConstraint
    | tableConstraint
    | contentsConstraint
    ;

userDefinedConstraintParameter
    : governor (COLON value | valueSet | object_ | objectSet)?
    ;

tableConstraint
    : /*simpleTableConstraint |*/ componentRelationConstraint
    ;

simpleTableConstraint
    : objectSet
    ;

contentsConstraint
    : CONTAINING_LITERAL asnType
    | ENCODED_LITERAL BY_LITERAL value
    | CONTAINING_LITERAL asnType ENCODED_LITERAL BY_LITERAL value
    | WITH_LITERAL COMPONENTS_LITERAL L_BRACE componentPresenceLists R_BRACE
    ;

componentPresenceLists
    : componentPresenceList? ((COMMA ELLIPSIS | EXTENSTIONENDMARKER) (COMMA componentPresenceList)?)?
    | ELLIPSIS (COMMA componentPresenceList)?
    ;

componentPresenceList
    : componentPresence (COMMA componentPresence)*
    ;

componentPresence
    : IDENTIFIER constraint? (ABSENT_LITERAL | PRESENT_LITERAL | OPTIONAL_LITERAL)?
    ;

subtypeConstraint
    : elementSetSpecs
    //((value | MIN_LITERAL) LESS_THAN? DOUBLE_DOT LESS_THAN?  (value | MAX_LITERAL) )
    //	| sizeConstraint
    //	| value
    ;

value
    : builtinValue
    ;

builtinValue
    : enumeratedValue
    | integerValue
    | choiceValue
    | objectIdentifierValue
    | booleanValue
    | CSTRING
    | BSTRING
    ;

objectIdentifierValue
    : L_BRACE /*(definedValue)?*/ objIdComponentsList R_BRACE
    ;

objIdComponentsList
    : objIdComponents objIdComponents*
    ;

objIdComponents
    : NUMBER
    | IDENTIFIER (L_PARAN (NUMBER | definedValue) R_PARAN)?
    | definedValue
    ;

integerValue
    : signedNumber
    | IDENTIFIER
    ;

choiceValue
    : IDENTIFIER COLON value
    ;

enumeratedValue
    : IDENTIFIER
    ;

signedNumber
    : MINUS? NUMBER
    ;

choiceType
    : CHOICE_LITERAL L_BRACE alternativeTypeLists R_BRACE
    ;

alternativeTypeLists
    : rootAlternativeTypeList (
        COMMA extensionAndException extensionAdditionAlternatives optionalExtensionMarker
    )?
    ;

extensionAdditionAlternatives
    : (COMMA extensionAdditionAlternativesList)?
    ;

extensionAdditionAlternativesList
    : extensionAdditionAlternative (COMMA extensionAdditionAlternative)*
    ;

extensionAdditionAlternative
    : extensionAdditionAlternativesGroup
    | namedType
    ;

extensionAdditionAlternativesGroup
    : DOUBLE_L_BRACKET versionNumber alternativeTypeList DOUBLE_R_BRACKET
    ;

rootAlternativeTypeList
    : alternativeTypeList
    ;

alternativeTypeList
    : namedType (COMMA namedType)*
    ;

namedType
    : IDENTIFIER asnType
    ;

enumeratedType
    : ENUMERATED_LITERAL L_BRACE enumerations R_BRACE
    ;

enumerations
    : rootEnumeration ((COMMA ELLIPSIS | EXTENSTIONENDMARKER) exceptionSpec? (COMMA additionalEnumeration)?)?
    ;

rootEnumeration
    : enumeration
    ;

enumeration
    : enumerationItem (COMMA enumerationItem)*
    ;

enumerationItem
    : IDENTIFIER
    | namedNumber
    | value
    ;

namedNumber
    : IDENTIFIER L_PARAN (signedNumber | definedValue) R_PARAN
    ;

definedValue
    :
    // externalValueReference
    //| valuereference
    parameterizedValue
    ;

parameterizedValue
    : simpleDefinedValue actualParameterList?
    ;

simpleDefinedValue
    : IDENTIFIER (DOT IDENTIFIER)?
    ;

actualParameterList
    : L_BRACE actualParameter (COMMA actualParameter)* R_BRACE
    ;

actualParameter
    : asnType
    | value /*| valueSet | definedObjectClass | object | objectSet*/
    ;

exceptionSpec
    : EXCLAM exceptionIdentification
    ;

exceptionIdentification
    : signedNumber
    | definedValue
    | asnType COLON value
    ;

additionalEnumeration
    : enumeration
    ;

integerType
    : INTEGER_LITERAL (L_BRACE namedNumberList R_BRACE)?
    ;

namedNumberList
    : namedNumber (COMMA namedNumber)*
    ;

objectidentifiertype
    : OBJECT_LITERAL IDENTIFIER_LITERAL
    ;

componentRelationConstraint
    : L_BRACE IDENTIFIER (DOT IDENTIFIER)? R_BRACE (L_BRACE atNotation (COMMA atNotation)* R_BRACE)?
    ;

atNotation
    : (A_ROND | A_ROND_DOT level) componentIdList
    ;

level
    : (DOT level)?
    ;

componentIdList
    : IDENTIFIER (DOT IDENTIFIER)* //?????
    ;

octetStringType
    : OCTET_LITERAL STRING_LITERAL
    ;

bitStringType
    : BIT_LITERAL STRING_LITERAL (L_BRACE namedBitList R_BRACE)?
    ;

namedBitList
    : namedBit (COMMA namedBit)*
    ;

namedBit
    : IDENTIFIER L_PARAN (NUMBER | definedValue) R_PARAN
    ;

booleanValue
    : TRUE_LITERAL
    | FALSE_LITERAL
    | TRUE_SMALL_LITERAL
    | FALSE_SMALL_LITERAL
    ;