#include "DB.h"
#include "User.h"
#include <iostream>
#include <vector>
#include <fstream>

void DB::saveUser(const User& data) {
    std::ofstream out;
    out.open(fileName, std::ios::app);
    if (out.is_open()) {
        out << data.getUsername() << ","  << data.getPassword() << "," << data.getName() << "," << data.getEmail() << std::endl;
        out.close();
    } else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }
}
