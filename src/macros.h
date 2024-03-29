#ifndef MACROS_H_HTLPPMY5
#define MACROS_H_HTLPPMY5

#include <stdio.h>

#ifdef DEBUG
#define HAS_DEBUG 1
#else
#define HAS_DEBUG 0
#endif

#define debug_putsf(fmt, ...) \
        do { if (HAS_DEBUG) printf("%s:%d in %s(): " fmt "\n", __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#define debug_puts(msg) \
        do { if (HAS_DEBUG) printf("%s:%d in %s(): " msg "\n", \
                                __FILE__, __LINE__, __func__); } while (0)

#endif /* end of include guard: MACROS_H_HTLPPMY5 */
