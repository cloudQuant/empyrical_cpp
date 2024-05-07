#pragma once
#ifndef EMPYRICAL_CPP_CONSTANTS
#define EMPYRICAL_CPP_CONSTANTS
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

struct PdSeries{
    std::vector<std::any> index;
    std::vector<double> values;

    void addValue(const std::pair<std::string,double>& p) {
        index.push_back(p.first);
        values.push_back(p.second);
    }

    void addValue(const std::pair<double,double>& p) {
        index.push_back(p.first);
        values.push_back(p.second);
    }
    void print_index() {
        for (const auto& val : index) {
            if (val.type() == typeid(std::string)) {
                std::cout << std::any_cast<std::string>(val) << " " << std::endl;
            } else if (val.type() == typeid(double)) {
                std::cout << std::any_cast<double>(val) << " " << std::endl;
            }
        }
    }
    void print_values() {
        for (const auto& val : values) {
            std::cout << val<< " " << std::endl;
        }
    }
};




struct PdDataFrame{
    std::vector<std::string> index;
    std::vector<std::string> cols;
    std::vector<std::vector<double>> values;
};

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

struct datetime_t{
    uint year;
    uint month;
    uint day;
    uint week;
    uint hour;
    uint minute;
    uint seconds;
    uint milliseconds;
    uint microseconds;
    uint nanoseconds;
};

enum class Interpolation { Linear, Lower, Higher, Midpoint, Nearest };

#endif // EMPYRICAL_CPP_CONSTANTS