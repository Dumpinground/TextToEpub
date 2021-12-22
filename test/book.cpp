//
// Created by hasee on 2021/12/13.
//

#include "test.h"

TEST(testBook, testNewBook) {
    Book book;
    saveJson(book, "new book.json");
}

TEST(testBook, testExtractChapter) {
    Book book = getJson("missing 2.json").get<Book>();
    book.extract(book.TextRoot() + "missing 2.txt", OutPutRoot + "text/");
    book.buildPackage(OutPutRoot + "text/");
}

TEST(testBook, testUuid) {
    cout << uuid4() << endl;
}

TEST(testBook, testBuildInit) {
    Book book;
    book.BuildInit(OutPutRoot);
}

TEST(testBook, testBuildPack) {
    Book book = getJson("missing 2.json", "../test/result/resources/data/").get<Book>();
    book.CreateBuildDir(OutPutRoot);
    book.extract(book.TextRoot() + "missing 2.txt", book.dir_path() + "EPUB/");
    book.PackBook();
}