#ifndef VECTORDAS_H
#define VECTORDAS_H

#include <iostream>
#include <iomanip>


template <typename T>
struct MyVector {
    T* data;      //сам массив
    int len;        //длина
    int cap;        //capacity - объем
    int LoadFactor; //с какого процента заполнения увеличиваем объем = 50%
};


// перегрузка оператора вывода
template <typename T>
std::ostream& operator << (std::ostream& os, const MyVector<T>& vec) {
    for (int i = 0; i < vec.len; i++) {
        std::cout << vec.data[i];
        if (i < vec.len - 1) std::cout << std::setw(25);
    }
    return os;
}

// инициализация вектора
template <typename T>
MyVector<T>* CreateVector(int initCapacity, int initLoadFactor) {
    if (initCapacity <= 0 || initLoadFactor <= 0 || initLoadFactor > 100) {
        throw std::runtime_error("Index out of range");
    }

    MyVector<T>* vec = new MyVector<T>;
    vec->data = new T[initCapacity];
    vec->len = 0;
    vec->cap = initCapacity;
    vec->LoadFactor = initLoadFactor;
    return vec;
}

// увеличение массива
template <typename T>
void Expansion(MyVector<T>& vec) {
    int newCap = vec.cap * 2;
    T* newData = new T[newCap];
    for (int i = 0; i < vec.len; i++) {     //копируем данные из старого массива в новый
        newData[i] = vec.data[i];
    }
    delete[] vec.data;                      // очистка памяти
    vec.data = newData;
    vec.cap = newCap;
}

// добавление элемента в вектор
template <typename T>
void AddVector(MyVector<T>& vec, T value) {
    if ((vec.len + 1) * 100 / vec.cap >= vec.LoadFactor) { //обновление размера массива
        Expansion(vec);
    }
    vec.data[vec.len] = value;
    vec.len++;
}


//удаление элемента из вектора
template <typename T>
void DeleteVector(MyVector<T>& vec, int index) {
    if (index < 0 || index >= vec.len) {
        throw std::runtime_error("Index out of range");
    }

    for (int i = index; i < vec.len - 1; i++) {
        vec.data[i] = vec.data[i + 1];
    }

    vec.len--;
}


// замена элемента по индексу
template <typename T>
void ReplaceVector(MyVector<T>& vec, int index, T value) {
    if (index < 0 || index >= vec.len) {
        throw std::runtime_error("Index out of range");
    }
    vec.data[index] = value;
}

#endif