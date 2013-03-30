#define DEBUG 1

#define debug_putsf(fmt, ...) \
        do { if (DEBUG) fprintf(stdout, "%s:%d in %s(): " fmt "\n", __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#define debug_puts(msg) \
        do { if (DEBUG) fprintf(stdout, "%s:%d in %s(): " msg "\n", \
                                __FILE__, __LINE__, __func__); } while (0)
