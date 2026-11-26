#include <gtest/gtest.h>
#include <algorithm>
#include "Vector.h"

// Конструктор по умолчанию: пустой вектор
TEST(Constructors, Default) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0u);
    EXPECT_EQ(v.capacity(), 0u);
    EXPECT_TRUE(v.empty());
}

// Конструктор по размеру: нужный размер, элементы равны 0
TEST(Constructors, BySize) {
    Vector<int> v(4);
    EXPECT_EQ(v.size(), 4u);
    for (std::size_t i = 0; i < v.size(); ++i) EXPECT_EQ(v[i], 0);
}

// Конструктор из списка: элементы совпадают со списком
TEST(Constructors, InitializerList) {
    Vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[2], 3);
}

// Конструктор копирования: копия не зависит от оригинала
TEST(Constructors, Copy) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b(a);
    b[0] = 100;
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(b[0], 100);
}

// Конструктор перемещения: данные у нового, источник пуст
TEST(Constructors, Move) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b(std::move(a));
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[1], 2);
    EXPECT_EQ(a.size(), 0u);
}

// Копирующее присваивание: элементы скопированы, объекты независимы
TEST(Assignment, Copy) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b = {9};
    b = a;
    a[0] = 100;
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 1);
}

// Копирующее присваивание в пустой вектор из большего
TEST(Assignment, CopyToEmpty) {
    Vector<int> a = {1, 2, 3, 4, 5};
    Vector<int> b;
    b = a;
    EXPECT_EQ(b.size(), 5u);
    EXPECT_EQ(b[4], 5);
}

// Присваивание самому себе: данные не теряются
TEST(Assignment, CopySelf) {
    Vector<int> a = {1, 2, 3};
    Vector<int>& ref = a;
    a = ref;
    EXPECT_EQ(a.size(), 3u);
    EXPECT_EQ(a[2], 3);
}

// Перемещающее присваивание: данные у приёмника, источник пуст
TEST(Assignment, Move) {
    Vector<int> a = {1, 2, 3};
    Vector<int> b;
    b = std::move(a);
    EXPECT_EQ(b.size(), 3u);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(a.size(), 0u);
}

// operator[]: чтение и запись по индексу
TEST(Access, Index) {
    Vector<int> v = {1, 2, 3};
    v[1] = 20;
    EXPECT_EQ(v[1], 20);
}

// front и back: первый и последний элементы
TEST(Access, FrontBack) {
    Vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.front(), 1);
    EXPECT_EQ(v.back(), 3);
    v.back() = 30;
    EXPECT_EQ(v[2], 30);
}

// size, capacity, empty: значения после добавления
TEST(State, SizeCapacityEmpty) {
    Vector<int> v;
    EXPECT_TRUE(v.empty());
    v.push_back(1);
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(v.size(), 1u);
    EXPECT_GE(v.capacity(), v.size());
}

// push_back (lvalue): элемент добавляется в конец
TEST(Modify, PushBackLvalue) {
    Vector<int> v;
    int x = 7;
    v.push_back(x);
    EXPECT_EQ(v.back(), 7);
    EXPECT_EQ(v.size(), 1u);
}

// push_back (rvalue): элемент добавляется в конец
TEST(Modify, PushBackRvalue) {
    Vector<int> v;
    v.push_back(5);
    EXPECT_EQ(v.back(), 5);
}

// push_back: ёмкость растёт, старые элементы сохраняются
TEST(Modify, PushBackGrowth) {
    Vector<int> v;
    for (int i = 0; i < 100; ++i) v.push_back(i);
    EXPECT_EQ(v.size(), 100u);
    EXPECT_GE(v.capacity(), 100u);
    for (int i = 0; i < 100; ++i) EXPECT_EQ(v[i], i);
}

// push_back своего элемента при полном буфере
TEST(Modify, PushBackOwnElement) {
    Vector<int> v = {1, 2, 3};
    v.push_back(v[0]);
    EXPECT_EQ(v.size(), 4u);
    EXPECT_EQ(v.back(), 1);
}

// pop_back: возвращает последний элемент и уменьшает размер
TEST(Modify, PopBack) {
    Vector<int> v = {1, 2, 3};
    EXPECT_EQ(v.pop_back(), 3);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v.back(), 2);
}

// pop_back на пустом векторе: исключение
TEST(Modify, PopBackEmpty) {
    Vector<int> v;
    EXPECT_THROW(v.pop_back(), std::out_of_range);
}

// clear: размер 0, ёмкость сохраняется
TEST(Modify, Clear) {
    Vector<int> v = {1, 2, 3};
    std::size_t cap = v.capacity();
    v.clear();
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.capacity(), cap);
}

// reserve: ёмкость увеличивается, размер и данные не меняются
TEST(Modify, Reserve) {
    Vector<int> v = {1, 2, 3};
    v.reserve(50);
    EXPECT_GE(v.capacity(), 50u);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_EQ(v[2], 3);
}

// reserve меньше текущей ёмкости: ничего не меняется
TEST(Modify, ReserveSmaller) {
    Vector<int> v = {1, 2, 3};
    std::size_t cap = v.capacity();
    v.reserve(1);
    EXPECT_EQ(v.capacity(), cap);
}

// resize (увеличение): новые элементы равны 0, старые целы
TEST(Modify, ResizeGrow) {
    Vector<int> v = {1, 2};
    v.resize(5);
    EXPECT_EQ(v.size(), 5u);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[4], 0);
}

// resize (уменьшение): размер уменьшается, начало сохраняется
TEST(Modify, ResizeShrink) {
    Vector<int> v = {1, 2, 3, 4};
    v.resize(2);
    EXPECT_EQ(v.size(), 2u);
    EXPECT_EQ(v.back(), 2);
}

// begin/end: обход через итераторы по порядку
TEST(Iterator, Traversal) {
    Vector<int> v = {1, 2, 3};
    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it) EXPECT_EQ(*it, expected++);
    EXPECT_EQ(expected, 4);
}

// begin == end у пустого вектора
TEST(Iterator, EmptyRange) {
    Vector<int> v;
    EXPECT_TRUE(v.begin() == v.end());
}

// Итератор: инкремент, декремент, арифметика, разность, сравнение
TEST(Iterator, Operations) {
    Vector<int> v = {10, 20, 30, 40};
    auto it = v.begin();
    EXPECT_EQ(*(it + 2), 30);
    EXPECT_EQ(it[3], 40);
    ++it;
    EXPECT_EQ(*it, 20);
    it++;
    EXPECT_EQ(*it, 30);
    --it;
    EXPECT_EQ(*it, 20);
    it += 2;
    EXPECT_EQ(*it, 40);
    it -= 3;
    EXPECT_EQ(*it, 10);
    EXPECT_EQ(v.end() - v.begin(), 4);
    EXPECT_TRUE(v.begin() < v.end());
}

// std::sort: сортировка по возрастанию
TEST(Algorithms, Sort) {
    Vector<int> v = {5, 1, 4, 2, 3};
    std::sort(v.begin(), v.end());
    for (std::size_t i = 0; i < v.size(); ++i) EXPECT_EQ(v[i], static_cast<int>(i + 1));
}

// std::find: находит существующий элемент, для отсутствующего возвращает end
TEST(Algorithms, Find) {
    Vector<int> v = {1, 2, 3};
    auto it = std::find(v.begin(), v.end(), 2);
    EXPECT_NE(it, v.end());
    EXPECT_EQ(*it, 2);
    EXPECT_EQ(std::find(v.begin(), v.end(), 99), v.end());
}

// std::for_each: изменяет каждый элемент
TEST(Algorithms, ForEach) {
    Vector<int> v = {1, 2, 3};
    std::for_each(v.begin(), v.end(), [](int& x) { x *= 2; });
    EXPECT_EQ(v[0], 2);
    EXPECT_EQ(v[2], 6);
}

// Работа с другим числовым типом
TEST(Types, Double) {
    Vector<double> v = {1.5, 2.5};
    v.push_back(3.5);
    EXPECT_EQ(v.size(), 3u);
    EXPECT_DOUBLE_EQ(v.back(), 3.5);
}

// Концепт Numeric: числа проходят, char и bool нет
TEST(Concepts, Numeric) {
    static_assert(Numeric<int>);
    static_assert(Numeric<double>);
    static_assert(Numeric<unsigned long>);
    static_assert(!Numeric<char>);
    static_assert(!Numeric<bool>);
    SUCCEED();
}

// Итератор соответствует требованиям random access итератора C++20
TEST(Concepts, IteratorCategory) {
    static_assert(std::random_access_iterator<VectorIterator<int>>);
    SUCCEED();
}