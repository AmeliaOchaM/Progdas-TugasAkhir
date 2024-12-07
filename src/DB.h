#ifndef DB_H
#define DB_H
#include <fstream>
#include <string>
#include "User.h"
#include "iostream"

class DB {
public:
    std::ifstream in;
    std::ofstream out;
    std::string fileName;

    DB(const char* fileName){
    DB::fileName = fileName;}
    void saveUser(const User& data);
};

#endif
