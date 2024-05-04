#pragma once
#ifndef EMPYRICAL_CPP_STATS
#define EMPYRICAL_CPP_STATS
#include <stdexcept>
#include <numeric>
#include "constants.h"
#include "utils.h"
#include "optimizer.h"
#include <algorithm>
#include <numeric>


/*
 * 1. implement downside_risk, not same as original ways to calculate annualised downside_risk
 */

// 定义函数 adjust_returns
inline std::vector<double> adjust_returns(const std::vector<double>& returns, const std::vector<double>& adjustment_factor) {
    // 计算调整后的 returns
    std::vector<double> adjusted_returns;
    adjusted_returns.reserve(returns.size());
    for (size_t i = 0; i < returns.size(); ++i) {
        adjusted_returns.push_back(returns[i] - adjustment_factor[i]);
    }
    return adjusted_returns;
}

inline std::vector<double> adjust_returns(const std::vector<double>& returns, double adjustment_factor = 0.0) {
    // 如果 adjustment_factor 为 0，则直接返回 returns
    if (adjustment_factor==0.0) {
        return returns;
    }
    // 计算调整后的 returns
    std::vector<double> adjusted_returns;
    adjusted_returns.reserve(returns.size());
    for (double i : returns) {
        adjusted_returns.push_back(i - adjustment_factor);
    }
    return adjusted_returns;
}

inline std::size_t annualization_factor(const std::string& period, std::size_t annualization = 0) {
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

inline std::vector<double> simple_returns(const std::vector<double>& prices) {
    std::vector<double> returns;

    if (prices.empty() || prices.size() == 1) {
        throw std::invalid_argument("Prices vector must contain at least two elements.");
    }

    returns.reserve(prices.size() - 1); // Allocate space for returns

    for (size_t i = 1; i < prices.size(); ++i) {
        double price_diff = prices[i] - prices[i - 1];
        if (prices[i - 1] == 0){
            returns.push_back(NAN);
        }else{
            double simple_return = price_diff / prices[i - 1];
            returns.push_back(simple_return);
        }
    }

    return returns;
}

inline std::vector<double> cum_returns(const std::vector<double>& returns, double starting_value = 0) {
    std::vector<double> cumulative_returns(returns.size());
    double cumulative_value = 1.0;

    for (size_t i = 0; i < returns.size(); ++i) {
        double ret = returns[i];
        if (std::isnan(ret)){
            ret = 0.0;
        }
        //std::cout << "ret " << i << " = " << ret << std::endl;
        cumulative_value *= (1.0 + ret);
        if (starting_value==0){
            cumulative_returns[i] = cumulative_value - 1.0;
        }else{
            cumulative_returns[i] = cumulative_value * starting_value;
        }
    }

    return cumulative_returns;
}

// 计算累积收益率
inline double cum_returns_final(const std::vector<double>& returns, double starting_value = 0) {
    if (returns.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double cumulative_value = 1.0;

    for (double ret : returns) {
        if (std::isnan(ret)){
            ret = 0.0;
        }
        //std::cout << "ret " << i << " = " << ret << std::endl;
        cumulative_value *= (1.0 + ret);
    }

    if (starting_value == 0) {
        cumulative_value -= 1.0;
    } else {
        cumulative_value *= starting_value;
    }

    return cumulative_value;
}

inline double max_drawdown_from_simple_return(const std::vector<double>& simple_returns) {
    if (simple_returns.empty()){
        return NAN;
    }
    if (simple_returns.size()==1){
        return 0;
    }
    std::vector<double> returns = cum_returns(simple_returns, 1);
    double max_dd = std::numeric_limits<double>::infinity();  // 初始设定为正无穷大
    double peak = 1.0;  // 峰值设定为第一个元素

    for (double i : returns) {
        if (i >= peak) {
            peak = i;  // 更新峰值
        }
        double dd = (i - peak) / peak;  // 计算回撤比例
        max_dd = std::min(max_dd, dd);  // 更新最大回撤

//        std::cout << "ret["<<i<<"]="<<returns[i] <<" peak = " << peak <<  " max_dd = " << max_dd << std::endl;
    }

    return max_dd;  // 返回负值表示最大回撤
}

inline double max_drawdown_from_net_values(const std::vector<double>& net_values) {
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
inline double annual_return_from_simple_return(const std::vector<double>& returns,
                                        const std::string & period="daily",
                                        std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN()) {
    if (returns.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    std::size_t ann_factor = annualization_factor(period, annualization);
    double num_years = static_cast<double>(returns.size()) / static_cast<double>(ann_factor);
    double ending_value = cum_returns_final(returns, 1.0);  // 传入累积收益率
    //return std::pow(ending_value, static_cast<double>(returns.size()) / num_years) - 1.0;
    return std::pow(ending_value, 1 / num_years) - 1.0;
}


inline double annual_return_from_net_values(const std::vector<double>& netValues,
                                     std::size_t annualization) {
    std::size_t size = netValues.size();
    double totalReturn = netValues[size - 1] / netValues[0];
    double annualizedReturn = std::pow(totalReturn,  static_cast<double>(annualization) / static_cast<double>(size)) - 1.0;
    return annualizedReturn;
}

// 计算年化波动率
inline double annual_volatility_from_simple_return(const std::vector<double>& returns,
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
inline double calmar_ratio_from_simple_return(const std::vector<double>& returns,
                    const std::string& period = "daily",
                    std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN()) {
    double max_dd = max_drawdown_from_simple_return(returns);
    std::cout << "max_dd = " << max_dd << std::endl;
    if (max_dd < 0) {
        double rate = annual_return_from_simple_return(returns, period, annualization);
        double temp = rate / std::abs(max_dd);
        if (std::isinf(temp) || std::isnan(temp)) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        std::cout << "rate = " << rate << " max_dd = " << max_dd <<" temp = " << temp << std::endl;
        return temp;
    } else {
        return std::numeric_limits<double>::quiet_NaN();
    }
}

inline double calmar_ratio_from_net_values(const std::vector<double>& net_values,
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

inline double cal_omega_from_simple_return(const std::vector<double>& returns,
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
inline double calculate_sharpe_ratio(std::vector<double>& returns,
                              double riskFreeRate,
                              std::size_t d_dof=1) {
    std::size_t size = returns.size();
    if (size <= 1) {
        std::cerr << "Error: Returns vector should have at least two elements." << std::endl;
        return 0.0;
    }
    if (riskFreeRate!=0){
        std::vector<double> new_returns(returns);
        for (double & v: new_returns){
            v -= riskFreeRate;
        }
        double meanReturn = cal_func::nan_mean(new_returns);
        double volatility = cal_func::nan_std(new_returns,d_dof);

        if (volatility == 0){
            return std::numeric_limits<double>::infinity();
        }
        double sharpeRatio = meanReturn / volatility;
        std::cout << "meanReturn = " << meanReturn << " volatility = " << volatility << " sharpeRatio = " << sharpeRatio << std::endl;
        return sharpeRatio;
    } else{
        double meanReturn = cal_func::nan_mean(returns);
        double volatility = cal_func::nan_std(returns,d_dof);

        if (volatility == 0){
            return std::numeric_limits<double>::infinity();
        }
        double sharpeRatio = meanReturn / volatility;
        std::cout << "meanReturn = " << meanReturn << " volatility = " << volatility << " sharpeRatio = " << sharpeRatio << std::endl;
        return sharpeRatio;
    }

}

// 计算年化夏普率
inline double calculate_annualized_sharpe_ratio(std::vector<double>& returns,
                                                int annualization, double riskFreeRate = 0.0) {
    if (annualization>0 && returns.size()>1){
        double sharpe_ratio = calculate_sharpe_ratio(returns, riskFreeRate);
        double annualized_sharpe_ratio = sharpe_ratio * std::sqrt(annualization);
        return annualized_sharpe_ratio;
    } else{
        return std::numeric_limits<double>::quiet_NaN();
    }

}

inline double calculate_downside_risk(std::vector<double>& returns){
    double sum = 0.0;
    double nan_count = 0.0;
    for (double v: returns){
        if (v<0){
            sum+=v*v;
        }
        if (std::isnan(v)){
            nan_count += 1.0;
        }
    }
    sum/=(static_cast<double>(returns.size())-nan_count);
    return std::sqrt(sum);
}

inline double calculate_downside_risk(std::vector<double>& returns,
                            double required_return){
    std::vector<double> new_return = adjust_returns(returns, required_return);
    double sum = 0.0;
    double nan_count = 0.0;
    for (double v: new_return){
        if (v<0){
            sum+=v*v;
        }
        if (std::isnan(v)){
            nan_count += 1.0;
        }
    }
    sum/=(static_cast<double>(returns.size())-nan_count);
    return std::sqrt(sum);
}

inline double calculate_downside_risk(const std::vector<double>& returns,
                                      const std::vector<double>& required_return){
    std::vector<double> new_return = adjust_returns(returns, required_return);
    double sum = 0.0;
    double nan_count = 0.0;
    std::cout << "adjust_returns = [";
    for (double v: new_return){
        std::cout << v << std::endl;
        if (v<0){
            sum+=v*v;
        }
        if (std::isnan(v)){
            nan_count += 1.0;
        }
    }
    std::cout << "]" << std::endl;
    sum/=(static_cast<double>(returns.size())-nan_count);
    return std::sqrt(sum);
}

inline double calculate_upside_risk(std::vector<double>& returns){
    double sum = 0.0;
    double nan_count = 0.0;
    for (double v: returns){
        if (v>0){
            sum+=v*v;
        }
        if (std::isnan(v)){
            nan_count += 1.0;
        }
    }
    sum/=(static_cast<double>(returns.size())-nan_count);
    return std::sqrt(sum);
}

inline double calculate_upside_risk(std::vector<double>& returns,
                                      double required_return){
    std::vector<double> new_return = adjust_returns(returns, required_return);
    double sum = 0.0;
    double nan_count = 0.0;
    for (double v: new_return){
        if (v>0){
            sum+=v*v;
        }
        if (std::isnan(v)){
            nan_count += 1.0;
        }
    }
    sum/=(static_cast<double>(returns.size())-nan_count);
    return std::sqrt(sum);
}

inline double calculate_sortino_ratio(std::vector<double>& returns,
                              double riskFreeRate,
                              std::size_t d_dof=1) {
    std::size_t size = returns.size();
    if (size <= 1) {
        std::cerr << "Error: Returns vector should have at least two elements." << std::endl;
        return NAN;
    }
    std::vector<double> new_returns = adjust_returns(returns, riskFreeRate);
    double meanReturn = cal_func::nan_mean(new_returns);
    double now_downside_risk = calculate_downside_risk(new_returns);

    if (now_downside_risk > 0){
        double sortino_ratio = meanReturn  / now_downside_risk;
        return sortino_ratio;
    } else{
        return std::numeric_limits<double>::quiet_NaN();
    }

}

inline double calculate_sortino_ratio(std::vector<double>& returns,
                                      std::vector<double>& riskFreeRate,
                                      std::size_t d_dof=1) {
    std::size_t size = returns.size();
    if (size <= 1) {
        std::cerr << "Error: Returns vector should have at least two elements." << std::endl;
        return NAN;
    }
    std::vector<double> new_returns = adjust_returns(returns, riskFreeRate);
    double meanReturn = cal_func::nan_mean(new_returns);
    double now_downside_risk = calculate_downside_risk(new_returns);

    if (now_downside_risk > 0){
        double sortino_ratio = meanReturn / now_downside_risk;
        return sortino_ratio;
    } else{
        return std::numeric_limits<double>::quiet_NaN();
    }

}

inline double calculate_annualized_sortino_ratio(std::vector<double>& returns, int annualization, double riskFreeRate = 0.0) {
    if (annualization>0){
        double sortino_ratio = calculate_sortino_ratio(returns, riskFreeRate);
        double annualized_sortino_ratio = sortino_ratio * std::sqrt(annualization);
        return annualized_sortino_ratio;
    } else{
        return std::numeric_limits<double>::quiet_NaN();
    }

}

inline double calculate_annualized_sortino_ratio(std::vector<double>& returns,
                                                 int annualization,
                                                 std::vector<double>& riskFreeRate) {
    if (annualization>0){
        double sortino_ratio = calculate_sortino_ratio(returns, riskFreeRate);
        double annualized_sortino_ratio = sortino_ratio * std::sqrt(annualization);
        return annualized_sortino_ratio;
    } else{
        return std::numeric_limits<double>::quiet_NaN();
    }

}

/*
 * The excess Sharpe is a simplified Information Ratio that uses
    tracking error rather than "active risk" as the denominator.
 */
inline double calculate_excess_sharpe_ratio(std::vector<double>& returns,
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

inline double calculate_excess_sharpe_ratio(std::vector<double>& returns,
                                     double factor_returns,
                                     std::size_t d_dof = 1
                                     ){
    std::size_t length = returns.size();
    std::vector<double> active_returns(length, 0);
    for (std::size_t i=0;i<length;i++){
        active_returns[i] = returns[i] - factor_returns;
    }
    double average_return = cal_func::nan_mean(active_returns);
    double tracking_error = cal_func::nan_std(active_returns,d_dof);
    if (tracking_error > 0){
        return average_return/tracking_error;
    }else{
        return std::numeric_limits<double>::quiet_NaN();
    }
}

inline double stability_of_timeseries(const std::vector<double>& returns) {
    if (returns.size() < 2) {
        return NAN;
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


inline double percentile(const std::vector<double>& arr,
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
    //std::cout <<"n = " << n << " q = " << q << " k = "<< k << " d = "<< d << std::endl;
    switch (interpolation) {
        case Interpolation::Linear:
            if (d==0.0) {
                return sorted_arr[k];
            } else {
                //std::cout << "sorted_arr[k] = "<<sorted_arr[k]<<std::endl;
                //std::cout << "sorted_arr[k + 1] =" << sorted_arr[k + 1] << std::endl;
                //std::cout << "d = " << d << std::endl;
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

inline double tail_ratio(const std::vector<double>& returns) {
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

inline double capture(const std::vector<double>& returns,
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

inline double value_at_risk(const std::vector<double>& returns, double cutoff = 0.05){
    return percentile(returns, cutoff);
}

inline double conditional_value_at_risk(const std::vector<double>& returns, double cutoff=0.05){
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

inline std::pair<std::vector<double>,
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

inline std::pair<std::vector<double>,
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


inline double up_capture(std::vector<double>& returns,
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

inline double down_capture(std::vector<double>& returns,
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

inline double up_down_capture(std::vector<double>& returns,
                        std::vector<double>&factor_returns,
                        const std::string& period="daily",
                        std::size_t annualization = std::numeric_limits<std::size_t>::quiet_NaN() ){
    return up_capture(returns,factor_returns,period,annualization)/down_capture(returns,factor_returns,period,annualization);
}

inline std::vector<double> roll_up_capture(std::vector<double>& returns,
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

inline std::vector<double> roll_down_capture(std::vector<double>& returns,
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

inline std::vector<double> roll_up_down_capture(std::vector<double>& returns,
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



inline double beta_aligned(const std::vector<double>& returns,
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

inline double calculate_beta(const std::vector<double>& returns,
                    const std::vector<double>& factor_returns){
    return beta_aligned(returns, factor_returns);
}

// 计算 alpha 值的函数
inline double alpha_aligned(const std::vector<double>& returns,
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
    for (std::size_t i = 0; i < N; ++i) {
        alpha = returns[i] - (beta * factor_returns[i]);
        alpha_sum += alpha;
    }

    double alpha_mean = alpha_sum / static_cast<double>(N);
    double alpha_annualized = pow(1 + alpha_mean, static_cast<double>(ann_factor)) - 1;
    // todo 修改算法，可能比原始的算法更准确
    //double alpha_annualized = pow(1 + alpha_sum, ann_factor/N) - 1;

    return alpha_annualized;
}

// 计算 alpha 值的函数
inline double calculate_alpha(const std::vector<double>& returns,
             const std::vector<double>& factor_returns,
             double risk_free = 0.0,
             const std::string& period = "daily",
             int annualization = 252,
             double _beta = 0.0) {

    // 调用 alpha_aligned 函数计算 alpha
    return alpha_aligned(returns, factor_returns,
                         risk_free, period, annualization, _beta);
}

inline std::pair<double, double> alpha_beta_aligned(const std::vector<double>& returns,
                   const std::vector<double>& factor_returns,
                   double risk_free = 0.0,
                   const std::string& period = "daily",
                   int annualization = 252) {
    double beta = calculate_beta(returns, factor_returns);
    double alpha = calculate_alpha(returns, factor_returns, risk_free,
                                   period, annualization, beta);
    return std::make_pair(alpha, beta);
}

inline std::pair<double, double> calculate_alpha_beta(const std::vector<double>& returns,
                                             const std::vector<double>& factor_returns,
                                             double risk_free = 0.0,
                                             const std::string& period = "daily",
                                             int annualization = 252) {
    return alpha_beta_aligned(returns, factor_returns, risk_free, period, annualization);
}

inline std::vector<std::pair<double, double>> roll_alpha_beta(std::vector<double>& returns,
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

inline std::pair<double,double> up_alpha_beta(std::vector<double>& returns,
                     std::vector<double>&factor_returns,
                     double risk_free = 0.0,
                     const std::string& period = "daily",
                     int annualization = 252 ){
    auto result = only_up(returns, factor_returns);
    std::vector<double> new_returns = result.first;
    std::vector<double> new_factor_returns = result.second;
    return calculate_alpha_beta(returns,factor_returns,risk_free,period,annualization);
}

inline std::pair<double,double> down_alpha_beta(std::vector<double>& returns,
                                       std::vector<double>&factor_returns,
                                       double risk_free = 0.0,
                                       const std::string& period = "daily",
                                       int annualization = 252 ){
    auto result = only_down(returns, factor_returns);
    std::vector<double> new_returns = result.first;
    std::vector<double> new_factor_returns = result.second;
    return calculate_alpha_beta(returns,factor_returns,risk_free,period,annualization);
}

inline double beta_fragility_heuristic_aligned(const std::vector<double>& returns,
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

inline double calculate_beta_fragility_heuristic(const std::vector<double>& returns,
                                          const std::vector<double>& factor_returns){
    return beta_fragility_heuristic_aligned(returns, factor_returns);
}



inline double gpd_es_calculator(double var_estimate, double threshold, double scale_param, double shape_param) {
    double result = 0.0;

    if ((1 - shape_param) != 0) {
        double var_ratio = (var_estimate / (1 - shape_param));
        double param_ratio = ((scale_param - (shape_param * threshold)) / (1 - shape_param));
        result = var_ratio + param_ratio;
    }

    return result;
}

inline double gpd_var_calculator(double threshold, double scale_param, double shape_param,
                          double probability, double total_n, double exceedance_n) {
    double result = 0.0;

    if (exceedance_n > 0 && shape_param > 0) {
        double param_ratio = scale_param / shape_param;
        double prob_ratio = (total_n / exceedance_n) * probability;
        result = threshold + (param_ratio * (std::pow(prob_ratio, -shape_param) - 1));
    }

    return result;
}

inline double gpd_loglikelihood_scale_only(double scale, const std::vector<double>& price_data) {
    std::size_t n = price_data.size();
    double data_sum = 0.0;
    for (double price : price_data) {
        data_sum += price;
    }

    double result = -std::numeric_limits<double>::max();
    if (scale >= 0) {
        result = (-static_cast<double> (n) * std::log(scale)) - (data_sum / scale);
    }

    return result;
}

// 基于尺度和形状参数的 GPD 对数似然函数计算
inline double gpd_loglikelihood_scale_and_shape(double scale, double shape, const std::vector<double>& price_data) {
    int n = price_data.size();
    double result = -std::numeric_limits<double>::max();

    if (scale != 0) {
        double param_factor = shape / scale;
        if (shape != 0 && param_factor >= 0 && scale >= 0) {
            double sum_log = 0.0;
            for (double price : price_data) {
                sum_log += std::log((param_factor * price) + 1);
            }
            result = (-n * std::log(scale)) - (((1.0 / shape) + 1.0) * sum_log);
        }
    }

    return result;
}

// GPD 对数似然函数计算，根据参数类型调用不同的函数
inline double gpd_loglikelihood(const std::vector<double>& params, const std::vector<double>& price_data) {
    double result = 0.0;

    if (params.size() != 2) {
        std::cerr << "Error: Parameters should have size 2." << std::endl;
        return result;
    }

    if (params[1] != 0) {
        // 调用基于尺度和形状参数的 GPD 对数似然函数计算
        result = -gpd_loglikelihood_scale_and_shape(params[0], params[1], price_data);
    } else {
        // 调用仅基于尺度参数的 GPD 对数似然函数计算
        result = -gpd_loglikelihood_scale_only(params[0], price_data);
    }

    return result;
}



inline double function_to_minimize(const std::array<double,2>& x) {
    return x[0]*x[0] + x[1]*x[1];
}
// Gradient descent optimization
inline std::vector<double> gpd_loglikelihood_minimizer_aligned(const std::vector<double> &price_data) {

    std::vector<double> initial_params = {1.0, 1.0};
    std::array<double,2> start = { 1.0, 1.0 };
    std::array<double,2> step = { 0.00001, 0.00001 };
    std::function<double(const std::array<double, 2> &)> optimized_function = [&price_data](const std::array<double, 2> &arr) {
        return -1*gpd_loglikelihood_scale_and_shape(arr[0], arr[1], price_data);
    };
    nelder_mead_result<double,2> result = nelder_mead<double,2>(
        optimized_function,
        start,
        1.0e-25, // the terminating limit for the variance of function values
        step
    );

    initial_params[0] = result.xmin[0];
    initial_params[1] = result.xmin[1];
    cal_func::print_vector(initial_params, "initial_params");
    return initial_params;
}

inline std::vector<double> gpd_risk_estimates_aligned(std::vector<double>& returns, double var_p = 0.01) {
    std::vector<double> result(5, 0.0);
    if (returns.size() >= 3) {
        const double DEFAULT_THRESHOLD = 0.2;
        const double MINIMUM_THRESHOLD = 0.000000001;

        std::vector<double> flipped_returns(returns.size());
        std::transform(returns.begin(), returns.end(), flipped_returns.begin(), [](double val) { return -val; });
        std::vector<double> losses;
        for (double val : flipped_returns) {
            if (val > 0) {
                losses.push_back(val);
            }
        }

        double threshold = DEFAULT_THRESHOLD;
        bool finished = false;
        double var_estimate =0.0;
        double scale_param = 0.0;
        double shape_param = 0.0;

        while (!finished && threshold > MINIMUM_THRESHOLD) {
            std::vector<double> losses_beyond_threshold;
            for (double loss : losses) {
                if (loss >= threshold) {
                    losses_beyond_threshold.push_back(loss);
                }
            }
            //cal_func::print_vector(losses_beyond_threshold, "losses_beyond_threshold");
            std::vector<double> param_result = gpd_loglikelihood_minimizer_aligned(losses_beyond_threshold);
            //cal_func::print_vector(param_result, "param_result");
            if (!param_result.empty()) {
                scale_param = param_result[0];  // Placeholder assignment
                shape_param = param_result[1];  // Placeholder assignment
//                std::cout << "threshold = " << threshold << std::endl;
//                std::cout << "scale_param = " << scale_param << std::endl;
//                std::cout << "shape_param = " << shape_param << std::endl;
//                std::cout << "var_p = " << var_p << std::endl;
//                std::cout << "losses.size() = " << losses.size() << std::endl;
//                std::cout << "losses_beyond_threshold.size() = " << losses_beyond_threshold.size() << std::endl;
                var_estimate = gpd_var_calculator(threshold,
                                                         scale_param,
                                                         shape_param,
                                                         var_p,
                                                         losses.size(),
                                                         losses_beyond_threshold.size());

                std::cout << "var_estimate = " << var_estimate << std::endl;
                if (shape_param > 0 && var_estimate > 0) {
                    finished = true;
                }
            }

            if (!finished) {
                threshold /= 2;
            }else{
                break;
            }
        }

        if (finished) {
            double es_estimate = gpd_es_calculator(var_estimate, result[0], scale_param, shape_param);
            result = {threshold, scale_param, shape_param, var_estimate, es_estimate};
        }
    }

    return result;
}

inline std::vector<double> gpd_risk_estimates(std::vector<double>& returns, double var_p = 0.01) {
    if (returns.size() < 3) {
        std::vector<double> result(5, 0.0);
        return result;
    }

    return gpd_risk_estimates_aligned(returns, var_p);
}


#endif // EMPYRICAL_CPP_STATS