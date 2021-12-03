//
// Created by hasee on 2021/12/3.
//

#ifndef TEXTTOEPUB_LINE_H
#define TEXTTOEPUB_LINE_H

#include "Annotate.h"

using namespace std;

class Line {
public:
    string _content;
    bool _blank;
    Annotate _annotate;

    Line();

    Line(Annotate annotate);

    [[nodiscard]] bool blank() const;

    [[nodiscard]] bool empty() const;

    bool hasAnnotate();

    bool deal();

    bool deal(string &line);

    friend ostream &operator<<(ostream &out, Line &line);
};


#endif //TEXTTOEPUB_LINE_H
