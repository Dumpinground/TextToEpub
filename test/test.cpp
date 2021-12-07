//
// Created by hasee on 2021/11/29.
//

#include "../util/Book.h"

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <pugixml.hpp>
#include <boost/filesystem.hpp>
#include <codecvt>

using namespace std;
using json = nlohmann::json;

#define OutPutRoot string("../test/result/")
#define OriginRoot string("../test/origin/")
#define AccessibleEpub3Root string("F:/Epub/learn/accessible_epub_3/")

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

TEST(test, testXHTML) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file((AccessibleEpub3Root + "EPUB/ch01.xhtml").data());
    cout << "Load result: " << result.description() << endl;
    doc.save_file((TemplateRoot + "chapter.xhtml").data());
}

TEST(test, testOpf) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file((AccessibleEpub3Root + "EPUB/package.opf").data());
    cout << "Load result: " << result.description() << endl;
    doc.save_file((TemplateRoot + "package.opf").data());
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

TEST(test, testSystem) {
    system("sudo md ../test/result/1");
}

TEST(test, testFileSystem) {
    boost::filesystem::path path(ImagesRoot);
    boost::filesystem::directory_iterator dir_end;
    for (boost::filesystem::directory_iterator dir_it(path); dir_it != dir_end; ++dir_it) {
        cout << dir_it->path().filename() << endl;
    }
}

TEST(test, testCodeCvt) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt_utf;
    string str = "../test/result/[甲]/";
    std::wstring wstr;
    wstr = cvt_utf.from_bytes(str);
    str = cvt_utf.to_bytes(wstr);
    boost::filesystem::create_directory(wstr);
}

TEST(test, testMkDir) {

    mkDir("../test/result/book_dir/", [](const string &path) {

        ofstream os(path + "mimetype");
        os << "application/epub+zip";
        os.close();

        mkDir(path + "META-INF/", [](const string &path) {
            pugi::xml_document doc;
            doc.load_file((TemplateRoot + "container.xml").data());
            bool flag = doc.save_file((path + "container.xml").data());
            cout << flag << endl;
        });

        mkDir(path + "EPUB/", [](const string &path) {
            mkDir(path + "Styles/");
            mkDir(path + "Text/");
            mkDir(path + "Images/", [](const string &path) {
                boost::filesystem::path image_path(ImagesRoot);
                boost::filesystem::directory_iterator dir_end;
                for (boost::filesystem::directory_iterator dir_it(image_path); dir_it != dir_end; ++dir_it) {
                    cout << dir_it->path().string() << " " << path + dir_it->path().filename().string() << endl;
                    ifstream i(dir_it->path().string(), ios_base::binary);
                    ofstream o(path + dir_it->path().filename().string(), ios_base::binary);
                    o << i.rdbuf();
                }
            });
        });
    });

    boost::filesystem::rename("../test/result/book_dir/", WS("../test/result/[甲].Ⅱ/"));
}

TEST(test, testCreateBuild) {
    json j;
    getJson(j, "missing 2.json");
    Book book = j.get<Book>();
    book.CreateBuild(OutPutRoot);
//    book.PackBuild();
}