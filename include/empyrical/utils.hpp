//#include <vector>
//#include <algorithm>
//#include <functional>
//#include <map>
//#include <string>
//#include <chrono>
//#include <iomanip> // 用于处理时间格式

// 假设 func 是一个接受两个参数的函数
using CustomFunc = std::function<double(const std::vector<double>&,
                                        const std::map<std::string, double>)>;

using DataFrame = std::vector<std::vector<double>>;

//std::vector<double> _roll_ndarray(const std::vector<double>& arr,
//                                const CustomFunc& func,
//                                const std::map<std::string, double>& params) {
//    std::vector<double> data;
//    std::size_t window = static_cast<std::size_t>(params["window"]);
//    for (std::size_t i = window; i <= arr.size(); ++i) {
//        std::vector<double> rets(arr.begin() + (i - window), arr.begin() + i));
//        data.push_back(func(rets, params));  // 调用 func 函数
//    }
//    return data;
//}

//std::string get_utc_timestamp(const std::string& dt_str) {
//    // 解析时间字符串
//    std::tm tm = {};
//    std::istringstream ss(dt_str);
//    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
//
//    if (ss.fail()) {
//        return "Invalid input format!";
//    }
//
//    // 将解析后的时间转换为时间点
//    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
//
//    // 转换为 UTC 时间
//    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
//    std::string utc_time = std::ctime(&tt);
//    return utc_time;
//}

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
//// 示例自定义函数，计算两个向量的平均值
//double custom_avg_func(const std::vector<double>& arr1, const std::vector<double>& arr2) {
//    double sum = 0.0;
//    for (std::size_t i = 0; i < arr1.size(); ++i) {
//        sum += arr1[i] + arr2[i];
//    }
//    return sum / arr1.size();
//}
//int main() {
//    // 测试示例
//    std::string input_dt = "2024-04-10 12:00:00";  // 输入的时间字符串
//    std::string utc_dt = get_utc_timestamp(input_dt);
//    std::cout << "UTC Time: " << utc_dt;
//
//    return 0;
//}

