//
// Created by hasee on 2021/11/30.
//

#include <gtest/gtest.h>

using namespace std;

#define FLAG 1

int main(int argc, char **argv) {

//    启用测试
    if (FLAG == 1) {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }

//    启动程序

    return 0;
}
