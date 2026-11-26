cmake_minimum_required(VERSION 3.20)
project(LB3_26 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Основная программа
add_executable(main main.cpp)

# GoogleTest — скачивается при конфигурации
include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Тесты
enable_testing()
add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)

# CPack
install(TARGETS main DESTINATION bin)
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Student")
include(CPack)