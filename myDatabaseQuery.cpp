#include <iostream>
#include <filesystem>

#include "include/mapDas.h"
#include "include/vectorDas.h"

#include "readJson.h"
#include "supportFiles.h"
#include "select.h"
#include "insert.h"
#include "delete.h"

using namespace std;


// Парсит и выполняет SQL-запросы
void parsingQuery(const string& query, const string& filePath, const string& schemaName, const int tuplesLimit, const MyMap<string, MyVector<string>*>& jsonStructure) {
    MyVector<string>* words = Split(query, ' ');
    if (words->data[0] == "SELECT") {
        try {
            ParsingSelect(*words, filePath, schemaName, jsonStructure);
        } catch (const exception& err) {
            cerr << err.what() << endl;
        }
    
    } else if (words->data[0] == "INSERT" && words->data[1] == "INTO") {
        try {
            ParsingInsert(*words, filePath, schemaName, tuplesLimit, jsonStructure);
        } catch (const exception& err) {
            cerr << err.what() << endl;
        }
    
    } else if (words->data[0] == "DELETE" && words->data[1] == "FROM") {
        try {
            ParsingDelete(*words, filePath, schemaName,jsonStructure);
        } catch (const exception& err) {
            cerr << err.what() << endl;
        }
        
    } else { 
        cout << "Unknown command" << endl;
    }

}



// ввод имени файла и директории
int InputNames(string& jsonFileName, string& filePath) {
    while (true) {
        cout << "Введите имя json файла: ";
        getline(cin, jsonFileName);
        cout << "Введите путь к файлу: ";
        getline(cin, filePath);

        try {
            if (!filesystem::exists(filePath + "/" + jsonFileName)) {
                throw std::runtime_error("JSON file not found");
            } else {
                return 0;
            }
        } catch (const exception& err) {
            cerr << "Error: " << err.what() << endl;
        }
    }
}


int main() {
    string jsonFileName;
    string filePath;
    InputNames(jsonFileName, filePath);

    MyMap<string, MyVector<string>*>* jsonStructure = CreateMap<string, MyVector<string>*>(10, 50);

    // создание директорий
    int tuplesLimit = 0;
    string schemaName = ReadJsonFile(jsonFileName, filePath, tuplesLimit, *jsonStructure);
    while (true) {
        cout << endl;
        cout << "Введите SQL запрос или введите \"q\" для выхода" << endl;
        cout << "<<< ";
        string query;
        getline(cin, query);
        // парсинг запроса
        if (query == "q") break;
        cout << endl;
        parsingQuery(query, filePath, schemaName, tuplesLimit, *jsonStructure);
    }
    
    DestroyMap<string, MyVector<string>*>(*jsonStructure);

    return 0;
}