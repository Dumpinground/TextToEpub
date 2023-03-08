//
// Created by hasee on 2023/3/8.
//

#include "test.h"

TEST(testUtil, testJoin) {
    std::vector<string> ss;

    for (int i = 5; i > 0; --i) {
        ss.emplace_back(to_string(i));
    }

    EXPECT_EQ(join(ss, "+"), "5+4+3+2+1");
    EXPECT_EQ(join(ss, "-"), "5-4-3-2-1");
}
