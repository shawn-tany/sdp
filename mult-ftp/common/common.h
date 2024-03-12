#ifndef MFTP_COMMON
#define MFTP_COMMON

typedef char INT8_T;
typedef unsigned char UINT8_T;

typedef short INT16_T;
typedef unsigned short UINT16_T;

typedef int INT32_T;
typedef unsigned int UINT32_T;

typedef long long INT64_T;
typedef unsigned long long UINT64_T;

/* point check */
#define PTR_CHECK_VOID(p)   \
    if (!p) {               \
        return ;            \
    }

#define PTR_CHECK_N1(p)     \
    if (!p) {               \
        return -1;          \
    }

#define PTR_CHECK_0(p)      \
    if (!p) {               \
        return 0;           \
    }

#define PTR_CHECK_NULL(p)   \
    if (!p) {               \
        return NULL;        \
    }

#define ITEM(a) (sizeof(a) / sizeof(a[0]))

#define MIN(a, b) ((a < b) ? b : a)

#define MAX(a, b) ((a < b) ? a : b)

#define UNUSED(a) ((void)a)

#endif
