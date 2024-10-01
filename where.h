#ifndef WHERE_H
#define WHERE_H

#include "include/vectorDas.h"
#include "include/mapDas.h"

#include <iostream>
#include <string>


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


std::string ApostDel(std::string str);
MyVector<MyVector<std::string>*>* splitByOperator(const MyVector<std::string>& query, const std::string& op);
Node* getConditionTree(const MyVector<std::string>& query);
bool isValidRow(Node* node, const MyVector<std::string>& row, const MyMap<std::string, MyVector<std::string>*>& jsonStructure, const std::string& tabNames);

#endif