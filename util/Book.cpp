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
    return "[" + contributor.author + "]." + metadata.title + " " + metadata.subtitle + "." + metadata.volume;
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
    wrapped = "^[" + whitespace + "]*(" + wrapped + ")$";
    return wrapped;
}

void Book::extract(const string &inputTextPath, const string &outPutDir, bool showContent) {
    ifstream text(inputTextPath);
    string line;
    whitespace = boost::join(metadata.whitespace, "") + "\\s";

    auto colorImageName = illustrations.color.begin();
    auto chapterTitle = content.chapters.begin();

    context::Chapter *chapter;
    context::Section *section;

    map<string, bool> flag;
    map<string, regex*> expression;

    expression["start"] = new regex("^" + metadata.title);
    expression["preface"] = new regex(wrap(content.preface));
    expression["illustration"] = new regex("^\\[" + *colorImageName + "\\]" );
    expression["preface2"] = new regex(wrap(content.preface2));
    expression["chapter"] = new regex(wrap(*chapterTitle));
    expression["afterword"] = new regex(wrap(content.afterword));

    expression["separator"] = new regex(wrap(boost::join(metadata.separators, "|")));
    expression["space"] = new regex("^[" + whitespace + "]*");

    auto appendParagraph = [&expression, &line] (context::Section *section) {
        if (regex_match(line, *expression["separator"])) {
            int index = section->paragraphs.size();
            section->separators.emplace_back(index);

            line = regex_replace(line, *expression["space"], "");
        }

        if (!line.empty()) {
            section->setInterval();
        }

        section->paragraphs.emplace_back(line);
    };
    bool sectionMode = false;

    auto checkStatus = [&] (const string &status,
            const function<void()> &before = [] {},
            const function<void()> &begin = [] {}) {
        flag[status] = regex_match(line, *expression[status]);
        if (!flag[status]) {
            before();
        } else {
            begin();
        }
    };

    auto newChapter = [&] {
        chapter = new context::Chapter("zh-CN", *chapterTitle);
        chapter->nextSection = metadata.sectionNumberBegin;
        expression["section"] = new regex(wrap(to_string(chapter->nextSection)));
        chapters.emplace_back(chapter);
        sectionMode = false;

        if (chapterTitle != content.chapters.end())
            ++chapterTitle;
        if (chapterTitle != content.chapters.end())
            expression["chapter"] = new regex(wrap(*chapterTitle));
    };

    while (!text.eof()) {

        getline(text, line);

        if (!flag["preface"]) {
            checkStatus("preface", [] {}, [&] {
                preface = new context::Chapter("zh-CN",content.preface);
            });
            continue;
        }

//        if (!flag["illustration"]) {
//            flag["illustration"] = regex_match(line, *expression["illustration"]);
//            if (!flag["illustration"]) {
//
//                appendParagraph(preface);
//            }
//            continue;
//        }

        if (!flag["preface2"]) {
            checkStatus("preface2", [&] {
                appendParagraph(preface);
                }, [&] {
                preface2 = new context::Chapter("zh-CN", content.preface2);
            });
            continue;
        }

        if (!flag["chapter"]) {
            checkStatus("chapter", [&] {
                appendParagraph(preface2);
                }, [&] {
                newChapter();
            });
            continue;
        }

        if (!flag["afterword"]) {
            checkStatus("afterword", [&] {

                if (regex_match(line, *expression["chapter"])) {
                    newChapter();
                } else if (regex_match(line, *expression["section"])) {
                    section = new context::Section();
                    section->title = line.back();
                    chapter->sections.emplace_back(section);
                    expression["section"] = new regex(wrap(to_string(++chapter->nextSection)));
                    sectionMode = true;
                } else {
                    appendParagraph(sectionMode ? section : chapter);
                }

            }, [&] {
                afterword = new context::Chapter("zh-CN", content.afterword);
            });
            continue;
        }

        appendParagraph(afterword);
    }

    if (showContent) {
        for (auto c: chapters) {
            cout << *c << endl;
        }
    }

    string name;

    name = "preface.xhtml";
    preface->to_xml(outPutDir + name);

    name = "preface2.xhtml";
    preface2->to_xml(outPutDir + name);

    for (int i = 0; i < chapters.size(); ++i) {
        name = "chapter" + to_string(i) + ".xhtml";
        chapters[i]->to_xml(outPutDir + name);
    }

    name = "afterword.xhtml";
    afterword->to_xml(outPutDir + name);
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

    for (int i = min_index, j = 0; i <= max_index; ++i) {

        auto p = section.append_child("p");

        if (paragraphs[i].empty())
            p.append_child("br");
        else
            p.text() = paragraphs[i].data();

        // separators empty -> pass
        if (j < separators.size() && separators[j] == i) {
            p.append_attribute("class") = "tiny title";
            j++;
        }
    }
    return section;
}

ostream &context::operator<<(ostream &out, Section &section) {
    out << section.title << endl;
    for (auto i: section.separators) {
        cout << section.paragraphs[i] << endl;
    }
    return out;
}

/** set the interval of valid index \n
 *  compared with paragraph.size() \n
 */
void context::Section::setInterval() {
    if (min_index == -1) min_index = paragraphs.size();
    max_index = max(max_index, paragraphs.size());
}

ostream &context::operator<<(ostream &out, Chapter &chapter) {
    out << chapter.title << endl;
    for (auto i: chapter.separators) {
        out << chapter.paragraphs[i] << endl;
    }
    for (auto s: chapter.sections) {
        out << *s;
    }
    return out;
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
