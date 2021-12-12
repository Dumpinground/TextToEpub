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
        int sectionNumberBegin;
        std::vector<string> whitespace, separators, extra;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Metadata, title, subtitle, volume, cover, backCover, sectionNumberBegin, whitespace, separators, extra)
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
        int min_index = -1;
        unsigned long long max_index = 0;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Section, title, separators, paragraphs)

        pugi::xml_node append(pugi::xml_node &node) const;

        void to_xml(const string &path);

        Section();
        Section(const Section &section);
        Section(string title);

        friend std::ostream &operator<<(std::ostream &out, Section &section);

        void setInterval();
    };

    std::ostream &operator<<(std::ostream &out, Section &section);

    struct Chapter : public Section {

        string lang;
        std::vector<Section *> sections;
        int nextSection;

//        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Chapter, lang, sections, title, separators, paragraphs)

        Chapter();
        Chapter(const Chapter &chapter);
        Chapter(string lang, const string &title);

        void to_xml(const string &path);

        void load_xml();

//        void set_title(const string &title);
//
//        bool find_title(const string &line);

        friend std::ostream &operator<<(std::ostream &out, Chapter &chapter);

    private:
        pugi::xml_document doc;
//        std::regex *expression;
    };

    std::ostream &operator<<(std::ostream &out, Chapter &chapter);
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

    string eBookName() const;

    void CreateBuild(const string &path);
    void PackBuild();

    string wrap(string wrapped) const;

    void extract(const string &inputTextPath, const string &outPutDir, bool showContent = false);

    bool find(const string& text);

    friend std::ostream &operator<<(std::ostream &out, Book &book);

private:
    string whitespace;

    context::Chapter *preface, *preface2, *afterword;
    std::vector<context::Chapter *> chapters;
};

std::wstring WS(const string &s);

void mkDir(const string &path, const std::function<void(const string &path)> &visit = [] (const string &path) {});

bool find(const string& target, const string& text);

#endif //TEXTTRANSFORM_BOOK_H
