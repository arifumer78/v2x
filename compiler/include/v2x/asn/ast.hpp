#pragma once

// Flattened AST for parsed ASN.1 modules, built from the ANTLR concrete parse
// tree by AstBuilder (compiler/src/ast_builder.h). Not the semantic IR: no
// cross-module import resolution, no AUTOMATIC TAGS computation, no
// encoding-rule-specific layout decisions. See
// docs/design/compiler-frontend-design.md §6.

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace v2x::asn {

struct Field;

enum class TypeKind {
    Boolean,
    Null,
    Integer,
    Enumerated,
    BitString,
    OctetString,
    Sequence,
    SequenceOf,
    Set,
    SetOf,
    Choice,
    ObjectIdentifier,
    Tagged,     // [N] Type — elementType holds the inner type
    Reference,  // a defined-type reference to another assignment
};

enum class TagDefault { Explicit, Implicit, Automatic, Unspecified };

// A constraint bound (one side of a range, or a single value).
struct Bound {
    enum class Kind {
        Literal,  // a literal number; raw holds the digits (incl. leading '-')
        Min,      // the MIN keyword; raw is empty
        Max,      // the MAX keyword; raw is empty
        Named,    // an identifier / defined-value reference, or any value form
                  // not resolved further (e.g. a BSTRING); raw holds its text
    };
    Kind kind = Kind::Named;
    std::string raw;
};

// One endpoint pair; a single-value constraint is modeled as lower == upper.
struct Range {
    Bound lower;
    Bound upper;
};

enum class ConstraintKind {
    SizeRange,   // SIZE(...) — maps to v2x::per::SizeRange for PER, or COER's
                 // octet-length decision; same structured data, different
                 // downstream packing per encoding rule.
    ValueRange,  // a plain value/range constraint (e.g. INTEGER bounds)
    Raw,         // anything not a plain range/size shape: WITH COMPONENTS,
                 // PATTERN, ALL EXCEPT, table/user-defined constraints, real
                 // unions/intersections of more than one term. Only rawText
                 // is populated.
};

struct Constraint {
    ConstraintKind kind = ConstraintKind::Raw;
    std::vector<Range> rootRanges;       // usually 1; 2 for "SIZE(1..16,...,17..40)"-style
    std::vector<Range> extensionRanges;  // only if extensible with an additional range given
    bool extensible = false;
    std::string rawText;  // verbatim source text (via getText()); always populated
};

enum class TagClass { Universal, Application, Private, ContextSpecific };
enum class TagMode { Implicit, Explicit, Unspecified };  // Unspecified = module's tagDefault applies

struct Tag {
    TagClass tagClass = TagClass::ContextSpecific;
    int number = 0;
    TagMode mode = TagMode::Unspecified;
};

struct EnumerationItem {
    std::string name;
    std::optional<std::string> rawValue;  // from a namedNumber, if present
};

struct NamedBit {
    std::string name;
    std::string rawValue;
};

struct TypeNode {
    TypeKind kind = TypeKind::Reference;

    std::optional<std::string> referencedName;  // Reference target name
    std::optional<Tag> tag;                     // Tagged only

    std::vector<Field> fields;  // Sequence/Set/Choice
    bool extensible = false;    // "..." present among fields/alternatives

    std::unique_ptr<TypeNode> elementType;  // SequenceOf/SetOf element type, or Tagged's inner type

    std::vector<EnumerationItem> enumItems;           // root
    std::vector<EnumerationItem> enumExtensionItems;  // after "..."
    bool enumExtensible = false;

    std::vector<NamedBit> namedBits;  // BitString { ... }

    std::vector<Constraint> constraints;  // applies to any TypeNode
};

struct Field {
    std::string name;
    TypeNode type;
    bool optional = false;
    std::optional<std::string> defaultValueRaw;
    bool isExtensionAddition = false;  // beyond the root "..." marker
};

struct TypeAssignment {
    std::string name;
    TypeNode type;
};

// A CLASS or parameterized-type assignment — the full X.681 Information
// Object System and parameterized types are out of scope (see design doc
// §8); these are recorded so the AST doesn't silently drop or crash on
// real constructs the grammar only partially supports (e.g. CAM's
// EXTENSION-CONTAINER-ID-AND-TYPE, DSRC's parameterized RegionalExtension).
struct UnsupportedAssignment {
    std::string name;
    std::string reason;
};

struct Import {
    std::vector<std::string> symbols;
    std::string fromModule;
};

struct Module {
    std::string name;
    TagDefault tagDefault = TagDefault::Unspecified;
    bool extensibilityImplied = false;

    std::vector<Import> imports;
    std::vector<TypeAssignment> typeAssignments;
    std::vector<UnsupportedAssignment> unsupportedAssignments;
};

}  // namespace v2x::asn
