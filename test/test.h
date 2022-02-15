//
// Created by hasee on 2021/12/13.
//

#ifndef TEXTTOEPUB_TEST_H
#define TEXTTOEPUB_TEST_H

#include "../src/util/Book.h"

#include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <codecvt>

using namespace std;
using json = nlohmann::json;

#define OutPutRoot string("../test/result/")
#define OriginRoot string("../test/origin/")
#define AccessibleEpub3Root string("F:/Epub/learn/accessible_epub_3/")
#define MissingRoot string("F:/Epub/Missing/")

void saveJson(const json &j, const string &name, const string& root = OutPutRoot);

json getJson(const string &name, const string& root = OriginRoot);

#endif //TEXTTOEPUB_TEST_H
