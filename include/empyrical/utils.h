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
namespace dtf {

enum { bufsize = 32 };

struct flags {
    enum: std::size_t {
        yyyy_mm_dd = 1u << 0u
        ,dd_mm_yyyy = 1u << 1u
        ,sep1       = 1u << 2u // 2018-12-11 13:58:56
        ,sep2       = 1u << 3u // 2018.12.11-13.58.59
        ,sep3       = 1u << 4u // 2018.12.11-13:58:59
        ,secs       = 1u << 5u // seconds resolution
        ,msecs      = 1u << 6u // milliseconds resolution
        ,usecs      = 1u << 7u // microseconds resolution
        ,nsecs      = 1u << 8u // nanoseconds resolution
    };
};

inline  std::vector<std::chrono::system_clock::time_point> date_range(const std::string& start_date_str,
                                                                      int periods,
                                                                      const std::string& freq,
                                                                      bool v=true) {
        // Define time step based on frequency
        // Convert start date string to time point
        std::tm tm = {};
        strptime(start_date_str.c_str(), "%Y-%m-%d", &tm);
        std::time_t start_time = std::mktime(&tm);
        std::chrono::system_clock::time_point start_date =
                std::chrono::system_clock::from_time_t(start_time);
        // Generate datetime list
        std::vector<std::chrono::system_clock::time_point> datetime_list;
        // Default to daily frequency
        if (freq == "H") {
            std::chrono::hours time_step = std::chrono::hours(1);  // Hourly frequency
            for (int i = 0; i < periods; ++i) {
                datetime_list.push_back(start_date + i * time_step);
            }
        } else if (freq == "M") {
            std::chrono::minutes time_step = std::chrono::minutes(1);  // Minutely frequency
            for (int i = 0; i < periods; ++i) {
                datetime_list.push_back(start_date + i * time_step);
            }
        } else {
            std::chrono::hours time_step(24);
            for (int i = 0; i < periods; ++i) {
                datetime_list.push_back(start_date + i * time_step);
            }
        }
        return datetime_list;
    }

    // 将时间点转换为字符串格式（包含毫秒、微秒、纳秒）
inline  std::string timePointToString(const std::chrono::system_clock::time_point& timePoint) {
        // 转换为time_t类型
        std::time_t time = std::chrono::system_clock::to_time_t(timePoint);

        // 使用std::put_time和std::strftime将time_t格式化为字符串
        std::tm tm = *std::localtime(&time);
        auto duration = timePoint.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        auto fractional_seconds = milliseconds % 1000;

        // 计算微秒和纳秒
        auto micro = std::chrono::duration_cast<std::chrono::microseconds>(duration);
        auto micro_fraction = micro % 1000;
        auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
        auto nano_fraction = nano % 1000;

        std::stringstream ss;
        ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        ss << "." << std::setfill('0') << std::setw(3) << fractional_seconds.count(); // 添加毫秒
        ss << std::setfill('0') << std::setw(3) << micro_fraction.count(); // 添加微秒
        ss << std::setfill('0') << std::setw(3) << nano_fraction.count(); // 添加纳秒

        return ss.str();
    }

inline std::vector<std::string> date_range(const std::string& start_date_str,
                                           int periods,
                                           const std::string& freq,
                                           int type = 0) {
        // Define time step based on frequency
        // Convert start date string to time point
        std::tm tm = {};
        strptime(start_date_str.c_str(), "%Y-%m-%d", &tm);
        std::time_t start_time = std::mktime(&tm);
        std::chrono::system_clock::time_point start_date =
                std::chrono::system_clock::from_time_t(start_time);
        // Generate datetime list
        std::vector<std::string> datetime_list;
        // Default to daily frequency
        if (freq == "H") {
            std::chrono::hours time_step = std::chrono::hours(1);  // Hourly frequency
            for (int i = 0; i < periods; ++i) {
                std::string r = timePointToString(start_date + i * time_step);
                datetime_list.push_back(r);
            }
        } else if (freq == "MIN") {
            std::chrono::minutes time_step = std::chrono::minutes(1);  // Minutely frequency
            for (int i = 0; i < periods; ++i) {
                std::string r = timePointToString(start_date + i * time_step);
                datetime_list.push_back(r);
            }
        } else if (freq == "W"){
            std::chrono::hours time_step(24*7);
            for (int i = 0; i < periods; ++i) {
                std::string r = timePointToString(start_date + i * time_step);
                datetime_list.push_back(r);
            }
        } else {
            std::chrono::hours time_step(24);
            for (int i = 0; i < periods; ++i) {
                std::string r = timePointToString(start_date + i * time_step);
                datetime_list.push_back(r);
            }
        }
        return datetime_list;
    }

/*************************************************************************************************/

/*
 * 这段代码定义了一个名为 timestamp 的函数，
 * 它接受一个整数 offset 作为参数，
 * 并返回一个 std::uint64_t 类型的值。
 * 这个函数用于获取当前的时间戳，并根据提供的偏移量进行调整。
 */
inline std::uint64_t timestamp(int offset) {
    std::uint64_t ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

    std::uint64_t val = 60 * 60 * 1000000000ull * std::abs(offset);
    ts = (offset < 0) ? ts - val : ts + val;

    return ts;
}

/*************************************************************************************************/
/*
 * 这段代码定义了一个名为 num_chars 的函数，
 * 它接受一个 std::size_t 类型的参数 v，
 * 并返回一个 std::size_t 类型的值。
 * 这个函数用于计算一个数字的位数。
 */
inline std::size_t num_chars(std::size_t v) {
    // MAX LEN OF SIZE_T IS 20
    std::size_t n = 1;
    v = (v >= 100000000000000000ull) ? ((n += 17),v / 100000000000000000ull) : v;
    v = (v >= 100000000ull) ? ((n += 8),v / 100000000ull) : v;
    v = (v >= 10000ull) ? ((n += 4),v / 10000ull) : v;
    v = (v >= 100ull) ? ((n += 2),v / 100ull) : v;
    n = (v >= 10ull) ? n + 1 : n;

    return n;
}

/*!
 * 它将一个无符号 64 位整数 v 转换为一个以 null 结尾的字符串，并将结果存储在 ptr 指向的缓冲区中。
 * @param ptr: char *ptr：指向一个字符数组的指针，用于存储转换后的字符串。
 * @param n: std::size_t n：缓冲区 ptr 的大小，包括 null 终止符。
 * @param v: std::uint64_t v：要转换的无符号 64 位整数。
 */
inline void utoa(char *ptr, std::size_t n, std::uint64_t v) {
    // switch is not so good to write, while is more appropriate
    char *p = ptr + n - 1;
    while (n>0){
        *p-- = static_cast<char>('0'+(v % 10));
        v /= 10;
        n-=1;
    }
}

/*************************************************************************************************/

/*!
 * 它将一个时间戳转换为字符串格式，并将结果存储在一个字符数组中。
 * @param buf:一个指向字符数组的指针，用于存储转换后的字符串
 * @param ts:一个无符号的64位整数，表示时间戳。
 * @param f:一个std::size_t类型的值，表示转换的标志。
 * @return n: std::size_t
 */
inline std::size_t timestamp_to_chars(char *buf, std::uint64_t ts, std::size_t f) {

    if (f & flags::secs){
        ts = ts / 1000000000ull;
    }
    else if (f & flags::msecs){
        ts = ts / 1000000ull;
    }
    else if (f & flags::usecs){
        ts = ts / 1000ull;
    } else{
    }

    const auto n = num_chars(ts);
    utoa(buf, n, ts);

    return n;
}

/*!
 * 它将一个时间戳转换为字符串格式。
 * @param ts: 一个无符号的64位整数，表示时间戳。
 * @param f: 一个std::size_t类型的值，表示转换的标志。
 * @return std::string
 */
inline std::string timestamp_to_str(std::uint64_t ts, std::size_t f) {
    char buf[bufsize];
    const auto n = timestamp_to_chars(buf, ts, f);
    buf[n] = 0;

    return buf;
}

/*!
 * 返回一个表示当前时间戳的字符串。
 * @param f:一个std::size_t类型的值，表示转换的标志。
 * @param offset: 一个整数，表示时间戳的偏移量
 * @return std::string
 */
inline std::string timestamp_str(std::size_t f, int offset) {
    const auto ts = timestamp(offset);
    return timestamp_to_str(ts, f);
}

/*************************************************************************************************/
inline char * dtf_year(char *p, std::size_t v){
    // Copy the digits to the output buffer
    *p++ = static_cast<char>((v / 1000)%10 + '0');
    *p++ = static_cast<char>((v / 100)%10 + '0');
    *p++ = static_cast<char>((v / 10)%10 + '0');
    *p++ = static_cast<char>(v % 10 + '0');
    return p;
}


inline char* dtf_month(char *p, std::size_t v){
    *p++ = static_cast<char>(((v + 1) / 10) % 10 + '0');
    *p++ = static_cast<char>(((v + 1)) % 10 + '0');
    return p;
}

inline char* dtf_day(char *p, std::size_t v){
    *p++ = static_cast<char>((v / 10) % 10 + '0');
    *p++ = static_cast<char>((v % 10) + '0');
    return p;
}


inline char* dtf_hms(char *p, std::size_t v){
    *p++ = static_cast<char>((v / 10) % 10 + '0');
    *p++ = static_cast<char>(v % 10 + '0');
    return p;
}
/*!
 * 它将一个时间戳转换为日期和时间的字符串格式，并将结果存储在一个字符数组中。
 * @param ptr:一个指向字符数组的指针，用于存储转换后的字符串
 * @param ts:一个无符号的64位整数，表示时间戳。
 * @param f:一个std::size_t类型的值，表示转换的标志。
 * @return
 */
inline std::size_t timestamp_to_dt_chars(char *ptr, std::uint64_t ts, std::size_t f) {
    const auto datesep = (f & flags::sep1) ? '-' : '.';
    const auto timesep = (f & flags::sep1)||(f & flags::sep3) ? ':' : '.';
    const auto secsep  = (f & flags::sep1) ? ' ' : '-';
    const auto ss = ts / 1000000000ull;
    const auto ps = ts % 1000000000ull;

    static const std::size_t SPD = 24 * 60 * 60;
    static const std::uint16_t spm[13] = {
            0
            ,(31)
            ,(31 + 28)
            ,(31 + 28 + 31)
            ,(31 + 28 + 31 + 30)
            ,(31 + 28 + 31 + 30 + 31)
            ,(31 + 28 + 31 + 30 + 31 + 30)
            ,(31 + 28 + 31 + 30 + 31 + 30 + 31)
            ,(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31)
            ,(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30)
            ,(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31)
            ,(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30)
            ,(31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31)
    };
    static const auto is_leap_year = [](std::size_t t) {
        return (!(t % 4) && ((t % 100) || !(t % 400)));
    };

    std::size_t work = ss % SPD;
    std::size_t secs =  work % 60; work /= 60;
    std::size_t mins = work % 60;//add a work /= 60; compare to original code
    std::size_t hours = work / 60;
    work = ss / SPD;
    std::size_t i = 1970;
    for ( ; ; ++i ) {
        std::size_t k = is_leap_year(i) ? 366 : 365;
        if ( work >= k ) {
            work -= k;
        } else {
            break;
        }
    }
    std::size_t  years = i;

    std::size_t days = 1;
    if ( is_leap_year(i) && (work > 58) ) {
        if (work == 59) {
            days = 2;
        }
        work -= 1;
    }
    // std::cout << "new_work << " << work << std::endl;
    for ( i = 11; i && (spm[i] > work); --i )
        ;
    std::size_t mons = i;
    days += work - spm[i];
    char *p = ptr;
    std::memset(p, '0', bufsize);

    if ( f & flags::yyyy_mm_dd ) {
        p = dtf_year(p, years);
        *p++ = datesep;
        p = dtf_month(p, mons);
        *p++ = datesep;
        p = dtf_day(p, days);

    } else if ( f & flags::dd_mm_yyyy ) {
        p = dtf_day(p, days);
        *p++ = datesep;
        p=dtf_month(p, mons);
        *p++ = datesep;
        p = dtf_year(p, years);
    } else {
        assert(!"unreachable");
    }

    *p++ = secsep;

    p = dtf_hms(p, hours);
    *p++ = timesep;
    p = dtf_hms(p, mins);
    *p++ = timesep;
    p = dtf_hms(p, secs);

    const auto pi = (f & flags::secs) ? std::make_pair(ps / 1000000000ull, 0u)
                                      : (f & flags::msecs) ? std::make_pair(ps / 1000000ull, 3u)
                                                           : (f & flags::usecs) ? std::make_pair(ps / 1000ull, 6u)
                                                                                : std::make_pair(ps, 9u)
    ;
    if ( pi.second ) {
        *p++ = '.';

        auto n = num_chars(pi.first);
        p += pi.second - n;

        utoa(p, n, pi.first);
        p += n;
    }

    return p - ptr;
}

/*************************************************************************************************/
/*!
 * 它将一个时间戳转换为日期字符串格式，并将结果存储在一个字符数组中。
 * @param ts:一个无符号的64位整数，表示时间戳。
 * @param f:一个std::size_t类型的值，表示转换的标志。
 * @return
 */
inline std::string timestamp_to_dt_str(std::uint64_t ts, std::size_t f) {
    char buf[bufsize];
    const auto n = timestamp_to_dt_chars(buf, ts, f);
    buf[n] = 0;

    return buf;
}

/*!
* 它返回一个表示当前时间戳的日期和时间字符串。
* @param f:一个std::size_t类型的值，表示转换的标志。
* @param offset:一个整数，表示时间戳的偏移量。
* @return std::string
*/
inline std::string timestamp_dt_str(std::size_t f, int offset) {
    const auto ts = timestamp(offset);
    return timestamp_to_dt_str(ts, f);
}

/*************************************************************************************************/

/*!
 * 根据输入的日期和时间字符串来确定相应的标志。
 * @param buf: 一个指向字符数组的指针，该数组包含了日期和时间的字符串。
 * @param n: 一个std::size_t类型的值，表示字符串的长度。
 * @return std::size_t
 */
inline std::size_t dt_str_flags(const char *buf, std::size_t n) {
    std::size_t res{};

    const char c2  = buf[2];
    const char c10 = buf[10];
    const char c13 = buf[13];

    res |= (static_cast<unsigned>(c2 - '0') < 10u) ? flags::yyyy_mm_dd : flags::dd_mm_yyyy;
    res |= c10 == ' ' ? flags::sep1 : c13 == '.' ? flags::sep2 : flags::sep3;
    res |= n == 19 ? flags::secs : n == 23 ? flags::msecs : n == 26 ? flags::usecs : flags::nsecs;

    return res;
}

inline std::uint64_t dt_to_timestamp(const std::string& input_dt, std::size_t f){
    std::tm tm = {};
    std::istringstream ss(input_dt);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    // Extract microseconds from the input string
    std::string microsec_str;
    ss >> microsec_str;
    std::size_t pos = microsec_str.find('.');
    int microseconds = 0;
    if (pos != std::string::npos && pos + 1 < microsec_str.size()) {
        microsec_str = microsec_str.substr(pos + 1); // Extract fractional part
        microseconds = std::stoi(microsec_str);
    }

    // Convert std::tm to std::chrono::system_clock::time_point
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    // Add microseconds to the time_point
    tp += std::chrono::microseconds(microseconds);

    // Convert time_point to specified time unit timestamp
    bool v1 = f == dtf::flags::msecs;
    bool v2 = f == dtf::flags::usecs;
    bool v3 = f == dtf::flags::nsecs;
    if (v1) {
        std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
        return duration.count();
    } else if (v2) {
        std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(
                tp.time_since_epoch());
        return duration.count();
    }else if (v3){
        std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch());
        return duration.count();
    } else {
        std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
        return duration.count();
    }
    }

/*************************************************************************************************/

} // ns dtf

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





