#ifndef PD_DEFINES_H
#define PD_DEFINES_H

#ifdef __cplusplus
    #define PD_C_DECLS_BEGIN extern "C" {
    #define PD_C_DECLS_END }
#else
    #define PD_C_DECLS_BEGIN
    #define PD_C_DECLS_END
#endif // __cplusplus

#define PD_stringify_internal(_x) #_x
#define PD_stringify(_x) PD_stringify_internal(_x)

#endif // PD_DEFINES_H
