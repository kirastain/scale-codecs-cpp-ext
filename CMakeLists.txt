cmake_minimum_required(VERSION 3.5.0)

project(scale-codecs-lib VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_library(scale-codecs-lib STATIC)
target_sources(scale-codecs-lib PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/scale-codecs-lib.cpp
    ${CMAKE_SOURCE_DIR}/src/encoder.cpp)
target_include_directories(scale-codecs-lib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# set(CMAKE_INSTALL_PREFIX
#         "${CMAKE_SOURCE_DIR}/install"
#         CACHE PATH "Where the library will be installed to" FORCE
#     )


enable_testing()

add_executable(test_lib)
target_sources(test_lib PRIVATE simple_tests.cpp node_tests.cpp)
target_link_libraries(test_lib scale-codecs-lib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test_lib)
