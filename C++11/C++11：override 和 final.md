## 一、重载

一个类 A 中声明的虚函数 fun 在其派生类 B 中再次被定义，且 B 中的函数 fun 跟 A 中 fun 的原型一样（函数名、参数列表等一样），那么就称 B 重载（overload ）了 A 的 fun 函数。对于任何 B 类型的变量，调用成员函数 fun 都是调用了 B 重载的版本。而如果同时有 A 的派生类 C，却并没有重载 A 的 fun 函数，那么调用成员函数 fun 则会调用 A 中的版本。这在 C++ 中就实现多态。

在传统 C++ 中，经常容易发生意外重载虚函数的事情。例如：

```c++
class Base {
    virtual void foo();
};
W
class Derived : public Base {
    void foo();
};
```

Derived::foo 可能并不是程序员尝试重载虚函数，只是恰好加入了一个具有相同名字的函数。另一个可能的情形是，当基类的虚函数被删除后，子类拥有旧的函数就不再重载该虚拟函数并摇身一变成为了一个普通的类方法，这将造成灾难性的后果。

C++11 引入了 **override** 和 **final** 这两个关键字来防止上述情形的发生。

## 二、override

C++ 中重载有一个特点，就是对于基类声明为 virtual 的函数，之后的重载版本都不需要再声明该重载函数为 virtual，即使在派生类中声明了 virtual，该关键字也是编译器可以忽略的，另外一点是，在 C++ 中有的虚函数会“跨层”，没有在父类中声明的接口有可能是祖先的虚函数接口。导致代码难以阅读。

于是引入了虚函数描述符 override，如果派生类在虚函数声明时使用了 override 描述符，那么该函数必须重载其基类中的同名函数，否则代码无法编译通过。

当重载虚函数时，引入 override 关键字将显式的告知编译器进行重载，编译器将检查基类是否存在这样的虚函数，否则将无法通过编译。

```c++
class Base {
	virtual void Turing() = 0;
    virtual void Dijkstra() = 0;
    virtual void VNeumann(int g) = 0;
    virtual void DKnuth() const;
    void Print();
};

class DerivedMid : public Base {
};    

class DerivedTop : public DeriveMid {
    void Turing() override;
    void Dikjstra() override;  // 无法通过编译，拼写错误，并非重载
    void VNeumann(double g) override;  // 无法通过编译，参数不一致，并非重载
    void DKnuth() override;  // 无法通过编译，常量性不一致，并非重载
    void Print() override;  // 无法通过编译，非虚函数重载
};                            
```

## 三、final

final 则是为了防止类被继续继承以及终止虚函数继续重载引入的。

final 关键字来阻止函数继续重写， 使派生类不可覆盖它所修饰的虚函数，阻止派生类重载虚函数。final 可以在派生过程中任意地阻止一个接口的可重载性。

```c++
class Object {
    virtual void fun() = 0;
};

class Base : public Object {
    virtual void fun() final;  // 声明为 final
};

class Derived1 final : public Base {
};// 合法

class Derived2 : public Derived1 {
};  // 非法, Derived1 已 final

class Derived3 : public Base {
    void fun(); // 非法, fun 已 final
};
```

final 关键字都是用于描述一个派生类的。那么基类中的虚函数是否可以使用 final 关键字呢？答案是肯定的，不过这样将使用该虚函数无法被重载，也就失去了虚函数的意义。如果不想成员函数被重载，程序员可以直接将该成员函数定义为非虚的。而 final 通常只在继承关系的“中途”终止派生类的重载中有意义。从接口派生的角度而言，final 可以在派生过程中任意地阻止一个接口的可重载性，这就给面向对象的程序员带来了更大的控制力。