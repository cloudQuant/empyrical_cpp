#ifndef EMPYRICAL_UTILS
#define EMPYRICAL_UTILS

#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <chrono>
#include <iomanip> // 用于处理时间格式
#include <chrono>
#include <utility>
//#include <cmath>
#include <cassert>
#include <cstring>
#define DTF_INLINE inline

// 假设 func 是一个接受两个参数的函数
using CustomFunc = std::function<double(const std::vector<double>&,
                                        const std::map<std::string, double>)>;

using DataFrame = std::vector<std::vector<double>>;



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

/*************************************************************************************************/

/*
 * 这段代码定义了一个名为 timestamp 的函数，它接受一个整数 offset 作为参数，并返回一个 std::uint64_t 类型的值。
 * 这个函数用于获取当前的时间戳，并根据提供的偏移量进行调整。
 */
DTF_INLINE std::uint64_t timestamp(int offset) {
    std::uint64_t ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

    std::uint64_t val = 60 * 60 * 1000000000ull * std::abs(offset);
    ts = (offset < 0) ? ts - val : ts + val;

    return ts;
}

/*************************************************************************************************/
/*
 * 这段代码定义了一个名为 num_chars 的函数，它接受一个 std::size_t 类型的参数 v，并返回一个 std::size_t 类型的值。这个函数用于计算一个数字的位数。
 */
DTF_INLINE std::size_t num_chars(std::size_t v) {
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
DTF_INLINE void utoa(char *ptr, std::size_t n, std::uint64_t v) {
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
DTF_INLINE std::size_t timestamp_to_chars(char *buf, std::uint64_t ts, std::size_t f) {

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
DTF_INLINE std::string timestamp_to_str(std::uint64_t ts, std::size_t f) {
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
DTF_INLINE std::string timestamp_str(std::size_t f, int offset) {
    const auto ts = timestamp(offset);
    return timestamp_to_str(ts, f);
}

/*************************************************************************************************/
DTF_INLINE char * dtf_year(char *p, std::size_t v){
    // Copy the digits to the output buffer
    *p++ = static_cast<char>((v / 1000)%10 + '0');
    *p++ = static_cast<char>((v / 100)%10 + '0');
    *p++ = static_cast<char>((v / 10)%10 + '0');
    *p++ = static_cast<char>(v % 10 + '0');
    return p;
}


DTF_INLINE char* dtf_month(char *p, std::size_t v){
    *p++ = static_cast<char>(((v + 1) / 10) % 10 + '0');
    *p++ = static_cast<char>(((v + 1)) % 10 + '0');
    return p;
}

DTF_INLINE char* dtf_day(char *p, std::size_t v){
    *p++ = static_cast<char>((v / 10) % 10 + '0');
    *p++ = static_cast<char>((v % 10) + '0');
    return p;
}


DTF_INLINE char* dtf_hms(char *p, std::size_t v){
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
DTF_INLINE std::size_t timestamp_to_dt_chars(char *ptr, std::uint64_t ts, std::size_t f) {
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
//    std::cout << "work = " << work <<std::endl;
//    std::cout << "years = " << years << std::endl;
//    std::cout << "month = " << mons << std::endl;
//    std::cout << "day = " << days << std::endl;
//    std::cout << "hours = " << hours << std::endl;
//    std::cout << "mins = " << mins << std::endl;
//    std::cout << "secs = " << secs << std::endl;
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
DTF_INLINE std::string timestamp_to_dt_str(std::uint64_t ts, std::size_t f) {
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
DTF_INLINE std::string timestamp_dt_str(std::size_t f, int offset) {
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
DTF_INLINE std::size_t dt_str_flags(const char *buf, std::size_t n) {
    std::size_t res{};

    const char c2  = buf[2];
    const char c10 = buf[10];
    const char c13 = buf[13];

    res |= (static_cast<unsigned>(c2 - '0') < 10u) ? flags::yyyy_mm_dd : flags::dd_mm_yyyy;
    res |= c10 == ' ' ? flags::sep1 : c13 == '.' ? flags::sep2 : flags::sep3;
    res |= n == 19 ? flags::secs : n == 23 ? flags::msecs : n == 26 ? flags::usecs : flags::nsecs;

    return res;
}

/*************************************************************************************************/

} // ns dtf

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

#endif // EMPYRICAL_UTILS

