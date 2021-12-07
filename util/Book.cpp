//
// Created by hasee on 2021/11/30.
//

#include "Book.h"

#include <iomanip>
#include <pugixml.hpp>
#include <boost/filesystem.hpp>
#include <codecvt>

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
