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
    o << setw(2) << j << endl;
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

TEST(test, testGetJson) {
    json j;
    getJson(j, "j2.json");
    cout << j["happy"] << j["name"] << endl;
    getJson(j, "missing 2.json");
    cout << j["title"] << endl;
}

TEST(test, testNewBook) {
    Book::Create(OutPutRoot);
}

TEST(test, testStructToJson) {
    using namespace outline;

    Illustration illustration;
    Context context;
    Content content;
    Contributor contributor;
    Metadata metadata;
    json j = illustration;
    saveJson(j, "illus.json");
    j = context;
    saveJson(j, "context.json");
    j = contributor;
    saveJson(j, "contributor.json");
    j = metadata;
    saveJson(j, "metadata.json");

    Book book;
    j = book;
    saveJson(j, "book.json");
}

TEST(test, testJsonToStrut) {

}

TEST(test, testBook) {
    Book book(OriginRoot + "missing 2.json");
    cout << book << endl;
}

TEST(test, testCreateBuild) {
    Book book(OriginRoot + "missing 2.json");
    book.CreateBuild(OutPutRoot);
}