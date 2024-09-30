#ifndef DELETE_H
#define DELETE_H

#include <string>
#include "include/vectorDas.h"
#include "include/mapDas.h"

void ParsingDelete(const MyVector<std::string>& words, const std::string& filePath, const std::string& schemaName, const MyMap<std::string, MyVector<std::string>*>& jsonStructure);

#endif