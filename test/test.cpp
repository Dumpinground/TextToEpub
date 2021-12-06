//
// Created by hasee on 2021/11/29.
//

#include "../util/Book.h"

#include <gtest/gtest.h>

#include <iostream>
#include <fstream>
#include <string>
#include <pugixml.hpp>

using namespace std;
using json = nlohmann::json;

#define OutPutRoot string("../test/result/")
#define OriginRoot string("../test/origin/")
#define TemplateRoot string("../template/")

void saveJson(const json &j, const string &name, const string& root = OutPutRoot) {
    ofstream o(root + name);
    o << setw(2) << j << endl;
}

void getJson(json &j, const string &name, const string& root = OriginRoot) {
    ifstream i(root + name);
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

TEST(test, testXML) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file((TemplateRoot + "container.xml").data());
    cout << "Load result: " << result.description() << endl
    << ", mesh name: " << doc.child("container").child("rootfiles").child("rootfile").attribute("full-path").value() << endl;
    doc.save_file((TemplateRoot + "new container.xml").data());
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

TEST(StructToJson, testIllustration) {
    outline::Illustration illustration;
    saveJson(illustration, "illustration.json");
}

TEST(StructToJson, testContext) {
    outline::Context context;
    saveJson(context, "context.json");
}

TEST(StructToJson, testContributor) {
    outline::Contributor contributor;
    saveJson(contributor, "contributor.json");
}

TEST(StructToJson, testMetadata) {
    outline::Metadata metadata;
    saveJson(metadata, "metadata.json");
}

TEST(StructToJson, testBook) {
    Book book;
    saveJson(book, "book.json");
}

TEST(JsonToStrut, testIllustration) {
    outline::Illustration illustration;
    json j;
    getJson(j, "illustration.json", OutPutRoot);
    illustration = j.get<outline::Illustration>();
}

TEST(JsonToStrut, testContext) {
    outline::Context context;
    json j;
    getJson(j, "context.json", OutPutRoot);
    context = j.get<outline::Context>();
}

TEST(JsonToStrut, testContributor) {
    outline::Contributor contributor;
    json j;
    getJson(j, "contributor.json", OutPutRoot);
    contributor = j.get<outline::Contributor>();
}

TEST(JsonToStrut, testMetadata) {
    outline::Metadata metadata;
    json j;
    getJson(j, "metadata.json", OutPutRoot);
    metadata = j.get<outline::Metadata>();
}

TEST(JsonToStrut, testBook) {
    json j;
    getJson(j, "missing 2.json");
    Book book = j.get<Book>();
    cout << book << endl;
}

TEST(test, testCreateBuild) {
    json j;
    getJson(j, "missing 2.json");
    Book book = j.get<Book>();
    book.CreateBuild(OutPutRoot);
}