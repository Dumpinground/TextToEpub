//
// Created by hasee on 2021/11/30.
//

#include "Book.h"

#include <iomanip>
#include <boost/filesystem.hpp>
#include <codecvt>
#include <utility>
#include <queue>

using namespace std;

void saveJson(const json &j, const string &name, const string& root) {
    ofstream o(root + name);
    o << setw(2) << j << endl;
}

json getJson(const string &name, const string& root) {
    json j;
    ifstream i(root + name);
    i  >> j;
    return j;
}

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

string Book::fullTitle() const {
    return metadata.title + " " + metadata.subtitle + " " + metadata.volume;
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

void Book::BuildInit(const string &path) {
    CreateResourceDir(path);
}

void Book::CreateResourceDir(const string &path) {
    ResourceRoot = path + "resources/";
    boost::filesystem::create_directory(ResourceRoot);
    boost::filesystem::create_directory(ImagesRoot());
    boost::filesystem::create_directory(TextRoot());
    boost::filesystem::create_directory(DataRoot());
    saveJson(*this, "new book.json", DataRoot());
}

string Book::dir_path() {
    return book_dir + dir_name + "/";
}

void Book::CreateBuildDir(const string &path) {

    book_dir = path;
    pugi::xml_document doc;

    mkDir(dir_path(), [this](const string &path) {

        ofstream os(path + "mimetype");
        os << "application/epub+zip";
        os.close();

        mkDir(path + "META-INF/", [](const string &path) {
            pugi::xml_document doc;
            doc.load_file((TemplateRoot + "container.xml").data());
            doc.save_file((path + "container.xml").data());
        });

        mkDir(path + "EPUB/", [this](const string &path) {

            pugi::xml_document doc;
            doc.load_file((TemplateRoot + "package.opf").data());
            doc.save_file((path + "package.opf").data());

            mkDir(path + "Styles/",[](const string &path) {
                boost::filesystem::copy_file("../test/origin/style.css", path + "style.css");
            });
            mkDir(path + "Images/", [this](const string &path) {
                copyFiles(ImagesRoot(), path);
            });
        });
    });
}

void Book::PackBook() {
    if (!book_dir.empty())
        boost::filesystem::rename(book_dir + dir_name, WS(book_dir + eBookName()));
}

string Book::wrap(string wrapped) const {
    wrapped = "^[" + whitespace + "]*(" + wrapped + ")$";
    return wrapped;
}

string Book::imageWrap(string wrapped) const {
    boost::replace_last(wrapped, ".", "\\.");
    wrapped = "^\\[" + wrapped + "\\]$";
    return wrapped;
}

struct StatusProcess {
    function<void()> begin, process;
};

void Book::extract(const string &inputTextPath, const string &outPutDir, bool showContent) {
    ifstream text(inputTextPath);
    if (!text.is_open()) {
        cout << "no file found in " + inputTextPath << endl;
        return;
    }
    string line;
    whitespace = boost::join(metadata.whitespace, "") + "\\s";

    auto colorImageName = illustrations.color.begin();
    auto chapterTitle = contents.chapters.begin();

    context::Chapter *chapter;
    context::ColorIllustration *colorChapter;
    context::Section *section;

    map<string, bool> flag;
    map<string, regex*> expression;
    map<string, StatusProcess*> statusProcess;

    queue<string> status_queue;

    status_queue.push("start");
    status_queue.push("preface");
    status_queue.push("illustration");
    status_queue.push("preface2");
    status_queue.push("chapter");
    status_queue.push("afterword");

    expression["start"] = new regex("^" + metadata.title);
    expression["preface"] = new regex(wrap(contents.preface));
    expression["illustration"] = new regex(imageWrap(*colorImageName));
    expression["preface2"] = new regex(wrap(contents.preface2));
    expression["chapter"] = new regex(wrap(*chapterTitle));
    expression["afterword"] = new regex(wrap(contents.afterword));

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

    auto newChapter = [&] {
        chapter = new context::Chapter(lang, *chapterTitle);
        chapter->nextSection = metadata.sectionNumberBegin;
        expression["section"] = new regex(wrap(to_string(chapter->nextSection)));
        chapters.emplace_back(chapter);
        sectionMode = false;

        if (chapterTitle != contents.chapters.end())
            ++chapterTitle;
        if (chapterTitle != contents.chapters.end())
            expression["chapter"] = new regex(wrap(*chapterTitle));
    };

    auto newColorIllustration = [&] {
        colorChapter = new context::ColorIllustration(lang, *colorImageName);
        colorIllustrations.emplace_back(colorChapter);
        if (colorImageName != illustrations.color.end())
            ++colorImageName;
        if (colorImageName != illustrations.color.end())
            expression["illustration"] = new regex(imageWrap(*colorImageName));
    };

    statusProcess["start"] = new StatusProcess {
        [] {}, [] {}
    };

    statusProcess["preface"] = new StatusProcess {
            [&] {
                preface = new context::Chapter(lang, contents.preface);
            },
            [&] {
                appendParagraph(preface);
            }
    };

    statusProcess["illustration"] = new StatusProcess {
        [&] {
            newColorIllustration();
        },
        [&] {
            if (regex_match(line, *expression["illustration"])) {
                newColorIllustration();
            } else {
                appendParagraph(colorChapter);
            }
        }
    };

    statusProcess["preface2"] = new StatusProcess {
        [&] {
            preface2 = new context::Chapter(lang, contents.preface2);
            },
            [&] {
            appendParagraph(preface2);
        }
    };

    statusProcess["chapter"] = new StatusProcess {
        [&] {
            newChapter();
        },
        [&] {
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
        }
    };

    statusProcess["afterword"] = new StatusProcess {
        [&] {
            afterword = new context::Chapter(lang, contents.afterword);
        },
        [&] {
            appendParagraph(afterword);
        }
    };

    auto setStatus = [&status_queue] (string &status) {
        status = status_queue.front();
        status_queue.pop();
    };

    string status;
    setStatus(status);
    while (!text.eof()) {

        getline(text, line);

        if (!status_queue.empty() && regex_match(line, *expression[status_queue.front()])) {
            setStatus(status);
            statusProcess[status]->begin();
        } else {
            statusProcess[status]->process();
        }
    }



    if (showContent) {
        for (auto c: chapters) {
            cout << *c << endl;
        }
    }

    string name;
    name = "cover.xhtml";
    {
        pugi::xml_document cover;
        cover.load_file((TemplateRoot + name).data());
        auto html = cover.child("html");
        html.attribute("xml:lang") = lang.data();
        html.attribute("lang") = lang.data();
        auto img = html.child("body").child("figure").child("img");
        img.attribute("src") = ("Images/" + metadata.cover).data();
        img.attribute("alt") = metadata.cover.data();
        cover.save_file((outPutDir + name).data());
    }

    name = "toc.xhtml";
    {
        pugi::xml_document toc;
        toc.load_file((TemplateRoot + name).data());
        auto html = toc.child("html");
        html.attribute("xml:lang") = lang.data();
        html.attribute("lang") = lang.data();
        html.child("head").child("title").text() = fullTitle().data();

        auto body = html.child("body");
        body.child("h1").text() = fullTitle().data();
        auto nav = body.child("nav");
        nav.child("h2").text() = "Contents";

        auto content_list = nav.child("ol");
        auto listAdd = [] (pugi::xml_node &ol, const string &href, const string &title) {
            auto a = ol.append_child("li").append_child("a");
            a.append_attribute("href") = href.data();
            a.text() = title.data();
        };
        listAdd(content_list, "preface.xhtml", contents.preface);
        listAdd(content_list, "illustration0.html", "插图");
        listAdd(content_list, "preface2.xhtml", contents.preface2);
        for (int i = 0; i < contents.chapters.size(); i++) {
            listAdd(content_list, "chapter" + to_string(i) + ".xhtml", contents.chapters[i]);
        }
        listAdd(content_list, "afterword.xhtml", contents.afterword);

        toc.save_file((outPutDir + name).data());
    }

    name = "preface.xhtml";
    preface->to_xml(outPutDir + name);

    for (int i = 0; i < colorIllustrations.size(); ++i) {
        name = "illustration" + to_string(i) + ".xhtml";
        colorIllustrations[i]->to_xml(outPutDir + name);
    }

    name = "preface2.xhtml";
    preface2->to_xml(outPutDir + name);

    for (int i = 0; i < chapters.size(); ++i) {
        name = "chapter" + to_string(i) + ".xhtml";
        chapters[i]->to_xml(outPutDir + name);
    }

    name = "afterword.xhtml";
    afterword->to_xml(outPutDir + name);

    buildPackage(outPutDir);
}

void Book::buildPackage(const string &outPutDir) {
    pugi::xml_document doc;
    doc.load_file((TemplateRoot + "package.opf").data());

    vector<string> chapterList = {"preface"};
    for (int i = 0; i < illustrations.color.size(); ++i) {
        chapterList.push_back("illustration" + to_string(i));
    }
    chapterList.emplace_back("preface2");
    for (int i = 0; i < chapters.size(); ++i) {
        chapterList.push_back("chapter" + to_string(i));
    }
    chapterList.emplace_back("afterword");

    auto package = doc.child("package");
    package.attribute("xml:lang") = lang.data();

    auto metadata = package.child("metadata");
    metadata.child("dc:identifier").append_attribute("opf:scheme") = "UUID";
    metadata.child("dc:identifier").text() = ("urn:uuid:" + uuid4()).data();
    metadata.child("dc:title").text() = fullTitle().data();
    metadata.child("dc:language").text() = lang.data();
    metadata.child("dc:creator").text() = contributor.author.data();

    auto manifest = package.child("manifest");
    auto spine = package.child("spine");

    auto appendItem = [&] (const string& id, const string& href, const string& media_type)->pugi::xml_node {
        auto item = manifest.append_child("item");
        item.append_attribute("id") = id.data();
        item.append_attribute("href") = href.data();
        item.append_attribute("media-type") = media_type.data();
        return item;
    };

    appendItem("cover-image", this->metadata.cover, "image/jpeg")
    .append_attribute("properties") = "cover-image";
    auto itemref = spine.append_child("itemref");
    itemref.append_attribute("idref") = "cover";
    itemref.append_attribute("linear") = "no";

    for (const auto& c: chapterList) {
        appendItem(c, c + ".xhtml", "application/xhtml+xml");
        spine.append_child("itemref").append_attribute("idref") = c.data();
    }

    for (const auto& i : illustrations.color) {
        appendItem(i, "Images/" + i, "image/jpeg");
    }

    for (const auto& i : illustrations.gray) {
        appendItem(i, "Images/" + i, "image/jpeg");
    }

    doc.save_file((outPutDir + "package.opf").data());
}

string Book::ImagesRoot() {
    return ResourceRoot + "images/";
}

string Book::TextRoot() {
    return ResourceRoot + "text/";
}

string Book::DataRoot() {
    return ResourceRoot + "data/";
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
            p.append_attribute("class") = "tiny-title";
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

context::Chapter::Chapter() = default;

context::Chapter::Chapter(const context::Chapter &chapter)
: Section(chapter), lang(chapter.lang), sections(chapter.sections) {}

context::Chapter::Chapter(string lang, const string &title)
: Section(title), lang(std::move(lang)), sections() {}

context::ColorIllustration::ColorIllustration() = default;

context::ColorIllustration::ColorIllustration(const ColorIllustration &illustration)
: Section(illustration), lang(illustration.lang) {}

context::ColorIllustration::ColorIllustration(const string &lang, const string &title)
: Section(title), lang(lang) {}

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

void context::ColorIllustration::to_xml(const string &path) {
    doc.load_file((TemplateRoot + "illustration.xhtml").data());
    pugi::xml_node html = doc.child("html");
    html.attribute("xml:lang") = lang.data();
    html.attribute("lang") = lang.data();
    html.child("head").child("title").text() = title.data();

    auto figure = html.child("body").child("figure");

    auto img = figure.child("img");
    img.attribute("src") = ("../Images/" + title).data();
    img.attribute("alt") = title.data();

    for (int i = min_index; i <= max_index; i++) {
        if (paragraphs[i].empty())
            figure.append_child("p").append_child("br");
        else
            figure.append_child("p").text() = paragraphs[i].data();
    }
    doc.save_file(path.data());
}
