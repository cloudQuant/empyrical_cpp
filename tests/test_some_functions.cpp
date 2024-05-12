#include "gtest/gtest.h"
#include <numbers>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
//import <numbers>;
class FunctionTest : public ::testing::Test {
protected:
    // You can add setup and teardown code here if needed
};

TEST_F(FunctionTest, test_nums){
    ASSERT_NEAR(std::numbers::pi, 3.1415926, 0.000001);
}

TEST_F(FunctionTest, test_2){
    std::tm t = {};
    std::istringstream ss("2011-Februar-18 23:12:34");
    ss >> std::get_time(&t, "%Y-%B-%d %H:%M:%S");

    if (ss.fail()) {
        std::cout << "Parse failed\n";
    } else {
        std::cout << std::put_time(&t, "%c") << '\n';
    }

}