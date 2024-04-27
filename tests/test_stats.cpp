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

TEST_F(StatsTest, DefaultValues) {
    // 测试默认情况下的行为
    EXPECT_EQ(annualization_factor("monthly"), 12);
    EXPECT_EQ(annualization_factor("weekly"), 52);
    EXPECT_EQ(annualization_factor("daily"), 252);
}

TEST_F(StatsTest, CustomValue) {
    // 测试自定义年化频率的情况
    EXPECT_EQ(annualization_factor("monthly", 6), 6);
    EXPECT_EQ(annualization_factor("weekly", 26), 26);
    EXPECT_EQ(annualization_factor("daily", 300), 300);
}

TEST_F(StatsTest, InvalidPeriod) {
    // 测试传入无效周期的情况
    EXPECT_THROW(annualization_factor("hourly"), std::invalid_argument);
    EXPECT_THROW(annualization_factor("minutely"), std::invalid_argument);
}

// 测试 simple_returns 函数
TEST_F(StatsTest, PositiveValues) {
    std::vector<double> prices = {100.0, 105.0, 110.0, 115.0};
    std::vector<double> expected_returns = {0.05, 0.047619, 0.045455}; // 预期的简单收益率

    // 调用 simple_returns 函数计算收益率
    std::vector<double> returns = simple_returns(prices);

    // 检查返回的收益率是否与预期值匹配
    ASSERT_EQ(returns.size(), expected_returns.size());
    for (size_t i = 0; i < returns.size(); ++i) {
        EXPECT_NEAR(returns[i], expected_returns[i], 1e-6); // 限定小数位数为6位;
    }
}

TEST_F(StatsTest, EmptyVector) {
    std::vector<double> prices = {}; // 空价格向量
    // 空价格向量应该导致抛出异常
    EXPECT_THROW(simple_returns(prices), std::invalid_argument);
}

TEST_F(StatsTest, SingleValueVector) {
    std::vector<double> prices = {100.0}; // 单个价格值的向量
    // 单个价格值的向量应该导致抛出异常
    EXPECT_THROW(simple_returns(prices), std::invalid_argument);
}

// 测试 cum_returns 函数
TEST_F(StatsTest, cum_returns_PositiveValues) {
    std::vector<double> returns = {0.01, 0.02, -0.005, 0.03};
    std::vector<double> expected_cum_returns = {0.01, 0.0302, 0.025049, 0.05580047}; // 预期的累积收益率

    // 调用 cum_returns 函数计算累积收益率
    std::vector<double> cumulative_returns = cum_returns(returns);

    // 检查返回的累积收益率是否与预期值匹配
    ASSERT_EQ(cumulative_returns.size(), expected_cum_returns.size());
    for (size_t i = 0; i < cumulative_returns.size(); ++i) {
        EXPECT_NEAR(cumulative_returns[i], expected_cum_returns[i], 1e-6); // 限定小数位数为6位
    }
}

// 测试累积收益率函数 cum_returns_final
TEST_F(StatsTest, EmptyReturns) {
    std::vector<double> empty_returns;
    double expected_result = std::numeric_limits<double>::quiet_NaN();
    double actual_result = cum_returns_final(empty_returns);
    //ASSERT_DOUBLE_EQ(actual_result, expected_result);
    EXPECT_TRUE(std::isnan(expected_result));
    EXPECT_TRUE(std::isnan(actual_result));
}

TEST_F(StatsTest, NonEmptyReturnsNoStartingValue) {
    std::vector<double> returns = {0.01, 0.02, -0.005, 0.03};
    double expected_result = 1.01 * 1.02 * 0.995 * 1.03 - 1.0;
    double actual_result = cum_returns_final(returns);
    ASSERT_DOUBLE_EQ(actual_result, expected_result);
}

TEST_F(StatsTest, NonEmptyReturnsWithStartingValue) {
    std::vector<double> returns = {0.01, 0.02, -0.005, 0.03};
    double starting_value = 1000.0;
    double expected_result = 1000.0 * (1.01 * 1.02 * 0.995 * 1.03);
    double actual_result = cum_returns_final(returns, starting_value);
    ASSERT_DOUBLE_EQ(actual_result, expected_result);
}

// 测试最大回撤函数
TEST_F(StatsTest, MaxDrawdownTest_SimpleReturns) {
    // 准备测试数据
    std::vector<double> simple_returns = {0.1, -0.05, -0.02, 0.07, -0.03};

    // 调用被测试的函数
    double max_dd = max_drawdown_from_simple_return(simple_returns);

    // 检查返回的最大回撤值是否符合预期
    EXPECT_NEAR(max_dd, -0.069, 1e-6);  // 预期最大回撤为 -0.10
}

TEST_F(StatsTest,MaxDrawdownTest_NetValues) {
    // 准备测试数据
    std::vector<double> net_values = {90, 100, 60, 120, 80};

    // 调用被测试的函数
    double max_dd = max_drawdown_from_net_values(net_values);

    // 检查返回的最大回撤值是否符合预期
    EXPECT_DOUBLE_EQ(max_dd, -0.4);  // 预期最大回撤为 -0.2
}

// 测试 annual_return_from_simple_return 函数
TEST_F(StatsTest,AnnualReturnFromSimpleReturnTest_PositiveTest) {
    std::vector<double> returns = {0.05, 0.02, -0.03, 0.01};  // 测试数据
    std::string period = "monthly";  // 假设期间是按月计算的
    std::size_t annualization = 12;  // 年化因子为12

    // 调用函数计算年化复利收益率
    double result = annual_return_from_simple_return(returns, period, annualization);

    // 验证返回值是否符合预期
    EXPECT_NEAR(result, 0.155174888, 0.00001);  // 使用近似值来检查
}

// 测试 annual_return_from_net_values 函数
TEST_F(StatsTest,AnnualReturnFromNetValuesTest_PositiveTest) {
    std::vector<double> netValues = {1000.0, 1050.0, 1100.0};  // 测试数据
    std::size_t annualization = 3;  // 年化因子为3

    // 调用函数计算年化收益率
    double result = annual_return_from_net_values(netValues, annualization);

    // 验证返回值是否符合预期
    EXPECT_NEAR(result, 0.1, 0.0001);  // 使用近似值来检查
}

// 测试计算年化波动率函数
TEST_F(StatsTest, annual_volatility_BasicTest) {
    // 准备测试数据
    std::vector<double> returns1 = {0.05, -0.02, 0.1, -0.03, 0.08};
    std::vector<double> returns2 = {0.02, 0.04, 0.03, -0.05, 0.06, 0.02};

    // 调用函数计算年化波动率
    double annual_vol1 = annual_volatility_from_simple_return(returns1, "daily");
    double annual_vol2 = annual_volatility_from_simple_return(returns2, "weekly", 1);

    // 验证计算结果
    EXPECT_NEAR(annual_vol1, 0.83155757, 1e-6);
    EXPECT_NEAR(annual_vol2, 0.2698147512, 1e-6);
}

TEST_F(StatsTest, CalmarRatioTest_MaxDrawdownNegative) {
    std::vector<double> returns = {-0.05, 0.1, -0.02, 0.03, -0.08};  // 负回报率示例
    double calmar = calmar_ratio_from_simple_return(returns, "daily");
    EXPECT_FALSE(std::isnan(calmar));
}

TEST_F(StatsTest, CalmarRatioTest_MaxDrawdownPositive) {
    std::vector<double> returns = {0.02, 0.03, 0.01, 0.05, 0.02};  // 正回报率示例
    double calmar = calmar_ratio_from_simple_return(returns, "daily", 252);
    EXPECT_TRUE(std::isnan(calmar));
}

TEST_F(StatsTest, CalmarRatioTest_MaxDrawdownZero) {
    std::vector<double> returns = {0.01, 0.02, 0.0, 0.03, 0.02};  // 零回报率示例
    double calmar = calmar_ratio_from_simple_return(returns, "daily");
    EXPECT_TRUE(std::isnan(calmar));
}

TEST_F(StatsTest, CalmarRatioNetValuesTest_MaxDrawdownNegative) {
    std::vector<double> net_values = {1000.0, 1100.0, 950.0, 1050.0, 900.0};  // 示例净值序列
    double calmar = calmar_ratio_from_net_values(net_values, 252);  // 年化频率为252
    EXPECT_FALSE(std::isnan(calmar));
}

TEST_F(StatsTest, CalmarRatioNetValuesTest_MaxDrawdownPositive) {
    std::vector<double> net_values = {1000.0, 1050.0, 980.0, 1030.0, 1100.0};  // 示例净值序列
    double calmar = calmar_ratio_from_net_values(net_values, 252);  // 年化频率为252
    EXPECT_FALSE(std::isnan(calmar));
}

TEST_F(StatsTest, CalmarRatioNetValuesTest_MaxDrawdownZero) {
    std::vector<double> net_values = {1000.0, 1020.0, 1030.0, 1040.0, 1050.0};  // 示例净值序列
    double calmar = calmar_ratio_from_net_values(net_values, 252);  // 年化频率为252
    EXPECT_TRUE(std::isnan(calmar));
}

// 测试计算 omega 的函数
TEST_F(StatsTest, OmegaCalculatorTest_CalculatesOmegaCorrectly) {
    std::vector<double> returns = {0.1, 0.2, 0.3, -0.1, -0.2, -0.3};
    double omega = cal_omega_from_simple_return(returns);
    EXPECT_DOUBLE_EQ(omega, 0.7777777777777779);  // 期望的结果
}

// 测试当 down 为零时的返回值
TEST_F(StatsTest, OmegaCalculatorTest_ReturnsNaNWhenDownIsZero) {
    std::vector<double> returns = {0.1, 0.2, 0.3, -0.1, -0.2, -0.3};
    double omega = cal_omega_from_simple_return(returns, -1.0);  // 设置一个使 down 为零的 L 值
    EXPECT_TRUE(std::isnan(omega));  // 期望返回 NaN
}

TEST_F(StatsTest, SharpeRatioTest_ValidInput) {
    std::vector<double> returns = {0.1, 0.05, -0.02, 0.03, 0.08};  // 示例收益率序列
    double risk_free_rate = 0.02;  // 无风险利率
    double sharpe_ratio = calculate_sharpe_ratio(returns, risk_free_rate);
    EXPECT_NEAR(sharpe_ratio, 1.028709, 1e-5);  // 预期值
}

TEST_F(StatsTest, SharpeRatioTest_EmptyInput) {
    std::vector<double> returns;  // 空收益率序列
    double risk_free_rate = 0.02;  // 无风险利率
    double sharpe_ratio = calculate_sharpe_ratio(returns, risk_free_rate);
    EXPECT_EQ(sharpe_ratio, 0.0);  // 期望为0.0
}

TEST_F(StatsTest, AnnualizedSharpeRatioTest_ValidInput) {
    std::vector<double> returns = {0.1, 0.05, -0.02, 0.03, 0.08};  // 示例收益率序列
    int annualization = 252;  // 年化频率
    double risk_free_rate = 0.02;  // 无风险利率
    double annualized_sharpe_ratio = calculate_annualized_sharpe_ratio(returns, annualization, risk_free_rate);
    EXPECT_NEAR(annualized_sharpe_ratio, 16.330255, 1e-3);  // 预期值
}

TEST_F(StatsTest, AnnualizedSharpeRatioTest_ZeroAnnualization) {
    std::vector<double> returns = {0.1, 0.05, -0.02, 0.03, 0.08};  // 示例收益率序列
    int annualization = 0;  // 年化频率为0，无法计算
    double risk_free_rate = 0.02;  // 无风险利率
    double annualized_sharpe_ratio = calculate_annualized_sharpe_ratio(returns, annualization, risk_free_rate);
    EXPECT_EQ(std::isnan(annualized_sharpe_ratio), true);  // 期望为NaN
}
