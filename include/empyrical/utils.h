#pragma once
#ifndef EMPYRICAL_UTILS
#define EMPYRICAL_UTILS

#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <chrono>
#include <utility>
#include <cmath>
#include <limits>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <random>
#include <chrono>
#include "constants.h"


namespace cal_func{
    inline std::vector<double> roll_ndarray(const std::vector<double>& arr,
                                     const CustomFunc& func,
                                     const std::map<std::string, double>& params) {
        std::vector<double> data;
        int window = static_cast<int>(params.at("window"));
        data.reserve(arr.size() - window + 1);  // 提前预留空间

        // 初始化窗口数据
        std::vector<double> rets(arr.begin(), arr.begin() + window);

        // 计算初始窗口的值
        data.push_back(func(rets, params));

        // 滑动窗口计算后续值
        for (std::size_t i = window; i < arr.size(); ++i) {
            // 滑动窗口移除第一个元素，添加新元素
            rets.erase(rets.begin());
            rets.push_back(arr[i]);
            data.push_back(func(rets, params));
        }

        return data;
    }


    inline  DataFrameRollingWindowResult rolling_window(const DataFrame & df, int length) {
        // Check for valid input
        if (length <= 0) {
            throw std::invalid_argument("Window length must be positive");
        }

        std::size_t origRows = df.size();
        std::size_t origCols = df[0].size();

        if (origRows < static_cast<std::size_t>(length)) {
            throw std::invalid_argument("Cannot create windows of length " + std::to_string(length)
                                        + " from an array with fewer rows");
        }

        std::size_t numWindows = origRows - length + 1;

        DataFrameRollingWindowResult result;
        result.numWindows = numWindows;

        // Resize the data vector to hold the result
        result.data.resize(numWindows, DataFrame(length, std::vector<double>(origCols, 0)));

        // Populate the rolling window result
        for (std::size_t i = 0; i < numWindows; ++i) {
            for (int j = 0; j < length; ++j) {
                for (std::size_t k = 0; k < origCols; ++k) {
                    result.data[i][j][k] = df[i + j][k];
                }
            }
        }


        return result;
    }

    inline  ArrayRollingWindowResult rolling_window(const Array & arr, int length) {
        // Check for valid input
        if (length <= 0) {
            throw std::invalid_argument("Window length must be positive");
        }

        std::size_t origRows = arr.size();

        if (origRows < static_cast<std::size_t>(length)) {
            throw std::invalid_argument("Cannot create windows of length " + std::to_string(length)
                                        + " from an array with fewer rows");
        }

        std::size_t numWindows = origRows - length + 1;

        ArrayRollingWindowResult result;
        result.numWindows = numWindows;

        // Resize the data vector to hold the result
        result.data.resize(numWindows);

        // Populate the rolling window result
        for (std::size_t i = 0; i < numWindows; ++i) {
            result.data[i].resize(length);
            for (int j = 0; j < length; ++j) {
                result.data[i][j] = arr[i + j];
            }
        }

        return result;
    }


    inline  double nan_mean(const std::vector<double>& arr) {
        double sum = 0.0;
        int count = 0;
        for (const auto& num : arr) {
            if (!std::isnan(num)) {
                sum += num;
                ++count;
            }
        }
        return count > 0 ? sum / count : std::numeric_limits<double>::quiet_NaN();
    }

    inline double nan_std(const std::vector<double>& arr, std::size_t d_dof=0) {
        double mean = nan_mean(arr);
        double sum = 0.0;
        std::size_t count = 0;
        for (const auto& num : arr) {
            if (!std::isnan(num)) {
                sum += (num - mean) * (num - mean);
                ++count;
            }
        }
        return count > 1 ? std::sqrt(sum / static_cast<double>(count - d_dof)) : std::numeric_limits<double>::quiet_NaN();
    }

    inline double nan_sum(const std::vector<double>& arr) {
        double sum = 0.0;
        for (const auto& num : arr) {
            if (!std::isnan(num)) {
                sum += num;
            }
        }
        return sum;
    }

    inline double nan_max(const std::vector<double>& arr) {
        auto it = std::max_element(arr.begin(), arr.end(), [](double a, double b) {
            return std::isnan(a) || a < b;
        });
        return (it != arr.end() && !std::isnan(*it)) ? *it : std::numeric_limits<double>::quiet_NaN();
    }

    inline double nan_min(const std::vector<double>& arr) {
        auto it = std::min_element(arr.begin(), arr.end(), [](double a, double b) {
            // If 'a' is NaN, return false
            if (std::isnan(a)) return false;
            // If 'b' is NaN, return true
            if (std::isnan(b)) return true;
            // Otherwise, return whether 'a' is less than 'b'
            return a < b;
        });
        // Check if a valid iterator was found and if the value is not NaN
        return (it != arr.end() && !std::isnan(*it)) ? *it : std::numeric_limits<double>::quiet_NaN();
    }

    inline std::size_t nan_argmax(const std::vector<double>& arr) {
        std::size_t idx = -1;
        double max_val = std::numeric_limits<double>::quiet_NaN();
        for (std::size_t i = 0; i < arr.size(); ++i) {
            if (!std::isnan(arr[i]) && (std::isnan(max_val) || arr[i] > max_val)) {
                max_val = arr[i];
                idx = i;
            }
        }
        return idx;
    }

    inline std::size_t nan_argmin(const std::vector<double>& arr) {
        std::size_t idx = -1;
        double min_val = std::numeric_limits<double>::quiet_NaN();
        for (std::size_t i = 0; i < arr.size(); ++i) {
            if (!std::isnan(arr[i]) && (std::isnan(min_val) || arr[i] < min_val)) {
                min_val = arr[i];
                idx = i;
            }
        }
        return idx;
    }
    // 对vector中的数据取相反数
    inline std::vector<double> inverseVector(const std::vector<double> & vec){
        std::size_t size = vec.size();
        std::vector<double> result(size,0);
        for (std::size_t i=0; i<size; i++){
            result[i] = -1*vec[i];
        }
        return result;
    }

    // Generate random numbers with normal distribution
    inline std::vector<double> generateRandomNormal(int numValues, double mean, double stddev) {
        std::vector<double> values;
        //std::default_random_engine generator(std::time(nullptr));
        std::default_random_engine generator(std::time(nullptr));
        std::normal_distribution<double> distribution(mean, stddev);

        for (int i = 0; i < numValues; ++i) {
            double number = distribution(generator);
            values.push_back(number);
        }

        return values;
    }

    // 生成范围在[min, max]之间的均匀分布随机数
    inline double uniformRandom(double min, double max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    inline std::vector<double> generateUniformRandom(double min, double max, int num){
        std::vector<double> result(num, 0);
        for(int i = 0; i<num;++i){
            result[i] = uniformRandom(min, max);
        }
        return result;
    }

    // 生成包含 num 个元素，从 start 到 end 的等差数组
    inline std::vector<double> generateLinspace(double start, double end, int num) {
        std::vector<double> result(num);
        double step = (end - start) / (num - 1);
        for (int i = 0; i < num; ++i) {
            result[i] = start + i * step;
        }
        return result;
    }

    // 生成包含 min 到 max 之间的随机整数
    inline int randomInt(int min, int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(min, max);
        return distrib(gen);
    }

    // 把vector的值随机用一个NAN代替
    inline std::vector<double> replaceOneRandomValueWithNan(const std::vector<double>& vec){
        int i = randomInt(0, static_cast<int>(vec.size())-1);
        std::vector<double> result(vec);
        result[i] = NAN;
        return result;
    }
    // add a double value to vector
    inline std::vector<double> vector_add_value(std::vector<double> &vec, double v){
        std::vector<double> new_vec = vec;
        for (double &value : new_vec){
            value+=v;
        }
        return new_vec;
    }
    inline std::vector<double> vector_multiply_value(std::vector<double> &vec, double v){
        std::vector<double> new_vec = vec;
        for (double &value : vec){
            value*=v;
        }
        return new_vec;
    }
    inline std::vector<double> vector_add_vector(std::vector<double> &v1, std::vector<double> &v2){
        std::vector<double> result;
        //result.reserve(v1.size());
        for(std::size_t i=0; i<v1.size(); ++i){
            result.push_back(v1[i]+v2[i]);
        }
        return result;
    }
    inline std::vector<double> vector_minus_vector(std::vector<double> &v1, std::vector<double> &v2){
        std::vector<double> result;
        //result.reserve(v1.size());
        for(std::size_t i=0; i<v1.size(); ++i){
            result.push_back(v1[i]-v2[i]);
        }
        return result;
    }
    inline void print_vector(std::vector<double> &vec, const std::string& name="vector"){
        std::cout << name << " =[ ";
        for (double v:vec){
            std::cout << v << " ";
        }
        std::cout <<" ]" << std::endl;
    }
}
#endif // EMPYRICAL_UTILS





//std::vector<double> _roll_pandas(const DataFrame& df,
//                                  CustomFunc func,
//                                  const std::map<std::string, double> params) {
//    std::vector<double> data;
//    std::size_t window = static_cast<std::size_t>(params["window"]);
//    for (int i = window; i <= args[0].size(); ++i) {
//        std::vector<double> rets(vec.begin() + (i - window), vec.begin() + i));
//        data.push_back(func(rets, params));  // 调用 func 函数
//    }
//    return data;
//}
//





