# dolphin3
The kernel for the purpose of the experiment mainly verifies whether the kernel is capable of tracking taint by itself.
这是一个以实验为目的的内核，主要验证内核是否可以通过他自己来tracking taint。

## 内核运行的平台
### 内核将会运行在arm平台下，有以下几点原因：
1. arm平台的汇编语言比较简洁，X86平台的汇编语言有许多种写法，at&t和intel语法，gcc貌似不支持编译at&t语法，所以每次都需要指定NASM来编译，相对麻烦
2. arm平台的boot相对比较简洁，x86平台在boot下需要做许多操作，因为这个项目不再用作学习用途，所以使用简单arm汇编更加好。
3. 不支持多平台也是为了简化系统，提高完成速度


### 内核将会采用C++编写
C++相对比C有面向对象的支持，同时也可以学习一下嵌入式裸机下如何对类进行重载

### 内核代码命名规范
代码将会采用小驼峰命名法，但是函数名还是采用Linux内核的写法，比如get_value()。因为我认为，类在大部分情况下看作一个复杂的变量，可以被认为是加强版的结构体，变量采用小驼峰，类采用大驼峰，这样很直观，只有函数有别于类和变量。类中私有变量需要在前面加入下划线。
example:

    class MemoryMap
    {
    private:
        int _a;
        int _b;
        int _c;
    public:
        ...
    }
    int threadCount;
    void link_page()
    {
    ...
    }
常量必须大写并且加入下划线：

    #define ALL_INODE_SIZE 1024;
    #define SUPER_BLOCK_ADDR 0xabcd;

函数内部变量与全局变量。
很多时候命名空间内的全局变量不可以被修改，而函数内部变量生命周期短，所以我决定函数内部变量尽量采用缩写，因为很多时候阅读者不需要仔细看函数内部的逻辑代码，只需要看函数注释和返回值就大致知道函数做了什么操作。所以函数内部的变量可以缩写，全局变量同样采用小驼峰但是不能缩写，并以_结束。这样很直观，在编程的时候也能提醒程序员函数内部变量生命周期短，不要总是返回函数内部变量地址。

    uint64_t pageCount_;
    uint64_t iNodeSize_;
    uint64_t taskFeature_;
    
    void foo2()
    {
        int iSize;
        int pgCount;
        int tkFeature;
    }

类模版，函数模版采用标准写法

函数返回值的时候禁止调用新函数或者类。例如

    return foo(a/2+3, b*4) + foo2(c, d);

这种写法非常愚蠢，因为在调试的时候并不能知道这个foo+foo2到底是什么值，需要返回到最上层才知道，非常不利于调试，应该写成：

    float rt = foo(a/2+3, b*4) + foo2(c, d);
    return rt;

这样写很直观，我们能快速知道两个函数的返回类型并且在调试的时候也能快速知道rt的值。

### 代码编译方法
代码将采用CMake构建工程，因为CMake使用广泛并且简洁易懂，现在chatgpt能够很好的部署和调整。
