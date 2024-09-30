#include <iostream>
#include <fstream>
#include <filesystem>

#include "where.hpp"
#include "supportFiles.h"
#include "delete.h"

using namespace std; 

void DeleteData(MyVector<string>& tableNames, MyVector<string>& conditionList, const string& schemaName, const string& path, const MyMap<string, MyVector<string>*>& jsonStructure) {
    Node* nodeWere = getConditionTree(conditionList);
    for (int i = 00; i < tableNames.len; i++) {
        int fileIndex = 1;
        cout << tableNames.data[i] << endl;
        while (filesystem::exists(path + "/" + schemaName + "/" + tableNames.data[i] + "/" + to_string(fileIndex) + ".csv")) {
            ifstream file(path + "/" + schemaName + "/" + tableNames.data[i] + "/" + to_string(fileIndex) + ".csv");
            if (!file.is_open()) {
                throw runtime_error("Failed to open " + (path + "/" + schemaName + "/" + tableNames.data[i] + "/" + to_string(fileIndex) + ".csv"));
            }
            ofstream tempFile(path + "/" + schemaName + "/" + tableNames.data[i] + "/" + to_string(fileIndex) + "_temp.csv");

            string line;
            getline(file, line);
            tempFile << line;
            while (getline(file, line)) {
                MyVector<string>* row = Split(line, ',');
                try {
                    if (!isValidRow(nodeWere, *row, jsonStructure, tableNames.data[i])) {
                        tempFile << endl << line;
                    }
                } catch (const exception& err) {
                    cerr << err.what() << endl;
                    tempFile.close();
                    file.close();
                    std::remove((path + "/" + schemaName + "/" + tableNames.data[i] + "/" + to_string(fileIndex) + "_temp.csv").c_str());
                    return;
                }
            }
            tempFile.close();
            file.close();
            if (std::remove((path + "/" + schemaName + "/" + tableNames.data[i] + "/" + to_string(fileIndex) + ".csv").c_str()) != 0) {
                std::cerr << "Error deleting file" << std::endl;
                return;
            }
            if (std::rename((path + "/" + schemaName + "/" + tableNames.data[i] + "/" + to_string(fileIndex) + "_temp.csv").c_str(), (path + "/" + schemaName + "/" + tableNames.data[i] + "/" + to_string(fileIndex) + ".csv").c_str()) != 0) {
                std::cerr << "Error renaming file" << std::endl;
                return;
            }

            fileIndex++;
        }
    }
}

void ParsingDelete(const MyVector<string>& words, const string& filePath, const string& schemaName, const MyMap<string, MyVector<string>*>& jsonStructure) {
    MyVector<string>* tableNames = CreateVector<string>(5, 50);
    MyVector<string>* conditionList = CreateVector<string>(5, 50);
    int countTabNames = 0;
    int countWereData = 0;
    bool afterWhere = false;
    for (int i = 2; i < words.len; i++ ) {
        if (words.data[i][words.data[i].size() - 1] == ',') {
            words.data[i] = Substr(words.data[i], 0, words.data[i].size() - 1);
        }
        if (words.data[i] == "WHERE") {
            afterWhere = true;
        } else if (afterWhere) {
            AddVector<string>(*conditionList, words.data[i]);
            countWereData++;
        } else {
            countTabNames++;
            try {
                GetMap(jsonStructure, words.data[i]);
            } catch (const exception& err) {
                cerr << err.what() << ": table " << words.data[i] << " is missing" << endl;
                return;
            }
            AddVector<string>(*tableNames, words.data[i]);
        }
    }
    if (countTabNames == 0 || countWereData == 0) {
        throw runtime_error("missing table name or data in WERE");
    }

    try {
        DeleteData(*tableNames, *conditionList, schemaName, filePath, jsonStructure);
    } catch (const exception& err) {
        cerr << err.what()<< endl;
        return;
    }
    // проверка вывод результатов
    cout << *tableNames << endl;
}