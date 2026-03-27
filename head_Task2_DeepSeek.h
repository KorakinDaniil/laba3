#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include <cctype>
#include <map>
#include <functional>
#include <stack>

// Узел дерева выражения
class TreeNode
{
public:
    char Operator;                      // оператор ('+', '-', '*', '/', '%', '^') или '\0' для операндов
    std::string Value;                  // число или 'x' (используется только если Operator == '\0')
    std::shared_ptr<TreeNode> LeftChild;
    std::shared_ptr<TreeNode> RightChild;

    // Конструктор для операнда (число или x)
    explicit TreeNode(const std::string& InValue)
        : Operator('\0')
        , Value(InValue)
        , LeftChild(nullptr)
        , RightChild(nullptr)
    {
    }

    // Конструктор для оператора
    TreeNode(char InOperator, std::shared_ptr<TreeNode> InLeft, std::shared_ptr<TreeNode> InRight)
        : Operator(InOperator)
        , Value("")
        , LeftChild(InLeft)
        , RightChild(InRight)
    {
    }
};

// Функции для определения приоритета операций
int GetPriority(char InOperator);

// Проверка, является ли строка оператором
bool IsOperator(char InCharacter);

// Проверка, является ли строка числом
bool IsNumber(const std::string& InToken);

// Преобразование инфиксного выражения в постфиксную форму (обратная польская запись)
std::vector<std::string> InfixToPostfix(const std::string& InExpression);

// Построение дерева из постфиксной записи
std::shared_ptr<TreeNode> BuildTree(const std::vector<std::string>& InPostfix);

// Вычисление значения выражения
int Evaluate(const std::shared_ptr<TreeNode>& InNode, int InX);

// Преобразование: замена A+x на x+A
std::shared_ptr<TreeNode> TransformAddCommute(std::shared_ptr<TreeNode> InNode);

// Вывод дерева в префиксной форме
void PrintPrefix(std::ostream& OutStream, const std::shared_ptr<TreeNode>& InNode);

// Вывод дерева в постфиксной форме
void PrintPostfix(std::ostream& OutStream, const std::shared_ptr<TreeNode>& InNode);

// Вывод дерева в инфиксной форме с избыточными скобками
void PrintInfixWithRedundantParens(std::ostream& OutStream, const std::shared_ptr<TreeNode>& InNode);

// Печать дерева
void PrintTree(const std::shared_ptr<TreeNode>& InNode, int InIndent = 0);

int Task2_DeepSeek();
