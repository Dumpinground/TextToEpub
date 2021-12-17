//
// Created by hasee on 2021/12/16.
//

#include "Context.h"

using namespace std;

context::Section::Section() = default;

context::Section::Section(const Section &section) = default;

context::Section::Section(string title)
        : title(std::move(title)), separators(), paragraphs() {}

pugi::xml_node context::Section::append_to(pugi::xml_node &node) {
    auto section = node.append_child("section");
    section.append_attribute("title") = title.data();
    section.append_child("h4").append_attribute("class") = "title";
    section.child("h4").text() = title.data();

    for (int i = min_index, j = 0; i <= max_index; ++i) {

        auto p = section.append_child("p");

        if (paragraphs[i] == "__illustration__") {
            auto img = p.append_child("br");
            img.set_name("img");
            img.append_attribute("src") = paragraphs[i].data();
            img.append_attribute("alt") = paragraphs[i].data();
            continue;
        }

        if (paragraphs[i].empty()) {
            p.append_child("br");
        }

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

    map<string, pugi::xml_node> node;

    if (type == illustration)
        doc.load_file((TemplateRoot + "illustration.xhtml").data());
    else
        doc.load_file((TemplateRoot + "chapter.xhtml").data());

    node["html"] = doc.child("html");
    node["html"].attribute("xml:lang") = lang.data();
    node["html"].attribute("lang") = lang.data();
    node["html"].child("head").child("title").text() = title.data();

    node["body"] = node["html"].child("body");

    if (type == illustration) {
        node["figure"] = node["body"].child("figure");
    } else
        node["section"] = append_to(node["body"]);

    switch (type) {
        case illustration:
            node["img"] = node["figure"].child("img");
            node["img"].attribute("src") = ("../Images/" + title).data();
            node["img"].attribute("alt") = title.data();

            for (int i = min_index; i <= max_index; i++) {
                if (paragraphs[i].empty())
                    node["figure"].append_child("p").append_child("br");
                else
                    node["figure"].append_child("p").text() = paragraphs[i].data();
            }

            break;

        case within_sections:
            node["section"].append_attribute("class") = "chapter";
            node["section"].append_attribute("epub:type") = "chapter";
            node["section"].child("h4").set_name("h2");
            break;

        case single_chapter:
            break;
    }
    if (!sections.empty()) {
        for (const auto &s: sections) {
            s->append_to(node["section"]);
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

string context::Annotation::mark() const {
    return symbol + to_string(number);
}
