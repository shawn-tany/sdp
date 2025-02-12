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

typedef int FD_T;

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

#define DO_ONCE_START       do {
#define DO_ONCE_END         } while(0);

#define DO_LOOP_START       while (1) {
#define DO_LOOP_N_START(n)  for (int _n_loop_index = 0; _n_loop_index < n; ++_n_loop_index) {
#define DP_LOOP_END         }

#define ITEM(a) (sizeof(a) / sizeof(a[0]))

#define MIN(a, b) ((a < b) ? a : b)

#define MAX(a, b) ((a < b) ? b : a)

#define UNUSED(a) ((void)a)

enum 
{
    FALSE = 0,
    TRUE,
};

#endif
