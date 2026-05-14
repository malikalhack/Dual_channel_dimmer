/**
 * @file    Standard.h
 * @version 3.3.0
 * @authors Anton Chernov
 * @date    15.05.2017
 * @date    13.07.2025
 */

#ifndef STANDARD_H_
#define STANDARD_H_
/******************************* Definition ***********************************/

#define BIT(offset)         (1 << (offset))
#define BIT64(offset)       (1LL << (offset))
#define UNUSED(x)           (void)(x)
#define DISCARD_RETURN(x)   UNUSED(x)
#define VOID_PTR_CAST(x)    (void*)(x)
#define NULLPTR             ((void*)0)
#define NORETURN            __declspec(noreturn) void

#define FALSE               (0)
#define TRUE                (1)

#define SIZEOF_UINT8        (1)
#define SIZEOF_UINT16       (2)
#define SIZEOF_UINT24       (3)
#define SIZEOF_UINT32       (4)
#define SIZEOF_UINT64       (8)

#define TO_LTR(x)           #x

#define GET_LOW_BYTE(num)   ((uint8_t)num)
#define GET_HIGH_BYTE(num)  ((uint8_t)(num >> 8))

#define WORD_FROM_BYTES(hiByte, loByte) \
    ((uint16_t)(((uint16_t)(hiByte) << 8) | ((uint8_t)(loByte))))

#define SIZEOF_ARRAY(arr) \
    ((uint16_t)(sizeof(arr) / sizeof(arr[0])))

#define SET_FILE_NAME(name) static const char log_file_name[] = #name
#define FILE_NAME           (log_file_name)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/************************ Something like a <limits.h> ***************************/

#define SBYTE_MIN   -128
#define SWORD_MIN   -32768
#define SDWORD_MIN  -2147483648
#define SQWORD_MIN  -9223372036854775808
#define SBYTE_MAX   127
#define SWORD_MAX   32767
#define SDWORD_MAX  2147483647
#define SQWORD_MAX  9223372036854775807
#define BYTE_MAX    255
#define WORD_MAX    65535
#define DWORD_MAX   4294967295
#define QWORD_MAX   18446744073709551615

/******************************************************************************/

typedef unsigned char       byte_t;     ///<  8-bit unsigned integer.
typedef signed char         sbyte_t;    ///<  8-bit signed integer.
typedef unsigned int        word_t;     ///< 16 bit unsigned integer.
typedef signed int          sword_t;    ///< 16 bit signed integer.
typedef unsigned long int   dword_t;    ///< 32 bit unsigned integer.
typedef signed long int     sdword_t;   ///< 32 bit signed integer.
typedef unsigned long long  qword_t;    ///< 64 bit unsigned integer.
typedef signed long long    sqword_t;   ///< 64 bit signed integer.
/********************************************************************************/
#endif /* STANDARD_H */
