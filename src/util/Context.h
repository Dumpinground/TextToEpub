//
// Created by hasee on 2021/12/16.
//

#ifndef TEXTTOEPUB_CONTEXT_H
#define TEXTTOEPUB_CONTEXT_H

#include "util.h"

namespace context {
    enum mode {
        single_chapter,
        within_sections,
        illustration
    };

    struct Annotation {
        string symbol;
        string title;
        int number;
        std::vector<string> brackets;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Annotation, symbol, title, number, brackets)

        enum FormatType {
            title_only, symbol_title, brackets_wrapped
        };

        string mark(FormatType type = symbol_title) const;
    };

    struct NotePair {
        Annotation note;
        int word;
        int line;
    };

    struct Section {

        string title;
        std::vector<int> separators;
        std::vector<string> paragraphs;

        int min_index = -1;
        unsigned long long max_index = 0;

        std::map<int, NotePair> note_pairs;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Section, title, separators, paragraphs)

        pugi::xml_node append_to(pugi::xml_node &node);

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
        mode type = single_chapter;

        Chapter();
        Chapter(const Chapter &chapter);
        Chapter(string lang, const string &title);

        void to_xml(const string &path);

        void load_xml();

        friend std::ostream &operator<<(std::ostream &out, Chapter &chapter);

    private:
        pugi::xml_document doc;
    };

    std::ostream &operator<<(std::ostream &out, Chapter &chapter);
}

#endif //TEXTTOEPUB_CONTEXT_H
