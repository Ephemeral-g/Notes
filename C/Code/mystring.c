// mystring.c
#include "mystring.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const size_t c_string_min_size = 32;

struct c_string {
    char *str;
    size_t alloced;
    size_t len;
};

c_string_t *c_string_create(void) {
    c_string_t *cs;

    cs = (c_string_t*)calloc(1, sizeof(*cs));
    cs->str = (char*)malloc(c_string_min_size);
    *cs->str = '\0';
    cs->alloced = c_string_min_size;
    cs->len = 0;

    return cs;
}

void c_string_destroy(c_string_t *cs) {
    if (cs != NULL) {
        free(cs->str);
        free(cs);
    }
}

static void c_string_ensure_space(c_string_t *cs, size_t add_len) {
    if (cs == NULL || add_len == 0) {
        return;
    }

    if (cs->alloced >= cs->len + add_len + 1) {
        return;
    }

    while (cs->alloced < cs->len + add_len + 1) {
        cs->alloced <<= 1;
        if (cs->alloced == 0) {
            cs->alloced--;
        }
    }
    cs->str = (char*)realloc(cs->str, cs->alloced);
}

void c_string_append_str(c_string_t *cs, const char *str, size_t len) {
    if (cs == NULL || str == NULL || *str == '\0') {
        return;
    }

    if (len == 0) {
        len = strlen(str);
    }

    c_string_ensure_space(cs, len);
    memmove(cs->str + cs->len, str, len);
    cs->len += len;
    cs->str[cs->len] = '\0';
}

void c_string_append_char(c_string_t *cs, char c) {
    if (cs == NULL) {
        return;
    }
    c_string_ensure_space(cs, 1);
    cs->str[cs->len] = c;
    cs->len++;
    cs->str[cs->len] = '\0';
}

void c_string_append_int(c_string_t *cs, int val) {
    char str[12];

    if (cs == NULL) {
        return;
    }

    snprintf(str, sizeof(str), "%d", val);
    c_string_append_str(cs, str, 0);
}

void c_string_front_str(c_string_t *cs, const char *str, size_t len) {
    if (cs == NULL || str == NULL || *str == '\0') {
        return;
    }

    if (len == 0) {
        len = strlen(str);
    }

    c_string_ensure_space(cs, len);
    memmove(cs->str + len, cs->str, cs->len);
    memmove(cs->str, str, len);
    cs->len += len;
    cs->str[cs->len] = '\0';
}

void c_string_front_char(c_string_t *cs, char c) {
    if (cs == NULL) {
        return;
    }
    c_string_ensure_space(cs, 1);
    memmove(cs->str + 1, cs->str, cs->len);
    cs->str[0] = c;
    cs->len++;
    cs->str[cs->len] = '\0';
}

void c_string_front_int(c_string_t *cs, int val) {
    char str[12];

    if (cs == NULL) {
        return;
    }

    snprintf(str, sizeof(str), "%d", val);
    c_string_front_str(cs, str, 0);
}

void c_string_clear(c_string_t *cs) {
    if (cs != NULL) {
        c_string_truncate(cs, 0);
    }
}

void c_string_truncate(c_string_t *cs, size_t len) {
    if (cs == NULL || len >= cs->len) {
        return;
    }

    cs->len = len;
    cs->str[cs->len] = '\0';
}

void c_string_drop_begin(c_string_t *cs, size_t len) {
    if (cs == NULL || len == 0) {
        return;
    }

    if (len >= cs->len) {
        c_string_clear(cs);
        return;
    }

    cs->len -= len;
    /* +1 to move the NULL. */
    memmove(cs->str, cs->str + len, cs->len + 1);
}

void c_string_drop_end(c_string_t *cs, size_t len) {
    if (cs == NULL || len == 0) {
        return;
    }

    if (len >= cs->len) {
        c_string_clear(cs);
        return;
    }
    cs->len -= len;
    cs->str[cs->len] = '\0';
}

size_t c_string_len(const c_string_t *cs) {
    return cs == NULL ? 0 : cs->len;
}

const char *c_string_peek(const c_string_t *cs) {
    return cs == NULL ? NULL : cs->str;
}

char *c_string_dump(const c_string_t *cs, size_t *len) {
    char *out;

    if (cs == NULL) {
        return NULL;
    }

    if (len != NULL) {
        *len = cs->len;
    }
    out = (char*)malloc(cs->len + 1);
    memcpy(out, cs->str, cs->len + 1);
    return out;
}