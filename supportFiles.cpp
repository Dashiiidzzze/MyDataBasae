#include <string>

#include "include/vectorDas.h"

#include "supportFiles.h"

using namespace std;

// Возвращает длину строки
int Strlen(const string &str) {
    int len = 0;
    while (str[len]!= '\0') {
        len++;
    }
    return len;
}

// Возвращает подстроку от start до end (не включая end)
string Substr(const string &str, int start, int end) {
    string result;
    for (int i = start; i < end; i++) {
        result += str[i];
    }
    return result;
}

// Разбивает строку на слова с разделителем delim.
MyVector<string>* Split(const string &str, char delim) {
    int index = 0;
    MyVector<string>* words = CreateVector<string>(10, 50);
    int length = Strlen(str);

    while (true) {
        int delimIndex = index;
        while (str[delimIndex] != delim && delimIndex != length) delimIndex++;

        string word = Substr(str, index, delimIndex);
        AddVector(*words, word);
        index = delimIndex + 1;
        if (delimIndex == length) break;
    }

    return words;
}