//
// Created by hasee on 2021/11/30.
//

#include "Book.h"

#include <iostream>
#include <fstream>

using namespace std;



void Book::Create(const string &dir) {
    ifstream i("../template/template.json");
    ofstream o(dir + "/newBook.json");
    o << i.rdbuf();
}

Book::Book() = default;

Book::Book(json jsonFile) {
    contentJson = std::move(jsonFile);
    contentJson.get<outline::Illustration>();
}

Book::Book(const string& jsonFilePath) {
    ifstream i(jsonFilePath);
    i >> contentJson;
}

std::ostream &operator<<(std::ostream &out, Book &book) {
    out << setw(4) << book.contentJson << endl;
    return out;
}

void to_json(json &j, const Book &b) {
    j = json {
            {"metadata", b.metadata},
            {"contributor", b.contributor},
            {"context", b.context},
            {"illustration", b.illustration}
    };
}

void Book::CreateBuild(const string &path) {
    string command;
    sprintf(command.data(), "md %s/", path.data());

    auto mkdir = [&command] (const string &path) {
        cout << command + path << endl;
    };

    string dir_name;
    sprintf(dir_name.data(), "[%s].%s%s.%s",
            contentJson["author"].get<string>().data(),
            contentJson["title"].get<string>().data(),
            contentJson["subtitle"].get<string>().data(),
            contentJson["volume"].get<string>().data());
    mkdir("");

//    system(command + "");
}

void outline::to_json(json &j, const outline::Context &c)  {
    j = json {
            {"preface", c.preface},
            {"preface2", c.preface2},
            {"chapter", c.chapter},
            {"afterword", c.afterword}
    };
}

void outline::from_json(const json &j, outline::Context &c) {

}

void outline::to_json(json &j, const outline::Illustration &i) {
    j = json {
            {"color", i.color},
            {"gray", i.gray}
    };
}

void outline::from_json(const json &j, outline::Illustration &i) {

}

void outline::to_json(json &j, const outline::Content &c) {
    j = json {
            {"title", c.title},
            {"subtitle", c.subtitle},
            {"volume", c.volume},
            {"author", c.author},
            {"cover", c.cover},
            {"extraContributor", c.extraContributor},
            {"illustrator", c.illustrator},
            {"context", c.context},
            {"illustration", c.illustration}
    };
}

void outline::from_json(const json &j, outline::Content &c) {

}

void outline::to_json(json &j, const outline::Contributor &c) {
    j = json {
            {"author", c.author},
            {"illustrator", c.illustrator},
            {"others", c.others}
    };
}

void outline::from_json(const json &j, outline::Contributor &c) {

}

void outline::to_json(json &j, const outline::Metadata &m) {
    j = json {
            {"title", m.title},
            {"subtitle", m.subtitle},
            {"volume", m.volume},
            {"cover", m.cover},
            {"backCover", m.backCover},
            {"extra", m.extra}
    };
}

void outline::from_json(const json &j, outline::Metadata &m) {

}
