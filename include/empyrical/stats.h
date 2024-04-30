#pragma once
#ifndef EMPYRICAL_CPP_STATS
#define EMPYRICAL_CPP_STATS
#include <stdexcept>
#include <numeric>
#include "constants.h"
#include "utils.h"
#include <algorithm>
#include <numeric>
//#include <nlopt.hpp>

// 定义函数 adjust_returns
std::vector<double> adjust_returns(const std::vector<double>& returns, const std::vector<double>& adjustment_factor) {
    // 判断 adjustment_factor 是否为 0
    bool is_zero_adjustment = true;
    for (const auto& val : adjustment_factor) {
        if (val != 0) {
            is_zero_adjustment = false;
            break;
        }
    }
    // 如果 adjustment_factor 为 0，则直接返回 returns
    if (is_zero_adjustment) {
        return returns;
    }
    // 计算调整后的 returns
    std::vector<double> adjusted_returns;
    adjusted_returns.reserve(returns.size());
    for (size_t i = 0; i < returns.size(); ++i) {
        adjusted_returns.push_back(returns[i] - adjustment_factor[i]);
    }
    return adjusted_returns;
}

std::size_t annualization_factor(const std::string& period, std::size_t annualization = 0) {
    std::size_t factor;
    if (annualization==0) {
        try {
            factor = ANNUALIZATION_FACTORS.at(period);
        } catch (const std::out_of_range& e) {
            throw std::invalid_argument("Period cannot be '" + period + "'.");
        }
    } else {
        factor = annualization;
    }
    return factor;
}

std::vector<double> simple_returns(const std::vector<double>& prices) {
    std::vector<double> returns;

    if (prices.empty() || prices.size() == 1) {
        throw std::invalid_argument("Prices vector must contain at least two elements.");
    }

    returns.reserve(prices.size() - 1); // Allocate space for returns

    for (size_t i = 1; i < prices.size(); ++i) {
        double price_diff = prices[i] - prices[i - 1];
        double simple_return = price_diff / prices[i - 1];
        returns.push_back(simple_return);
    }

    return returns;
}

std::vector<double> cum_returns(const std::vector<double>& returns, double starting_value = 0) {
    std::vector<double> cumulative_returns(returns.size());
    double cumulative_value = starting_value != 0 ? starting_value : 1.0;

    for (size_t i = 0; i < returns.size(); ++i) {
        cumulative_value *= (1.0 + returns[i]);
        cumulative_returns[i] = cumulative_value - (starting_value != 0 ? starting_value : 1.0);
    }

    return cumulative_returns;
}

// 计算累积收益率
double cum_returns_final(const std::vector<double>& returns, double starting_value = 0) {
    if (returns.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double result = std::accumulate(returns.begin(), returns.end(), 1.0, [](double acc, double val) { return acc * (1.0 + val); });

    if (starting_value == 0) {
        result -= 1.0;
    } else {
        result *= starting_value;
    }

    return result;
}

double max_drawdown_from_simple_return(const std::vector<double>& simple_returns) {
    std::vector<double> returns = cum_returns(simple_returns, 1);
    double max_dd = std::numeric_limits<double>::infinity();  // 初始设定为正无穷大
    double peak = returns[0]+1.0;  // 峰值设定为第一个元素
    double trough = returns[0]+1.0;  // 谷值设定为第一个元素

    for (size_t i = 1; i < returns.size(); ++i) {
        if (returns[i]+1.0 > peak) {
            peak = returns[i]+1.0;  // 更新峰值
            trough = peak;  // 重置谷值为峰值
        } else if (returns[i]+1.0 < trough) {
            trough = returns[i]+1.0;  // 更新谷值
            double dd = (trough - peak) / peak;  // 计算回撤比例
            max_dd = std::min(max_dd, dd);  // 更新最大回撤
        }
        std::cout << "ret["<<i<<"]="<<returns[i] <<" peak = " << peak << " trough = " << trough << " max_dd = " << max_dd << std::endl;
    }

    return max_dd;  // 返回负值表示最大回撤
}

double max_drawdown_from_net_values(const std::vector<double>& net_values) {
    double max_dd = std::numeric_limits<double>::infinity();  // 初始设定为正无穷大
    double peak = net_values[0];  // 峰值设定为第一个元素
    double trough = net_values[0];  // 谷值设定为第一个元素

    for (size_t i = 1; i < net_values.size(); ++i) {
        if (net_values[i] > peak) {
            peak = net_values[i];  // 更新峰值
            trough = peak;  // 重置谷值为峰值
        } else if (net_values[i] < trough) {
            trough = net_values[i];  // 更新谷值
            double dd = (trough - peak) / peak;  // 计算回撤比例
            max_dd = std::min(max_dd, dd);  // 更新最大回撤
        }
    }

    return max_dd;  // 返回负值表示最大回撤
}

// 计算复利年化收益率
double annual_return_from_simple_return(const std::vector<double>& returns,
                                        const std::string & period,
                                        std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN()) {
    if (returns.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    std::size_t ann_factor = annualization_factor(period, annualization);
    double num_years = static_cast<double>(returns.size()) / static_cast<double>(ann_factor);
    double ending_value = cum_returns_final(returns, 1.0);  // 传入累积收益率
    std::cout << "ending_value = " << ending_value << std::endl;
    return std::pow(ending_value, 1.0 / num_years) - 1.0;
}


double annual_return_from_net_values(const std::vector<double>& netValues,
                                     std::size_t annualization) {
    std::size_t size = netValues.size();
    double totalReturn = netValues[size - 1] / netValues[0];
    double annualizedReturn = std::pow(totalReturn,  static_cast<double>(annualization) / static_cast<double>(size)) - 1.0;
    return annualizedReturn;
}

// 计算年化波动率
double annual_volatility_from_simple_return(const std::vector<double>& returns,
                         const std::string& period = "daily",
                         int d_dof = 0,
                         std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN()) {

    if (returns.size() < 2) {
        return std::nan("");
    }

    std::size_t ann_factor = annualization_factor(period, annualization);
    double std_dev = cal_func::nan_std(returns, d_dof);

    double annual_vol = std_dev * std::pow(ann_factor, 1.0 / 2.0);

    return annual_vol;
}

// the Calmar ratio
double calmar_ratio_from_simple_return(const std::vector<double>& returns,
                    const std::string& period = "daily",
                    std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN()) {
    double max_dd = max_drawdown_from_simple_return(returns);
    std::cout << "max_dd = " << max_dd << std::endl;
    if (max_dd < 0) {
        double temp = annual_return_from_simple_return(returns, period, annualization) / std::abs(max_dd);
        if (std::isinf(temp) || std::isnan(temp)) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        std::cout << "temp = " << temp << std::endl;
        return temp;
    } else {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

double calmar_ratio_from_net_values(const std::vector<double>& net_values,
                                    std::size_t annualization){
    double max_dd = max_drawdown_from_net_values(net_values);
    std::cout << "max_dd = " << max_dd << std::endl;
    if (max_dd < 0) {
        double temp = annual_return_from_net_values(net_values, annualization) / std::abs(max_dd);
        if (std::isinf(temp) || std::isnan(temp)) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        std::cout << "temp = " << temp << std::endl;
        return temp;
    } else {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

double cal_omega_from_simple_return(const std::vector<double>& returns,
                                    double L = 0) {
    std::vector<double> new_return(returns);
    std::sort(new_return.begin(), new_return.end());
    auto size = static_cast<double>(new_return.size());
    double up = 0.0, down = 0.0;
    for (size_t i = 1; i < new_return.size(); ++i) {
        double x0 = new_return[i - 1], x1 = new_return[i];
        double y0 = static_cast<double>(i - 1) / size;
        double y1 = static_cast<double>(i) / size;
        double width = x1 - x0;
        double height = 0.5*(y0+y1);
        if (x1 <= L) {
            down += width*height;
        } else if (x0 >= L) {
            up += width * (1 - height);
        } else {
            down += (L - x0) * height;
            up +=  (x1 - L)  * (1 - height);
            break;  // No need to continue after crossing L
        }
    }

    if (down == 0) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return up / down;
}

// 计算夏普比率
double calculate_sharpe_ratio(std::vector<double>& returns,
                              double riskFreeRate,
                              std::size_t d_dof=1) {
    std::size_t size = returns.size();
    if (size <= 1) {
        std::cerr << "Error: Returns vector should have at least two elements." << std::endl;
        return 0.0;
    }

    double sumReturn = std::accumulate(returns.begin(), returns.end(), 0.0);
    double meanReturn = sumReturn / static_cast<double>(size);

    double sumSquareReturn = 0.0;
    for (double ret : returns) {
        double diff = ret - meanReturn;
        sumSquareReturn += diff * diff;
    }
    double volatility = std::sqrt(sumSquareReturn / static_cast<double>((size - d_dof)));  // 标准差

    double sharpeRatio = (meanReturn - riskFreeRate/APPROX_BDAYS_PER_YEAR) / volatility;
    return sharpeRatio;
}

// 计算年化夏普率
double calculate_annualized_sharpe_ratio(std::vector<double>& returns, int annualization, double riskFreeRate = 0.0) {
    if (annualization>0){
        double sharpe_ratio = calculate_sharpe_ratio(returns, riskFreeRate);
        double annualized_sharpe_ratio = sharpe_ratio * std::sqrt(annualization);
        return annualized_sharpe_ratio;
    } else{
        return std::numeric_limits<double>::quiet_NaN();
    }

}

double calculate_sortino_ratio(std::vector<double>& returns,
                              double riskFreeRate,
                              std::size_t d_dof=1) {
    std::size_t size = returns.size();
    if (size <= 1) {
        std::cerr << "Error: Returns vector should have at least two elements." << std::endl;
        return 0.0;
    }

    double sumReturn = std::accumulate(returns.begin(), returns.end(), 0.0);
    double meanReturn = sumReturn / static_cast<double>(size);

    double sumSquareReturn = 0.0;
    for (double ret : returns) {
        double diff = ret - meanReturn;
        if (diff < 0){
            sumSquareReturn += diff * diff;
        }

    }
    double volatility = std::sqrt(sumSquareReturn / static_cast<double>((size - d_dof)));  // 标准差
    if (volatility > 0){
        double sharpeRatio = (meanReturn - riskFreeRate/APPROX_BDAYS_PER_YEAR) / volatility;
        return sharpeRatio;
    } else{
        return std::numeric_limits<double>::quiet_NaN();
    }

}

double calculate_annualized_sortino_ratio(std::vector<double>& returns, int annualization, double riskFreeRate = 0.0) {
    if (annualization>0){
        double sharpe_ratio = calculate_sortino_ratio(returns, riskFreeRate);
        double annualized_sortino_ratio = sharpe_ratio * std::sqrt(annualization);
        return annualized_sortino_ratio;
    } else{
        return std::numeric_limits<double>::quiet_NaN();
    }

}

/*
 * The excess Sharpe is a simplified Information Ratio that uses
    tracking error rather than "active risk" as the denominator.
 */
double calculate_excess_sharpe_ratio(std::vector<double>& returns,
                                     std::vector<double>& factor_returns,
                                     std::size_t d_dof = 1
                                     ){
    std::size_t length = returns.size();
    std::vector<double> active_returns(length, 0);
    for (std::size_t i=0;i<length;i++){
        active_returns[i] = returns[i] - factor_returns[i];
    }
    double average_return = cal_func::nan_mean(active_returns);
    double tracking_error = cal_func::nan_std(active_returns,d_dof);
    if (tracking_error > 0){
        return average_return/tracking_error;
    }else{
        return std::numeric_limits<double>::quiet_NaN();
    }
}

double stability_of_timeseries(const std::vector<double>& returns) {
    if (returns.size() < 2) {
        return std::nan("");
    }

    std::vector<double> cum_log_returns(returns.size());
    double cum_sum = 0.0;
    for (size_t i = 0; i < returns.size(); ++i) {
        cum_sum += std::log1p(returns[i]);
        cum_log_returns[i] = cum_sum;
    }

    auto n = static_cast<double>(returns.size());
    double sum_x = n * (n - 1) * 0.5; // Sum of 1, 2, 3, ..., n-1
    double sum_x_sq = n * (n - 1) * (2 * n - 1) / 6.0; // Sum of squares of 1, 2, 3, ..., n-1
    double sum_y = std::accumulate(cum_log_returns.begin(), cum_log_returns.end(), 0.0);
    double sum_xy = 0.0;
    for (size_t i = 0; i < returns.size(); ++i) {
        sum_xy += (static_cast<double>(i) * cum_log_returns[i]);
    }

    double slope = (n * sum_xy - sum_x * sum_y) / (n * sum_x_sq - sum_x * sum_x);
    double intercept = (sum_y - slope * sum_x) / n;

    double ssr = 0.0; // Regression sum of squares
    double sst = 0.0; // Total sum of squares
    for (size_t i = 0; i < returns.size(); ++i) {
        double y_hat = slope * static_cast<double>(i) + intercept;
        ssr += (y_hat - cum_log_returns[i]) * (y_hat - cum_log_returns[i]);
        sst += (cum_log_returns[i] - (sum_y / n)) * (cum_log_returns[i] - (sum_y / n));
    }

    double r_squared = 1.0 - (ssr / sst);
    return r_squared;
}


double percentile(const std::vector<double>& arr,
                  double q,
                  Interpolation interpolation=Interpolation::Linear) {
    assert(!arr.empty());
    assert(q >= 0.0 && q <= 1.0);

    std::vector<double> sorted_arr = arr;
    std::sort(sorted_arr.begin(), sorted_arr.end());

    auto n = static_cast<double>(sorted_arr.size());
    double pos = (n - 1.0) * q ;

    int k = static_cast<int>(std::floor(pos));
    double d = pos - k;
    std::cout << "q = " << q << " k = "<< k << " d = "<< d << std::endl;
    switch (interpolation) {
        case Interpolation::Linear:
            if (d==0.0) {
                return sorted_arr[k];
            } else {
                return sorted_arr[k] + d * (sorted_arr[k + 1] - sorted_arr[k]);
            }
        case Interpolation::Lower:
            return sorted_arr[k];
        case Interpolation::Higher:
            return sorted_arr[k + 1];
        case Interpolation::Midpoint:
            return (sorted_arr[k] + sorted_arr[k + 1]) / 2.0;
        case Interpolation::Nearest:
            if (d < 0.5) {
                return sorted_arr[k];
            } else {
                return sorted_arr[k + 1];
            }
        default:
            return std::numeric_limits<double>::quiet_NaN();
    }
}

double tail_ratio(const std::vector<double>& returns) {
    if (returns.empty()) {
        return std::nan("");
    }


    double right_tail = percentile(returns, 0.95);
    double left_tail = percentile(returns, 0.05);

    if (left_tail == 0.0) {
        return std::numeric_limits<double>::infinity();
    }

    return std::abs(right_tail) / std::abs(left_tail);
}

double capture(const std::vector<double>& returns,
               const std::vector<double>& factor_returns,
               const std::string& period = "daily") {
    // 计算捕获比率
    double strategy_annual_return = annual_return_from_simple_return(returns, period);
    double factor_annual_return = annual_return_from_simple_return(factor_returns, period);

    if (factor_annual_return == 0.0) {
        return std::numeric_limits<double>::quiet_NaN();  // Handle division by zero
    }

    return strategy_annual_return / factor_annual_return;
}

double value_at_risk(const std::vector<double>& returns, double cutoff = 0.05){
    return percentile(returns, cutoff);
}

double conditional_value_at_risk(const std::vector<double>& returns, double cutoff=0.05){
    double threshold = percentile(returns, cutoff);
    double sum = 0.0;
    std::size_t count = 0;
    for (double ret :returns){
        if (ret < threshold){
            sum+=ret;
            count+=1;
        }
    }
    return sum/static_cast<double>(count);
}

std::pair<std::vector<double>,
        std::vector<double>> only_up(std::vector<double> & returns,
                                     std::vector<double> & factor_returns){
        std::size_t size = returns.size();
        std::vector<double> new_returns;
        std::vector<double> new_factor_returns;
        for (std::size_t i=0; i<size;i++){
            if (factor_returns[i] > 0){
                new_returns.push_back(returns[i]);
                factor_returns.push_back(factor_returns[i]);
            }
        }
        return std::make_pair(new_returns, new_factor_returns);
}

std::pair<std::vector<double>,
        std::vector<double>> only_down(std::vector<double> & returns,
                                     std::vector<double> & factor_returns){
    std::size_t size = returns.size();
    std::vector<double> new_returns;
    std::vector<double> new_factor_returns;
    for (std::size_t i=0; i<size;i++){
        if (factor_returns[i] < 0){
            new_returns.push_back(returns[i]);
            factor_returns.push_back(factor_returns[i]);
        }
    }
    return std::make_pair(new_returns, new_factor_returns);
}


double up_capture(std::vector<double>& returns,
                  std::vector<double>&factor_returns,
                  const std::string& period="daily",
                  std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN() ){
    auto result = only_up(returns, factor_returns);
    std::vector<double> new_returns = result.first;
    std::vector<double> new_factor_returns = result.second;
    double a = annual_return_from_simple_return(new_returns,period,annualization);
    double b = annual_return_from_simple_return(new_factor_returns,period,annualization);
    return a/b;
}

double down_capture(std::vector<double>& returns,
                  std::vector<double>&factor_returns,
                  const std::string& period="daily",
                  std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN() ){
    auto result = only_down(returns, factor_returns);
    std::vector<double> new_returns = result.first;
    std::vector<double> new_factor_returns = result.second;
    double a = annual_return_from_simple_return(new_returns,period,annualization);
    double b = annual_return_from_simple_return(new_factor_returns,period,annualization);
    return a/b;
}

double up_down_capture(std::vector<double>& returns,
                        std::vector<double>&factor_returns,
                        const std::string& period="daily",
                        std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN() ){
    return up_capture(returns,factor_returns,period,annualization)/down_capture(returns,factor_returns,period,annualization);
}

std::vector<double> roll_up_capture(std::vector<double>& returns,
                       std::vector<double>&factor_returns,
                       const std::string& period="daily",
                       int window=10,
                       std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN()){

    std::vector<double> data;
    std::size_t size = returns.size();
    data.reserve(size - window + 1);  // 提前预留空间

    // 初始化窗口数据
    std::vector<double> new_returns(returns.begin(), returns.begin() + window);
    std::vector<double> new_factor_returns(returns.begin(), returns.begin() + window);
    // 计算初始窗口的值
    double v = up_capture(new_returns, new_factor_returns, period, annualization);
    data.push_back(v);

    // 滑动窗口计算后续值
    for (std::size_t i = window; i < size; ++i) {
        // 滑动窗口移除第一个元素，添加新元素
        new_returns.erase(new_returns.begin());
        new_returns.push_back(new_returns[i]);
        new_factor_returns.erase(new_factor_returns.begin());
        new_factor_returns.push_back(new_factor_returns[i]);
        v = up_capture(new_returns, new_factor_returns, period, annualization);
        data.push_back(v);
    }

    return data;
}

std::vector<double> roll_down_capture(std::vector<double>& returns,
                                    std::vector<double>&factor_returns,
                                    const std::string& period="daily",
                                    int window=10,
                                    std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN()){

    std::vector<double> data;
    std::size_t size = returns.size();
    data.reserve(size - window + 1);  // 提前预留空间

    // 初始化窗口数据
    std::vector<double> new_returns(returns.begin(), returns.begin() + window);
    std::vector<double> new_factor_returns(returns.begin(), returns.begin() + window);
    // 计算初始窗口的值
    double v = down_capture(new_returns, new_factor_returns, period, annualization);
    data.push_back(v);

    // 滑动窗口计算后续值
    for (std::size_t i = window; i < size; ++i) {
        // 滑动窗口移除第一个元素，添加新元素
        new_returns.erase(new_returns.begin());
        new_returns.push_back(new_returns[i]);
        new_factor_returns.erase(new_factor_returns.begin());
        new_factor_returns.push_back(new_factor_returns[i]);
        v = up_capture(new_returns, new_factor_returns, period, annualization);
        data.push_back(v);
    }

    return data;
}

std::vector<double> roll_up_down_capture(std::vector<double>& returns,
                                      std::vector<double>&factor_returns,
                                      const std::string& period="daily",
                                      int window=10,
                                      std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN()){

    std::vector<double> data;
    std::size_t size = returns.size();
    data.reserve(size - window + 1);  // 提前预留空间

    // 初始化窗口数据
    std::vector<double> new_returns(returns.begin(), returns.begin() + window);
    std::vector<double> new_factor_returns(returns.begin(), returns.begin() + window);
    // 计算初始窗口的值
    double v = up_down_capture(new_returns, new_factor_returns, period, annualization);
    data.push_back(v);

    // 滑动窗口计算后续值
    for (std::size_t i = window; i < size; ++i) {
        // 滑动窗口移除第一个元素，添加新元素
        new_returns.erase(new_returns.begin());
        new_returns.push_back(new_returns[i]);
        new_factor_returns.erase(new_factor_returns.begin());
        new_factor_returns.push_back(new_factor_returns[i]);
        v = up_capture(new_returns, new_factor_returns, period, annualization);
        data.push_back(v);
    }

    return data;
}



double beta_aligned(const std::vector<double>& returns,
                    const std::vector<double>& factor_returns) {
    if (returns.empty() || factor_returns.empty() || returns.size() != factor_returns.size()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    int N = returns.size();  // 数据点数量

    // 计算回归变量的均值
    double x_mean = 0.0;
    for (int i = 0; i < N; ++i) {
        x_mean += factor_returns[i];  // 假设只有一个回归变量
    }
    x_mean /= N;

    // 计算因变量的均值
    double y_mean = 0.0;
    for (int i = 0; i < N; ++i) {
        y_mean += returns[i];  // 假设只有一个因变量
    }
    y_mean /= N;

    // 计算回归系数 b 的分子和分母
    double numerator = 0.0;
    double denominator = 0.0;
    for (int i = 0; i < N; ++i) {
        double x_diff = factor_returns[i] - x_mean;  // 假设只有一个回归变量
        double y_diff = returns[i] - y_mean;  // 假设只有一个因变量
        numerator += x_diff * y_diff;
        denominator += x_diff * x_diff;
    }

    // 避免除以零的情况
    if (denominator == 0.0) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // 计算回归系数 b
    double beta = numerator / denominator;
    return beta;
}

double calculate_beta(const std::vector<double>& returns,
                    const std::vector<double>& factor_returns){
    return beta_aligned(returns, factor_returns);
}

// 计算 alpha 值的函数
double alpha_aligned(const std::vector<double>& returns,
                     const std::vector<double>& factor_returns,
                     double risk_free = 0.0,
                     const std::string& period = "daily",
                     int annualization = 252,
                     double beta = 0.0) {
    auto N = returns.size();  // 数据点数量
    if (N == 0 || factor_returns.size() != N) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // 如果 beta 值未提供，则计算 beta
    if (beta == 0.0) {
        // 这里调用你的 beta 计算函数
        beta = calculate_beta(returns, factor_returns);
    }

    auto ann_factor = annualization_factor(period, annualization);

    double alpha_sum = 0.0;
    double alpha = 0.0;
    for (int i = 0; i < N; ++i) {
        alpha = returns[i] - (beta * factor_returns[i]);
        alpha_sum += alpha;
    }
    // todo 修改了算法，可能比原始的算法更准确
    // double alpha_mean = alpha_sum / static_cast<double>(N);
    // double alpha_annualized = pow(1 + alpha_mean, ann_factor) - 1;
    double alpha_annualized = pow(1 + alpha_sum, ann_factor/N) - 1;

    return alpha_annualized;
}

// 计算 alpha 值的函数
double calculate_alpha(const std::vector<double>& returns,
             const std::vector<double>& factor_returns,
             double risk_free = 0.0,
             const std::string& period = "daily",
             int annualization = 252,
             double _beta = 0.0) {

    // 调用 alpha_aligned 函数计算 alpha
    return alpha_aligned(returns, factor_returns,
                         risk_free, period, annualization, _beta);
}

std::pair<double, double> alpha_beta_aligned(const std::vector<double>& returns,
                   const std::vector<double>& factor_returns,
                   double risk_free = 0.0,
                   const std::string& period = "daily",
                   int annualization = 252) {
    double beta = calculate_beta(returns, factor_returns);
    double alpha = calculate_alpha(returns, factor_returns, risk_free,
                                   period, annualization, beta);
    return std::make_pair(alpha, beta);
}

std::pair<double, double> calculate_alpha_beta(const std::vector<double>& returns,
                                             const std::vector<double>& factor_returns,
                                             double risk_free = 0.0,
                                             const std::string& period = "daily",
                                             int annualization = 252) {
    return alpha_beta_aligned(returns, factor_returns, risk_free, period, annualization);
}

std::vector<std::pair<double, double>> roll_alpha_beta(std::vector<double>& returns,
                                         std::vector<double>&factor_returns,
                                         int window=10,double risk_free = 0.0,
                                         const std::string& period="daily",
                                         int annualization = 252){

    std::vector<std::pair<double, double>> data;
    std::size_t size = returns.size();
    data.reserve(size - window + 1);  // 提前预留空间

    // 初始化窗口数据
    std::vector<double> new_returns(returns.begin(), returns.begin() + window);
    std::vector<double> new_factor_returns(returns.begin(), returns.begin() + window);
    // 计算初始窗口的值
    std::pair<double, double> v = calculate_alpha_beta(new_returns, new_factor_returns, risk_free, period, annualization);
    data.push_back(v);

    // 滑动窗口计算后续值
    for (std::size_t i = window; i < size; ++i) {
        // 滑动窗口移除第一个元素，添加新元素
        new_returns.erase(new_returns.begin());
        new_returns.push_back(new_returns[i]);
        new_factor_returns.erase(new_factor_returns.begin());
        new_factor_returns.push_back(new_factor_returns[i]);
        v = calculate_alpha_beta(new_returns, new_factor_returns, risk_free, period, annualization);
        data.push_back(v);
    }

    return data;
}

std::pair<double,double> up_alpha_beta(std::vector<double>& returns,
                     std::vector<double>&factor_returns,
                     double risk_free = 0.0,
                     const std::string& period = "daily",
                     int annualization = 252 ){
    auto result = only_up(returns, factor_returns);
    std::vector<double> new_returns = result.first;
    std::vector<double> new_factor_returns = result.second;
    return calculate_alpha_beta(returns,factor_returns,risk_free,period,annualization);
}

std::pair<double,double> down_alpha_beta(std::vector<double>& returns,
                                       std::vector<double>&factor_returns,
                                       double risk_free = 0.0,
                                       const std::string& period = "daily",
                                       int annualization = 252 ){
    auto result = only_down(returns, factor_returns);
    std::vector<double> new_returns = result.first;
    std::vector<double> new_factor_returns = result.second;
    return calculate_alpha_beta(returns,factor_returns,risk_free,period,annualization);
}

double beta_fragility_heuristic_aligned(const std::vector<double>& returns,
                                        const std::vector<double>& factor_returns) {
    if (returns.size() < 3 || factor_returns.size() < 3) {
        return std::nan("");
    }

    // Combine returns and factor returns into pairs
    std::vector<std::pair<double, double>> pairs;
    for (size_t i = 0; i < returns.size(); ++i) {
        if (!std::isnan(returns[i]) && !std::isnan(factor_returns[i])) {
            pairs.emplace_back(returns[i], factor_returns[i]);
        }
    }

    if (pairs.size() < 3) {
        return std::nan("");
    }

    // Sort by beta
    std::sort(pairs.begin(), pairs.end(), [](const auto& a, const auto& b) {
        return a.second < b.second;
    });

    // Find the three vectors, using median of 3
    size_t start_index = 0;
    size_t mid_index = pairs.size() / 2;
    size_t end_index = pairs.size() - 1;

    double start_returns = pairs[start_index].first;
    double mid_returns = pairs[mid_index].first;
    double end_returns = pairs[end_index].first;

    double start_factor_returns = pairs[start_index].second;
    double mid_factor_returns = pairs[mid_index].second;
    double end_factor_returns = pairs[end_index].second;

    double factor_returns_range = end_factor_returns - start_factor_returns;
    double start_returns_weight = 0.5;
    double end_returns_weight = 0.5;

    // Find weights for the start and end returns
    // using a convex combination
    if (factor_returns_range != 0) {
        start_returns_weight = (mid_factor_returns - start_factor_returns) / factor_returns_range;
        end_returns_weight = (end_factor_returns - mid_factor_returns) / factor_returns_range;
    }

    // Calculate fragility heuristic
    double heuristic = (start_returns_weight * start_returns) +
                       (end_returns_weight * end_returns) -
                       mid_returns;

    return heuristic;
}

double calculate_beta_fragility_heuristic(const std::vector<double>& returns,
                                          const std::vector<double>& factor_returns){
    return beta_fragility_heuristic_aligned(returns, factor_returns);
}



//double gpd_es_calculator(double var_estimate, double threshold, double scale_param, double shape_param) {
//    double result = 0.0;
//
//    if ((1 - shape_param) != 0) {
//        double var_ratio = (var_estimate / (1 - shape_param));
//        double param_ratio = ((scale_param - (shape_param * threshold)) / (1 - shape_param));
//        result = var_ratio + param_ratio;
//    }
//
//    return result;
//}
//
//double gpd_var_calculator(double threshold, double scale_param, double shape_param,
//                          double probability, double total_n, double exceedance_n) {
//    double result = 0.0;
//
//    if (exceedance_n > 0 && shape_param > 0) {
//        double param_ratio = scale_param / shape_param;
//        double prob_ratio = (total_n / exceedance_n) * probability;
//        result = threshold + (param_ratio * (std::pow(prob_ratio, -shape_param) - 1));
//    }
//
//    return result;
//}
//
//double gpd_loglikelihood_scale_only(double scale, const std::vector<double>& price_data) {
//    std::size_t n = price_data.size();
//    double data_sum = 0.0;
//    for (double price : price_data) {
//        data_sum += price;
//    }
//
//    double result = -std::numeric_limits<double>::max();
//    if (scale >= 0) {
//        result = (-static_cast<double> (n) * std::log(scale)) - (data_sum / scale);
//    }
//
//    return result;
//}
//
//// 基于尺度和形状参数的 GPD 对数似然函数计算
//double gpd_loglikelihood_scale_and_shape(double scale, double shape, const std::vector<double>& price_data) {
//    int n = price_data.size();
//    double result = -std::numeric_limits<double>::max();
//
//    if (scale != 0) {
//        double param_factor = shape / scale;
//        if (shape != 0 && param_factor >= 0 && scale >= 0) {
//            double sum_log = 0.0;
//            for (double price : price_data) {
//                sum_log += std::log((param_factor * price) + 1);
//            }
//            result = (-n * std::log(scale)) - (((1.0 / shape) + 1.0) * sum_log);
//        }
//    }
//
//    return result;
//}
//
//// GPD 对数似然函数计算，根据参数类型调用不同的函数
//double gpd_loglikelihood(const std::vector<double>& params, const std::vector<double>& price_data) {
//    double result = 0.0;
//
//    if (params.size() != 2) {
//        std::cerr << "Error: Parameters should have size 2." << std::endl;
//        return result;
//    }
//
//    if (params[1] != 0) {
//        // 调用基于尺度和形状参数的 GPD 对数似然函数计算
//        result = -gpd_loglikelihood_scale_and_shape(params[0], params[1], price_data);
//    } else {
//        // 调用仅基于尺度参数的 GPD 对数似然函数计算
//        result = -gpd_loglikelihood_scale_only(params[0], price_data);
//    }
//
//    return result;
//}
//
//std::function<double(double)> gpd_loglikelihood_scale_only_factory(const std::vector<double>& price_data) {
//    return [&price_data](double scale) {
//        return -gpd_loglikelihood_scale_only(scale,price_data);
//    };
//}
//
//std::function<double(std::vector<double>)> gpd_loglikelihood_scale_and_shape_factory(const std::vector<double>& price_data) {
//    return [&price_data](std::vector<double> params) {
//        if (params.size() == 2) {
//            return -gpd_loglikelihood_scale_and_shape(params[0], params[1], price_data);
//        } else {
//            // Handle invalid input
//            return 0.0;  // Placeholder return value
//        }
//    };
//}
//
//std::function<double(std::vector<double>)> gpd_loglikelihood_factory(const std::vector<double>& price_data) {
//    return [&price_data](const std::vector<double>& params) {
//        return gpd_loglikelihood(params, price_data);
//    };
//}
//
//std::vector<double> gpd_loglikelihood_minimizer_aligned(const std::vector<double> &price_data) {
//    std::vector<double> result = {false, false};
//    const double DEFAULT_SCALE_PARAM = 1.0;
//    const double DEFAULT_SHAPE_PARAM = 1.0;
//
//    if (!price_data.empty()) {
//        nlopt::opt opt(nlopt::LN_NELDERMEAD, 2); // Nelder-Mead optimization with 2 parameters
//
//        // Set objective function and data
//        opt.set_min_objective(gpd_loglikelihood, &price_data);
//
//        // Set initial guess
//        std::vector<double> x(2);
//        x[0] = DEFAULT_SCALE_PARAM;
//        x[1] = DEFAULT_SHAPE_PARAM;
//        opt.set_initial_step(0.1);
//
//        // Set optimization options
//        opt.set_xtol_rel(1e-6);
//
//        // Perform optimization
//        double minf; // Minimum function value
//        nlopt::result result_status = opt.optimize(x, minf);
//
//        if (result_status == nlopt::SUCCESS) {
//            result[0] = x[0];
//            result[1] = x[1];
//        }
//    }
//    return result;
//}
//
//std::vector<double> gpd_risk_estimates_aligned(std::vector<double>& returns, double var_p = 0.01) {
//    std::vector<double> result(5, 0.0);
//    if (returns.size() >= 3) {
//        const double DEFAULT_THRESHOLD = 0.2;
//        const double MINIMUM_THRESHOLD = 0.000000001;
//
//        std::vector<double> flipped_returns(returns.size());
//        std::transform(returns.begin(), returns.end(), flipped_returns.begin(), [](double val) { return -val; });
//
//        std::vector<double> losses;
//        for (double val : flipped_returns) {
//            if (val > 0) {
//                losses.push_back(val);
//            }
//        }
//
//        double threshold = DEFAULT_THRESHOLD;
//        bool finished = false;
//        double scale_param = 0.0;
//        double shape_param = 0.0;
//
//        while (!finished && threshold > MINIMUM_THRESHOLD) {
//            std::vector<double> losses_beyond_threshold;
//            for (double loss : losses) {
//                if (loss >= threshold) {
//                    losses_beyond_threshold.push_back(loss);
//                }
//            }
//
//            std::vector<double> param_result = gpd_loglikelihood_minimizer_aligned(losses_beyond_threshold);
//            if (!param_result.empty()) {
//                scale_param = param_result[0];  // Placeholder assignment
//                shape_param = param_result[1];  // Placeholder assignment
//                double var_estimate = gpd_var_calculator(threshold, scale_param, shape_param, var_p,
//                                                         losses.size(), losses_beyond_threshold.size());
//
//                if (shape_param > 0 && var_estimate > 0) {
//                    finished = true;
//                }
//            }
//
//            if (!finished) {
//                threshold /= 2;
//            }
//        }
//
//        if (finished) {
//            double es_estimate = gpd_es_calculator(result[3], result[0], scale_param, shape_param);
//            result = {threshold, scale_param, shape_param, result[3], es_estimate};
//        }
//    }
//
//    return result;
//}
//
//std::vector<double> gpd_risk_estimates(std::vector<double>& returns, double var_p = 0.01) {
//    if (returns.size() < 3) {
//        std::vector<double> result(5, 0.0);
//        return result;
//    }
//
//    return gpd_risk_estimates_aligned(returns, var_p);
//}


#endif // EMPYRICAL_CPP_STATS