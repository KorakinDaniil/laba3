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
int GetPriority(char InOperator)
{
    if (InOperator == '^') return 4;
    if (InOperator == '*' || InOperator == '/' || InOperator == '%') return 3;
    if (InOperator == '+' || InOperator == '-') return 2;
    return 1;
}

// Проверка, является ли строка оператором
bool IsOperator(char InCharacter)
{
    return InCharacter == '+' || InCharacter == '-' || InCharacter == '*' ||
        InCharacter == '/' || InCharacter == '%' || InCharacter == '^';
}

// Проверка, является ли строка числом
bool IsNumber(const std::string& InToken)
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

// Преобразование инфиксного выражения в постфиксную форму (обратная польская запись)
std::vector<std::string> InfixToPostfix(const std::string& InExpression)
{
    std::vector<std::string> Output;
    std::vector<char> Operators;

    std::string CurrentToken;

    for (size_t i = 0; i <= InExpression.length(); ++i)
    {
        char CurrentChar = (i < InExpression.length()) ? InExpression[i] : ' ';

        // Сбор операнда (число или переменная x)
        if (i < InExpression.length() && (std::isdigit(static_cast<unsigned char>(CurrentChar)) || CurrentChar == 'x'))
        {
            CurrentToken += CurrentChar;
        }
        else
        {
            // Добавляем собранный операнд в выходной вектор
            if (!CurrentToken.empty())
            {
                Output.push_back(CurrentToken);
                CurrentToken.clear();
            }

            if (i < InExpression.length())
            {
                if (CurrentChar == '(')
                {
                    Operators.push_back(CurrentChar);
                }
                else if (CurrentChar == ')')
                {
                    // Выталкиваем операторы до открывающей скобки
                    while (!Operators.empty() && Operators.back() != '(')
                    {
                        Output.push_back(std::string(1, Operators.back()));
                        Operators.pop_back();
                    }
                    // Удаляем открывающую скобку
                    if (!Operators.empty() && Operators.back() == '(')
                    {
                        Operators.pop_back();
                    }
                }
                else if (IsOperator(CurrentChar))
                {
                    // Выталкиваем операторы с большим или равным приоритетом
                    while (!Operators.empty() && Operators.back() != '(' &&
                        GetPriority(Operators.back()) >= GetPriority(CurrentChar))
                    {
                        Output.push_back(std::string(1, Operators.back()));
                        Operators.pop_back();
                    }
                    Operators.push_back(CurrentChar);
                }
            }
        }
    }

    // Выталкиваем оставшиеся операторы
    while (!Operators.empty())
    {
        Output.push_back(std::string(1, Operators.back()));
        Operators.pop_back();
    }

    return Output;
}

// Построение дерева из постфиксной записи
std::shared_ptr<TreeNode> BuildTree(const std::vector<std::string>& InPostfix)
{
    std::vector<std::shared_ptr<TreeNode>> Stack;

    for (const std::string& Token : InPostfix)
    {
        if (Token.length() == 1 && IsOperator(Token[0]))
        {
            // Оператор: извлекаем два операнда из стека
            auto RightOperand = Stack.back();
            Stack.pop_back();
            auto LeftOperand = Stack.back();
            Stack.pop_back();

            Stack.push_back(std::make_shared<TreeNode>(Token[0], LeftOperand, RightOperand));
        }
        else
        {
            // Операнд: помещаем в стек
            Stack.push_back(std::make_shared<TreeNode>(Token));
        }
    }

    return Stack.empty() ? nullptr : Stack.back();
}

// Вычисление значения выражения
int Evaluate(const std::shared_ptr<TreeNode>& InNode, int InX)
{
    if (!InNode)
    {
        return 0;
    }

    // Операнд (число или переменная)
    if (InNode->Operator == '\0')
    {
        if (InNode->Value == "x")
        {
            return InX;
        }
        return std::stoi(InNode->Value);
    }

    int LeftValue = Evaluate(InNode->LeftChild, InX);
    int RightValue = Evaluate(InNode->RightChild, InX);

    switch (InNode->Operator)
    {
    case '+': return LeftValue + RightValue;
    case '-': return LeftValue - RightValue;
    case '*': return LeftValue * RightValue;
    case '/': return LeftValue / RightValue;
    case '%': return LeftValue % RightValue;
    case '^': return static_cast<int>(std::pow(LeftValue, RightValue));
    default: return 0;
    }
}

// Преобразование: замена A+x на x+A
std::shared_ptr<TreeNode> TransformAddCommute(std::shared_ptr<TreeNode> InNode)
{
    if (!InNode)
    {
        return nullptr;
    }

    // Рекурсивно преобразуем поддеревья
    InNode->LeftChild = TransformAddCommute(InNode->LeftChild);
    InNode->RightChild = TransformAddCommute(InNode->RightChild);

    // Если узел - операция сложения и правый операнд - переменная x
    if (InNode->Operator == '+' && InNode->RightChild &&
        InNode->RightChild->Operator == '\0' && InNode->RightChild->Value == "x")
    {
        // Меняем местами левое и правое поддеревья
        std::swap(InNode->LeftChild, InNode->RightChild);
    }

    return InNode;
}

// Вывод дерева в префиксной форме
void PrintPrefix(std::ostream& OutStream, const std::shared_ptr<TreeNode>& InNode)
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
        if (InNode->LeftChild || InNode->RightChild)
        {
            OutStream << " ";
            PrintPrefix(OutStream, InNode->LeftChild);
            if (InNode->RightChild)
            {
                OutStream << " ";
                PrintPrefix(OutStream, InNode->RightChild);
            }
        }
    }
}

// Вывод дерева в постфиксной форме
void PrintPostfix(std::ostream& OutStream, const std::shared_ptr<TreeNode>& InNode)
{
    if (!InNode)
    {
        return;
    }

    if (InNode->LeftChild)
    {
        PrintPostfix(OutStream, InNode->LeftChild);
        OutStream << " ";
    }

    if (InNode->RightChild)
    {
        PrintPostfix(OutStream, InNode->RightChild);
        OutStream << " ";
    }

    if (InNode->Operator == '\0')
    {
        OutStream << InNode->Value;
    }
    else
    {
        OutStream << InNode->Operator;
    }
}

// Вывод дерева в инфиксной форме с избыточными скобками
void PrintInfixWithRedundantParens(std::ostream& OutStream, const std::shared_ptr<TreeNode>& InNode)
{
    if (!InNode)
    {
        return;
    }

    // Лист: число или переменная
    if (InNode->Operator == '\0')
    {
        OutStream << InNode->Value;
        return;
    }

    // Оператор: обрамляем скобками
    OutStream << "(";
    PrintInfixWithRedundantParens(OutStream, InNode->LeftChild);
    OutStream << InNode->Operator;
    PrintInfixWithRedundantParens(OutStream, InNode->RightChild);
    OutStream << ")";
}

// Печать дерева
void PrintTree(const std::shared_ptr<TreeNode>& InNode, int InIndent = 0)
{
    if (!InNode)
    {
        return;
    }

    for (int i = 0; i < InIndent; ++i)
    {
        std::cout << "  ";
    }

    if (InNode->Operator == '\0')
    {
        std::cout << InNode->Value << std::endl;
    }
    else
    {
        std::cout << InNode->Operator << std::endl;
    }

    PrintTree(InNode->LeftChild, InIndent + 1);
    PrintTree(InNode->RightChild, InIndent + 1);
}

// Основная функция
int Task2_DeepSeek()
{
    setlocale(LC_ALL, "Russian");

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

    // Запрашиваем значение x
    int XValue = 0;
    std::cout << "Введите значение x: ";
    std::cin >> XValue;

    // Построение дерева
    std::vector<std::string> Postfix = InfixToPostfix(Expression);
    auto Tree = BuildTree(Postfix);

    if (!Tree)
    {
        std::cerr << "Ошибка: не удалось построить дерево выражения!" << std::endl;
        return 1;
    }

    // Вычисление значения
    int Result = Evaluate(Tree, XValue);

    // Вывод исходного дерева в консоль
    std::cout << "\nИсходное дерево выражения:" << std::endl;
    PrintTree(Tree);

    // Преобразование дерева
    auto TransformedTree = TransformAddCommute(Tree);

    // Запись результатов в файл
    std::ofstream OutputFile(OutputFileName);
    if (!OutputFile.is_open())
    {
        std::cerr << "Ошибка открытия файла " << OutputFileName << " для записи" << std::endl;
        return 1;
    }

    OutputFile << "Результат вычисления при x = " << XValue << ": " << Result << std::endl;
    OutputFile << std::endl;

    OutputFile << std::endl;

    OutputFile << "Префиксная форма: ";
    PrintPrefix(OutputFile, TransformedTree);
    OutputFile << std::endl;

    OutputFile << "Постфиксная форма: ";
    PrintPostfix(OutputFile, TransformedTree);
    OutputFile << std::endl;

    OutputFile << "Инфиксная форма (с избыточными скобками): ";
    PrintInfixWithRedundantParens(OutputFile, TransformedTree);
    OutputFile << std::endl;

    OutputFile.close();

    std::cout << "\nРезультат записан в файл " << OutputFileName << std::endl;
    std::cout << "Значение выражения: " << Result << std::endl;

    return 0;
}
