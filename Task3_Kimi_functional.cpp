/*
Дано описание неориентированного графа в текстовом файле с именем FileName1. в виде
матрицы смежности. Первая строка файла содержит количество вершин графа (n), а следующие
n строк содержат матрицу смежности (a), a[i][j]=0, если ребра между вершинами i и j не существует. Построить матрицу инцидентности данного графа и вывести ее в файл с именем
FileName2. Для справки: матрица инцидентности (b) имеет размер n x m, m - число ребер графа,
b[i][j]=1, если ребро j инцидентно вершине i, в противном случае b[i][j]=0. Нумерацию ребер осуществлять в следующем порядке: сначала ребра, инцидентные вершине номер 1, потом ребра инцидентные вершине номер 2 и т.д. до вершины номер n. Ребра, инцидентные вершине с номером
i перечислять в порядке возрастания номера второй вершины, инцидентной данному ребру. При
выводе в первой строке указать размер матрицы инцидентности: числа n и m, а в следующих n
строках разместить матрицу инцидентности.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

//FIX_ME: using namespace std; запрещено стандартом
//using namespace std;

// Структура для хранения ребра графа
struct Edge2
{
    int VertexU;    // Первая вершина ребра (0-based)
    int VertexV;    // Вторая вершина ребра (0-based)

    Edge2(int InU, int InV) : VertexU(InU), VertexV(InV) {}
};

int Task3_Kimi()
{
    const std::string InputFileName = "input2.txt";
    const std::string OutputFileName = "output2.txt";

    // Открытие входного файла
    std::ifstream InputFile(InputFileName);
    if (!InputFile.is_open())
    {
        std::cerr << "Ошибка открытия файла " << InputFileName << std::endl;
        return 1;
    }

    // Чтение количества вершин
    int VertexCount = 0;
    InputFile >> VertexCount;

    // Проверка корректности ввода
    if (VertexCount <= 0)
    {
        std::cerr << "Ошибка: количество вершин должно быть положительным числом!" << std::endl;
        InputFile.close();
        return 1;
    }

    // Чтение матрицы смежности
    std::vector<std::vector<int>> AdjacencyMatrix(VertexCount, std::vector<int>(VertexCount));

    for (int i = 0; i < VertexCount; ++i)
    {
        for (int j = 0; j < VertexCount; ++j)
        {
            if (!(InputFile >> AdjacencyMatrix[i][j]))
            {
                std::cerr << "Ошибка: некорректный формат матрицы смежности!" << std::endl;
                InputFile.close();
                return 1;
            }

            // Проверка корректности значения (0 или 1)
            if (AdjacencyMatrix[i][j] != 0 && AdjacencyMatrix[i][j] != 1)
            {
                std::cerr << "Ошибка: элементы матрицы смежности должны быть 0 или 1!" << std::endl;
                InputFile.close();
                return 1;
            }
        }
    }
    InputFile.close();

    // Подсчет количества ребер (только верхний треугольник, так как граф неориентированный)
    int EdgeCount = 0;
    for (int i = 0; i < VertexCount; ++i)
    {
        for (int j = i + 1; j < VertexCount; ++j)
        {
            if (AdjacencyMatrix[i][j] == 1)
            {
                ++EdgeCount;
            }
        }
    }

    // Создание списка ребер согласно правилам нумерации
    // Порядок: сначала ребра, инцидентные вершине 1, потом вершине 2 и т.д.
    // Для вершины i ребра перечисляются в порядке возрастания номера второй вершины
    std::vector<Edge2> Edges;

    for (int i = 0; i < VertexCount; ++i)
    {
        for (int j = i + 1; j < VertexCount; ++j)
        {
            if (AdjacencyMatrix[i][j] == 1)
            {
                Edges.push_back(Edge2(i, j));
            }
        }
    }

    // Построение матрицы инцидентности размером VertexCount x EdgeCount
    // IncidenceMatrix[i][j] = 1, если ребро j инцидентно вершине i
    std::vector<std::vector<int>> IncidenceMatrix(VertexCount, std::vector<int>(EdgeCount, 0));

    for (int EdgeIndex = 0; EdgeIndex < EdgeCount; ++EdgeIndex)
    {
        int VertexU = Edges[EdgeIndex].VertexU;
        int VertexV = Edges[EdgeIndex].VertexV;

        IncidenceMatrix[VertexU][EdgeIndex] = 1;
        IncidenceMatrix[VertexV][EdgeIndex] = 1;
    }

    // Запись результатов в выходной файл
    std::ofstream OutputFile(OutputFileName);
    if (!OutputFile.is_open())
    {
        std::cerr << "Ошибка открытия файла " << OutputFileName << " для записи" << std::endl;
        return 1;
    }

    // Первая строка: размер матрицы VertexCount EdgeCount
    OutputFile << VertexCount << " " << EdgeCount << std::endl;

    // Следующие строки: матрица инцидентности
    for (int i = 0; i < VertexCount; ++i)
    {
        for (int j = 0; j < EdgeCount; ++j)
        {
            OutputFile << IncidenceMatrix[i][j];

            if (j < EdgeCount - 1)
            {
                OutputFile << " ";
            }
        }
        OutputFile << std::endl;
    }

    OutputFile.close();

    // Вывод информации на экран для проверки
    std::cout << "Матрица смежности (" << VertexCount << "x" << VertexCount << "):" << std::endl;

    std::cout << "\nНайдено ребер: " << EdgeCount << std::endl;
    std::cout << "Список ребер (нумерация с 1):" << std::endl;
    for (int i = 0; i < EdgeCount; ++i)
    {
        std::cout << "Ребро " << (i + 1) << ": (" << (Edges[i].VertexU + 1)
            << ", " << (Edges[i].VertexV + 1) << ")" << std::endl;
    }

    std::cout << "\nМатрица инцидентности (" << VertexCount << "x" << EdgeCount << "):" << std::endl;

    std::cout << "\nРезультат записан в файл " << OutputFileName << std::endl;

    return 0;
}
