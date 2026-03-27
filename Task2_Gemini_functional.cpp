/*
В текстовом файле с именем FN1 дано арифметическое выражение в инфиксной
форме. В выражении могут использоваться операции: сложение(+), вычитание(-), умножение(*),
деление нацело(/), остаток от деления(%), возведение в степень(^), а так же целые числа из промежутка [1; 30] и переменная x. Для операции возведения в степень показатель степени неотрицательное целое число. Постройте дерево выражения. После этого вычислите значение выражения при заданном значении переменной x и выведите результат в текстовый файл с именем FN2.
Преобразуйте дерево, заменив все поддеревья вида A+x на x+A, где A - некоторое поддерево, а x
- переменная. Распечатайте дерево после преобразования в файл FN2 в префиксной и постфиксной форме, а так же в инфиксной форме с избыточными скобками. При наличии нескольких подряд идущих одинаковых операций дерево должно строиться по правилу: операции одинакового
приоритета вычисляются по порядку слева направо. Иными словами, выражение 2+3+4+5, например, должно трактоваться как ((2+3)+4)+5, и не может трактоваться как (2+3)+(4+5) или
2+(3+(4+5)). Результаты всех вычислений, включая промежуточные, принадлежат типу int.
*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <cctype>
#include <map>
#include <functional>
#include <stack>
#include <algorithm>

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
void PrintTree3(Node2* Root, int Indent = 0)
{
    if (!Root) return;

    // Сдвиг для иерархии
    for (int i = 0; i < Indent; i++) std::cout << "  ";

    // Вывод значения (оператор, число или x)
    std::cout << Root->Value << std::endl;

    PrintTree3(Root->LeftChild, Indent + 1);
    PrintTree3(Root->RightChild, Indent + 1);
}

// Приоритеты операций
int GetPriority2(const std::string& InOperator)
{
    if (InOperator == "+" || InOperator == "-")
    {
        return 1;
    }
    if (InOperator == "*" || InOperator == "/" || InOperator == "%")
    {
        return 2;
    }
    if (InOperator == "^")
    {
        return 3;
    }
    return 0;
}

// Проверка, является ли строковым числом или x
bool IsOperand(const std::string& InToken)
{
    if (InToken.empty())
    {
        return false;
    }

    // Проверка на число
    if (std::isdigit(static_cast<unsigned char>(InToken[0])))
    {
        return true;
    }

    // Проверка на переменную x
    return InToken == "x";
}

// Проверка, является ли токен числом
bool IsNumber2(const std::string& InToken)
{
    if (InToken.empty())
    {
        return false;
    }

    for (char Character : InToken)
    {
        if (!std::isdigit(static_cast<unsigned char>(Character)))
        {
            return false;
        }
    }
    return true;
}

// Разбиение строки на токены
std::vector<std::string> Tokenize(const std::string& InExpression)
{
    std::vector<std::string> Tokens;

    for (size_t i = 0; i < InExpression.length(); ++i)
    {
        // Пропускаем пробелы
        if (std::isspace(static_cast<unsigned char>(InExpression[i])))
        {
            continue;
        }

        // Сбор числа
        if (std::isdigit(static_cast<unsigned char>(InExpression[i])))
        {
            std::string Number;
            while (i < InExpression.length() && std::isdigit(static_cast<unsigned char>(InExpression[i])))
            {
                Number += InExpression[i];
                ++i;
            }
            Tokens.push_back(Number);
            --i; // Корректируем индекс после выхода из цикла
        }
        else
        {
            // Оператор, скобка или переменная
            Tokens.push_back(std::string(1, InExpression[i]));
        }
    }

    return Tokens;
}

// Преобразование инфиксного выражения в постфиксную форму(ОПЗ)
std::vector<std::string> InfixToPostfix2(const std::vector<std::string>& InTokens)
{
    std::vector<std::string> Postfix;
    std::stack<std::string> Operators;

    for (const std::string& Token : InTokens)
    {
        // Операнд: добавляем в выходную строку
        if (IsOperand(Token))
        {
            Postfix.push_back(Token);
        }
        // Открывающая скобка: помещаем в стек
        else if (Token == "(")
        {
            Operators.push(Token);
        }
        // Закрывающая скобка: выталкиваем операторы до открывающей скобки
        else if (Token == ")")
        {
            while (!Operators.empty() && Operators.top() != "(")
            {
                Postfix.push_back(Operators.top());
                Operators.pop();
            }
            // Удаляем открывающую скобку
            if (!Operators.empty())
            {
                Operators.pop();
            }
        }
        // Оператор
        else
        {
            // Выталкиваем операторы с большим или равным приоритетом
            // Для ^ (правоассоциативный) - условие: приоритет >, а не >=
            while (!Operators.empty() && Operators.top() != "(" &&
                (GetPriority2(Operators.top()) > GetPriority2(Token) ||
                    (GetPriority2(Operators.top()) == GetPriority2(Token) && Token != "^")))
            {
                Postfix.push_back(Operators.top());
                Operators.pop();
            }
            Operators.push(Token);
        }
    }

    // Выталкиваем оставшиеся операторы
    while (!Operators.empty())
    {
        Postfix.push_back(Operators.top());
        Operators.pop();
    }

    return Postfix;
}

// Построение дерева из постфиксной формы
Node2* BuildTree3(const std::vector<std::string>& InPostfix)
{
    std::stack<Node2*> Stack;

    for (const std::string& Token : InPostfix)
    {
        if (IsOperand(Token))
        {
            // Операнд: создаем узел и помещаем в стек
            Stack.push(new Node2(Token));
        }
        else
        {
            // Оператор: извлекаем два операнда из стека
            Node2* NewNode = new Node2(Token);
            NewNode->RightChild = Stack.top();
            Stack.pop();
            NewNode->LeftChild = Stack.top();
            Stack.pop();
            Stack.push(NewNode);
        }
    }

    return Stack.empty() ? nullptr : Stack.top();
}

// Вычисление значения
int Evaluate3(Node2* InNode, int InX)
{
    if (!InNode)
    {
        return 0;
    }

    // Переменная x
    if (InNode->Value == "x")
    {
        return InX;
    }

    // Число
    if (IsNumber2(InNode->Value))
    {
        return std::stoi(InNode->Value);
    }

    int LeftValue = Evaluate3(InNode->LeftChild, InX);
    int RightValue = Evaluate3(InNode->RightChild, InX);

    if (InNode->Value == "+") return LeftValue + RightValue;
    if (InNode->Value == "-") return LeftValue - RightValue;
    if (InNode->Value == "*") return LeftValue * RightValue;
    if (InNode->Value == "/") return LeftValue / RightValue;
    if (InNode->Value == "%") return LeftValue % RightValue;
    if (InNode->Value == "^") return static_cast<int>(std::pow(LeftValue, RightValue));

    return 0;
}

// Проверка, является ли узел переменной x
bool IsVariableX2(Node2* InNode)
{
    return InNode && InNode->Value == "x" && !InNode->LeftChild && !InNode->RightChild;
}

// Трансформация: A + x -> x + A
void TransformTree2(Node2* InOutNode)
{
    if (!InOutNode)
    {
        return;
    }

    // Сначала рекурсивно обрабатываем поддеревья
    TransformTree2(InOutNode->LeftChild);
    TransformTree2(InOutNode->RightChild);

    // Проверяем условие: текущий узел - '+', левое поддерево - x, правое - не x
    // По условию задачи: A+x → x+A
    if (InOutNode->Value == "+" && IsVariableX2(InOutNode->LeftChild) && !IsVariableX2(InOutNode->RightChild))
    {
        // Меняем местами левое и правое поддеревья
        std::swap(InOutNode->LeftChild, InOutNode->RightChild);
    }
}

// Вывод дерева в префиксной форме (корень-левое-правое)
void PrintPrefix3(Node2* InNode, std::ofstream& OutStream)
{
    if (!InNode)
    {
        return;
    }

    OutStream << InNode->Value << " ";
    PrintPrefix3(InNode->LeftChild, OutStream);
    PrintPrefix3(InNode->RightChild, OutStream);
}

// Вывод дерева в постфиксной форме(левое - правое - корень)
void PrintPostfix3(Node2* InNode, std::ofstream& OutStream)
{
    if (!InNode)
    {
        return;
    }

    PrintPostfix3(InNode->LeftChild, OutStream);
    PrintPostfix3(InNode->RightChild, OutStream);
    OutStream << InNode->Value << " ";
}

// Вывод дерева в инфиксной форме с избыточными скобками
void PrintInfixWithFullParentheses(Node2* InNode, std::ofstream& OutStream)
{
    if (!InNode)
    {
        return;
    }

    // Лист: число или переменная - выводим без скобок
    if (IsOperand(InNode->Value))
    {
        OutStream << InNode->Value;
        return;
    }

    // Оператор: обрамляем скобками
    OutStream << "(";
    PrintInfixWithFullParentheses(InNode->LeftChild, OutStream);
    OutStream << InNode->Value;
    PrintInfixWithFullParentheses(InNode->RightChild, OutStream);
    OutStream << ")";
}

int Task2_Gemini()
{
    const std::string InputFileName = "input.txt";
    const std::string OutputFileName = "output.txt";

    // Чтение выражения из файла
    std::ifstream InputFile(InputFileName);
    if (!InputFile.is_open())
    {
        std::cerr << "Ошибка открытия файла " << InputFileName << std::endl;
        return 1;
    }

    std::string Expression;
    std::getline(InputFile, Expression);
    InputFile.close();

    // Проверка на пустое выражение
    if (Expression.empty())
    {
        std::cerr << "Ошибка: файл " << InputFileName << " пуст!" << std::endl;
        return 1;
    }

    // Удаляем пробелы из выражения
    Expression.erase(std::remove_if(Expression.begin(), Expression.end(),
        [](unsigned char ch) { return std::isspace(ch); }),
        Expression.end());

    // Лексический анализ
    std::vector<std::string> Tokens = Tokenize(Expression);

    // Преобразование в постфиксную форму
    std::vector<std::string> Postfix = InfixToPostfix2(Tokens);

    // Построение дерева
    Node2* Root = BuildTree3(Postfix);

    if (!Root)
    {
        std::cerr << "Ошибка: не удалось построить дерево выражения!" << std::endl;
        return 1;
    }

    std::cout << "\nДерево выражения:" << std::endl;
    PrintTree3(Root);

    // Ввод значения x
    int XValue = 0;
    std::cout << "Введите значение x: ";
    std::cin >> XValue;

    // Вычисление значения
    int Result = Evaluate3(Root, XValue);

    // Преобразование дерева
    TransformTree2(Root);

    // Запись результатов в файл
    std::ofstream OutputFile(OutputFileName);
    if (!OutputFile.is_open())
    {
        std::cerr << "Ошибка открытия файла " << OutputFileName << std::endl;
        return 1;
    }

    OutputFile << "Результат вычисления: " << Result << std::endl;
    OutputFile << std::endl;

    OutputFile << "Префиксная форма: ";
    PrintPrefix3(Root, OutputFile);
    OutputFile << std::endl;

    OutputFile << "Постфиксная форма: ";
    PrintPostfix3(Root, OutputFile);
    OutputFile << std::endl;

    OutputFile << "Инфиксная (избыточные скобки): ";
    PrintInfixWithFullParentheses(Root, OutputFile);
    OutputFile << std::endl;

    OutputFile.close();

    std::cout << "Результат записан в файл " << OutputFileName << std::endl;

    return 0;
}
