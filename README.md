## x86_asm_intrin_demo

This project is the demo for x86 assembly and intrinsic optimization.

### 一、支持平台

Windows, Linux, macOS(Apple Silicon M1)

### 二、各平台编译说明

1、Windows

x86_asm_demo.sln

2、Linux

make platform=x86_32

3、macOS

make target_plat=mac platform=arm64

### 三、性能比较

1、macOS平台：纯C算法循环500000次，耗时28ms，aarch64 neon intrinsic耗时1ms。

2、X86平台：

（1）windows平台：纯C算法循环500000次，耗时11ms，ssse3 asm耗时4ms, ssse3 intrinsic耗时1ms。

（2）Linux平台：纯C算法循环500000次，耗时21ms,ssse3 asm耗时3ms, ssse3 intrinsic耗时2ms。

### 四、疑问与发现

1、根据不同系统平台的测试结果可知：

intrinsic汇编的优化效率并不一定低于纯汇编的优化效率，有时intrinsic函数经过编译器的优化可能效率还更高一些；不过在这个测试demo中dc计算比较简单，或许在复杂函数中纯汇编的优化效率比intrinsic更好一些。
