#include <utility>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <concepts>


template<typename T>
concept Numeric =
(std::integral<T> || std::floating_point<T>)
&& !std::same_as<T, bool>
&& !std::same_as<T, char>
&& !std::same_as<T, wchar_t>
&& !std::same_as<T, char8_t>
&& !std::same_as<T, char16_t>
&& !std::same_as<T, char32_t>;


template <Numeric T>
class Vector;

template <Numeric T>
class VectorIterator;

template <Numeric T>
class Vector
{
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
public:
    using iterator = VectorIterator<T>;

    // конструкторы //


    // конструктор по умолчанию
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    // конструктор по размеру
    explicit Vector(std::size_t size) : data_(nullptr), size_(size), capacity_(size) {
        if (capacity_ > 0) data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));
        for (std::size_t i = 0; i < size_; ++i) {
            new (data_ + i) T();
        }
    }

    // конструктор при передаче списка элементов
    Vector(std::initializer_list<T> list) : data_(nullptr), size_(list.size()), capacity_(list.size()) {
        if (capacity_ > 0) data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));
        const T* lb = list.begin();
        for (std::size_t i = 0; i < size_; ++i) {
            new (data_ + i) T(lb[i]);
        }
    }

    // конструктор копирования
    Vector(const Vector<T>& vec) : data_(nullptr), size_(vec.size()), capacity_(vec.size()) {
        if (capacity_ > 0) data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));
        for (std::size_t i = 0; i < size_; ++i) {
            new (data_ + i) T(vec.data_[i]);
        }
    }

    // конструктор перемещения
    Vector(Vector<T>&& vec) noexcept : data_(vec.data_), size_(vec.size_), capacity_(vec.capacity_) {
        vec.data_ = nullptr;
        vec.size_ = 0;
        vec.capacity_ = 0;
    }

    // конструктор копирования присваивания
    Vector& operator=(const Vector<T>& vec) {
        if (this == &vec) {
            return *this;
        }
        T* new_data;
        if (vec.capacity_ > 0) new_data = static_cast<T*>(::operator new(vec.capacity_ * sizeof(T))); else new_data = nullptr;
        for (std::size_t i = 0; i < vec.size_; ++i) {
            new (new_data + i) T(vec.data_[i]);
        }

        for (std::size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        ::operator delete(data_);
        data_ = new_data;
        size_ = vec.size_;
        capacity_ = vec.capacity_;

        return *this;
    }

    // конструктор перемещения присваивания
    Vector& operator=(Vector&& vec) noexcept {
        if (this == &vec) {
            return *this;
        }

        for (std::size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        ::operator delete(data_);

        data_ = vec.data_;
        size_ = vec.size_;
        capacity_ = vec.capacity_;

        vec.data_ = nullptr;
        vec.size_ = 0;
        vec.capacity_ = 0;

        return *this;
    }

    // Деструктор //
    ~Vector() {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        ::operator delete(data_);
    }

    // обращение к элементу по индексу
    const T& operator[](size_t index) const {
        if (index < 0 || index >= size_) throw std::out_of_range("Выход за пределы вектора");
        return data_[index];
    }

    T& operator[](size_t index) {
        if (index < 0 || index >= size_) throw std::out_of_range("Выход за пределы вектора");
        return data_[index];
    }

    
    std::size_t size() const {return size_;}
    std::size_t capacity() const {return capacity_;}
    bool empty() const {return size_ == 0;}
    

    // добавление и удаление элементов //

    void push_back(const T& el) {
        if (size_ == capacity_) {
            T copy(el);
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
            new (data_ + size_) T(std::move(copy));
        }
        else {new (data_ + size_) T(el);}
        
        ++size_;
    }

    void push_back(T&& el) {
        if (size_ == capacity_) {
            T copy(std::move(el));
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
            new (data_ + size_) T(std::move(copy));
        }
        else {
            new (data_ + size_) T(std::move(el));
        }
        ++size_;
    }

    T pop_back() {
        if (empty()) throw std::out_of_range("Vector::pop_back(): vector is empty");
        size_--;
        T el = std::move(*end());
        end()->~T();
        return el;
    }

    void clear() {
        if (size_ == 0) return;
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }


    void reserve(std::size_t new_capacity) {
        if (new_capacity <= capacity_) return;
        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        for (std::size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
        return;
    }

    void resize(std::size_t new_size) {
        if (new_size > size_) {
            reserve(new_size);
            for (std::size_t i = size_; i < new_size; ++i) {
                new (data_ + i) T();
            }
        }
        else if (new_size < size_) {
            for (std::size_t i = new_size; i < size_; ++i) {
                data_[i].~T();
            }
        }
        size_ = new_size;
        return;
    }

    // begin, end, front, back
    T& front() {return data_[0];}
    const T& front() const {return data_[0];}
    
    T& back() {return data_[size_-1];}
    const T& back() const {return data_[size_-1];}
    
    iterator begin() {return iterator(data_);}

    iterator end() {return iterator(data_)+size_;}
};


template <Numeric T>
class VectorIterator
{
    private:
    T* ptr_;
    public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::random_access_iterator_tag;
    VectorIterator() : ptr_(nullptr) {}
    explicit VectorIterator(T* ptr) : ptr_(ptr) {}
    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }
    reference operator[](difference_type n) const { return ptr_[n]; }

    VectorIterator& operator++() { ++ptr_; return *this; }
    VectorIterator operator++(int) { VectorIterator tmp = *this; ++ptr_; return tmp; }
    VectorIterator& operator--() { --ptr_; return *this; }
    VectorIterator operator--(int) { VectorIterator tmp = *this; --ptr_; return tmp; }

    VectorIterator& operator+=(difference_type n) { ptr_ += n; return *this; }
    VectorIterator& operator-=(difference_type n) { ptr_ -= n; return *this; }

    VectorIterator operator+(difference_type n) const { return VectorIterator(ptr_ + n); }
    VectorIterator operator-(difference_type n) const { return VectorIterator(ptr_ - n); }

    friend VectorIterator operator+(difference_type n, const VectorIterator& it) { return it + n; }

    difference_type operator-(const VectorIterator& other) const { return ptr_ - other.ptr_; }

    bool operator==(const VectorIterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const VectorIterator& other) const { return ptr_ != other.ptr_; }
    bool operator<(const VectorIterator& other) const { return ptr_ < other.ptr_; }
    bool operator>(const VectorIterator& other) const { return ptr_ > other.ptr_; }
    bool operator<=(const VectorIterator& other) const { return ptr_ <= other.ptr_; }
    bool operator>=(const VectorIterator& other) const { return ptr_ >= other.ptr_; }

};
