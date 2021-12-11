//
// Created by hasee on 2021/11/29.
//

#include "../util/Book.h"

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <codecvt>

using namespace std;
using json = nlohmann::json;

#define OutPutRoot string("../test/result/")
#define OriginRoot string("../test/origin/")
#define AccessibleEpub3Root string("F:/Epub/learn/accessible_epub_3/")
#define MissingRoot string("F:/Epub/Missing/")

void saveJson(const json &j, const string &name, const string& root = OutPutRoot) {
    ofstream o(root + name);
    o << setw(2) << j << endl;
}

json getJson(const string &name, const string& root = OriginRoot) {
    json j;
    ifstream i(root + name);
    i  >> j;
    return j;
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
    j = getJson("j2.json");
    cout << j["happy"] << j["name"] << endl;
    j = getJson("missing 2.json");
    cout << j["title"] << endl;
}

TEST(test, testNewBook) {
    Book::Create(OutPutRoot);
}

TEST(StructToJson, testIllustration) {
    outline::Illustrations illustrations;
    saveJson(illustrations, "illustration.json");
}

TEST(StructToJson, testContext) {
    outline::Content context;
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
    outline::Illustrations illustrations;
    json j = getJson("illustration.json", OutPutRoot);
    illustrations = j.get<outline::Illustrations>();
}

TEST(JsonToStrut, testContext) {
    outline::Content context;
    json j = getJson("context.json", OutPutRoot);
    context = j.get<outline::Content>();
}

TEST(JsonToStrut, testContributor) {
    outline::Contributor contributor;
    json j = getJson("contributor.json", OutPutRoot);
    contributor = j.get<outline::Contributor>();
}

TEST(JsonToStrut, testMetadata) {
    outline::Metadata metadata;
    json j = getJson("metadata.json", OutPutRoot);
    metadata = j.get<outline::Metadata>();
}

TEST(JsonToStrut, testBook) {
    json j = getJson("missing 2.json");
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
    Book book = getJson("missing 2.json").get<Book>();
    book.CreateBuild(OutPutRoot);
//    book.PackBuild();
}

TEST(testChapter, testSection) {
    context::Section s;
    saveJson(s, "section.json");
}

TEST(testChapter, testChapter) {
    context::Chapter chapter;
    saveJson(chapter, "chapter.json");
}

TEST(testChapter, testXML) {
    context::Chapter chapter = getJson("chapter1.json").get<context::Chapter>();
    chapter.to_xml(OutPutRoot + "chapter1.xhtml");
}

TEST(testChapter, testXML2) {
    context::Chapter chapter = getJson("missing 2 chapter 1.json").get<context::Chapter>();
    chapter.to_xml(OutPutRoot + "missing 2/chapter 1.xhtml");
}

TEST(testChapter, testReg) {
    string text[] = {"  1  ", "1  ", "  1", "及  1 和 "};
    for (const auto &t: text) {
        cout << t << " -> " << find("1", t) << endl;
    }
}

TEST(test, testCode) {
    wchar_t L = L'　';
    wchar_t L2 = L'＊';
    wprintf(L"\\u%d \\u%d", L, L2);
}

TEST(testChapter, testFindSection) {
    string txt_path = MissingRoot + "text/missing 2.txt";
    Book book = getJson("missing 2.json");

    string exp;
    ifstream i(OriginRoot + "Reg.txt");
    i >> exp;
    i.close();
    regex expression(exp);

    i.open(txt_path);
    string line;
    while (!i.eof()) {
        getline(i, line);
        if (regex_match(line, expression)) {
            cout << line << endl;
        }
    }
}

TEST(testChapter, testFindTitle) {
    string txt_path = MissingRoot + "text/missing 2.txt";
    Book book = getJson("missing 2.json");
    auto chapter = book.content.chapters.begin();
    string separator = "＊";

    auto wrap = [](const vector<string> &v)->string {
        string wrapped = boost::join(v, "|");
        wrapped = "^[\\s　]*(" + wrapped + ")$";
//        cout << wrapped << endl;
        return wrapped;
    };

    smatch result;
    regex *expression;

    auto expressionUpdate = [&]() {
        string wrapped = wrap({*chapter, "[0-9]", separator});
        expression = new regex(wrapped);
    };

    expressionUpdate();

    ifstream i(txt_path);
    string line;
    while (!i.eof()) {
        getline(i, line);
        if (regex_match(line, result, *expression)) {
            cout << result.str() << endl;

            if (result.str() == *chapter) {
                if (chapter != book.content.chapters.end())
                    chapter++;
                if (chapter != book.content.chapters.end())
                    expressionUpdate();
            }
        }
    }
}

TEST(testChapter, testChapterVector) {
    context::Chapter *chapter;
    chapter = new context::Chapter("zh-CN", "1");
    vector<context::Chapter *> chapters;
    chapters.emplace_back(chapter);
    chapter->paragraphs.emplace_back("read it");
    cout << chapters[0]->paragraphs.empty() << endl;
}

TEST(testChapter, testLoad) {
    pugi::xml_document doc;
    auto result = doc.load_file((AccessibleEpub3Root + "EPUB/ch01.xhtml").data());

    cout << result << endl;
}

TEST(testBook, testExtractChapter) {
    Book book = getJson("missing 2.json").get<Book>();
    book.extractChapter(TextRoot + "missing 2 chapters.txt", OutPutRoot + "text/");
}