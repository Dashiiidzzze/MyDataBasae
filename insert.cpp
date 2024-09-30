#include <iostream>
#include <fstream>

#include "supportFiles.h"
#include "insert.h"

using namespace std;

// удаление опострафа и проверка синтаксиса
string ApostrovDel(string& str) {
    if (str[str.size() - 1] == ',' && str[str.size() - 2] == ')') {
        str = Substr(str, 0, str.size() - 2);
    } else if (str[str.size() - 1] == ',' || str[str.size() - 1] == ')') {
        str = Substr(str, 0, str.size() - 1);
    }

    if (str[0] == '\'' && str[str.size() - 1] == '\'') {
        str = Substr(str, 1, str.size() - 1);
        return str;
    } else {
        throw runtime_error("invalid sintaxis in VALUES " + str);
    }
}

// проверка количества аргументов относительно столбцов таблиц
void TestAddition(int colLen, const MyVector<string>& tableNames, const MyMap<string, MyVector<string>*>& jsonStructure) {
    for (int i = 0; i < tableNames.len; i++) {
        MyVector<string>* temp = GetMap<string, MyVector<string>*>(jsonStructure, tableNames.data[i]);
        if (temp->len != colLen) {
            throw runtime_error("the number of arguments is not equal to the columns in " + tableNames.data[i]);
        }
    }
}


// проверка на занятость таблицы длугим пользоватклкм
void BusyTable(const string& path, const string& fileName, const int rank) {
    fstream lockFile(path + "/" + fileName);
    if (!lockFile.is_open()) {
        throw runtime_error("Failed to open " + (path + "/" + fileName));
    }
    int lock = 0;
    lockFile >> lock;
    cout << lock << endl;
    if (lock == 1 && rank == 1) {
        lockFile.close();
        throw runtime_error("Table " + fileName + " is locked by another process");
    } else {
        lockFile << rank;
    }
    lockFile.close();
}


int PkSequenceRead(const string& path, const bool record, const int newID) {
    fstream pkFile(path);
    if (!pkFile.is_open()) {
        throw runtime_error("Failed to open" + path);
    }
    int lastID = 0;
    if (record) {
        pkFile << newID;
    } else {
        pkFile >> lastID;
    }
    pkFile.close();
    return lastID;
}

// добавление строк в файл
void InsertInTab(MyVector<MyVector<string>*>& addData, MyVector<string>& tableNames, const string& schemaName, const int tuplesLimit, const string& filePath) {
    for (int i = 0; i < tableNames.len; i++) {
        int lastID = 0;
        try {
            BusyTable(filePath + "/" + schemaName + "/" + tableNames.data[i], tableNames.data[i] + "_lock.txt", 1);
            lastID = PkSequenceRead(filePath + "/" + schemaName + "/" + tableNames.data[i] + "/" + tableNames.data[i] + "_pk_sequence.txt", false, 0);
        } catch (const std::exception& err) {
            cerr << err.what() << endl;
            return;
        }

        int newID = lastID;
        for (int j = 0; j < addData.len; j++) {
            newID++;
            string tempPath;
            if (lastID / tuplesLimit < newID / tuplesLimit) {
                tempPath = filePath + "/" + schemaName + "/" + tableNames.data[i] + "/" + to_string(newID / tuplesLimit + 1) + ".csv";
            } else {
                tempPath = filePath + "/" + schemaName + "/" + tableNames.data[i] + "/" + to_string(lastID / tuplesLimit + 1) + ".csv";
            }
            fstream csvFile(tempPath, ios::app);
            if (!csvFile.is_open()) {
                throw runtime_error("Failed to open" + tempPath);
            }
            csvFile << endl << newID;
            for (int k = 0; k < addData.data[j]->len; k++) {
                csvFile << "," << addData.data[j]->data[k];
            }
            csvFile.close();
        }
        PkSequenceRead(filePath + "/" + schemaName + "/" + tableNames.data[i] + "/" + tableNames.data[i] + "_pk_sequence.txt", true, newID);
        BusyTable(filePath + "/" + schemaName + "/" + tableNames.data[i], tableNames.data[i] + "_lock.txt", 0);
    }
}


void ParsingInsert(const MyVector<string>& words, const string& filePath, const string& schemaName, const int tuplesLimit, const MyMap<string, MyVector<string>*>& jsonStructure) {
    MyVector<string>* tableNames = CreateVector<string>(5, 50);
    MyVector<MyVector<string>*>* addData = CreateVector<MyVector<string>*>(10, 50);
    bool afterValues = false;
    int countTabNames = 0;
    int countAddData = 0;
    for (int i = 2; i < words.len; i++) {
        if (words.data[i][words.data[i].size() - 1] == ',') {
            words.data[i] = Substr(words.data[i], 0, words.data[i].size() - 1);
        }
        if (words.data[i] == "VALUES") {
            afterValues = true;
        } else if (afterValues) {
            countAddData++;
            if (words.data[i][0] == '(') {
                MyVector<string>* tempData = CreateVector<string>(5, 50);
                words.data[i] = Substr(words.data[i], 1, words.data[i].size());

                while (words.data[i][words.data[i].size() - 1] != ')' && words.data[i][words.data[i].size() - 2] != ')') {
                    try {
                        ApostrovDel(words.data[i]);
                    } catch (const exception& err) {
                        cerr << err.what() << words.data[i] << endl;
                        return;
                    }
                    
                    AddVector<string>(*tempData, words.data[i]);
                    i++;
                }
                try {
                    ApostrovDel(words.data[i]);
                    AddVector<string>(*tempData, words.data[i]);
                    TestAddition(tempData->len, *tableNames, jsonStructure);
                } catch (const exception& err) {
                    cerr << err.what() << endl;
                    return;
                }
                AddVector<MyVector<string>*>(*addData, tempData);
            }
            
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
    if (countTabNames == 0 || countAddData == 0) {
        throw runtime_error("missing table name or data in VALUES");
    }

    try {
        InsertInTab(*addData, *tableNames, schemaName, tuplesLimit, filePath);
    } catch (const exception& err) {
        cerr << err.what() << endl;
        return;
    }
}