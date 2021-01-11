#include "mystring.h"
#include <stdio.h>

int main() {
    c_string_t *cs = c_string_create();
    c_string_append_str(cs, "world", 0);
    c_string_append_char(cs, '0');
    c_string_append_int(cs, 666);
    printf("%s\n", c_string_peek(cs));
    c_string_front_str(cs, "hello", 0);
    printf("%s\n", c_string_peek(cs));
    c_string_drop_begin(cs, 2);
    printf("%s\n", c_string_peek(cs));
    c_string_drop_end(cs, 2);
    printf("%s\n", c_string_peek(cs));
    c_string_destroy(cs);
    return 0;
}