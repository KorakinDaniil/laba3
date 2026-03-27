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

int Task1_Kimi()
{
    std::string InputWord;
    std::cout << "Введите слово: ";
    std::cin >> InputWord;

    // Проверка на пустой ввод
    if (InputWord.empty())
    {
        std::cout << "Ошибка: введено пустое слово!" << std::endl;
        return 1;
    }

    std::string OriginalWord = InputWord;
    std::string ShuffledWord = InputWord;
    int Attempts = 0;

    // Инициализация генератора случайных чисел
    std::mt19937 RandomGenerator(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << std::endl;

    // Переставляем буквы до тех пор, пока не получим исходное слово
    do
    {
        std::shuffle(ShuffledWord.begin(), ShuffledWord.end(), RandomGenerator);
        std::cout << ShuffledWord << std::endl;
        ++Attempts;
    } while (ShuffledWord != OriginalWord);

    std::cout << Attempts << " попыток" << std::endl;

    return 0;
}
