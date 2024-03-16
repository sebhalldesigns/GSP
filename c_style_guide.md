# GSP C Style Guide

## Naming

All types, variables and functions to be in ``snake_case``.
Types to be appended with ``_t``.

All globally visible types and functions should be prefixed with a global module path.
E.g ``module_submodule_typename_t``.

Static globals to be prefixed with ``g_``.

## Types

``int`` for status return types (as can have -1, 0, 1).

``bool`` for flags.

``size_t`` for any situtation when imposing an arbitrary size limit isn't necessary.
