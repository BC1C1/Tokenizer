#include <iostream>
#include "TypeDefine.h"
using AST::Parser;
using exe::Executor;
using Lexer::Tokenizer;
using std::string;

int main() {
    string code1 = R"(
# 3. while循环
i = 0
while i < 3:
    print("while循环：", i)
    i = i + 1

# 4. for循环（简化版，基于次数迭代）
for j in [0, 1, 2]:
    print("for循环：", j)

# 1. 基础赋值
a = 10
b = "hello"
c = a + 5

# 2. if-elif-else 分支
if a > 15:
    print("a大于15")
elif a == 10:
    print("a等于10")
    print("c的值是：", c)
else:
    print("a小于10")

# 5. 函数定义+调用（含参数+闭包）
def add(x, y):
    return x + y

def outer(n):
    def inner(m):
        return n * m
    return inner

# 调用函数
sum_val = add(3, 4)
print("3+4的结果：", sum_val)

# 闭包调用
multi = outer(5)
res = multi(6)
print("5*6的结果：", res)

# 6. 综合测试（循环+条件+函数）
def check_num(num):
    if num % 2 == 0:
        print(num, "是偶数")
    else:
        print(num, "是奇数")

k = 0
while k < 4:
    check_num(k)
    k = k + 1
)";
    string code = R"(
def add(a, b):
    return a + b
c = 1
d = 2
e = add(c, d)
print(e)
)";

    try {
        // 分词
        Tokenizer tokenizer;
        auto tokens = tokenizer.scanTokens(code);
        printf("<---------- tokens ---------->\n");
        for (auto token : tokens) {
            std::cout << token << "\n";
        }

        // 解析
        Parser parser(tokens);
        auto ast = parser.parse();
        if (!ast) {
            std::cerr << "解析失败：AST 为空" << std::endl;
            return 1;
        }
        std::cout << "\n<---------- parse success ---------->\n";

        // 执行
        Executor executor(ast);
        executor.run();

        std::cout << "\n=== 测试跑通！===" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "\n=== 测试失败：" << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "\n=== 测试失败：未知异常 ===" << std::endl;
        return 1;
    }
}