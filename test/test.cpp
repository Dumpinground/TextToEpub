//
// Created by hasee on 2021/11/29.
//

#include "../util/Book.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using json = nlohmann::json;

#define OutPutRoot string("../test/result/")

void saveJson(const json &j, const string &name) {
    ofstream o(OutPutRoot + name + ".json");
    o << setw(4) << j << endl;
}

TEST(test, testJson) {
    cout << "test" << endl;
    json j2 = {
            {"pi", 3.141},
            {"happy", true},
            {"name", "Niels"},
            {"nothing", nullptr},
            {"answer", {
                    {"everything", 42}
            }},
            {"list", {1, 0, 2}},
            {"object", {
                    {"currency", "USD"},
                    {"value", 42.99}
            }}
    };

    saveJson(j2, "j2");
}

TEST(test, testNewJson) {
    saveJson(newJson(), "newBook");
}

TEST(test, testBook) {

}