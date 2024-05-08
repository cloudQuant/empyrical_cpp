#pragma once
#ifndef EMPYRICAL_DATETIME
#define EMPYRICAL_DATETIME

#include <utility>
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

struct datetime_t{
    bool empty;
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

inline std::ostream& operator<<(std::ostream& os, const datetime_t& dt) {
    os << "Datetime: ";
    if (dt.empty) {
        os << "Empty";
    } else {
        os << std::setfill('0') << std::setw(4) << dt.year << "-"
            << std::setw(2) << dt.month << "-"
            << std::setw(2) << dt.day << " "
            << std::setw(2) << dt.hour << ":"
            << std::setw(2) << dt.minute << ":"
            << std::setw(2) << dt.seconds << "."
            << std::setw(3) << dt.milliseconds
            << std::setw(3) << dt.microseconds
            << std::setw(3) << dt.nanoseconds;
    }
    return os;
}

class DateTime{

public:
    // DateTime() : m_timestamp(NAN), m_datetime_t{true, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}
    explicit DateTime(std::string  str) : m_str_datetime(std::move(str)), m_timestamp(0), m_datetime_t{true, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}
    explicit DateTime(std::uint64_t timestamp) : m_timestamp(timestamp), m_datetime_t{true, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}
    explicit DateTime(const datetime_t& now_datetime) : m_timestamp(0), m_datetime_t(now_datetime) {}

    static datetime_t timestamp_to_datetime(std::uint64_t current_time_ns){
        datetime_t now_datetime{};
        // 将纳秒级别的时间戳转换为秒级别
        auto current_time_sec = current_time_ns / 1000000000; // 1秒 = 10^9纳秒

        // 将秒级别时间戳转换为time_t类型
        auto current_time_t = static_cast<std::time_t>(current_time_sec);

        // 获取本地时间结构体
        std::tm* local_time = std::localtime(&current_time_t);

        now_datetime.year = local_time->tm_year + 1900;
        now_datetime.month =  local_time->tm_mon + 1;
        now_datetime.day = local_time->tm_mday;
        now_datetime.week = local_time->tm_wday;
        // 获取当前时分秒信息
        now_datetime.hour = local_time->tm_hour;
        now_datetime.minute = local_time->tm_min;
        now_datetime.seconds = local_time->tm_sec;

        // 获取当前的毫秒、微秒和纳秒信息

        auto current_time_remainder_ns = current_time_ns % 1000000000; // 获取纳秒部分
        now_datetime.milliseconds = current_time_remainder_ns / 1000000; // 1毫秒 = 10^6纳秒
        auto current_time_remainder = current_time_remainder_ns % 1000000;
        now_datetime.microseconds = current_time_remainder / 1000; // 1微秒 = 10^3纳秒
        current_time_remainder = current_time_remainder % 1000;
        now_datetime.nanoseconds = current_time_remainder_ns / 1000; // 剩余纳秒部分
        now_datetime.empty = false;
        return now_datetime;

    }

    static std::string timestamp_to_string(std::uint64_t timestamp){
        datetime_t now_datetime = timestamp_to_datetime(timestamp);
        return datetime_to_string(now_datetime, flags::nsecs);
    }

    static datetime_t string_to_datetime(const std::string& str_datetime, std::size_t f = flags::nsecs){
        std::uint64_t timestamp = string_to_timestamp(str_datetime, f);
        datetime_t now_datetime = timestamp_to_datetime(timestamp);
        return now_datetime;
    }

    static std::string datetime_to_string(datetime_t & now_datetime_t, std::size_t f){
        std::ostringstream oss;

        // 根据不同的标志位进行日期时间转换
        if (f==flags::msecs) {
            oss << std::setfill('0') << std::setw(4) << now_datetime_t.year << "-"
                << std::setw(2) << now_datetime_t.month << "-"
                << std::setw(2) << now_datetime_t.day << " "
                << std::setw(2) << now_datetime_t.hour << ":"
                << std::setw(2) << now_datetime_t.minute << ":"
                << std::setw(2) << now_datetime_t.seconds << "."
                << std::setw(3) << now_datetime_t.milliseconds;
        } else if (f==flags::usecs) {
            oss << std::setfill('0') << std::setw(4) << now_datetime_t.year << "-"
                << std::setw(2) << now_datetime_t.month << "-"
                << std::setw(2) << now_datetime_t.day << " "
                << std::setw(2) << now_datetime_t.hour << ":"
                << std::setw(2) << now_datetime_t.minute << ":"
                << std::setw(2) << now_datetime_t.seconds << "."
                << std::setw(6) << now_datetime_t.microseconds;
        } else if (f==flags::nsecs) {
            oss << std::setfill('0') << std::setw(4) << now_datetime_t.year << "-"
                << std::setw(2) << now_datetime_t.month << "-"
                << std::setw(2) << now_datetime_t.day << " "
                << std::setw(2) << now_datetime_t.hour << ":"
                << std::setw(2) << now_datetime_t.minute << ":"
                << std::setw(2) << now_datetime_t.seconds << "."
                << std::setw(9) << now_datetime_t.nanoseconds;
        }

        return oss.str();
    }

    static std::uint64_t datetime_to_timestamp(datetime_t & now_datetime, std::size_t f) {
        // 定义时间单位的变量
        std::size_t duration = now_datetime.nanoseconds + now_datetime.microseconds*1000
                + now_datetime.milliseconds*1000000;
        std::tm tm = {};
        tm.tm_year = static_cast<int>(now_datetime.year) - 1900;
        tm.tm_mon = static_cast<int>(now_datetime.month)-1;
        tm.tm_mday = static_cast<int>(now_datetime.day);
        tm.tm_hour = static_cast<int>(now_datetime.hour);
        tm.tm_min = static_cast<int>(now_datetime.minute);
        tm.tm_sec = static_cast<int>(now_datetime.seconds);
        // Convert std::tm to std::chrono::system_clock::time_point
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        // Add milliseconds, microseconds, or nanoseconds to the time_point
        //auto tp_duration = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::nanoseconds(duration));
        //tp += tp_duration;
        std::chrono::nanoseconds duration_since_epoch = tp.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(duration_since_epoch).count()+duration;
        return value;
    }

    static std::uint64_t string_to_timestamp(const std::string& input_dt, std::size_t f){
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

        // Convert time_point to specified time unit timestamp
        bool v1 = f == flags::msecs;
        bool v2 = f == flags::usecs;
        bool v3 = f == flags::nsecs;
        if (v1) {
            std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
            return duration.count()+microseconds;
        } else if (v2) {
            std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(
                    tp.time_since_epoch());
            return duration.count()+microseconds;
        }else if (v3){
            std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch());
            return duration.count()+microseconds;
        } else {
            std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch());
            return duration.count()+microseconds;
        }
    }

    void init_datetime_t(){
        if (m_datetime_t.empty){
            if (m_timestamp==0){
                m_datetime_t = string_to_datetime(m_str_datetime, flags::nsecs);

            }else{
                m_datetime_t = timestamp_to_datetime(m_timestamp);
            }

        }
    }

    void init_timestamp(){
        if (m_timestamp==0){
            if (m_datetime_t.empty){
                m_timestamp = string_to_timestamp(m_str_datetime, flags::nsecs);
            }else{
                m_timestamp = datetime_to_timestamp(m_datetime_t, flags::nsecs);
            }

        }
    }

    void init_str_datetime(){
        if (m_str_datetime.empty()){
            if (m_datetime_t.empty){
                m_str_datetime = timestamp_to_string(m_timestamp);
            }else{
                m_str_datetime = datetime_to_string(m_datetime_t, flags::nsecs);
            }
        }
    }

    std::string get_str_datetime(){
        init_str_datetime();
        return m_str_datetime;
    }

    datetime_t get_datetime(){
        init_datetime_t();
        return m_datetime_t;
    }

    std::uint64_t get_timestamp(){
        init_timestamp();
        return m_timestamp;
    }

    uint get_year(){
        init_datetime_t();
        return m_datetime_t.year;
    }
    uint get_month(){
        init_datetime_t();
        return m_datetime_t.month;
    }
    uint get_day(){
        init_datetime_t();
        return m_datetime_t.day;
    }
    uint get_week(){
        init_datetime_t();
        return m_datetime_t.week;
    }
    uint get_hour(){
        init_datetime_t();
        return m_datetime_t.hour;
    }
    uint get_minute(){
        init_datetime_t();
        return m_datetime_t.minute;
    }
    uint get_second(){
        init_datetime_t();
        return m_datetime_t.seconds;
    }
    uint get_milliseconds(){
        init_datetime_t();
        return m_datetime_t.milliseconds;
    }
    uint get_microseconds(){
        init_datetime_t();
        return m_datetime_t.milliseconds;
    }
    uint get_nanoseconds(){
        init_datetime_t();
        return m_datetime_t.nanoseconds;
    }
private:
    std::string m_str_datetime;
    std::uint64_t m_timestamp;
    datetime_t m_datetime_t;

};

// 假设 func 是一个接受两个参数的函数
using CustomFunc = std::function<double(const std::vector<double>&,
                                        const std::map<std::string, double>)>;




struct ArrayRollingWindowResult {
    std::vector<std::vector<double>> data;
    std::size_t numWindows{};
};
struct DataFrameRollingWindowResult {
    std::vector<std::vector<std::vector<double>>> data;
    std::size_t numWindows{};
};

enum class Interpolation { Linear, Lower, Higher, Midpoint, Nearest };

struct MySeries{
    std::vector<DateTime> index;
    std::vector<double> values;
    std::string index_data_type = "DateTime";
    std::vector<std::string> string_index;
};

struct MyDataFrame{
    std::vector<DateTime> index;
    std::vector<std::string> cols;
    std::vector<std::vector<double>> values;
    std::string index_data_type = "DateTime";
    std::vector<std::string> string_index;
};

struct MyPanel{
    std::vector<DateTime> index;
    std::vector<MyDataFrame> values;
    std::string index_data_type = "DateTime";
    std::vector<std::string> string_index;
};
#endif //EMPYRICAL_DATETIME