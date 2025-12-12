#include <iostream>
#include "TypeDefine.h"
using AST::Parser;
using exe::Executor;
using Lexer::Tokenizer;
using std::string;

int main() {
    string code1 = R"(
class adder:
    def __init__(self, a, b):
        self.a = a
        self.b = b
    def get_result(self):
        return self.a + self.b
adder(1, 2)
print(adder.get_result())
)";
    string code = R"(
# 修正版：无列表操作的学生成绩管理系统
print("=== 学生成绩管理系统 ===")

# 基础数据初始化（使用单个变量模拟成绩录入）
total_score = 0
student_count = 0
highest = -1
lowest = 101
passed = 0
failed = 0

# 模拟录入5名学生成绩（通过循环步长控制）
current_step = 0
score = 0

while current_step < 5:
    # 手动指定每个步骤的成绩（无列表）
    if current_step == 0:
        score = 65
    elif current_step == 1:
        score = 88
    elif current_step == 2:
        score = 92
    elif current_step == 3:
        score = 59
    else:
        score = 77

    # 统计数据更新
    student_count = student_count + 1
    print(student_count)
    total_score = total_score + score
    
    # 最高分/最低分更新
    if score > highest:
        highest = score
    if score < lowest:
        lowest = score
    
    # 及格/不及格统计
    if score >= 60:
        passed = passed + 1
    else:
        failed = failed + 1
    
    current_step = current_step + 1

# 计算平均分
print(student_count)
average = total_score / student_count

# 输出基础统计结果
print("------------------------")
print("统计结果：")
print("总人数：", student_count)
print("总分：", total_score)
print("平均分：", average)
print("最高分：", highest)
print("最低分：", lowest)
print("及格人数：", passed)
print("不及格人数：", failed)

# 等级评定（嵌套条件判断）
print("------------------------")
print("等级分布：")
excellent = 0
good = 0
medium = 0
pass_level = 0

# 重新遍历成绩评定等级（无列表，通过步长再次循环）
current_step = 0
while current_step < 5:
    if current_step == 0:
        score = 65
    elif current_step == 1:
        score = 88
    elif current_step == 2:
        score = 92
    elif current_step == 3:
        score = 59
    else:
        score = 77

    # 等级判断
    if score >= 90:
        excellent = excellent + 1
    elif score >= 80:
        good = good + 1
    elif score >= 70:
        medium = medium + 1
    elif score >= 60:
        pass_level = pass_level + 1
    
    current_step = current_step + 1

print("优秀（90+）：", excellent)
print("良好（80-89）：", good)
print("中等（70-79）：", medium)
print("及格（60-69）：", pass_level)
print("不及格：", failed)

# 附加功能：指定位置成绩查询（无列表索引，直接判断步长）
print("------------------------")
query_step = 2  # 查询第3名学生（对应步长2）
query_score = 0
if query_step == 0:
    query_score = 65
elif query_step == 1:
    query_score = 88
elif query_step == 2:
    query_score = 92
elif query_step == 3:
    query_score = 59
elif query_step == 4:
    query_score = 77

if query_step >= 0 and query_step < 5:
    print("第", query_step + 1, "名学生成绩：", query_score)
else:
    print("查询索引无效")

print("------------------------")
print("=== 测试完成 ===")
)";

    try {
        // 分词
        Tokenizer tokenizer;
        auto tokens = tokenizer.scanTokens(code1);
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