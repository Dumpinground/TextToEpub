//
// Created by hasee on 2021/11/30.
//

#ifndef TEXTTRANSFORM_BOOK_H
#define TEXTTRANSFORM_BOOK_H

#include <nlohmann/json.hpp>

#include <string>
#include <functional>
#include <iostream>
#include <pugixml.hpp>
#include <regex>
#include <boost/algorithm/string.hpp>

using string = std::string;
using json = nlohmann::json;

#define VNAME(x) #x
#define VDUMP(x) std::cout << VLIST(x) << std::endl

#define TemplateRoot string("../template/")
#define ResourcesRoot string("../resources/")
#define ImagesRoot (ResourcesRoot + "images/")
#define TextRoot (ResourcesRoot + "text/")

json newJson();

namespace outline {

    struct Content {
        string preface, preface2;
        std::vector<string> chapters;
        string afterword;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Content, preface, preface2, chapters, afterword)
    };

    struct Illustrations {
        std::vector<string> color, gray;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Illustrations, color, gray)
    };

    struct Contributor {
        string author, illustrator;
        std::vector<string> others;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Contributor, author, illustrator, others)
    };

    struct Metadata {
        string title, subtitle, volume, cover, backCover;
        std::vector<string> whitespace, separators, extra;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Metadata, title, subtitle, volume, cover, backCover, whitespace, separators, extra)
    };

//    TODO translator

}

namespace context {

    struct Section {

        // <section><h4 class = "title"> title </h4>
        string title;

        std::vector<int> separators;

        // <p>
        std::vector<string> paragraphs;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Section, title, separators, paragraphs)

        pugi::xml_node append(pugi::xml_node &node) const;

        Section();
        Section(const Section &section);
        Section(string title);
    };

    struct Chapter : public Section {

        string lang;
        std::vector<Section *> sections;

//        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Chapter, lang, sections, title, separators, paragraphs)

        Chapter();
        Chapter(const Chapter &chapter);
        Chapter(string lang, const string &title);

        void to_xml(const string &path);

        void load_xml();

//        void set_title(const string &title);
//
//        bool find_title(const string &line);

    private:
        pugi::xml_document doc;
//        std::regex *expression;
    };
}

class Book {

private:
    string book_dir;
    string dir_name = "book_dir";

public:
    static void Create(const string &dir);

    outline::Metadata metadata;
    outline::Contributor contributor;
    outline::Content content;
    outline::Illustrations illustrations;

    Book();

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Book, metadata, contributor, content, illustrations)

    std::vector<context::Chapter *> chapters;

    string eBookName() const;

    void CreateBuild(const string &path);
    void PackBuild();

    string wrap(string wrapped) const;
    void extractChapter(const string &inputTextPath, const string &outPutDir);

    bool find(const string& text);

    friend std::ostream &operator<<(std::ostream &out, Book &book);
};

std::wstring WS(const string &s);

void mkDir(const string &path, const std::function<void(const string &path)> &visit = [] (const string &path) {});

bool find(const string& target, const string& text);

#endif //TEXTTRANSFORM_BOOK_H
