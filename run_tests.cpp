#include <tclap/CmdLine.h>

#include <gtest/gtest.h>
#include "scale-codecs-lib.hpp"

// int main(int argc, char** argv)
// {
//     TCLAP::CmdLine cmd("Configure params for running tests...", ' ', "0.9");
//     TCLAP::SwitchArg logArg("l", "log", "Show debug info", cmd, false);
//     TCLAP::ValueArg<std::string> nameArg("t", "tests", "Select what tests to run, default is \"all\"", false, "all", "string");

//     cmd.parse(argc, argv);

//     // ::testing::InitGoogleTest();
//     // ::testing::GTEST_FLAG(filter) = "Test_Cases1*";
//     // return RUN_ALL_TESTS();

//     return 0;
// }