#pragma once

#define TT_MACRO_CONCAT(a, b) a##b
#define TT_MACRO_CONCAT2(a, b) MACRO_CONCAT(a, b)

#define TT_MACRO_STRINGIFY(a) #a
#define TT_MACRO_STRINGIFY2(a) MACRO_STRINGIFY(a)

#define TT_ATTRIBUTES(...) __attribute__((__VA_ARGS__))
#define TT_ALWAYS_INLINE inline TT_ATTRIBUTES(always_inline)