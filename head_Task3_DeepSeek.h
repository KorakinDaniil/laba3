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

// Структура для хранения ребра графа
struct Edge
{
    int Vertex1;    // Первая вершина ребра
    int Vertex2;    // Вторая вершина ребра

    Edge(int InVertex1, int InVertex2) : Vertex1(InVertex1), Vertex2(InVertex2) {}
};

int Task3_DeepSeek();
