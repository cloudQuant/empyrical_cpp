#include "gtest/gtest.h"
#include "../include/empyrical/stats.h"
class StatsTest : public ::testing::Test {
protected:
    // You can add setup and teardown code here if needed
};

// 测试 _adjust_returns 函数
TEST_F(StatsTest, AdjustReturns) {
// 准备测试数据
std::vector<double> returns = {1.0, 2.0, 3.0, 4.0, 5.0};
std::vector<double> zero_adjustment = {0.0, 0.0, 0.0, 0.0, 0.0};
std::vector<double> adjustment_factor = {0.0, 1.0, 0.0, 2.0, 0.0};

// 调用 _adjust_returns 函数
auto adjusted_returns_zero = adjust_returns(returns, zero_adjustment);
auto adjusted_returns_nonzero = adjust_returns(returns, adjustment_factor);

// 检查返回结果是否正确
EXPECT_EQ(returns, adjusted_returns_zero);  // adjustment_factor 为 0 时，returns 不变
EXPECT_NE(returns, adjusted_returns_nonzero);  // adjustment_factor 不为 0 时，returns 有变化
EXPECT_DOUBLE_EQ(adjusted_returns_nonzero[1], 1.0);  // 检查调整后的第二个元素的值是否正确
}