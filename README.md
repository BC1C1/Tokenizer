# Tokenizer — Python 子集解释器（最早的原型版）

> **这是最初的原型**：写它的时候还没搞懂编译原理，所以没有编译成中间码这一步 —— `Parser` 产出 AST 之后，由 `Executor` **直接遍历 AST 求值**（tree-walking interpreter）。
>
> 它后来演化成了两个后续版本（都在同一个"从零写 Python 解释器"的路线下）：
> - `qt\python` —— 搬到 Qt Creator，加界面，仍是 tree-walking；
> - `c\python` —— 因为调试太痛苦搬到 Visual Studio，改成 **compiler → Instruction → PVM** 的字节码虚拟机路线（那个版本的 UI 文件是在 Qt Creator 里设计好再搬过去的）。
>
> 所以这个仓库既是能跑的解释器，也是一条学习路线的**起点记录**。

## 管道

```
源码
  → Lexer::Tokenizer    词法分析（scanTokens）
  → AST::Parser         语法分析，产出 AST（parse）
  → Executor            遍历 AST 求值执行（run）
```

入口在 `Project1/text_main.cpp`：

```cpp
Tokenizer tokenizer;
auto tokens = tokenizer.scanTokens(code1);
Parser parser(tokens);
auto ast = parser.parse();
executor.run();
```

## 已实现的特性

| 类别 | 内容 |
|---|---|
| 字面量与基础类型 | `Int`、`Float`、`Bool`、`String`、`None`、`List` |
| 表达式 | 赋值、二元/一元运算、下标访问（`Index`）、属性访问（`Attribute`）、函数调用（`Call`） |
| 语句 | 变量声明、`if`、`while`、`for`、`return`、块（`Block`） |
| 函数 | 函数定义、调用、**默认参数** |
| 类 | `class` 定义、实例化、属性、类内方法；**类内定义函数 / 函数内嵌套定义类** |
| 内置函数 | `print`、`range`（`Bulidins.cpp`） |
| 内置类 | `BulidinClasses.cpp` |
| 对象模型 | `PObject` 为根：`PInt`/`PFloat`/`PBool`/`PStr`/`PList`/`PNone`/`PFunction`/`PClass`/`PInstance`，统一用智能指针传递 |
| 迭代 | `Iterator`、`ListIterator`（`for` 与内置函数都靠它） |

## 模块地图

| 目录/文件 | 职责 |
|---|---|
| `Tokenizer.cpp/.h` | 词法分析 |
| `Parser.cpp/.h`、`NodeDefine.h` | 语法分析与 AST 节点定义 |
| `Executor.cpp/.h` | 求值执行 |
| `PObject.cpp/.h` 及 `P*.cpp` | 运行时对象模型（Python 值的 C++ 表示） |
| `Bulidins.cpp/.h`、`BulidinClasses.cpp/.h` | 内置函数与内置类 |
| `Assignment` / `Binary` / `Unary` / `Call` / `Index` / `Attribute` | 各类表达式节点 |
| `Block` / `If` / `While` / `For` / `Return` / `FunctionDefine` / `Class` | 各类语句节点 |
| `text_main.cpp` | 控制台入口 |

## 怎么跑

- **环境**：Visual Studio 2022（工程 `Tokenizer.sln`，项目 `Project1`），C++，无第三方依赖。
- **入口**：`text_main.cpp` 里 `code1` 是硬编码的待执行源码片段 —— 改它、编译、运行即可。
- 编译产物（`x64/`、`.vs/`）不入库。

## 开发时间线

| 时间 | 进展（取自提交信息） |
|---|---|
| 2025-11-29 | 项目起步、`PList` |
| 2025-12-01 ~ 12-10 | 类定义、函数声明与调用、参数 `self` 的传入方式 |
| 2025-12-14 ~ 12-16 | `dot` 属性访问、`PClass`/`PInstance`、内置函数框架与 `print` |
| 2025-12-17 | 默认参数逻辑、内置函数 `range` |

共 21 次提交，跨约三周。

## 参考与说明

- 整体管道（tokenizer → parser → AST → executor）的**架构思路**参考了 *Crafting Interpreters*（Robert Nystrom）的 Lox 实现路线；**代码与对象系统是本仓库自己写的**，并在此基础上自行扩展了默认参数、嵌套类/函数定义、列表迭代器等 Lox 没有的东西。
- 写这个版本时对编译原理还不熟，所以它是**最直接的 tree-walking 形态**；后续版本（`qt\python`、`c\python`）才逐步引入界面与字节码虚拟机。

## 已知取舍

- 语言是 Python 的**子集**，不是完整实现；内置函数目前只有 `print` / `range`。
- 入口在 `text_main.cpp`，待执行代码写在里面。
