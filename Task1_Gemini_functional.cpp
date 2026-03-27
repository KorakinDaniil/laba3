/* Ввести с клавиатуры любое слово. Используя генерацию случайных чисел, переставить
буквы этого слова в случайном порядке. Делать это до тех пор, пока полученное слово не совпадёт
с начальным словом. Выводить слово после каждой перестановки и посчитать общее количество
выведенных слов (не считая исходного). Пример выполнения программы:
Введите слово: корова
воакро
вокроа
ароовк
краоов
крваоо
оокавр
ооквра
вкраоо
корова
9 попыток */

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <ctime>

int Task1_Gemini()
{
    // Устанавливаем корректное отображение кириллицы
    setlocale(LC_ALL, "Russian");

    std::string Original;
    std::cout << "Введите слово: ";
    std::cin >> Original;

    // Проверка на пустой ввод
    if (Original.empty())
    {
        std::cout << "Ошибка: введено пустое слово!" << std::endl;
        return 1;
    }

    std::string Current = Original;
    int Attempts = 0;

    //FIX_ME: Непонятные названия переменных
    //std::random_device rd;
    //std::mt19937 g(rd());
    std::random_device RandomDevice;
    std::mt19937 Generator(RandomDevice());

    // Цикл выполняется, пока перемешанное слово не станет равным исходному
    do
    {
        // Перемешиваем буквы в строке
        std::shuffle(Current.begin(), Current.end(), Generator);

        // Выводим результат текущей перестановки
        std::cout << Current << std::endl;

        ++Attempts;
    } while (Current != Original);

    std::cout << Attempts << " попыток" << std::endl;

    return 0;
}
