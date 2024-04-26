#pragma once
#ifndef EMPYRICAL_CPP_STATS
#define EMPYRICAL_CPP_STATS
#include "constants.h"
#include "utils.h"

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

#endif // EMPYRICAL_CPP_STATS
