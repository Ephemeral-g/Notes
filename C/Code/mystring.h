// mystring.h
#ifndef MYSTRING_H
#define MYSTRING_H

#include <stddef.h>

struct c_string;
typedef struct c_string c_string_t;

c_string_t *c_string_create(void);

void c_string_destroy(c_string_t *cs);

void c_string_append_str(c_string_t *cs, const char *str, size_t len);

void c_string_append_char(c_string_t *cs, char c);

void c_string_append_int(c_string_t *cs, int val);

void c_string_front_str(c_string_t *cs, const char *str, size_t len);

void c_string_front_char(c_string_t *cs, char c);

void c_string_front_int(c_string_t *cs, int val);

void c_string_clear(c_string_t *cs);

void c_string_truncate(c_string_t *cs, size_t len);

void c_string_drop_begin(c_string_t *cs, size_t len);

void c_string_drop_end(c_string_t *cs, size_t len);

size_t c_string_len(const c_string_t *cs);

const char *c_string_peek(const c_string_t *cs);

char *c_string_dump(const c_string_t *cs, size_t *len);
#endif