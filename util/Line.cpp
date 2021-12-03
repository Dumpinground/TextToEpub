//
// Created by hasee on 2021/12/3.
//

#include "Line.h"

Line::Line()
        : _blank(true) {}

Line::Line(Annotate annotate)
: _annotate(std::move(annotate)) {}

bool Line::blank() const {
    return _blank;
}

bool Line::empty() const {
    return _content.empty();
}

bool Line::hasAnnotate() {
    return !_annotate.empty();
}

bool Line::deal() {
    if (empty())
        if (blank())
            return false;
        else {
            _content = "<br/>";
            _blank = true;
        }
    else
        _blank = false;
    if (hasAnnotate())
        if (_annotate.deal(_content)) {
            _content = "<p class=\"" + _annotate._format[0] + "\">" + _content + "</p>";
            return true;
        }
    _content = "<p>" + _content + "</p>";
    return true;
}

bool Line::deal(string &line) {
    _content = line;
    return deal();
}

ostream &operator<<(ostream &out, Line &line) {
    out << line._content;
    return out;
}
