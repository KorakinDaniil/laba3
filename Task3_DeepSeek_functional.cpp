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

// Структура для хранения ребра графа
struct Edge
{
    int Vertex1;    // Первая вершина ребра
    int Vertex2;    // Вторая вершина ребра

    Edge(int InVertex1, int InVertex2) : Vertex1(InVertex1), Vertex2(InVertex2) {}
};

int Task3_DeepSeek()
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

    // Построение списка ребер в заданном порядке
    // Порядок: сначала ребра, инцидентные вершине 1,
    // потом вершине 2 и т.д., для каждой вершины - по возрастанию номера второй вершины
    std::vector<Edge> Edges;

    for (int i = 0; i < VertexCount; ++i)
    {
        // Перебираем вершины j > i (чтобы не дублировать ребра)
        for (int j = i + 1; j < VertexCount; ++j)
        {
            if (AdjacencyMatrix[i][j] == 1)
            {
                Edges.push_back(Edge(i, j));
            }
        }
    }

    int EdgeCount = static_cast<int>(Edges.size());

    // Создание матрицы инцидентности (VertexCount x EdgeCount)
    std::vector<std::vector<int>> IncidenceMatrix(VertexCount, std::vector<int>(EdgeCount, 0));

    // Заполнение матрицы инцидентности
    for (int EdgeIndex = 0; EdgeIndex < EdgeCount; ++EdgeIndex)
    {
        int VertexU = Edges[EdgeIndex].Vertex1;
        int VertexV = Edges[EdgeIndex].Vertex2;

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

    // Вывод размера матрицы
    OutputFile << VertexCount << " " << EdgeCount << std::endl;

    // Вывод матрицы инцидентности
    for (int i = 0; i < VertexCount; ++i)
    {
        for (int j = 0; j < EdgeCount; ++j)
        {
            OutputFile << IncidenceMatrix[i][j];

            // Разделитель между элементами (пробел, кроме последнего)
            if (j < EdgeCount - 1)
            {
                OutputFile << " ";
            }
        }
        OutputFile << std::endl;
    }

    OutputFile.close();

    // Вывод информации в консоль
    std::cout << "Матрица инцидентности успешно построена." << std::endl;
    std::cout << "Количество вершин: " << VertexCount << std::endl;
    std::cout << "Количество ребер: " << EdgeCount << std::endl;
    std::cout << "Результат записан в файл " << OutputFileName << std::endl;

    return 0;
}
