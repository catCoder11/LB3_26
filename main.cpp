#include <utility>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "Vector.h"


int main() {
    Vector a = {5, 6, 8};

    //Присваивание
    Vector b = {7, 7, 7, 7};
    std::cout << a[1] << *(b.begin()) << std::endl;
    b = std::move(a);
    for (auto& el : b) {
        std::cout << el << std::endl;
    }

    // Добавление/удаление элементов
    b.push_back(9);
    int val = 15;
    std::cout << "Добавлен " << b.back() << ", размер " << b.size() << std::endl;
    b.push_back(val);
    std::cout << "Добавлен " << b.back() << ", размер " << b.size() << std::endl;
    std::cout << "Удалён " << b.pop_back() << ", размер " << b.size() << std::endl;

    // Доступ к элементам и изменение по индексу
    b[0] = 10;
    std::cout << "front: " << b.front() << ", back: " << b.back() << std::endl;

    // Копирование
    Vector c = b;
    c[1] = 20;
    std::cout << "c копия: ";
    for (auto& el : c) {
        std::cout << el << " ";
    }
    std::cout << std::endl << "оригинал b: ";
    for (auto& el : b) {
        std::cout << el << " ";
    }
    std::cout << std::endl;

    Vector<int> d;
    d = c;
    std::cout << "После d = c, размер d: " << d.size() << std::endl;

    // Добавление (lvalue), изменение размера и полная очистка
    d.resize(2);
    std::cout << "Новый размер d: " << d.size() << std::endl;

    d.clear();
    std::cout << "После clear(), empty: " << std::boolalpha << d.empty() << std::endl;


    // Обход по итераторам
    Vector m = {4,2,5,1,3};
    std::cout << "Вектор m:" << std::endl;
    for (auto it = m.begin(); it != m.end(); it++) {
        std::cout << *it << std::endl;
    }
    std::sort(m.begin(), m.end());
    std::cout << "Отсортированный вектор m:" << std::endl;
    for (auto it = m.begin(); it != m.end(); it++) {
        std::cout << *it << std::endl;
    }
    return 0;
}
