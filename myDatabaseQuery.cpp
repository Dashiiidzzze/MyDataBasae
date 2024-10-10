#include <iostream>
#include <filesystem>

#include "include/mapDas.h"
#include "include/vectorDas.h"

#include "header.h"

using namespace std;


// Парсит и выполняет SQL-запросы
//void parsingQuery(const string& query, const string& filePath, const string& schemaName, const int tuplesLimit, const MyMap<string, MyVector<string>*>& jsonStructure) {
void parsingQuery(const string& query, const SchemaInfo& schemaData) {
    MyVector<string>* words = Split(query, ' ');
    if (words->data[0] == "SELECT") {
        try {
            ParsingSelect(*words, schemaData.filepath, schemaData.name, *schemaData.jsonStructure);
        } catch (const exception& err) {
            cerr << err.what() << endl;
        }
    
    } else if (words->data[0] == "INSERT" && words->data[1] == "INTO") {
        try {
            ParsingInsert(*words, schemaData.filepath, schemaData.name, schemaData.tuplesLimit, *schemaData.jsonStructure);
        } catch (const exception& err) {
            cerr << err.what() << endl;
        }
    
    } else if (words->data[0] == "DELETE" && words->data[1] == "FROM") {
        try {
            ParsingDelete(*words, schemaData.filepath, schemaData.name, *schemaData.jsonStructure);
        } catch (const exception& err) {
            cerr << err.what() << endl;
        }
        
    } else { 
        cout << "Unknown command" << endl;
    }

}



// ввод имени файла и директории
int InputNames(string& jsonFileName, SchemaInfo& schemaData) {
    while (true) {
        cout << "Введите имя json файла: ";
        getline(cin, jsonFileName);
        cout << "Введите путь к файлу: ";
        getline(cin, schemaData.filepath);

        try {
            if (!filesystem::exists(schemaData.filepath + "/" + jsonFileName)) {
                throw runtime_error("JSON file not found");
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
    //string filePath;
    SchemaInfo schemaData;
    InputNames(jsonFileName, schemaData);
    //InputNames(jsonFileName, filePath);

    schemaData.jsonStructure = CreateMap<string, MyVector<string>*>(10, 50);

    //MyMap<string, MyVector<string>*>* jsonStructure = CreateMap<string, MyVector<string>*>(10, 50);

    // создание директорий
    //int tuplesLimit = 0;
    //string schemaName = ReadJsonFile(jsonFileName, filePath, tuplesLimit, *jsonStructure);
    ReadJsonFile(jsonFileName, schemaData);
    while (true) {
        cout << endl;
        cout << "Введите SQL запрос или введите \"q\" для выхода" << endl;
        cout << "<<< ";
        string query;
        getline(cin, query);
        // парсинг запроса
        if (query == "q") break;
        cout << endl;
        //parsingQuery(query, filePath, schemaName, tuplesLimit, *jsonStructure);
        parsingQuery(query, schemaData);
    }
    
    DestroyMap<string, MyVector<string>*>(*schemaData.jsonStructure);

    return 0;
}