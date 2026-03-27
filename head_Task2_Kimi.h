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

//FIX_ME: using namespace std; запрещено стандартом
//using namespace std;

// Узел дерева выражения
struct Node
{
    char Operator;                      // Операция: '+', '-', '*', '/', '%', '^', 'x' или '\0' для числа
    int Value;                          // Значение числа (если Operator == '\0')
    std::shared_ptr<Node> LeftChild;
    std::shared_ptr<Node> RightChild;

    // Конструктор для оператора или переменной
    explicit Node(char InOperator)
        : Operator(InOperator)
        , Value(0)
        , LeftChild(nullptr)
        , RightChild(nullptr)
    {
    }

    // Конструктор для числа
    explicit Node(int InValue)
        : Operator('\0')
        , Value(InValue)
        , LeftChild(nullptr)
        , RightChild(nullptr)
    {
    }
};

// Приоритет операций
int Priority(char InOperator);

// Проверка, является ли символ оператором
bool IsOperator2(char InChar);

// Проверка, является ли символ цифрой
bool IsDigit(char InChar);

// Проверка, является ли узел переменной x
bool IsVariableX(const std::shared_ptr<Node>& InNode);

// Построение дерева из инфиксного выражения (алгоритм Дейкстры - Shunting Yard)
std::shared_ptr<Node> BuildTree2(const std::string& InExpression);

// Вычисление значения выражения
int Evaluate2(const std::shared_ptr<Node>& InNode, int InX);

// Преобразование дерева: A+x → x+A
void TransformTree(std::shared_ptr<Node>& InOutNode);

// Префиксная форма (корень-левое-правое)
void PrintPrefix2(const std::shared_ptr<Node>& InNode, std::ostream& OutStream);

// Постфиксная форма (левое-правое-корень)
void PrintPostfix2(const std::shared_ptr<Node>& InNode, std::ostream& OutStream);

// Инфиксная форма с избыточными скобками
void PrintInfixWithBrackets(const std::shared_ptr<Node>& InNode, std::ostream& OutStream);

// Печать дерева
void PrintTree2(std::shared_ptr<Node> Root, int Indent = 0);

int Task2_Kimi();
