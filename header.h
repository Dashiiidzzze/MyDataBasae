#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>

#include "include/mapDas.h"
#include "include/vectorDas.h"

using namespace std;

struct SchemaInfo {
    string filepath;
    string name;
    int tuplesLimit;
    MyMap<string, MyVector<string>*>* jsonStructure;
};

// support functions
int Strlen(const string &str);
string Substr(const string &str, int start, int end);
MyVector<string>* Split(const string &str, char delim);
void BusyTable(const string& path, const string& fileName, const int rank);


// reading json file
void CreateDir(const string& pathToDir);
void CreateFile(const string& pathToFile, const string& fileName, const string& data, bool isDirectory);
//string ReadJsonFile(const string& fileName, const string& filePath, int& tuplesLimit, MyMap<string, MyVector<string>*>& jsonStructure);
void ReadJsonFile(const string& fileName, SchemaInfo& schemaData);

// where
// Тип узла
enum class NodeType {
    ConditionNode,
    OrNode,
    AndNode
};

// Структура
struct Node {
    NodeType nodeType;
    MyVector<std::string> value;
    Node* left;
    Node* right;

    Node(NodeType type, const MyVector<std::string> val = {}, Node* l = nullptr, Node* r = nullptr)
        : nodeType(type), value(val), left(l), right(r) {}
};

string ApostDel(string str);
MyVector<MyVector<string>*>* splitByOperator(const MyVector<string>& query, const string& op);
Node* getConditionTree(const MyVector<string>& query);
bool isValidRow(Node* node, const MyVector<string>& row, const MyMap<string, MyVector<string>*>& jsonStructure, const string& tabNames);



// select
MyVector<MyVector<string>*>* ReadTable(const string& tableName, const string& schemaName, const string& filePath, const MyVector<string>& colNames, const MyVector<string>& conditionList, const MyMap<string, MyVector<string>*>& jsonStructure, bool where);
void DecartMult(const MyVector<MyVector<MyVector<string>*>*>& tablesData, MyVector<MyVector<string>*>& temp, int counterTab, int tab);
void PreparationSelect(const MyVector<string>& colNames, const MyVector<string>& tableNames, const MyVector<string>& conditionList, const string& schemaName, const string& filePath, const MyMap<string, MyVector<string>*>& jsonStructure, bool where);
void ParsingSelect(const MyVector<string>& words, const string& filePath, const string& schemaName, const MyMap<string, MyVector<string>*>& jsonStructure);


// insert
string ApostrovDel(string& str);
void TestAddition(int colLen, const MyVector<string>& tableNames, const MyMap<string, MyVector<string>*>& jsonStructure);
int PkSequenceRead(const string& path, const bool record, const int newID);
void InsertInTab(MyVector<MyVector<string>*>& addData, MyVector<string>& tableNames, const string& schemaName, const int tuplesLimit, const string& filePath);
void ParsingInsert(const MyVector<string>& words, const string& filePath, const string& schemaName, const int tuplesLimit, const MyMap<string, MyVector<string>*>& jsonStructure);


// delete
void DeleteData(MyVector<string>& tableNames, MyVector<string>& conditionList, const string& schemaName, const string& path, const MyMap<string, MyVector<string>*>& jsonStructure);
void ParsingDelete(const MyVector<string>& words, const string& filePath, const string& schemaName, const MyMap<string, MyVector<string>*>& jsonStructure);
