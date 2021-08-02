#include "c-api-storage.h"
#include <stdio.h>

int main() {
    struct CField* field = CField_empty();
    struct CField* child = CField_new(cft_string, "hello");
    child->str_value = "Hello, world!";
    add_child(field, *child);
    const char* encoded = encode(field);

    struct CField* field_copy = CField_empty();
    decode(field_copy, encoded);
    struct CField* child_copy = get_child(field_copy, "hello");
    if (child_copy == NULL) {
        printf("Cannot find child \"hello\" in field\n");
        return -1;
    }
    printf(child_copy->str_value);
    return 0;
}