#include "header.h"
/*
// чтение таблицы из файла
MyVector<MyVector<string>*>* ReadTable(const string& tableName, const MyVector<string>& colNames, const MyVector<string>& conditionList, const SchemaInfo& schemaData, bool where) {
    string pathToCSV = schemaData.filepath + "/" + schemaData.name + "/" + tableName;


    
    MyVector<MyVector<string>*>* tabData = CreateVector<MyVector<string>*>(5, 50);
    int fileIndex = 1;
    try {
        BusyTable(pathToCSV, tableName + "_lock.txt", 1);
    } catch (const std::exception& err) {
        cerr << err.what() << endl;
        return tabData;
    }
    Node* nodeWere = getConditionTree(conditionList);
    while (filesystem::exists(pathToCSV + "/" + to_string(fileIndex) + ".csv")) {
        ifstream file(pathToCSV + "/" + to_string(fileIndex) + ".csv");
        if (!file.is_open()) {
            throw runtime_error("Failed to open file" + (pathToCSV + "/" + to_string(fileIndex) + ".csv"));
        }
        string firstLine;
        getline(file, firstLine);
        if (colNames.data[0] == "*") {
            string line;
            while (getline(file, line)) {
                MyVector<string>* row = Split(line, ',');
                if (where) {
                    try {
                        if (isValidRow(nodeWere, *row, *schemaData.jsonStructure, tableName)) {
                            DeleteVector<string>(*row, 0);
                            AddVector(*tabData, row);
                        }
                    } catch (const exception& err) {
                        cerr << err.what() << endl;
                        file.close();
                        return tabData;
                    }
                } else {
                    DeleteVector<string>(*row, 0);
                    AddVector(*tabData, row);
                }
            }
        } else {
            MyVector<string>* fileColNames = GetMap<string, MyVector<string>*>(*schemaData.jsonStructure, tableName);
            MyVector<int>* colIndex = CreateVector<int>(10, 50);
            for (int i = 0; i < fileColNames->len; i++) {
                for (int j = 1; j < colNames.len; j++) {
                    if (fileColNames->data[i] == colNames.data[j]) {
                        AddVector(*colIndex, i + 1);
                    }
                }
            }
            string line;
            while (getline(file, line)) { 
                MyVector<string>* row = Split(line, ',');
                if (where) {
                    try {
                        if (isValidRow(nodeWere, *row, *schemaData.jsonStructure, tableName)) {
                            MyVector<string>* newRow = CreateVector<string>(colIndex->len, 50);
                            for (int i = 0; i < colIndex->len; i++) {
                                AddVector(*newRow, row->data[colIndex->data[i]]);
                            }
                            AddVector(*tabData, newRow);
                        }
                    } catch (const exception& err) {
                        cerr << err.what() << endl;
                        file.close();
                        return tabData;
                    }
                } else {
                    MyVector<string>* newRow = CreateVector<string>(colIndex->len, 50);
                    for (int i = 0; i < colIndex->len; i++) {
                        AddVector(*newRow, row->data[colIndex->data[i]]);
                    }
                    AddVector(*tabData, newRow);
                }
            }
        }

        file.close();
        fileIndex += 1;
    }
    BusyTable(pathToCSV, tableName + "_lock.txt", 1);
    return tabData;
}*/

void FullReadCSVFile(const string& pathToCSV, const int fileIndex, MyVector<MyVector<string>*>& tabData, const SchemaInfo& schemaData) {
    cout << "fullReadCSVFile" << endl;
    ifstream file(pathToCSV + "/" + to_string(fileIndex) + ".csv");
    if (!file.is_open()) {
        throw runtime_error("Failed to open file" + (pathToCSV + "/" + to_string(fileIndex) + ".csv"));
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        MyVector<string>* row = Split(line, ',');
        for (int i = 0; i < tabData.len; i++) {
            // добавить проверкку на колонки select
            // добавить проверку условий where

            AddVector(*tabData.data[i], row->data[i + 1]);
        }
    }
    file.close();
}


MyVector<MyVector<string>*>* ReadTable(const string& tableName, const MyVector<string>& colNames, const MyVector<string>& conditionList, const SchemaInfo& schemaData, bool where) {
    cout << "ReadTable" << endl;
    string pathToCSV = schemaData.filepath + "/" + schemaData.name + "/" + tableName;
    MyVector<MyVector<string>*>* tabData = CreateVector<MyVector<string>*>(10, 50);
    int fileIndex = 1;
    try {
        BusyTable(pathToCSV, tableName + "_lock.txt", 1);
    } catch (const std::exception& err) {
        cerr << err.what() << endl;
        return tabData;
    }
    MyVector<string>* columnNames = GetMap<string, MyVector<string>*>(*schemaData.jsonStructure, tableName);
    for (int i = 0; i < columnNames->len; i++) {
        MyVector<string>* temp = CreateVector<string>(schemaData.tuplesLimit * 2, 50);
        AddVector<MyVector<string>*>(*tabData, temp);
        //AddMap<string, MyVector<string>*>(*tabData, columnNames->data[i], temp);
    }

    //Node* nodeWere = getConditionTree(conditionList);
    while (filesystem::exists(pathToCSV + "/" + to_string(fileIndex) + ".csv")) {
        try {
            FullReadCSVFile(pathToCSV, fileIndex, *tabData, schemaData);
        } catch (const exception& err) {
            cerr << err.what() << endl;
            return tabData;
        }
        fileIndex++;
    }

    BusyTable(pathToCSV, tableName + "_lock.txt", 1);

    return tabData;
}

/*
// вывод содержимого таблиц в виде декартового произведения
void DecartMult(const MyVector<MyVector<MyVector<string>*>*>& tablesData, MyVector<MyVector<string>*>& temp, int counterTab, int tab) {
    for (int i = 0; i < tablesData.data[counterTab]->len; i++) {
        temp.data[counterTab] = tablesData.data[counterTab]->data[i];

        if (counterTab < tab - 1) {
            DecartMult(tablesData, temp, counterTab + 1, tab);
        } else {
            for (int j = 0; j < tab; j++) {
                cout << *temp.data[j] << std::setw(25);
            }
            cout << endl;
        }
    }

    return;
}*/

void DecartMult(const MyMap<string, MyVector<MyVector<string>*>*>& tablesData, const MyVector<string>& colNames) {
    cout << "DecartMult" << endl;
    for (int i = 0; i < colNames.len; i++) {
        //MyVector<string>* tabColPair = Split(colNames.data[i], '.');

        MyVector<MyVector<string>*>* table = GetMap(tablesData, colNames.data[i]);
        for (int j =  0; j < table->len; j++) {
            cout << table->data[j]->data[1] << setw(25);
        }
        cout << endl;
    }
}

// подготовка к чтению и выводу данных
void PreparationSelect(const MyVector<string>& colNames, const MyVector<string>& tableNames, const MyVector<string>& conditionList, const SchemaInfo& schemaData, bool where) {
    //MyVector<MyVector<MyVector<string>*>*>* tablesData = CreateVector<MyVector<MyVector<string>*>*>(10, 50);
    MyMap<string, MyVector<MyVector<string>*>*>* tablesData = CreateMap<string, MyVector<MyVector<string>*>*>(10, 50);
    if (colNames.data[0] == "*") {      // чтение всех данных из таблиц
        for (int j = 0; j < tableNames.len; j++) {
            MyVector<MyVector<string>*>* table = ReadTable(tableNames.data[j], colNames, conditionList, schemaData, where);
            AddMap(*tablesData, tableNames.data[j], table);
        }
    } else {
        /*
        for (int i = 0; i < tableNames.len; i++) {



            MyVector<string>* tabColPair = CreateVector<string>(5, 50);
            AddVector(*tabColPair, tableNames.data[i]);
            for (int j = 0; j < colNames.len; j++) {
                MyVector<string>* splitColNames = Split(colNames.data[j], '.');
                try {
                    GetMap(jsonStructure, splitColNames->data[0]);
                } catch (const exception& err) {
                    cerr << err.what() << ": table " << splitColNames->data[0] << " is missing" << endl;
                    return;
                }
                if (splitColNames->data[0] == tableNames.data[i]) {
                    AddVector(*tabColPair, splitColNames->data[1]);
                }
            }
            MyVector<MyVector<string>*>* tableData = ReadTable(tabColPair->data[0], schemaName, filePath, *tabColPair, conditionList, jsonStructure, where);
            AddVector(*tablesData, tableData);
        }*/
    }
    //MyVector<MyVector<string>*>* temp = CreateVector<MyVector<string>*>(tablesData->len * 2, 50);
    //DecartMult(*tablesData, *temp, 0, tablesData->len);
    //DecartMult(*tablesData, colNames);
    //MyVector<string>* temp = GetMap(*schemaData.jsonStructure, )           CreateVector<string>(tablesData->len * 2, 50);
    DecartMult(*tablesData, tableNames);
}



// парсинг SELECT запроса
void ParsingSelect(const MyVector<string>& words, const SchemaInfo& schemaData) {
    MyVector<string>* colNames = CreateVector<string>(10, 50);          // названия колонок в формате таблица1.колонка1
    MyVector<string>* tableNames = CreateVector<string>(10, 50);        // названия таблиц в формате  таблица1
    MyVector<string>* conditionList = CreateVector<string>(10, 50);     // список условий where
    bool afterFrom = false;
    bool afterWhere = false;
    int countTabNames = 0;
    int countData = 0;
    int countWhereData = 0;
    for (int i = 1; i < words.len; i++) {
        if (words.data[i][words.data[i].size() - 1] == ',') {
            words.data[i] = Substr(words.data[i], 0, words.data[i].size() - 1);
        }
        if (words.data[i] == "FROM") {
            afterFrom = true;
        } else if (words.data[i] == "WHERE") {
            afterWhere = true;
        } else if (afterWhere) {
            countWhereData++;
            AddVector<string>(*conditionList, words.data[i]);
        } else if (afterFrom) {
            try {
                GetMap(*schemaData.jsonStructure, words.data[i]);
            } catch (const exception& err) {
                cerr << err.what() << ": table " << words.data[i] << " is missing" << endl;
                return;
            }
            countTabNames++;
            AddVector(*tableNames, words.data[i]);
        } else {
            countData++;
            AddVector(*colNames, words.data[i]);
        }
    }
    if (countTabNames == 0 || countData == 0) {
        throw runtime_error("missing table name or data in FROM");
    }
    cout << "dddd" << endl;
    if (countWhereData == 0) {
        PreparationSelect(*colNames, *tableNames, *conditionList, schemaData, false);
    } else {
        PreparationSelect(*colNames, *tableNames, *conditionList, schemaData, true);
    }
}