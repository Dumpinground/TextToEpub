//
// Created by hasee on 2021/11/30.
//

#include "Book.h"

#include <iomanip>
#include <boost/filesystem.hpp>
#include <codecvt>
#include <utility>

using namespace std;

void Book::Create(const string &dir) {
    ifstream i("../template/template.json");
    ofstream o(dir + "/newBook.json");
    o << i.rdbuf();
}

Book::Book() = default;

std::ostream &operator<<(std::ostream &out, Book &book) {
    out << setw(4) << json(book) << endl;
    return out;
}

//code transformer  utf-8 -> utf-16
std::wstring WS(const string &s)  {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt_utf;
    return cvt_utf.from_bytes(s);
}

void mkDir(const string &path, const function<void(const string &path)> &visit) {
    boost::filesystem::create_directory(WS(path));
    visit(path);
}

bool find(const string& target, const string& text) {
    regex expression("^\\s*" + target + "\\s*$");
    return regex_match(text, expression);
}

string Book::eBookName() const {
    return "[" + contributor.author + "]." + metadata.title + metadata.subtitle + "." + metadata.volume;
}

//copy all files in the src_path
void copyFiles(const string &src_path, const string &dist_path) {
    boost::filesystem::path path(src_path);
    boost::filesystem::directory_iterator dir_end;
    for (boost::filesystem::directory_iterator dir_it(path); dir_it != dir_end; ++dir_it) {
        boost::filesystem::copy(dir_it->path(), WS(dist_path + dir_it->path().filename().string()) );
//        ifstream i(dir_it->path().string(), ios_base::binary);
//        ofstream o(dist_path + dir_it->path().filename().string(), ios_base::binary);
//        o << i.rdbuf();
    }
}

void Book::CreateBuild(const string &path) {

    book_dir = path;
    pugi::xml_document doc;

    string dir_path = path + dir_name + "/";
    mkDir(dir_path, [](const string &path) {

        ofstream os(path + "mimetype");
        os << "application/epub+zip";
        os.close();

        mkDir(path + "META-INF/", [](const string &path) {
            pugi::xml_document doc;
            doc.load_file((TemplateRoot + "container.xml").data());
            doc.save_file((path + "container.xml").data());
        });

        mkDir(path + "EPUB/", [](const string &path) {

            pugi::xml_document doc;
            doc.load_file((TemplateRoot + "package.opf").data());
            doc.save_file((path + "package.opf").data());

            mkDir(path + "Styles/");
            mkDir(path + "Images/", [](const string &path) {
//                copyFiles(ImagesRoot, path);
            });
        });
    });
}

void Book::PackBuild() {
    if (!book_dir.empty())
        boost::filesystem::rename(book_dir + dir_name, WS(book_dir + eBookName()));
}

string Book::wrap(string wrapped) const {
    string whitespace = boost::join(metadata.whitespace, "|");
    wrapped = "^[\\s" + whitespace + "]*(" + wrapped + ")$";
    return wrapped;
}

void Book::extractChapter(const string &inputTextPath, const string &outPutDir) {
    ifstream text(inputTextPath);
    string line;

    auto chapterTitle = content.chapters.begin();

//    regex *title_exp, *section_exp;
    map<string, regex*> expression;
    expression["chapter"] = new regex(wrap(*chapterTitle));
    expression["section"] = new regex(wrap("[0-9]"));
    expression["separator"] = new regex(wrap(boost::join(metadata.separators, "|")));
    context::Chapter *chapter;

    while (!text.eof()) {

        getline(text, line);

        if (regex_match(line, *expression["chapter"])) {

            chapter = new context::Chapter("zh-CN", *chapterTitle);
            chapters.emplace_back(chapter);

            if (chapterTitle != content.chapters.end())
                ++chapterTitle;
            if (chapterTitle != content.chapters.end())
                expression["chapter"] = new regex(wrap(*chapterTitle));
        } else {
            if (regex_match(line, *expression["section"])) {

            } else {
                if (regex_match(line, *expression["separator"])) {

                }
                chapter->paragraphs.emplace_back(line);
            }
        }
    }

    for (auto c: chapters) {
        cout << c->title << endl;
    }
}

context::Section::Section() = default;

context::Section::Section(const Section &section) = default;

context::Section::Section(string title)
: title(std::move(title)), separators(), paragraphs() {}

pugi::xml_node context::Section::append(pugi::xml_node &node) const {
    auto section = node.append_child("section");
    section.append_attribute("title") = title.data();
    section.append_child("h4").append_attribute("class") = "title";
    section.child("h4").text() = title.data();
    for (const auto &p: paragraphs) {
        section.append_child("p").text() = p.data();
    }
    return section;
}

void context::Chapter::to_xml(const string &path) {

    doc.load_file((TemplateRoot + "chapter.xhtml").data());

    pugi::xml_node html = doc.child("html");
    html.attribute("xml:lang") = lang.data();
    html.attribute("lang") = lang.data();
    html.child("head").child("title").text() = title.data();

    pugi::xml_node body = html.child("body");
    auto section = append(body);
    section.append_attribute("class") = "chapter";
    section.append_attribute("epub:type") = "chapter";
    section.child("h4").set_name("h2");

    if (!sections.empty()) {
        for (const auto &s: sections) {
            s->append(section);
        }
    }
    doc.save_file(path.data());
}

context::Chapter::Chapter() = default;

context::Chapter::Chapter(const context::Chapter &chapter)
: Section(chapter), lang(chapter.lang), sections(chapter.sections) {}

context::Chapter::Chapter(string lang, const string &title)
: Section(title), lang(std::move(lang)), sections() {}

//void context::Chapter::set_title(const string &title) {
//    this->title = title;
//    expression = new regex(wrap(title));
//}
//
//bool context::Chapter::find_title(const string &line) {
//    return regex_match(line, *expression);
//}
