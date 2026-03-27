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

int Task3_Gemini()
{
    const std::string InputFileName = "input2.txt";
    const std::string OutputFileName = "output2.txt";

    // Открытие входного файла
    std::ifstream InputFile(InputFileName);
    if (!InputFile.is_open())
    {
        std::cerr << "Ошибка: не удалось открыть файл " << InputFileName << std::endl;
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

    // Считываем матрицу смежности
    std::vector<std::vector<int>> AdjacencyMatrix(VertexCount, std::vector<int>(VertexCount));
    int EdgeCount = 0;

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

            // Считаем ребра только один раз (где i < j)
            if (i < j && AdjacencyMatrix[i][j] == 1)
            {
                ++EdgeCount;
            }
        }
    }
    InputFile.close();

    // Создаем матрицу инцидентности размером VertexCount x EdgeCount, заполненную нулями
    // Строки - вершины, столбцы - ребра
    std::vector<std::vector<int>> IncidenceMatrix(VertexCount, std::vector<int>(EdgeCount, 0));

    int CurrentEdgeIndex = 0;

    // Проходим по вершинам в порядке возрастания номеров
    for (int i = 0; i < VertexCount; ++i)
    {
        for (int j = i + 1; j < VertexCount; ++j)
        {
            if (AdjacencyMatrix[i][j] == 1)
            {
                // Ребро инцидентно вершинам i и j
                IncidenceMatrix[i][CurrentEdgeIndex] = 1;
                IncidenceMatrix[j][CurrentEdgeIndex] = 1;
                ++CurrentEdgeIndex;
            }
        }
    }

    // Запись результатов в выходной файл
    std::ofstream OutputFile(OutputFileName);
    if (!OutputFile.is_open())
    {
        std::cerr << "Ошибка: не удалось открыть файл " << OutputFileName << std::endl;
        return 1;
    }

    // Выводим размерность матрицы
    OutputFile << VertexCount << " " << EdgeCount << std::endl;

    // Выводим матрицу инцидентности
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

    std::cout << "\nКоличество вершин: " << VertexCount << std::endl;
    std::cout << "Количество ребер: " << EdgeCount << std::endl;

    std::cout << "\nМатрица инцидентности (" << VertexCount << "x" << EdgeCount << "):" << std::endl;

    std::cout << "\nМатрица инцидентности успешно построена и сохранена в " << OutputFileName << std::endl;

    return 0;
}
