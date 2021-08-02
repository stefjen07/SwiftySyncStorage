#include "c-api-storage.h"
#include "SwiftySyncStorage.h"
#include <malloc.h>

struct CField* CField_fromField(Field* field) {
    struct CField* p = new CField();
    p->name = field->name.c_str();
    p->float_value = field->floatValue;
    p->num_value = field->numValue;
    p->str_value = field->strValue.c_str();
    p->type = CFieldType(field->type);

    size_t children_size = field->children.size();
    p->children_size = children_size;

    CField* children = (CField*)malloc(sizeof(CField) * children_size);
    for(int i=0;i<field->children.size();i++) {
        children[i] = *CField_fromField(&field->children[i]);
    }
    p->children = children;
    return p;
}

Field* Field_fromCField(CField* cfield) {
    Field* p = new Field();
    p->children = vector<Field>(cfield->children_size);
    for(int i=0;i<cfield->children_size;i++) {
        p->children[i] = *Field_fromCField(&cfield->children[i]);
    }
    p->name = cfield->name;
    p->type = FieldType(cfield->type);
    p->strValue = cfield->str_value;
    p->numValue = cfield->num_value;
    p->floatValue = cfield->float_value;
    return p;
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

    const char* encode(CField* field) {
        JSONEncoder encoder;
        auto container = encoder.container();
        container.encode(*Field_fromCField(field));
        static std::string str = container.content;
        return str.c_str();
    }

    void copy(CField* toField, CField* cfield) {
        toField->children = cfield->children;
        toField->type = cfield->type;
        toField->name = cfield->name;
        toField->children_size = cfield->children_size;
        toField->float_value = cfield->float_value;
        toField->num_value = cfield->num_value;
        toField->str_value = cfield->str_value;
    }

    void decode(CField* field, const char* content) {
        JSONDecoder decoder;
        auto container = decoder.container(content);
        static auto decodedField = container.decode(Field());
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
        p->name = name;
        return p;
    }

#ifdef __cplusplus
}
#endif