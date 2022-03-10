//
// Created by hasee on 2021/12/30.
//

#include "argument.h"
#include <iostream>

using namespace std;

int argument::Temp::length() const {
    return (int) titles.size() + 1;
}

void argument::Value::fillIn(const string &value) {
    values.emplace_back(value);
}

bool argument::Value::checkValue() const {
    return temp.titles.size() == values.size();
}

argument::Pairs &argument::Pairs::appendTemplate(const Temp& temp) {
    auto pair = new Value { temp };
    for (const auto& name : temp.names) {
        pairs[name] = pair;
    }
    return *this;
}

int argument::Pairs::getArguments(int argc, const char** argv) {

    string name = "default";

    for (int i = 1; i < argc; ++i) {

        string arg = argv[i];
        if (arg.starts_with('-')) {
            if (!pairs[name]->checkValue()) {
                cout << "command " << arg << " need " << pairs[name]->temp.titles.size() << " args. "
                << "The input num is " << pairs[name]->values.size() << "." << endl;
                exit(1);
            }
            name = arg;
        } else {
            try {
                pairs[name]->fillIn(arg);
            } catch (exception e) {
                cout << arg << " is not an argument" << endl;
                exit(2);
            }
        }
    }

    return 0;
}

argument::Pairs::Pairs() {
    pairs["default"] = new Value;
}
