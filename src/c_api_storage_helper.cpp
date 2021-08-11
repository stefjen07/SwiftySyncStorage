#include "c_api_storage.h"
#include "SwiftySyncStorage.hpp"
using namespace std;

struct CField* CField_fromField(Field* field) {
    struct CField* p = new CField();
    p->name = (char*)field->name.c_str();
    p->float_value = field->floatValue;
    p->num_value = field->numValue;
    p->str_value = (char*)field->strValue.c_str();
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