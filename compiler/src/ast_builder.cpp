#include "ast_builder.h"

#include <memory>
#include <string>

#include "AsnEtsiItsParser.h"

using v2x::asn::Bound;
using v2x::asn::Constraint;
using v2x::asn::ConstraintKind;
using v2x::asn::EnumerationItem;
using v2x::asn::Field;
using v2x::asn::Import;
using v2x::asn::Module;
using v2x::asn::NamedBit;
using v2x::asn::Range;
using v2x::asn::Tag;
using v2x::asn::TagClass;
using v2x::asn::TagDefault;
using v2x::asn::TagMode;
using v2x::asn::TypeAssignment;
using v2x::asn::TypeKind;
using v2x::asn::TypeNode;

namespace {

// --- Structured constraint extraction -------------------------------------
//
// Walks the real chain the design doc flagged as deepest in the CST
// (constraint -> constraintSpec -> subtypeConstraint -> elementSetSpecs ->
// rootElementSetSpec -> elementSetSpec -> unions -> intersections ->
// intersectionElements -> elements -> subtypeElements) and recognizes plain
// range/size shapes structurally. Anything else (real unions/intersections
// of more than one term, ALL EXCEPT, WITH COMPONENT(S), PATTERN) bails out
// (returns nullopt) and the caller (buildConstraint) falls back to raw text.

struct ExtractedRanges {
    ConstraintKind kind = ConstraintKind::ValueRange;
    std::vector<Range> rootRanges;
    std::vector<Range> extensionRanges;
    bool extensible = false;
};

std::optional<ExtractedRanges> tryExtractElementSetSpecs(AsnEtsiItsParser::ElementSetSpecsContext *ess);

Bound boundFromValue(AsnEtsiItsParser::ValueContext *v) {
    if (v) {
        if (auto *bv = v->builtinValue()) {
            if (auto *iv = bv->integerValue()) {
                if (auto *sn = iv->signedNumber()) {
                    std::string raw;
                    if (sn->MINUS() != nullptr) {
                        raw += "-";
                    }
                    if (sn->NUMBER() != nullptr) {
                        raw += sn->NUMBER()->getText();
                    }
                    return Bound{Bound::Kind::Literal, raw};
                }
                if (iv->IDENTIFIER() != nullptr) {
                    return Bound{Bound::Kind::Named, iv->IDENTIFIER()->getText()};
                }
            }
        }
        return Bound{Bound::Kind::Named, v->getText()};
    }
    return Bound{Bound::Kind::Named, std::string{}};
}

std::optional<ExtractedRanges> trySubtypeElements(AsnEtsiItsParser::SubtypeElementsContext *ste) {
    if (ste == nullptr) {
        return std::nullopt;
    }

    // sizeConstraint: recurse into its own inner constraint (which may itself be
    // extensible, e.g. "SIZE(1..16,...,17..40)") and force the result to SizeRange.
    if (auto *sc = ste->sizeConstraint()) {
        auto *inner = sc->constraint();
        auto *spec = inner != nullptr ? inner->constraintSpec() : nullptr;
        auto *sub = spec != nullptr ? spec->subtypeConstraint() : nullptr;
        if (sub == nullptr) {
            return std::nullopt;
        }
        auto extracted = tryExtractElementSetSpecs(sub->elementSetSpecs());
        if (!extracted) {
            return std::nullopt;
        }
        extracted->kind = ConstraintKind::SizeRange;
        return extracted;
    }

    if (ste->PATTERN_LITERAL() != nullptr) {
        return std::nullopt;
    }
    if (ste->WITH_LITERAL() != nullptr && ste->COMPONENT_LITERAL() != nullptr) {
        return std::nullopt;
    }

    if (ste->DOUBLE_DOT() != nullptr) {
        auto values = ste->value();
        size_t idx = 0;

        Bound lower;
        if (ste->MIN_LITERAL() != nullptr) {
            lower = Bound{Bound::Kind::Min, ""};
        } else if (idx < values.size()) {
            lower = boundFromValue(values[idx++]);
        } else {
            return std::nullopt;
        }

        Bound upper;
        if (ste->MAX_LITERAL() != nullptr) {
            upper = Bound{Bound::Kind::Max, ""};
        } else if (idx < values.size()) {
            upper = boundFromValue(values[idx++]);
        } else {
            return std::nullopt;
        }

        ExtractedRanges result;
        result.kind = ConstraintKind::ValueRange;
        result.rootRanges.push_back(Range{lower, upper});
        return result;
    }

    // A lone value used as a single-point constraint.
    auto values = ste->value();
    if (!values.empty()) {
        Bound b = boundFromValue(values[0]);
        ExtractedRanges result;
        result.kind = ConstraintKind::ValueRange;
        result.rootRanges.push_back(Range{b, b});
        return result;
    }

    return std::nullopt;
}

std::optional<ExtractedRanges> tryExtractElementSetSpec(AsnEtsiItsParser::ElementSetSpecContext *spec) {
    if (spec == nullptr) {
        return std::nullopt;
    }
    if (spec->ALL_LITERAL() != nullptr) {
        return std::nullopt;  // "ALL EXCEPT ..."
    }

    auto *unions = spec->unions();
    if (unions == nullptr) {
        return std::nullopt;
    }
    auto intersections = unions->intersections();
    if (intersections.size() != 1) {
        return std::nullopt;  // a real union of more than one term
    }

    auto intersectionElements = intersections[0]->intersectionElements();
    if (intersectionElements.size() != 1) {
        return std::nullopt;  // a real intersection
    }

    auto *ie = intersectionElements[0];
    if (ie->exclusions() != nullptr) {
        return std::nullopt;  // "... EXCEPT ..."
    }

    auto *elems = ie->elements();
    if (elems == nullptr) {
        return std::nullopt;
    }
    if (elems->objectSetElements() != nullptr || elems->constraint() != nullptr) {
        return std::nullopt;
    }

    return trySubtypeElements(elems->subtypeElements());
}

std::optional<ExtractedRanges> tryExtractElementSetSpecs(AsnEtsiItsParser::ElementSetSpecsContext *ess) {
    if (ess == nullptr) {
        return std::nullopt;
    }
    auto *root = ess->rootElementSetSpec();
    if (root == nullptr) {
        return std::nullopt;
    }
    auto extracted = tryExtractElementSetSpec(root->elementSetSpec());
    if (!extracted) {
        return std::nullopt;
    }

    if (ess->ELLIPSIS() != nullptr || ess->EXTENSTIONENDMARKER() != nullptr) {
        extracted->extensible = true;
    }
    if (auto *add = ess->additionalElementSetSpec()) {
        auto addExtracted = tryExtractElementSetSpec(add->elementSetSpec());
        if (!addExtracted || addExtracted->kind != extracted->kind) {
            return std::nullopt;
        }
        extracted->extensionRanges = std::move(addExtracted->rootRanges);
        extracted->extensible = true;
    }
    return extracted;
}

EnumerationItem buildEnumerationItem(AsnEtsiItsParser::EnumerationItemContext *ctx) {
    EnumerationItem item;
    if (ctx->IDENTIFIER() != nullptr) {
        item.name = ctx->IDENTIFIER()->getText();
    } else if (auto *nn = ctx->namedNumber()) {
        item.name = nn->IDENTIFIER() != nullptr ? nn->IDENTIFIER()->getText() : std::string{};
        if (auto *sn = nn->signedNumber()) {
            std::string raw;
            if (sn->MINUS() != nullptr) {
                raw += "-";
            }
            if (sn->NUMBER() != nullptr) {
                raw += sn->NUMBER()->getText();
            }
            item.rawValue = raw;
        } else if (nn->definedValue() != nullptr) {
            item.rawValue = nn->definedValue()->getText();
        }
    } else if (auto *v = ctx->value()) {
        item.name = v->getText();
    }
    return item;
}

}  // namespace

std::vector<Module> AstBuilder::build(AsnEtsiItsParser::ModulesContext *ctx) {
    std::vector<Module> modules;
    if (ctx == nullptr) {
        return modules;
    }
    for (auto *modDef : ctx->moduleDefinition()) {
        modules.push_back(buildModule(modDef));
    }
    return modules;
}

Module AstBuilder::buildModule(AsnEtsiItsParser::ModuleDefinitionContext *ctx) {
    Module module;
    module.name = ctx->IDENTIFIER() != nullptr ? ctx->IDENTIFIER()->getText() : std::string{};

    if (auto *td = ctx->tagDefault()) {
        if (td->AUTOMATIC_LITERAL() != nullptr) {
            module.tagDefault = TagDefault::Automatic;
        } else if (td->IMPLICIT_LITERAL() != nullptr) {
            module.tagDefault = TagDefault::Implicit;
        } else if (td->EXPLICIT_LITERAL() != nullptr) {
            module.tagDefault = TagDefault::Explicit;
        } else {
            module.tagDefault = TagDefault::Unspecified;
        }
    }
    if (auto *ed = ctx->extensionDefault()) {
        module.extensibilityImplied = ed->IMPLIED_LITERAL() != nullptr;
    }

    if (auto *body = ctx->moduleBody()) {
        if (auto *imp = body->imports()) {
            module.imports = buildImports(imp);
        }
        if (auto *list = body->assignmentList()) {
            for (auto *assign : list->assignment()) {
                addAssignment(assign, module);
            }
        }
    }
    return module;
}

std::vector<Import> AstBuilder::buildImports(AsnEtsiItsParser::ImportsContext *ctx) {
    std::vector<Import> result;
    auto *symbolsImported = ctx->symbolsImported();
    if (symbolsImported == nullptr) {
        return result;
    }
    auto *list = symbolsImported->symbolsFromModuleList();
    if (list == nullptr) {
        return result;
    }
    for (auto *sfm : list->symbolsFromModule()) {
        Import imp;
        if (auto *symbolList = sfm->symbolList()) {
            for (auto *sym : symbolList->symbol()) {
                if (sym->IDENTIFIER() != nullptr) {
                    imp.symbols.push_back(sym->IDENTIFIER()->getText());
                }
            }
        }
        if (auto *gmr = sfm->globalModuleReference()) {
            if (gmr->IDENTIFIER() != nullptr) {
                imp.fromModule = gmr->IDENTIFIER()->getText();
            }
        }
        result.push_back(std::move(imp));
    }
    return result;
}

void AstBuilder::addAssignment(AsnEtsiItsParser::AssignmentContext *ctx, Module &module) {
    std::string name = ctx->IDENTIFIER() != nullptr ? ctx->IDENTIFIER()->getText() : std::string{};

    if (auto *ta = ctx->typeAssignment()) {
        if (auto *type = ta->asnType()) {
            module.typeAssignments.push_back(TypeAssignment{name, buildAsnType(type)});
        }
        return;
    }
    if (ctx->objectClassAssignment() != nullptr) {
        module.unsupportedAssignments.push_back(
            {name, "objectClassAssignment (CLASS / X.681 Information Object System - not modeled)"});
        return;
    }
    if (ctx->parameterizedAssignment() != nullptr) {
        module.unsupportedAssignments.push_back({name, "parameterizedAssignment (parameterized type - not modeled)"});
        return;
    }
    // valueAssignment: not extracted yet; no real caller needs it for this proof.
}

TypeNode AstBuilder::buildAsnType(AsnEtsiItsParser::AsnTypeContext *ctx) {
    TypeNode node;
    if (ctx == nullptr) {
        return node;
    }
    if (auto *bt = ctx->builtinType()) {
        node = buildBuiltinType(bt);
    } else if (auto *rt = ctx->referencedType()) {
        node = buildReferencedType(rt);
    }
    for (auto *c : ctx->constraint()) {
        node.constraints.push_back(buildConstraint(c));
    }
    return node;
}

TypeNode AstBuilder::buildBuiltinType(AsnEtsiItsParser::BuiltinTypeContext *ctx) {
    if (ctx->BOOLEAN_LITERAL() != nullptr) {
        TypeNode n;
        n.kind = TypeKind::Boolean;
        return n;
    }
    if (ctx->NULL_LITERAL() != nullptr) {
        TypeNode n;
        n.kind = TypeKind::Null;
        return n;
    }
    if (ctx->octetStringType() != nullptr) {
        TypeNode n;
        n.kind = TypeKind::OctetString;
        return n;
    }
    if (auto *bst = ctx->bitStringType()) {
        return buildBitStringType(bst);
    }
    if (auto *ch = ctx->choiceType()) {
        return buildChoiceType(ch);
    }
    if (auto *en = ctx->enumeratedType()) {
        return buildEnumeratedType(en);
    }
    if (auto *it = ctx->integerType()) {
        TypeNode n;
        n.kind = TypeKind::Integer;
        if (auto *nnl = it->namedNumberList()) {
            Constraint c;
            c.kind = ConstraintKind::Raw;
            c.rawText = nnl->getText();
            n.constraints.push_back(std::move(c));
        }
        return n;
    }
    if (auto *seq = ctx->sequenceType()) {
        return buildSequenceOrSetType(seq->componentTypeLists(), /*isSet=*/false);
    }
    if (auto *sof = ctx->sequenceOfType()) {
        return buildSequenceOfOrSetOfType(sof->asnType(), sof->namedType(), sof->constraint(), sof->sizeConstraint(),
                                           /*isSet=*/false);
    }
    if (auto *st = ctx->setType()) {
        return buildSequenceOrSetType(st->componentTypeLists(), /*isSet=*/true);
    }
    if (auto *sof2 = ctx->setOfType()) {
        return buildSequenceOfOrSetOfType(sof2->asnType(), sof2->namedType(), sof2->constraint(),
                                           sof2->sizeConstraint(), /*isSet=*/true);
    }
    if (ctx->objectidentifiertype() != nullptr) {
        TypeNode n;
        n.kind = TypeKind::ObjectIdentifier;
        return n;
    }
    if (auto *tt = ctx->taggedType()) {
        return buildTaggedType(tt);
    }
    if (auto *ocf = ctx->objectClassFieldType()) {
        TypeNode n;
        n.kind = TypeKind::Reference;
        n.referencedName = ocf->getText();
        return n;
    }

    // Not reachable given builtinType's fixed alternative set; fall back to an
    // opaque reference rather than crash.
    TypeNode n;
    n.kind = TypeKind::Reference;
    n.referencedName = ctx->getText();
    return n;
}

TypeNode AstBuilder::buildReferencedType(AsnEtsiItsParser::ReferencedTypeContext *ctx) {
    TypeNode node;
    node.kind = TypeKind::Reference;
    if (auto *dt = ctx->definedType()) {
        node.referencedName = dt->getText();
    }
    return node;
}

TypeNode AstBuilder::buildSequenceOrSetType(AsnEtsiItsParser::ComponentTypeListsContext *listsCtx, bool isSet) {
    TypeNode node;
    node.kind = isSet ? TypeKind::Set : TypeKind::Sequence;
    if (listsCtx == nullptr) {
        return node;
    }

    for (auto *root : listsCtx->rootComponentTypeList()) {
        if (auto *ctl = root->componentTypeList()) {
            collectComponentTypeList(ctl, /*isExtensionAddition=*/false, node.fields);
        }
    }
    if (listsCtx->extensionAndException() != nullptr) {
        node.extensible = true;
    }
    if (auto *additions = listsCtx->extensionAdditions()) {
        if (auto *list = additions->extensionAdditionList()) {
            for (auto *addition : list->extensionAddition()) {
                if (auto *ct = addition->componentType()) {
                    node.fields.push_back(buildField(ct, /*isExtensionAddition=*/true));
                } else if (auto *group = addition->extensionAdditionGroup()) {
                    if (auto *ctl = group->componentTypeList()) {
                        collectComponentTypeList(ctl, /*isExtensionAddition=*/true, node.fields);
                    }
                }
            }
        }
    }
    return node;
}

void AstBuilder::collectComponentTypeList(AsnEtsiItsParser::ComponentTypeListContext *ctx, bool isExtensionAddition,
                                           std::vector<Field> &out) {
    for (auto *ct : ctx->componentType()) {
        out.push_back(buildField(ct, isExtensionAddition));
    }
}

Field AstBuilder::buildField(AsnEtsiItsParser::ComponentTypeContext *ctx, bool isExtensionAddition) {
    Field field;
    field.isExtensionAddition = isExtensionAddition;

    if (auto *nt = ctx->namedType()) {
        field.name = nt->IDENTIFIER() != nullptr ? nt->IDENTIFIER()->getText() : std::string{};
        field.type = buildAsnType(nt->asnType());
        field.optional = ctx->OPTIONAL_LITERAL() != nullptr;
        if (ctx->DEFAULT_LITERAL() != nullptr && ctx->value() != nullptr) {
            field.defaultValueRaw = ctx->value()->getText();
        }
    } else if (ctx->COMPONENTS_LITERAL() != nullptr) {
        // "COMPONENTS OF Type" splices another SEQUENCE/SET's fields in. Not
        // flattened here (would need that type's own definition resolved) —
        // kept as an opaque marker field referencing the spliced type.
        field.name = "<components-of>";
        field.type = buildAsnType(ctx->asnType());
    }
    return field;
}

TypeNode AstBuilder::buildSequenceOfOrSetOfType(AsnEtsiItsParser::AsnTypeContext *innerAsnType,
                                                 AsnEtsiItsParser::NamedTypeContext *innerNamedType,
                                                 AsnEtsiItsParser::ConstraintContext *constraintCtx,
                                                 AsnEtsiItsParser::SizeConstraintContext *sizeConstraintCtx,
                                                 bool isSet) {
    TypeNode node;
    node.kind = isSet ? TypeKind::SetOf : TypeKind::SequenceOf;

    TypeNode elem;
    if (innerAsnType != nullptr) {
        elem = buildAsnType(innerAsnType);
    } else if (innerNamedType != nullptr) {
        elem = buildAsnType(innerNamedType->asnType());
    }
    node.elementType = std::make_unique<TypeNode>(std::move(elem));

    if (constraintCtx != nullptr) {
        node.constraints.push_back(buildConstraint(constraintCtx));
    }
    if (sizeConstraintCtx != nullptr && sizeConstraintCtx->constraint() != nullptr) {
        Constraint c = buildConstraint(sizeConstraintCtx->constraint());
        c.kind = ConstraintKind::SizeRange;
        node.constraints.push_back(std::move(c));
    }
    return node;
}

TypeNode AstBuilder::buildChoiceType(AsnEtsiItsParser::ChoiceTypeContext *ctx) {
    TypeNode node;
    node.kind = TypeKind::Choice;
    auto *lists = ctx->alternativeTypeLists();
    if (lists == nullptr) {
        return node;
    }

    if (auto *root = lists->rootAlternativeTypeList()) {
        if (auto *atl = root->alternativeTypeList()) {
            for (auto *nt : atl->namedType()) {
                Field f;
                f.name = nt->IDENTIFIER() != nullptr ? nt->IDENTIFIER()->getText() : std::string{};
                f.type = buildAsnType(nt->asnType());
                node.fields.push_back(std::move(f));
            }
        }
    }
    if (lists->extensionAndException() != nullptr) {
        node.extensible = true;
    }
    if (auto *alts = lists->extensionAdditionAlternatives()) {
        if (auto *list = alts->extensionAdditionAlternativesList()) {
            for (auto *alt : list->extensionAdditionAlternative()) {
                if (auto *nt = alt->namedType()) {
                    Field f;
                    f.name = nt->IDENTIFIER() != nullptr ? nt->IDENTIFIER()->getText() : std::string{};
                    f.type = buildAsnType(nt->asnType());
                    f.isExtensionAddition = true;
                    node.fields.push_back(std::move(f));
                } else if (auto *group = alt->extensionAdditionAlternativesGroup()) {
                    if (auto *atl = group->alternativeTypeList()) {
                        for (auto *nt : atl->namedType()) {
                            Field f;
                            f.name = nt->IDENTIFIER() != nullptr ? nt->IDENTIFIER()->getText() : std::string{};
                            f.type = buildAsnType(nt->asnType());
                            f.isExtensionAddition = true;
                            node.fields.push_back(std::move(f));
                        }
                    }
                }
            }
        }
    }
    return node;
}

TypeNode AstBuilder::buildEnumeratedType(AsnEtsiItsParser::EnumeratedTypeContext *ctx) {
    TypeNode node;
    node.kind = TypeKind::Enumerated;
    auto *enums = ctx->enumerations();
    if (enums == nullptr) {
        return node;
    }

    if (auto *root = enums->rootEnumeration()) {
        if (auto *e = root->enumeration()) {
            for (auto *item : e->enumerationItem()) {
                node.enumItems.push_back(buildEnumerationItem(item));
            }
        }
    }
    if (enums->ELLIPSIS() != nullptr || enums->EXTENSTIONENDMARKER() != nullptr) {
        node.enumExtensible = true;
    }
    if (auto *add = enums->additionalEnumeration()) {
        if (auto *e = add->enumeration()) {
            for (auto *item : e->enumerationItem()) {
                node.enumExtensionItems.push_back(buildEnumerationItem(item));
            }
        }
    }
    return node;
}

TypeNode AstBuilder::buildBitStringType(AsnEtsiItsParser::BitStringTypeContext *ctx) {
    TypeNode node;
    node.kind = TypeKind::BitString;
    auto *list = ctx->namedBitList();
    if (list == nullptr) {
        return node;
    }
    for (auto *nb : list->namedBit()) {
        NamedBit bit;
        bit.name = nb->IDENTIFIER() != nullptr ? nb->IDENTIFIER()->getText() : std::string{};
        if (nb->NUMBER() != nullptr) {
            bit.rawValue = nb->NUMBER()->getText();
        } else if (nb->definedValue() != nullptr) {
            bit.rawValue = nb->definedValue()->getText();
        }
        node.namedBits.push_back(std::move(bit));
    }
    return node;
}

TypeNode AstBuilder::buildTaggedType(AsnEtsiItsParser::TaggedTypeContext *ctx) {
    TypeNode node;
    node.kind = TypeKind::Tagged;

    if (auto *ct = ctx->classTag()) {
        node.tag = buildTag(ct);
    }
    if (node.tag) {
        if (ctx->IMPLICIT_LITERAL() != nullptr) {
            node.tag->mode = TagMode::Implicit;
        } else if (ctx->EXPLICIT_LITERAL() != nullptr) {
            node.tag->mode = TagMode::Explicit;
        }
    }

    TypeNode inner = buildAsnType(ctx->asnType());
    node.elementType = std::make_unique<TypeNode>(std::move(inner));
    return node;
}

Tag AstBuilder::buildTag(AsnEtsiItsParser::ClassTagContext *ctx) {
    Tag tag;
    if (ctx->UNIVERSAL_LITERAL() != nullptr) {
        tag.tagClass = TagClass::Universal;
    } else if (ctx->APPLICATION_LITERAL() != nullptr) {
        tag.tagClass = TagClass::Application;
    } else if (ctx->PRIVATE_LITERAL() != nullptr) {
        tag.tagClass = TagClass::Private;
    } else {
        tag.tagClass = TagClass::ContextSpecific;
    }
    if (ctx->NUMBER() != nullptr) {
        tag.number = std::stoi(ctx->NUMBER()->getText());
    }
    return tag;
}

Constraint AstBuilder::buildConstraint(AsnEtsiItsParser::ConstraintContext *ctx) {
    Constraint result;
    if (ctx == nullptr) {
        return result;
    }
    result.rawText = ctx->getText();

    auto *spec = ctx->constraintSpec();
    auto *sub = spec != nullptr ? spec->subtypeConstraint() : nullptr;
    if (sub == nullptr) {
        return result;  // generalConstraint (table/user-defined/contents) -> Raw
    }

    auto extracted = tryExtractElementSetSpecs(sub->elementSetSpecs());
    if (!extracted) {
        return result;
    }

    result.kind = extracted->kind;
    result.rootRanges = std::move(extracted->rootRanges);
    result.extensionRanges = std::move(extracted->extensionRanges);
    result.extensible = extracted->extensible;
    return result;
}
