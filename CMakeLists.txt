cmake_minimum_required(VERSION 3.16)
project(MyQtApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Test REQUIRED)
find_package(Qwt REQUIRED)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()

