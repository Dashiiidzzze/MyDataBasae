#ifndef READJSON_H
#define READJSON_H

//#include <string>
#include "include/mapDas.h"
#include "include/vectorDas.h"

std::string ReadJsonFile(const std::string& fileName, const std::string& filePath, int& tuplesLimit, MyMap<std::string, MyVector<std::string>*>& jsonStructure);

#endif