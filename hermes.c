#include <janet.h>
#include "hermes.h"

static int pkg_gcmark(void *p, size_t s) {
    (void)s;
    Pkg *pkg = p;
    janet_mark(pkg->builder);
    janet_mark(pkg->name);
    janet_mark(pkg->out_hash);
    janet_mark(pkg->hash);
    janet_mark(pkg->path);
    janet_mark(pkg->force_refs);
    janet_mark(pkg->extra_refs);
    janet_mark(pkg->weak_refs);
    return 0;
}

static int pkg_get(void *ptr, Janet key, Janet *out) {
    Pkg *pkg = ptr;
    if (janet_keyeq(key, "hash")) {
        *out = pkg->hash;
        return 1;
    } else if (janet_keyeq(key, "path")) {
        *out = pkg->path;
        return 1;
    } else if (janet_keyeq(key, "builder")) {
        *out = pkg->builder;
        return 1;
    } else if (janet_keyeq(key, "name")) {
        *out = pkg->name;
        return 1;
    } else if (janet_keyeq(key, "out-hash")) {
        *out = pkg->out_hash;
        return 1;
    } else if (janet_keyeq(key, "force-refs")) {
        *out = pkg->force_refs;
        return 1;
    } else if (janet_keyeq(key, "weak-refs")) {
        *out = pkg->weak_refs;
        return 1;
    } else if (janet_keyeq(key, "extra-refs")) {
        *out = pkg->extra_refs;
        return 1;
    } else {
        return 0;
    }
}

static void validate_pkg(Pkg *pkg) {
    if (!janet_checktypes(pkg->builder, JANET_TFLAG_NIL|JANET_TFLAG_FUNCTION))
        janet_panicf("builder must be a function or nil, got %v", pkg->builder);

    if (!janet_checktypes(pkg->name, JANET_TFLAG_NIL|JANET_TFLAG_STRING))
        janet_panicf("name must be a string or nil, got %v", pkg->name);

    if janet_checktype(pkg->name, JANET_STRING) {
        JanetString name = janet_unwrap_string(pkg->name);
        size_t name_len = janet_string_length(name);
        if (name_len > 64) {
            janet_panicf("name %v is too long, must be less than 64 chars", pkg->name);
        }

        for (size_t i = 0; i < name_len; i++) {
            if (name[i] == '/') {
                janet_panicf("name %v contains path separator.", pkg->name);
            }
        }
    }

    if (!janet_checktypes(pkg->out_hash, JANET_TFLAG_NIL|JANET_TFLAG_STRING))
        janet_panicf("out-hash must be a string or nil, got %v", pkg->out_hash);

    if (!janet_checktypes(pkg->path, JANET_TFLAG_NIL|JANET_TFLAG_STRING))
        janet_panicf("path must be a string or nil, got %v", pkg->path);

    if (!janet_checktypes(pkg->hash, JANET_TFLAG_NIL|JANET_TFLAG_STRING))
        janet_panicf("hash must be a string or nil, got %v", pkg->hash);

#define CHECK_PKG_TUPLE(NAME, V) \
    do {  \
      if (janet_checktype(V, JANET_TUPLE)) { \
        const Janet *vs = janet_unwrap_tuple(V); \
        size_t n_vs = janet_tuple_length(vs); \
        for (size_t i = 0; i < n_vs; i++) { \
          if (!janet_checkabstract(vs[i], &pkg_type)) { \
            janet_panicf(NAME "[%d] must be a package, got %v", i, vs[i]); \
          } \
        } \
      } else if (janet_checktype(V, JANET_NIL)) { \
        ; \
      } else { \
        janet_panicf(NAME " must be a tuple or nil, got %v", V); \
      } \
    } while (0);

    CHECK_PKG_TUPLE("force-refs", pkg->force_refs);
    CHECK_PKG_TUPLE("extra-refs", pkg->extra_refs);
    CHECK_PKG_TUPLE("weak-refs", pkg->weak_refs);

#undef CHECK_PKG_TUPLE
}

static void pkg_marshal(void *p, JanetMarshalContext *ctx) {
    Pkg *pkg = p;
    janet_marshal_abstract(ctx, p);
    janet_marshal_janet(ctx, pkg->builder);
    janet_marshal_janet(ctx, pkg->name);
    janet_marshal_janet(ctx, pkg->out_hash);
    janet_marshal_janet(ctx, pkg->hash);
    janet_marshal_janet(ctx, pkg->path);
    janet_marshal_janet(ctx, pkg->force_refs);
    janet_marshal_janet(ctx, pkg->extra_refs);
    janet_marshal_janet(ctx, pkg->weak_refs);
}

static void* pkg_unmarshal(JanetMarshalContext *ctx) {
    Pkg *pkg = janet_unmarshal_abstract(ctx, sizeof(Pkg));
    pkg->builder = janet_unmarshal_janet(ctx);
    pkg->name = janet_unmarshal_janet(ctx);
    pkg->out_hash = janet_unmarshal_janet(ctx);
    pkg->hash = janet_unmarshal_janet(ctx);
    pkg->path = janet_unmarshal_janet(ctx);
    pkg->force_refs = janet_unmarshal_janet(ctx);
    pkg->extra_refs = janet_unmarshal_janet(ctx);
    pkg->weak_refs = janet_unmarshal_janet(ctx);
    validate_pkg(pkg);
    return pkg;
}

static Janet pkg(int argc, Janet *argv) {
    janet_fixarity(argc, 6);

    Pkg *pkg = janet_abstract(&pkg_type, sizeof(Pkg));
    pkg->builder = argv[0];
    pkg->name = argv[1];
    pkg->out_hash = argv[2];
    pkg->force_refs = argv[3];
    pkg->extra_refs = argv[4];
    pkg->weak_refs = argv[5];
    pkg->hash = janet_wrap_nil();
    pkg->path = janet_wrap_nil();

    validate_pkg(pkg);

    return janet_wrap_abstract(pkg);
}

const JanetAbstractType pkg_type = {
    "hermes/pkg", NULL, pkg_gcmark, pkg_get, NULL, pkg_marshal, pkg_unmarshal, NULL, NULL, NULL, NULL, NULL,
};

static const JanetReg cfuns[] = {
    {"pkg", pkg, NULL},
    {"pkg-hash", pkg_hash, NULL},
    {"pkg-dependencies", pkg_dependencies, NULL},
    {"hash-scan", hash_scan, NULL},
    {NULL, NULL, NULL}
};

JANET_MODULE_ENTRY(JanetTable *env) {
    janet_register_abstract_type(&pkg_type);
    janet_cfuns(env, "_hermes", cfuns);
}
