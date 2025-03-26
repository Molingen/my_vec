#pragma once
#include <cstddef>
#include <initializer_list>
#include <cmath>
#include <stdexcept>

template <typename T>
class MyVec {
public:
    MyVec() = default;
    MyVec(size_t size, T initValue);
    explicit MyVec(size_t size);
    MyVec(std::initializer_list<T> init);
    MyVec(const MyVec &other);
    MyVec(MyVec&& other) noexcept ;
    MyVec(T* start, T* end);
    ~MyVec() { delete[] data; }

    MyVec& operator=(const MyVec& other);
    MyVec& operator=(MyVec&& other) noexcept;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    T& at(size_t index);
    const T& at(size_t index) const;

    void swap(MyVec& other) noexcept {
        using std::swap;
        swap(sz, other.sz);
        swap(cap, other.cap);
        swap(data, other.data);
    }

    size_t size() const { return sz; }
    size_t capacity() const { return cap; }
    bool empty() const noexcept { return sz == 0; }
    void reserve(size_t newCap);

    void resize(size_t newSize, const T& value = 0);
    size_t insert(size_t pos, const T& value);
    size_t insert(size_t pos, T&& value);
    size_t insert(size_t pos, std::initializer_list<T> init);
    size_t erase(size_t pos);
    void clean();
    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();

private:
    size_t sz = 0;
    size_t cap = 1;
    T* data = nullptr;

    static size_t calculateCapacity(size_t size);
};

template<class T>
size_t MyVec<T>::calculateCapacity(size_t size) {
    if (size == 0) {
        return 1;
    }
    size_t log2Val = static_cast<size_t>(std::ceil(std::log2(size)));
    return static_cast<size_t>(1) << log2Val;
}

template<typename T>
MyVec<T>::MyVec(size_t size, T initValue)
    : sz(size), cap(calculateCapacity(sz)), data(new T[cap]) {
    for (size_t i = 0; i < sz; ++i) {
        data[i] = initValue;
    }
}

template<typename T>
MyVec<T>::MyVec(size_t size)
    : sz(size), cap(calculateCapacity(sz)), data(new T[cap]) {
    for (size_t i = 0; i < sz; ++i) {
        data[i] = T();
    }
}

template<typename T>
MyVec<T>::MyVec(std::initializer_list<T> init)
    : sz(init.size()), cap(calculateCapacity(sz)), data(new T[cap]){
    size_t i = 0;
    for (const auto& item : init) {
        data[i++] = item;
    }
}

template<typename T>
MyVec<T>::MyVec(const MyVec& other)
    : sz(other.sz), cap(other.cap), data(new T[cap]) {
    for (size_t i = 0; i < sz; ++i) {
        data[i] = other.data[i];
    }
}

template<typename T>
MyVec<T>::MyVec(MyVec&& other) noexcept
    : sz(other.sz), cap(other.cap), data(other.data) {
    other.sz = 0;
    other.cap = 1;
    other.data = nullptr;
}

template<typename T>
MyVec<T>::MyVec(T* start, T* end)
    : sz(end - start), cap(calculateCapacity(sz)), data(new T[cap]) {
    for (size_t i = 0; start != end; ++start, ++i) {
        data[i] = *start;
    }
}

template<typename T>
MyVec<T>& MyVec<T>::operator=(const MyVec& other) {
    if (this == &other) {
        return *this;
    }
    delete[] data;
    sz = other.sz;
    cap = other.cap;
    data = new T[cap];

    for (size_t i = 0; i < sz; ++i) {
        data[i] = other.data[i];
    }
    return *this;
}

template<typename T>
MyVec<T>& MyVec<T>::operator=(MyVec &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    delete[] data;

    sz = other.sz;
    cap = other.cap;
    data = other.data;

    other.sz = 0;
    other.cap = 0;
    other.data = nullptr;

    return *this;
}

template<typename T>
T& MyVec<T>::operator[](size_t index) {
    return data[index];
}

template<typename T>
const T& MyVec<T>::operator[](size_t index) const {
    return data[index];
}

template<typename T>
T& MyVec<T>::at(size_t index) {
    if (index >= sz) {
        throw std::__throw_out_of_range("index out of range");
    }
    return data[index];
}

template<typename T>
const T& MyVec<T>::at(size_t index) const {
    if (index >= sz) {
        throw std::__throw_out_of_range("index out of range");
    }
    return data[index];
}

template<typename T>
void MyVec<T>::reserve(size_t newCap) {
    if (newCap > cap) {
        T* new_data = new T[newCap];
        for (size_t i = 0; i < sz; ++i) {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        cap = newCap;
        data = new_data;
    }
}

template<typename T>
void MyVec<T>::resize(size_t newSize, const T& value) {
    if (newSize > sz) {
        size_t newCapacity = calculateCapacity(newSize);
        if (newCapacity > cap) {
            T* newData = new T[newCapacity];
            for (size_t i = 0; i < sz; ++i) {
                newData[i] = data[i];
            }
            for (size_t i = sz; i < newSize; ++i) {
                newData[i] = value;
            }
            delete[] data;
            cap = newCapacity;
            data = newData;
        } else {
            for (size_t i = sz; i < newSize; ++i) {
                data[i] = value;
            }
        }
        sz = newSize;
    } else if (newSize < sz) {
        size_t newCapacity = calculateCapacity(newSize);
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < newSize; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        sz = newSize;
        cap = newCapacity;
        data = newData;
    }
}

template<typename T>
size_t MyVec<T>::insert(size_t pos, const T& value) {
    if (pos < sz) {
        if (cap < sz + 1) {
            size_t newCap = calculateCapacity(sz + 1);
            T* newData = new T[newCap];
            for (size_t i = 0; i < pos; ++i) {
                newData[i] = data[i];
            }
            newData[pos] = value;
            for (size_t i = 0; i < sz + 1; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            cap = newCap;
            data = newData;
        } else {
            for (size_t i = sz; i > pos; --i) {
                data[i] = data[i - 1];
            }
            data[pos] = value;
        }
        sz += 1;
    }
    return pos;
}

template<typename T>
size_t MyVec<T>::insert(size_t pos, T&& value) {
    if (pos > sz) {
        throw std::out_of_range("insert position out of range");
    }

    if (cap < sz + 1) {
        size_t new_cap = cap == 0 ? 1 :calculateCapacity(sz + 1);
        reserve(new_cap);
    }

    for (size_t i = sz; i > pos; --i) {
        data[i] = std::move(data[i - 1]);
    }
    data[pos] = std::move(value);
    ++sz;
    return pos;
}

template<typename T>
size_t MyVec<T>::insert(size_t pos, std::initializer_list<T> init) {
    if (pos > sz) {
        throw std::out_of_range("insert position out of range");
    }
    size_t init_size = init.size();
    if (sz + init_size > cap) {
        size_t new_cap = calculateCapacity(sz + init_size);
        reserve(new_cap);
    }

    for (size_t i = sz + init_size - 1; i >= pos + init_size; --i) {
        data[i] = std::move(data[i - init_size]);
    }
    size_t i = pos;
    for (const T& value : init) {
        data[i++] = value;
    }
    sz += init_size;
    return pos;
}

template<typename T>
size_t MyVec<T>::erase(size_t pos) {
    if (pos >= sz) {
        throw std::out_of_range("index out of range");
    }
    for (size_t i = pos; i < sz - 1; ++i) {
        data[i] = std::move(data[i + 1]);
    }
    --sz;
    return pos;
}

template<typename T>
void MyVec<T>::clean() {
    for (size_t i = 0; i < sz; ++i) {
        data[i].~T();
    }
    sz = 0;
}

template<typename T>
void MyVec<T>::push_back(const T& value) {
    if (sz >= cap) {
        size_t newCapacity = calculateCapacity(sz+1);
        reserve(newCapacity);
    }
    data[sz] = value;
    ++sz;
}

template<typename T>
void MyVec<T>::push_back(T&& value) {
    if (sz >= cap) {
        size_t newCapacity = calculateCapacity(sz+1);
        reserve(newCapacity);
    }
    data[sz] = std::move(value);
    ++sz;
}

template<typename T>
void MyVec<T>::pop_back() {
    --sz;
}