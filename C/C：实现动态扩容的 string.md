# 动态扩容的 string

string 有以下功能：

- 创建字符串
- 删除字符串
- 尾部追加字符串
- 头部插入字符串
- 从尾部删除 N 个字符串
- 从头部删除 N 个字符串
- 裁剪字符串
- 获取字符串长度
- 获取完整字符串

首先声明一个 string 的结构体。

```c
struct c_string;
typedef struct c_string c_string_t;
```

string 的实现如下：

```c
// string 的初始内存大小
static const size_t c_string_min_size = 32;

struct c_string {
	char *str;  // 字符串指针
    size_t alloced;  // 已分配的内存大小
    size_t len;  // 字符串的实际长度
};
```

创建字符串

```c
c_string_t* c_string_create(void) {
	c_string_t *cs;
    cs = calloc(1, sizeof(*cs));
    cs->str = malloc(c_string_min_size);
    *cs->str = '\0';
    // 初始分配内存大小是 32，之后每次以 2 倍大小扩容
    cs->alloced = c_string_min_size;
    cs->len = 0;
}
```

销毁字符串

```c
void c_string_destroy(c_string_t *cs) {
	if (cs == NULL) {
		return;
	}
	free(cs->str);
	free(cs);
}
```

内部扩容

```c
static void c_string_ensure_space(c_string_t *cs, size_t add_len) {
	if (cs == NULL || add_len == 0) {
        return;
    }
    if (cs->alloced >= cs->len + add_len + 1) {
        return;
    }
    while (cs->alloced < cs->len + add_len + 1) {
        cs->alloced <<= 1;  // 每次以 2 倍大小扩容
		if (cs->alloced == 0) {
			// 左移到最后可能会变为0，由于 alloced 是无符号型，减一则会变成 UINT_MAX
			cs->alloced--;
		}
    }
	cs->str = (char*)realloc(cs->str, cs->alloced);
}
```

在尾部追加字符串

```c
void c_string_append_str(c_string_t *cs, const char *str, size_t len) {
	if (cs == NULL || str == NULL || *str == '\0') {
		return;
	}
	if (len == 0) {
		len = strlen(str);
	}
	c_string_ensure_space(cs, len);
	memmove(cs->str + cs->len, str, len);
	cs->len	+= len;
	cs->str[cs->len] = '\0';
}
```

在尾部追加字符

```c
void c_string_append_char(c_string_t *cs, char c) {
	if (cs == NULL) {
		return;
	}
	c_string_ensure_space(cs, 1);
	cs->str[cs->len] = c;
	cs->len++;
	cs->str[cs->len] = '\0';
}
```

在尾部追加整数

```c
void c_string_append_int(c_string_t *cs, int val) {
	char str[12];
	if (cs == NULL) {
		return;
	}
	snprintf(str, sizeof(str), "%d", val);  // 整数转为字符串
	c_string_append_str(cs, str, 0);
}
```

在头部插入字符串

```c
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
	cs->len	+= len;
	cs->str[cs->len] = '\0';
}
```

在头部插入字符

```c
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
```

在头部插入整数

```c
void c_string_front_int(c_string_t *cs, int val) {
	char str[12];
	if (cs == NULL) {
		return;
	}
	snprintf(str, sizeof(str), "%d", val);  // 整数转为字符串
	c_string_front_str(cs, str, 0);
}
```

清空字符串

```c
void c_string_clear(c_string_t *cs) {
	if (cs == NULL) {
		return;
	}
	c_string_truncate(cs, 0);
}
```

裁剪字符串

```c
void c_string_truncate(c_string_t *cs, size_t len) {
	if (cs == NULL || len >= cs->len) {
		return;
	}
	cs->len = len;
	cs->str[cs->len] = '\0';
}
```

删除头部的 N 个字符

```c
void c_string_drop_begin(c_string_t *cs, size_t len) {
	if (cs == NULL || len == 0) {
		return;
	}
	if (len >= cs->len) {
		c_string_clear(cs);
		return;
	}
	cs->len -= len;
	memmove(cs->str, cs->str + len, cs->len + 1);
}
```

删除尾部的 N 个字符

```c
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
```

获取字符串的长度

```c
size_t c_string_len(const c_string_t *cs) {
	if (cs == NULL) {
		return 0;
	}
	return cs->len;
}
```

返回字符串指针，使用的是内部的内存

```c
const char* c_string_peek(const c_string_t *cs) {
	if (cs == NULL) {
		return NULL;
	}
	return cs->str;
}
```

重新分配一块内存存储字符串返回

```c
char* c_string_dump(const c_string_t *cs, size_t *len) {
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
```

测试代码

```c
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
```

