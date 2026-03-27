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

// Структура узла дерева
struct Node2
{
    std::string Value;          // Значение узла: оператор, число или 'x'
    Node2* LeftChild;            // Указатель на левое поддерево
    Node2* RightChild;           // Указатель на правое поддерево

    explicit Node2(const std::string& InValue)
        : Value(InValue)
        , LeftChild(nullptr)
        , RightChild(nullptr)
    {
    }
};

// Печать дерева
void PrintTree3(Node2* Root, int Indent = 0);

// Приоритеты операций
int GetPriority2(const std::string& InOperator);

// Проверка, является ли строковым числом или x
bool IsOperand(const std::string& InToken);

// Проверка, является ли токен числом
bool IsNumber2(const std::string& InToken);

// Разбиение строки на токены
std::vector<std::string> Tokenize(const std::string& InExpression);

// Преобразование инфиксного выражения в постфиксную форму(ОПЗ)
std::vector<std::string> InfixToPostfix2(const std::vector<std::string>& InTokens);

// Построение дерева из постфиксной формы
Node2* BuildTree3(const std::vector<std::string>& InPostfix);

// Вычисление значения
int Evaluate3(Node2* InNode, int InX);

// Проверка, является ли узел переменной x
bool IsVariableX2(Node2* InNode);

// Трансформация: A + x -> x + A
void TransformTree2(Node2* InOutNode);

// Вывод дерева в префиксной форме (корень-левое-правое)
void PrintPrefix3(Node2* InNode, std::ofstream& OutStream);

// Вывод дерева в постфиксной форме(левое - правое - корень)
void PrintPostfix3(Node2* InNode, std::ofstream& OutStream);

// Вывод дерева в инфиксной форме с избыточными скобками
void PrintInfixWithFullParentheses(Node2* InNode, std::ofstream& OutStream);

int Task2_Gemini();
