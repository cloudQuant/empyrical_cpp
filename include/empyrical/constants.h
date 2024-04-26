#pragma once
#ifndef EMPYRICAL_CPP_PERIOD
#define EMPYRICAL_CPP_PERIOD
#include <unordered_map>
#include <string>

// 定义常量
constexpr std::size_t APPROX_BDAYS_PER_MONTH = 21;
constexpr std::size_t APPROX_BDAYS_PER_YEAR = 252;

constexpr std::size_t MONTHS_PER_YEAR = 12;
constexpr std::size_t WEEKS_PER_YEAR = 52;
constexpr std::size_t QTRS_PER_YEAR = 4;

// 定义字符串常量
const std::string DAILY = "daily";
const std::string WEEKLY = "weekly";
const std::string MONTHLY = "monthly";
const std::string QUARTERLY = "quarterly";
const std::string YEARLY = "yearly";

// 定义映射关系
const std::unordered_map<std::string, std::size_t> ANNUALIZATION_FACTORS = {
        {DAILY, APPROX_BDAYS_PER_YEAR},
        {WEEKLY, WEEKS_PER_YEAR},
        {MONTHLY, MONTHS_PER_YEAR},
        {QUARTERLY, QTRS_PER_YEAR},
        {YEARLY, 1}
};

// 假设 func 是一个接受两个参数的函数
using CustomFunc = std::function<double(const std::vector<double>&,
                                        const std::map<std::string, double>)>;

using Array = std::vector<double>;
using DataFrame = std::vector<std::vector<double>>;

struct ArrayRollingWindowResult {
    std::vector<Array> data;
    std::size_t numWindows{};
};
struct DataFrameRollingWindowResult {
    std::vector<DataFrame> data;
    std::size_t numWindows{};
};

#endif // EMPYRICAL_CPP_PERIOD