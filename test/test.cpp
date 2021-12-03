//
// Created by hasee on 2021/11/29.
//

#include "../util/Book.h"

#include <gtest/gtest.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using json = nlohmann::json;

#define OutPutRoot string("../test/result/")
#define OriginRoot string("../test/origin/")

void saveJson(const json &j, const string &name) {
    ofstream o(OutPutRoot + name);
    o << setw(4) << j << endl;
}

void getJson(json &j, const string &name) {
    ifstream i(OriginRoot + name);
    i  >> j;
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
    saveJson(newJson(), "newBook.json");
}

TEST(test, testGetJson) {
    json j;
    getJson(j, "j2.json");
}

TEST(test, testBook) {
    json j;
    getJson(j, "Missing 1.json");
    Book book(j);
    cout << book << endl;
}