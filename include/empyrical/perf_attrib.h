#pragma once
#ifndef EMPYRICAL_CPP_PERF_ATTRIB
#define EMPYRICAL_CPP_PERF_ATTRIB
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <iterator>
#include <numeric>

// 简化的日期类，用于索引
class Date {
public:
    Date(int year, int month, int day) : year_(year), month_(month), day_(day) {}

    [[nodiscard]] std::string str() const {
        return std::to_string(year_) + "-" + std::to_string(month_) + "-" + std::to_string(day_);
    }

private:
    int year_;
    int month_;
    int day_;
};

// 用于持仓百分比的结构体
struct Position {
    std::string ticker;
    double percentage;
};

// 用于因子载荷的结构体
struct FactorLoading {
    double momentum;
    double reversal;
};

//// 计算风险因子暴露
//std::unordered_map<std::string, FactorLoading> compute_exposures(
//        const std::vector<std::pair<Date, std::vector<Position>>>& positions,
//        const std::unordered_map<std::pair<Date, std::string>, FactorLoading>& factor_loadings
//) {
//    std::unordered_map<std::string, FactorLoading> risk_exposures_portfolio;
//
//    for (const auto& pos_entry : positions) {
//        for (const auto& pos : pos_entry.second) {
//            const auto& loading_key = std::make_pair(pos_entry.first, pos.ticker);
//            if (factor_loadings.find(loading_key) != factor_loadings.end()) {
//                const auto& factor_loading = factor_loadings.at(loading_key);
//                if (risk_exposures_portfolio.find(pos.ticker) == risk_exposures_portfolio.end()) {
//                    risk_exposures_portfolio[pos.ticker] = factor_loading;
//                } else {
//                    risk_exposures_portfolio[pos.ticker].momentum += factor_loading.momentum * pos.percentage;
//                    risk_exposures_portfolio[pos.ticker].reversal += factor_loading.reversal * pos.percentage;
//                }
//            }
//        }
//    }
//
//    return risk_exposures_portfolio;
//}
//
//// 计算绩效归因
//std::pair<std::unordered_map<std::string, FactorLoading>, std::vector<std::vector<double>>> perf_attrib(
//        const std::vector<double>& returns,
//        const std::vector<std::pair<Date, std::vector<Position>>>& positions,
//        const std::vector<std::pair<Date, FactorLoading>>& factor_returns,
//        const std::vector<std::pair<std::pair<Date, std::string>, FactorLoading>>& factor_loadings
//) {
//    std::unordered_map<std::string, FactorLoading> risk_exposures_portfolio = compute_exposures(positions,
//                                                                                                std::unordered_map<std::pair<Date, std::string>, FactorLoading>(
//                                                                                                        factor_loadings.begin(),
//                                                                                                        factor_loadings.end()
//                                                                                                ));
//
//    std::vector<std::vector<double>> perf_attrib_values;
//
//    for (size_t i = 0; i < factor_returns.size(); ++i) {
//        const auto& factor_return = factor_returns[i];
//        const auto& date = factor_return.first;
//        const auto& factor_loading = risk_exposures_portfolio[date.str()];
//
//        std::vector<double> attrib_values;
//        attrib_values.push_back(factor_loading.momentum * factor_return.second.momentum);
//        attrib_values.push_back(factor_loading.reversal * factor_return.second.reversal);
//
//        double common_return = std::inner_product(attrib_values.begin(), attrib_values.end(), returns.begin() + i, 0.0);
//        attrib_values.push_back(common_return);
//        attrib_values.push_back(returns[i] - common_return);
//
//        perf_attrib_values.push_back(attrib_values);
//    }
//
//    return {risk_exposures_portfolio, perf_attrib_values};
//}




#endif // EMPYRICAL_CPP_PERF_ATTRIB