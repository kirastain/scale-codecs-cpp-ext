cmake_minimum_required(VERSION 3.5.0)

project(scale-codecs-lib VERSION 0.1.0 LANGUAGES C CXX)

set(PROJECT_BUILD_ARM ON)
add_definitions( -DPROJECT_WITH_CUSTOM=${PROJECT_BUILD_ARM})

message("Build for ARM: " ${PROJECT_BUILD_ARM})

if (NOT PROJECT_BUILD_ARM)
    message("Building for system: " ${CMAKE_SYSTEM_NAME})
    set(CMAKE_CXX_STANDARD 20)
    message("CXX is set for C++20")
elseif(ANDROID OR PROJECT_BUILD_ARM)
    message("Building for system: " ${CMAKE_SYSTEM_NAME})
    set(CMAKE_CXX_STANDARD 11)
    message("CXX is set for C++11")
else()
    message("Cannot find the system")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT PROJECT_BUILD_ARM)
    include(GNUInstallDirs)
endif()

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
# )

enable_testing()

add_executable(test_lib)
target_sources(test_lib PRIVATE simple_tests.cpp node_tests.cpp metadata_tests.cpp)
target_link_libraries(test_lib scale-codecs-lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_lib)
