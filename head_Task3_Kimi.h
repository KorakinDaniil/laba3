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

// Структура для хранения ребра графа
struct Edge2
{
    int VertexU;    // Первая вершина ребра (0-based)
    int VertexV;    // Вторая вершина ребра (0-based)

    Edge2(int InU, int InV) : VertexU(InU), VertexV(InV) {}
};

int Task3_Kimi();
