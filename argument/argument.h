//
// Created by hasee on 2021/12/30.
//

#ifndef TEXTTOEPUB_ARGUMENT_H
#define TEXTTOEPUB_ARGUMENT_H

#include <string>
#include <vector>
#include <map>

namespace argument {

    using string = std::string;

    struct Temp {
        std::vector<string> names;
        std::vector<string> titles;

        [[nodiscard]] int length() const;
    };

    struct Value {
        Temp temp;
        std::vector<string> values;

        void fillIn(const string &value);
        [[nodiscard]] bool checkValue() const;
    };

    struct Pairs {
        std::map<string, Value *> pairs;

        Pairs();

        Pairs &appendTemplate(const Temp& temp);

        int getArguments(int argc, const char** argv);
    };
}

#endif //TEXTTOEPUB_ARGUMENT_H
