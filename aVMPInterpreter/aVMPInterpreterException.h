#ifndef AVMP_INTERPRETER_EXCEPTION_H
#define AVMP_INTERPRETER_EXCEPTION_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    void *vtable;
    const char *name;
} __type_info_t;

typedef struct {
    void *vtable;
    const char *name;
} __class_type_info_t;

typedef struct {
    void *vtable;
    const char *name;
    const __class_type_info_t *base_type;
} __si_class_type_info_t;

typedef struct {
    const __class_type_info_t *base_type;
    long offset_flags;
} __base_class_type_info_t;

typedef struct {
    void *vtable;
    const char *name;
    unsigned int flags;
    unsigned int base_count;
    __base_class_type_info_t base_info[1];
} __vmi_class_type_info_t;

typedef struct {
    const __class_type_info_t *dst_type;
    const void *static_ptr;
    const __class_type_info_t *static_type;
    const void *dst_ptr_leading_to_static_ptr;
    int path_dst_ptr_to_static_ptr;
    int number_to_static_ptr;
    bool search_done;
} __dynamic_cast_info_t;

typedef struct {
    void *reserve;
    size_t referenceCount;
    const void *exceptionType;
    void (*exceptionDestructor)(void *);
    void *unexpectedHandler;
    void *terminateHandler;
    void *nextException;
    int handlerCount;
    int handlerSwitchValue;
    const unsigned char *actionRecord;
    const unsigned char *languageSpecificData;
    void *catchTemp;
    void *adjustedPtr;
    void *unwindHeader[6];
} __cxa_exception_t;

#define RTTI_CLASS_TYPE 0
#define RTTI_SI_CLASS_TYPE 1
#define RTTI_VMI_CLASS_TYPE 2
#define BASE_VIRTUAL_MASK 0x1
#define BASE_PUBLIC_MASK 0x2
#define BASE_OFFSET_SHIFT 8

#ifdef __cplusplus
extern "C" {
#endif

void call_handler_with_exception_handling(uint64_t targetfunc_id);
void vmp_resume_unwind(void *exc);

#ifdef __cplusplus
}
#endif

#endif
