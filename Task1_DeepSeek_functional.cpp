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
    
int Task1_DeepSeek()
{
    std::string Word;
    std::cout << "Введите слово: ";
    std::cin >> Word;

    // Проверка на пустой ввод
    if (Word.empty())
    {
        std::cout << "Ошибка: введено пустое слово!" << std::endl;
        return 1;
    }

    std::string ShuffledWord = Word;
    int Attempts = 0;

    //FIX_ME: Инициализация генератора случайных чисел
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    unsigned int Seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 Generator(Seed);

    do
    {
        // Перемешиваем буквы в случайном порядке
        std::shuffle(ShuffledWord.begin(), ShuffledWord.end(), Generator);

        // Выводим полученное слово
        std::cout << ShuffledWord << std::endl;
        ++Attempts;

    } while (ShuffledWord != Word);

    std::cout << Attempts << " попыток" << std::endl;

    return 0;
}
