#ifndef HASHCODE_H
#define HASHCODE_H

#include <string>

// хэш-функция для ключа int
template <typename TK>
int HashCode(const TK& key) {
    const char* data = reinterpret_cast<const char*>(&key);
    size_t size = sizeof(TK);
    int hash = 0;
    for (size_t i = 0; i < size; i++) {
        hash = (hash * 31) + data[i];
    }
    return hash;
    //return 2;
}


template <>
int HashCode<std::string>(const std::string& key) {
    unsigned long hash = 5381;
    int c = 0;
    for (char ch : key) {
        hash = ((hash << 5) + hash) + ch; /* hash * 33 + c */
    }
    return hash;
}

#endif