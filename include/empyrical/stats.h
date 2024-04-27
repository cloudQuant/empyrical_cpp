#pragma once
#ifndef EMPYRICAL_CPP_STATS
#define EMPYRICAL_CPP_STATS
#include <stdexcept>
#include <numeric>
#include "constants.h"
#include "utils.h"
#include <algorithm>
#include <numeric>

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



#endif // EMPYRICAL_CPP_STATS
