//
// Created by hasee on 2021/11/30.
//
#include <iostream>
#include "util/Book.h"
#include <fstream>

using namespace std;

int main(int argc, char **argv) {

    testBook book;
    ifstream i;

    switch (argc) {
        case 1:
            cout << "Here is " << argv[0] << endl;
            break;

        case 3:
            if (string(argv[1]) == "--new_resource") {
                book.BuildInit(argv[2]);
                break;
            }
            break;

        case 5:
            if (string(argv[1]) == "pack") {
                // argv: 2: resourceDir 3: book_name 4: outputDir
                book = testBook::getJson(string(argv[3]) + ".json", string(argv[2]) + "data/").get<testBook>();
                book.CreateBuildDir(argv[4]);
                book.extract(book.TextRoot() + string(argv[3]) + ".txt", book.dir_path() + "EPUB/");
                book.PackBook();
                cout << "pack book succeeded" << endl;
            }
            break;

        case 7:
            if (string(argv[1]) == "add") {
                if (string(argv[4]) == "--image") {
                    book = testBook::getJson(string(argv[3]) + ".json", string(argv[2]) + "data/").get<testBook>();
                    cout << "adding images from " << book.ImagesRoot() << endl;
                    book.addIllustrations(book.ImagesRoot(), argv[5], argv[6]);
                    testBook::saveJson(book, string(argv[3]) + ".json", string(argv[2]) + "data/");
                    cout << "add image succeeded" << endl;
                }
            }
            break;

        default:
            cout << R"(
--help show this

--new_resource [dir]  create a resource folder

add [resource] [name] --image [color] [gray]  add images to color and gray automatically.

pack [resource] [name] [output]  output EPub dir from name.txt and name.json in resource.
)" << endl;

    }

    return 0;
}
