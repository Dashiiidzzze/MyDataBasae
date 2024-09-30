#include <iostream>
#include <fstream>

#include "include/json.hpp"
//#include "include/mapDas.h"
//#include "include/vectorDas.h"
#include "readJson.h"

using json = nlohmann::json;

using namespace std;


// создание директории
void CreateDir(const string& pathToDir) {
    filesystem::path path(pathToDir);
    if (!filesystem::exists(path)) {
        filesystem::create_directories(path);
    }
}


// создание файла с данными
void CreateFile(const string& pathToFile, const string& fileName, const string& data, bool isDirectory) {
    filesystem::path path(pathToFile);
    if (filesystem::exists(path / fileName)) {
        if (isDirectory) {
            ifstream file(path / fileName);
            string line;
            getline(file, line);
            if (line == data) {
                file.close();
                return; // данные уже есть в файле
            }
            file.close();
        } else {
            return;
        }
    }
    // если данные в файле не совпадают с JSON или отсутствуют
    ofstream lockFile(path / fileName);
    if (lockFile.is_open()) {
        lockFile << data;
        lockFile.close();
    } else {
        throw runtime_error("Failed to create lock file in directory");
    }
}


// чтение json файла и создание директорий
string ReadJsonFile(const string& fileName, const string& filePath, int& tuplesLimit, MyMap<string, MyVector<string>*>& jsonStructure) {
    ifstream file(filePath + "/" + fileName);
    if (!file.is_open()) {
        throw runtime_error("Failed to open schema.json");
    }

    // чтение json
    json schema;
    file >> schema;

    // чтение имени таблицы
    string schemaName = schema["name"];
    CreateDir(schemaName);

    // чтение максимального количества ключей
    tuplesLimit = schema["tuples_limit"];

    // чтение структуры таблицы
    json tableStructure = schema["structure"];
    for (auto& [key, value] : tableStructure.items()) {
        //cout << "test 1" << endl;
        // создание директорий
        CreateDir(schemaName + "/" + key);
        MyVector<string>* tempValue = CreateVector<string>(10, 50);
        //cout << "test 2" << endl;
        string colNames = key + "_pk";
        for (auto columns : value) {
            colNames += ",";
            string temp = columns;
            colNames += temp;
            //cout << "test 3" << endl;
            AddVector(*tempValue, temp);
        }
        //cout << "test 4" << endl;
        CreateFile(schemaName + "/" + key, "1.csv", colNames, true);
        CreateFile(schemaName + "/" + key, key + "_lock.txt", "0", false);
        CreateFile(schemaName + "/" + key, key + "_pk_sequence.txt", "0", false);
        //cout << "test 5" << endl;
        //cout << key << endl;
        AddMap<string, MyVector<string>*>(jsonStructure, key, tempValue);
        //cout << *tempValue << endl;
        //cout << *GetMap<string, MyVector<string>*>(jsonStructure, key) << endl;
    }

    file.close();
    return schemaName;
}