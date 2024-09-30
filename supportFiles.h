#ifndef SUPPORTFILES_H
#define SUPPORTFILES_H

#include <string>

#include "include/vectorDas.h"

int Strlen(const std::string &str);
std::string Substr(const std::string &str, int start, int end);
MyVector<std::string>* Split(const std::string &str, char delim);


#endif