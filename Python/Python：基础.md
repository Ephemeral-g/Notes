# 基础

## 一、链式比较

Python 支持链式比较，如 3 < 4 < 5 这种就叫链式比较，相当于 3 < 4 and 4 < 5。

## 二、循环

```python
while condition:
	# 条件成立时执行
else:
	# 条件不成立时执行
    
for i in add:
    # 条件成立时执行
else:
    # 条件不成立时执行
```

与 break 结合使用

```python
ss = "abcdefghijl,asdf"
for i in ss:
    if i == ',':
        #终止循环
        break
    print(i, end="")
else:
    print("执行 else 语句中的代码")
print("\n执行循环体外的代码")

# 输出：
# abcdefghijl
# 执行循环体外的代码
```

