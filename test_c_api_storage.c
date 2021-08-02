#include "c-api-storage.h"
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main() {
    for(int i=0;i<10000;i++) {
        struct CField *field = CField_empty();
        struct CField *child = CField_new(cft_string, "hello");
        child->str_value = "Hello, world!";
        add_child(field, *child);
        char *encoded = encode(field);

        struct CField *field_copy = CField_empty();
        decode(field_copy, encoded);
        struct CField *child_copy = get_child(field_copy, "hello");
        if (child_copy == NULL) {
            printf("Cannot find child \"hello\" in field\n");
            return -1;
        }

        printf(child_copy->str_value);

        free(child_copy);
        free(field_copy);
        free(encoded);
        free(child);
        free(field);
    }
#ifdef _WIN32
    Sleep(5000);
#else
    usleep(5000);
#endif
    return 0;
}