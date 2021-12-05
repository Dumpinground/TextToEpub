//
// Created by hasee on 2021/11/30.
//

#ifndef TEXTTRANSFORM_BOOK_H
#define TEXTTRANSFORM_BOOK_H

#include <nlohmann/json.hpp>

#include <string>

using string = std::string;
using json = nlohmann::json;

#define VNAME(x) #x
#define VDUMP(x) std::cout << VLIST(x) << std::endl

json newJson();

namespace outline {

    struct Context {
        string preface, preface2;
        std::vector<string> chapter;
        string afterword;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Context, preface, preface2, chapter, afterword)
    };

    struct Illustration {
        std::vector<string> color, gray;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Illustration, color, gray)
    };

    struct Contributor {
        string author, illustrator;
        std::vector<string> others;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Contributor, author, illustrator, others)
    };

    struct Metadata {
        string title, subtitle, volume, cover, backCover;
        std::vector<string> extra;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Metadata, title, subtitle, volume, cover, backCover, extra)
    };

//    TODO translator

}

class Book {
public:
    static void Create(const string &dir);

    outline::Metadata metadata;
    outline::Contributor contributor;
    outline::Context context;
    outline::Illustration illustration;

    Book();

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Book, metadata, contributor, context, illustration)

    void CreateBuild(const string &path);

    friend std::ostream &operator<<(std::ostream &out, Book &book);
};

#endif //TEXTTRANSFORM_BOOK_H
