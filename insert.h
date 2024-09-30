#ifndef INSERT_H
#define INSERT_H

#include <string>

#include "include/vectorDas.h"
#include "include/mapDas.h"

void ParsingInsert(const MyVector<std::string>& words, const std::string& filePath, const std::string& schemaName, const int tuplesLimit, const MyMap<std::string, MyVector<std::string>*>& jsonStructure);

#endif