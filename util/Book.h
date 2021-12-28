//
// Created by hasee on 2021/11/30.
//

#ifndef TEXTTOEPUB_BOOK_H
#define TEXTTOEPUB_BOOK_H

#include "util.h"
#include "Context.h"
#include "outline.h"

json newJson();

class Book {

private:
    string book_dir;
    string dir_name = "book_dir";

public:
    static void Create(const string &dir);
    string dir_path();

    outline::Metadata metadata;
    outline::Contributor contributor;
    outline::Contents contents;
    outline::Illustrations illustrations;
    string ResourceRoot = "../resources/";

    Book();

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Book, metadata, contributor, contents, illustrations, ResourceRoot)

    string lang = "zh-CN";
    string eBookName() const;
    string fullTitle() const;
    string ImagesRoot();
    string TextRoot();
    string DataRoot();

    void BuildInit(const string &path);
    void CreateResourceDir(const string &path);
    void CreateBuildDir(const string &path);
    void PackBook();

    string wrap(string wrapped, bool blank = false) const;

    void extract(const string &inputTextPath, const string &outPutDir);
    void buildPackage(const string &outPutDir);
    void buildToc(const string &outPutDir);
    void annotate();

    friend std::ostream &operator<<(std::ostream &out, Book &book);

private:
    string whitespace;
    std::vector<context::Chapter *> chapters;
    std::queue<context::Annotation> notes;
};

std::wstring WS(const string &s);

void mkDir(const string &path, const std::function<void(const string &path)> &visit = [] (const string &path) {});

bool find(const string& target, const string& text);

#endif //TEXTTOEPUB_BOOK_H
