#include "c_api_storage.h"
#include "c_api_storage_helper.hpp"
#include "SwiftySyncStorage.hpp"
#include <stdlib.h>

void free(CField* field) {
    free(field->name);
    free(field->str_value);
    for(int i=0;i<field->children_size;i++) {
        free(&field->children[i]);
    }
    free(field);
}

char* c_str_from_string(std::string str) {
    char* result = (char*)malloc(sizeof(char) * (str.size()+1));
    for(int i=0;i<str.size();i++) {
        result[i] = str[i];
    }
    result[str.size()] = '\0';
    return result;
}

#ifdef __cplusplus
extern "C" {
#endif

    struct CField* get_child(CField* field, const char* key) {
        for(size_t i=0;i<field->children_size;i++) {
            if(strcmp(field->children[i].name, key) == 0) {
                return &field->children[i];
            }
        }
        return nullptr;
    }

    void add_child(CField* field, struct CField child) {
        size_t new_size = field->children_size + 1;
        field->children = (CField*)realloc(field->children, sizeof(CField) * new_size);
        field->children[new_size-1] = child;
        field->children_size = new_size;
    }

    char* encode(CField* field) {
        JSONEncoder encoder;
        auto container = encoder.container();
        container.encode(*Field_fromCField(field));
        std::string str = container.content;
        return c_str_from_string(str);
    }

    void copy(CField* toField, CField* cfield) {
        size_t children_bytes = sizeof(CField) * cfield->children_size;
        CField* children = (CField*)malloc(children_bytes);
        for(int i=0;i<cfield->children_size;i++) {
            copy(&children[i], &cfield->children[i]);
        }
        toField->children = children;
        toField->type = cfield->type;
        toField->name = c_str_from_string(cfield->name);
        toField->children_size = cfield->children_size;
        toField->float_value = cfield->float_value;
        toField->num_value = cfield->num_value;
        toField->str_value = c_str_from_string(cfield->str_value);
    }

    void decode(CField* field, const char* content) {
        JSONDecoder decoder;
        auto container = decoder.container(content);
        auto decodedField = container.decode(Field());
        copy(field, CField_fromField(&decodedField));
    }

    struct CField* CField_empty() {
        CField* p = new CField();
        p->name = "";
        p->float_value = 0.0;
        p->num_value = 0;
        p->str_value = "";
        p->type = cft_array;;
        p->children_size = 0;
        p->children = nullptr;
        return p;
    }

    struct CField* CField_new(CFieldType type, const char* name) {
        CField* p = new CField();
        p->type = type;
        p->name = (char*)name;
        return p;
    }

#ifdef __cplusplus
}
#endif