//
// Created by hasee on 2021/12/16.
//

#ifndef TEXTTOEPUB_OUTLINE_H
#define TEXTTOEPUB_OUTLINE_H

#include "util.h"

namespace outline {

    struct Contents {
        string preface;
        std::vector<string> chapters;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Contents, preface, chapters)
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
        context::Annotation note;
        int sectionNumberBegin = 0;
        std::vector<string> whitespace, separators, extra;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Metadata, title, subtitle, volume, cover, backCover, sectionNumberBegin, whitespace, separators, note, extra)
    };

//    TODO translator

}

#endif //TEXTTOEPUB_OUTLINE_H
