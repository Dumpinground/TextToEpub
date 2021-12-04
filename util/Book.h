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
    };

    void to_json(json &j, const Context &c);

    void from_json(const json &j, Context &c);

    struct Illustration {
        std::vector<string> color, gray;
    };

    void to_json(json &j, const Illustration &i);

    void from_json(const json &j, Illustration &i);

    struct Content {
        string title, subtitle, volume, author,
                cover, illustrator;
        std::vector<string> extraContributor;
        Context context;
        Illustration illustration;
    };

    void to_json(json &j, const Content &c);

    void from_json(const json &j, Content &c);

    struct Contributor {
        string author, illustrator;
        std::vector<string> others;
    };

    void to_json(json &j, const Contributor &c);

    void from_json(const json &j, Contributor &c);

    struct Metadata {
        string title, subtitle, volume, cover, backCover;
        std::vector<string> extra;
    };

    void to_json(json &j, const Metadata &m);

    void from_json(const json &j, Metadata &m);
}

class Book {
public:
    static void Create(const string &dir);

    json contentJson;

    outline::Metadata metadata;
    outline::Contributor contributor;
    outline::Context context;
    outline::Illustration illustration;

    Book();
    explicit Book(json jsonFile);
    explicit Book(const string& jsonFilePath);

    void CreateBuild(const string &path);

    friend std::ostream &operator<<(std::ostream &out, Book &book);
};

void to_json(json &j, const Book &b);

#endif //TEXTTRANSFORM_BOOK_H
