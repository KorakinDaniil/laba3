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
int Priority(char InOperator)
{
    switch (InOperator)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
    case '%':
        return 2;
    case '^':
        return 3;
    default:
        return 0;
    }
}

// Проверка, является ли символ оператором
bool IsOperator2(char InChar)
{
    return InChar == '+' || InChar == '-' || InChar == '*' ||
        InChar == '/' || InChar == '%' || InChar == '^';
}

// Проверка, является ли символ цифрой
bool IsDigit(char InChar)
{
    return InChar >= '0' && InChar <= '9';
}

// Проверка, является ли узел переменной x
bool IsVariableX(const std::shared_ptr<Node>& InNode)
{
    return InNode && InNode->Operator == 'x' && !InNode->LeftChild && !InNode->RightChild;
}

// Проверка, является ли узел операцией сложения
bool IsPlus(const std::shared_ptr<Node>& InNode)
{
    return InNode && InNode->Operator == '+';
}

// Построение дерева из инфиксного выражения (алгоритм Дейкстры - Shunting Yard)
std::shared_ptr<Node> BuildTree2(const std::string& InExpression)
{
    std::stack<std::shared_ptr<Node>> ValuesStack;  // Стек операндов (узлов)
    std::stack<char> OperatorsStack;                // Стек операторов

    for (size_t i = 0; i < InExpression.length(); ++i)
    {
        char CurrentChar = InExpression[i];

        // Пропускаем пробелы
        if (CurrentChar == ' ' || CurrentChar == '\t')
        {
            continue;
        }

        // Число
        if (IsDigit(CurrentChar))
        {
            int Number = 0;
            while (i < InExpression.length() && IsDigit(InExpression[i]))
            {
                Number = Number * 10 + (InExpression[i] - '0');
                ++i;
            }
            --i;
            ValuesStack.push(std::make_shared<Node>(Number));
        }
        // Переменная x
        else if (CurrentChar == 'x')
        {
            ValuesStack.push(std::make_shared<Node>('x'));
        }
        // Открывающая скобка
        else if (CurrentChar == '(')
        {
            OperatorsStack.push(CurrentChar);
        }
        // Закрывающая скобка
        else if (CurrentChar == ')')
        {
            while (!OperatorsStack.empty() && OperatorsStack.top() != '(')
            {
                char OperatorChar = OperatorsStack.top();
                OperatorsStack.pop();

                auto RightOperand = ValuesStack.top();
                ValuesStack.pop();
                auto LeftOperand = ValuesStack.top();
                ValuesStack.pop();

                auto NewNode = std::make_shared<Node>(OperatorChar);
                NewNode->LeftChild = LeftOperand;
                NewNode->RightChild = RightOperand;
                ValuesStack.push(NewNode);
            }
            // Удаляем открывающую скобку
            if (!OperatorsStack.empty())
            {
                OperatorsStack.pop();
            }
        }
        // Оператор
        else if (IsOperator2(CurrentChar))
        {
            // Для левоассоциативности: пока приоритет текущего <= приоритета верхнего в стеке
            while (!OperatorsStack.empty() && OperatorsStack.top() != '(')
            {
                char TopOperator = OperatorsStack.top();
                int TopPriority = Priority(TopOperator);
                int CurrentPriority = Priority(CurrentChar);

                if (TopPriority >= CurrentPriority)
                {
                    OperatorsStack.pop();

                    auto RightOperand = ValuesStack.top();
                    ValuesStack.pop();
                    auto LeftOperand = ValuesStack.top();
                    ValuesStack.pop();

                    auto NewNode = std::make_shared<Node>(TopOperator);
                    NewNode->LeftChild = LeftOperand;
                    NewNode->RightChild = RightOperand;
                    ValuesStack.push(NewNode);
                }
                else
                {
                    break;
                }
            }
            OperatorsStack.push(CurrentChar);
        }
    }

    // Обрабатываем оставшиеся операторы
    while (!OperatorsStack.empty())
    {
        char OperatorChar = OperatorsStack.top();
        OperatorsStack.pop();

        auto RightOperand = ValuesStack.top();
        ValuesStack.pop();
        auto LeftOperand = ValuesStack.top();
        ValuesStack.pop();

        auto NewNode = std::make_shared<Node>(OperatorChar);
        NewNode->LeftChild = LeftOperand;
        NewNode->RightChild = RightOperand;
        ValuesStack.push(NewNode);
    }

    return ValuesStack.empty() ? nullptr : ValuesStack.top();
}

// Вычисление значения выражения
int Evaluate2(const std::shared_ptr<Node>& InNode, int InX)
{
    if (!InNode)
    {
        return 0;
    }

    // Лист - число или переменная
    if (!InNode->LeftChild && !InNode->RightChild)
    {
        if (InNode->Operator == 'x')
        {
            return InX;
        }
        return InNode->Value;
    }

    int LeftValue = Evaluate2(InNode->LeftChild, InX);
    int RightValue = Evaluate2(InNode->RightChild, InX);

    switch (InNode->Operator)
    {
    case '+':
        return LeftValue + RightValue;
    case '-':
        return LeftValue - RightValue;
    case '*':
        return LeftValue * RightValue;
    case '/':
        return LeftValue / RightValue;
    case '%':
        return LeftValue % RightValue;
    case '^':
    {
        int Result = 1;
        for (int i = 0; i < RightValue; ++i)
        {
            Result *= LeftValue;
        }
        return Result;
    }
    default:
        return 0;
    }
}

// Преобразование дерева: A+x → x+A
void TransformTree(std::shared_ptr<Node>& InOutNode)
{
    if (!InOutNode)
    {
        return;
    }

    // Сначала рекурсивно обрабатываем поддеревья
    TransformTree(InOutNode->LeftChild);
    TransformTree(InOutNode->RightChild);

    // Проверяем условие: текущий узел - '+', левое поддерево не x, правое - x
    if (IsPlus(InOutNode) && !IsVariableX(InOutNode->LeftChild) && IsVariableX(InOutNode->RightChild))
    {
        // Меняем местами левое и правое поддеревья
        std::swap(InOutNode->LeftChild, InOutNode->RightChild);
    }
}

// Префиксная форма (корень-левое-правое)
void PrintPrefix2(const std::shared_ptr<Node>& InNode, std::ostream& OutStream)
{
    if (!InNode)
    {
        return;
    }

    if (InNode->Operator == '\0')
    {
        OutStream << InNode->Value;
    }
    else
    {
        OutStream << InNode->Operator;
    }
    OutStream << " ";

    PrintPrefix2(InNode->LeftChild, OutStream);
    PrintPrefix2(InNode->RightChild, OutStream);
}

// Постфиксная форма (левое-правое-корень)
void PrintPostfix2(const std::shared_ptr<Node>& InNode, std::ostream& OutStream)
{
    if (!InNode)
    {
        return;
    }

    PrintPostfix2(InNode->LeftChild, OutStream);
    PrintPostfix2(InNode->RightChild, OutStream);

    if (InNode->Operator == '\0')
    {
        OutStream << InNode->Value;
    }
    else
    {
        OutStream << InNode->Operator;
    }
    OutStream << " ";
}

// Инфиксная форма с избыточными скобками
void PrintInfixWithBrackets(const std::shared_ptr<Node>& InNode, std::ostream& OutStream)
{
    if (!InNode)
    {
        return;
    }

    // Лист
    if (!InNode->LeftChild && !InNode->RightChild)
    {
        if (InNode->Operator == 'x')
        {
            OutStream << "x";
        }
        else
        {
            OutStream << InNode->Value;
        }
        return;
    }

    OutStream << "(";
    PrintInfixWithBrackets(InNode->LeftChild, OutStream);
    OutStream << InNode->Operator;
    PrintInfixWithBrackets(InNode->RightChild, OutStream);
    OutStream << ")";
}

// Печать дерева
void PrintTree2(std::shared_ptr<Node> Root, int Indent = 0)
{
    if (!Root) return;

    for (int i = 0; i < Indent; i++) std::cout << "  ";

    if (Root->Operator == '\0')
    {
        std::cout << Root->Value << std::endl;
    }
    else
    {
        std::cout << Root->Operator << std::endl;
    }

    PrintTree2(Root->LeftChild, Indent + 1);
    PrintTree2(Root->RightChild, Indent + 1);
}

int Task2_Kimi()
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

    std::cout << "Исходное выражение: " << Expression << std::endl;

    // Построение дерева
    auto Root = BuildTree2(Expression);

    if (!Root)
    {
        std::cerr << "Ошибка: не удалось построить дерево выражения!" << std::endl;
        return 1;
    }

    std::cout << "\nДерево выражения:" << std::endl;
    PrintTree2(Root);

    // Ввод значения x
    int XValue = 0;
    std::cout << "\nВведите значение x: ";
    std::cin >> XValue;

    // Вычисление значения
    int Result = Evaluate2(Root, XValue);

    // Преобразование дерева
    TransformTree(Root);

    // Запись результатов в файл
    std::ofstream OutputFile(OutputFileName);
    if (!OutputFile.is_open())
    {
        std::cerr << "Ошибка открытия файла " << OutputFileName << std::endl;
        return 1;
    }

    OutputFile << "Значение выражения при x = " << XValue << ": " << Result << std::endl;
    OutputFile << std::endl;

    OutputFile << "Префиксная форма: ";
    PrintPrefix2(Root, OutputFile);
    OutputFile << std::endl;

    OutputFile << "Постфиксная форма: ";
    PrintPostfix2(Root, OutputFile);
    OutputFile << std::endl;

    OutputFile << "Инфиксная форма с избыточными скобками: ";
    PrintInfixWithBrackets(Root, OutputFile);
    OutputFile << std::endl;

    OutputFile.close();

    std::cout << "\nРезультаты записаны в файл " << OutputFileName << std::endl;

    return 0;
}
