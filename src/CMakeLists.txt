cmake_minimum_required(VERSION 3.10)
project(BookRentalSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BookRental
    main.cpp
    User.cpp
    Admin.cpp
    Book.cpp
    Transaction.cpp
)
