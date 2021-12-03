//
// Created by hasee on 2021/11/30.
//

#ifndef TEXTTRANSFORM_ANNOTATE_H
#define TEXTTRANSFORM_ANNOTATE_H

#include <string>
#include <vector>
#include <queue>

using namespace std;

class Annotate {
public:
    string _sign;
    vector<string> _format;
    int _number = -1;
    queue<string> _aq;

    Annotate();

    Annotate(string sign, vector<string> format, int number = -1);

    [[nodiscard]] bool empty() const;

    void attach(string &text);

    string attachPop();

    void change(string &content, int p);

    string toString();

    int size();

    bool deal(string &content);
};


#endif //TEXTTRANSFORM_ANNOTATE_H
