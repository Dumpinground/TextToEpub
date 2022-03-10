//
// Created by hasee on 2022/2/19.
//

#include <gtest/gtest.h>
#include "../argument/argument.h"

using namespace std;

TEST(testArgument, 0) {
    string command = R"(TextToEpub.exe --new_resource "F:\Epub\Missing\builds")";

    const int argc = 6;
    const char* argv[] = {
            "TextToEpub.exe",
            "--new_resource",
            "F:/Epub/Missing/builds/2/resources",
            "--add",
            "F:/Epub/Missing/builds/2/resources",
            "missing 2"
    };

    argument::Temp at_NewResource {
            {"--new_resource", "-NR"},
            {"resource path"}
    };

    argument::Temp at_Add {
            {"--add", "-A"},
            {"resource path", "file name"}
    };

    argument::Pairs arguments;
    arguments.appendTemplate(at_NewResource).appendTemplate(at_Add);
    arguments.getArguments(argc, argv);
    arguments;
}