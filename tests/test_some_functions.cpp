#include "gtest/gtest.h"
#include <numbers>
//import <numbers>;
class FunctionTest : public ::testing::Test {
protected:
    // You can add setup and teardown code here if needed
};

TEST_F(FunctionTest, test_nums){
    ASSERT_NEAR(std::numbers::pi, 3.1415926, 0.000001);
}