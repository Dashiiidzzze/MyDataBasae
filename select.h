#ifndef SELECT_H
#define SELECT_H

#include <string>
#include "include/vectorDas.h"
#include "include/mapDas.h"

void ParsingSelect(const MyVector<std::string>& words, const std::string& filePath,const std::string& schemaName, const MyMap<std::string, MyVector<std::string>*>& jsonStructure);

#endif