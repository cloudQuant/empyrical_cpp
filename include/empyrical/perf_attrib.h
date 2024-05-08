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
#include "datetime.h"


// 计算风险因子暴露
MyDataFrame compute_exposures(
        const MyPanel& positions,
        const MyPanel& factor_loadings) {
    MyDataFrame risk_exposures_portfolio;
    std::vector<std::vector<double>> target_result;
    std::vector<DateTime> index = positions.index;
    std::vector<MyDataFrame> pos_values = positions.values;
    std::vector<MyDataFrame> factor_values = factor_loadings.values;
    risk_exposures_portfolio.index = index;
    risk_exposures_portfolio.cols = factor_values[0].cols;
    // 计算暴漏值
    for (std::size_t i=0; i<index.size(); ++i){
        MyDataFrame pos_value = pos_values[i];
        MyDataFrame factor_value = factor_values[i];
        std::vector<std::string> pos_index = pos_value.string_index;
        std::vector<std::string> pos_cols = pos_value.cols;
        std::vector<std::vector<double>> pos = pos_value.values;
        std::vector<std::string> factor_index = factor_value.string_index;
        std::vector<std::string> factor_cols = factor_value.cols;
        std::vector<std::vector<double>> factor = factor_value.values;
        std::vector<double> result(factor_cols.size(),0);
        for (std::size_t j = 0; j<factor_cols.size();++j){
            double sum = 0;
            for (std::size_t row=0; row<factor_index.size();++row){
                sum+=(pos[row][0]*factor[row][j]);
            }
            result[j] = sum;
        }
        target_result.push_back(result);
    }
    return risk_exposures_portfolio;
}

// 计算绩效归因
MyDataFrame perf_attrib(const MySeries& returns,
                        const MyPanel& positions,
                        const MyDataFrame& factor_returns,
                        const MyPanel& factor_loadings){
    MyDataFrame target_result;
    MyDataFrame risk_exposures_portfolio = compute_exposures(positions,
                                                 factor_loadings);

    return target_result;
}

#endif // EMPYRICAL_CPP_PERF_ATTRIB