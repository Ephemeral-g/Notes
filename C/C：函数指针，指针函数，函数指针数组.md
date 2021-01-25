# 函数指针，指针函数，函数指针数组

先看段代码

```c
void (*f[])(char *)
```

这个是个什么东西？

### 函数指针和指针函数的定义

```c
int *func(int a,int b)
```

由于 ***** 的优先级低于 () 的优先级，因此 func 首先和后面的 () 结合，也就意味着，func 是一个函数。即：

```c
int *(func)(int a,int b)
```

然后这个函数的返回值的类型是 **int*** 即：指向int类型的指针

然后修改上面的代码

```c
int (*func)(int a,int b)
```

**（\*func）**说明 func 是一个指针，然后后面跟着**（）**说明这个指针指向一个函数，即指向函数的指针。

- **函数指针：**首先是一个指针，这个指针指向一个函数
- **指针函数：**首先是一个函数，这个函数的返回值一个指针

### 用typedef声明一个函数指针

声明一个函数指针，正常方法是：

```c
int (*pfunc)(int a,int b)
```

当命名很多个函数指针的时候，用上面的方法显得非常不方便，所以可以这样做：

```c
typedef int (*PF) (int a,int b);
PF pfunc;
```

```c
#include <stdio.h>

typedef int(*PF)(int, int);

int add(int a, int b) {
    return a+b;
}

int sub(int a, int b) {
    return a-b;
}

void main() {
    PF pfunc = NULL;
    pfunc = add;
    printf("add(3, 4): %d\n", pfunc(3, 4));
    pfunc = sub;
    printf("sub(3, 4): %d\n", pfunc(3, 4));
}
```

输出：

```
add(3, 4): 7
sub(3, 4): -1
```

### 再看最初的那段代码

```c
void (*f[])(char *)
```

**[]** 的优先级比 ***** 的优先级高，所以 f 首先是修饰了数组，然后跟 * 组合，就说明这个数组里面住的都是 指针，这些指针是什么呢，再出来看看就看到了，这个指针是一个函数，这个函数的参数是 char*，返回值是 void。

```c
#include <stdio.h>

void (*f[3])(char *);

void print(char* s) {
    printf("%s\n", s);
}

void main() {
    f[0] = print;
    (*f[0])("hello world");
}
```

输出：

```
hello world
```

### 再来段代码

```c
void (*p)();
(*(void(*) ())0)();
```

有个方法：**右左原则**，从 p 开始看，往右走直到遇到 **)**，再往左走遇到 **(**，(*p) 就可以看出 p 是一个指针，继续分析往右走，遇到 **()**，说明 p 指向一个 (void) 的函数，往左走，知道 p 指向的函数返回值是 void。

```c
#include <stdio.h>

void Func() {
    printf("Call Func!\n");
}

int main() {
    void(*p)();
    *(int*)&p = (int)Function;
    (*p)();
}
```

输出：

```
Call Func!
```

**然后继续分析`(*(void(*) ())0)()`;**

1. void(*)()：上面分析了这个是一个函数指针，只是把 p 去掉了而已
2. 把上面的 void (\*)() 用 PN 代替，上面的表达式变成 (*(PN)0)(); PN 后面有一个 0，想一下 (char)a; 这样的表达式，所以 *(PN)0 就是把 0 当成一个地址，强制转换为PN类型，用 * 这个钥匙取出这个地址区域的值
3. 把 (*(PN)0)() 替换成 PM，原来的表达式变成 PM()，这样看起来比较容易了吧，就是正常的函数调用

### 例子

```c
#include <stdio.h>
#include <string.h>

char* func1(char* p) {
    printf("%s\n", p);
    return p;
}

char* func2(char* p) {
    printf("%s\n", p);
    return p;
}

char* func3(char* p) {
    printf("%s\n", p);
    return p;
}

int main() {
    char* (*pf[3])(char * p);
    pf[0] = fun1;  // 可以直接用函数名
    pf[1] = &fun2; // 可以用函数名加上取地址符
    pf[2] = &fun3;

    pf[0]("fun1");
    pf[0]("fun2");
    pf[0]("fun3");
}
```