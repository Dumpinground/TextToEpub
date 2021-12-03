//
// Created by hasee on 2021/11/30.
//

#include "Annotate.h"

Annotate::Annotate() = default;

Annotate::Annotate(string sign, vector<string> format, int number)
        : _sign(std::move(sign)),
          _format(std::move(format)),
          _number(number) {}

bool Annotate::empty() const {
    return _sign.empty();
}

void Annotate::attach(string &text) {
    text = "<span class=\"" + _format[1] + "\">" + _sign + to_string(_number++) + "</span>";
}

string Annotate::attachPop() {
    return "<span class=\"" + _format[1] + "\">" + _aq.front() + "</span>";
}

void Annotate::change(string &content, int p) {
    string text = content.substr(p, size());
    if (_aq.front() != text) {
        _aq.push(toString());
        attach(text);
    } else {
        text = attachPop();
    }
    content.erase(p, size());
    content.insert(p, text);
}

string Annotate::toString() {
    return _sign + to_string(_number);
}

int Annotate::size() {
    return toString().size();
}

bool Annotate::deal(string &content) {
    int p;
//        注释段
    if (!_aq.empty()) {
        p = content.find(_aq.front());
        if (p != -1) {
            change(content, p);
            _aq.pop();
            return true;
        }
    }
//        注释字
    p = 0;
    while (p < content.size() - 1) {
        p = content.find(toString(), p);
        if (p != -1) {
            string text = content.substr(p, size());
            change(content, p);
        } else {
            break;
        }
    }

    return false;
}