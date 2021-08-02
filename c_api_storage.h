#ifndef SWIFTYSYNCSTORAGE_C_API_STORAGE_H
#define SWIFTYSYNCSTORAGE_C_API_STORAGE_H


#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

    enum CFieldType {
        cft_boolean = 0,
        cft_number,
        cft_floatingPoint,
        cft_string,
        cft_array
    };

    struct CField {
        enum CFieldType type;
        char* name;
        long long num_value;
        double float_value;
        char* str_value;
        struct CField* children;
        size_t children_size;
    };

    struct CField* get_child(struct CField* field, const char* key);
    void copy(struct CField* toField, struct CField* cfield);
    void add_child(struct CField* field, struct CField child);
    char* encode(struct CField* field);
    void decode(struct CField* field, const char* content);

    struct CField* CField_empty();
    struct CField* CField_new(enum CFieldType type, const char* name);

#ifdef __cplusplus
}
#endif

#endif
