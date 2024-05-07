#include "gtest/gtest.h"
#include "../include/empyrical/dtf.h"
class DtfTest : public ::testing::Test {
protected:
    // You can add setup and teardown code here if needed
};

// Test case for dt_str_flags function
TEST_F(DtfTest, TestDtStrFlags) {
const char *buffer1 = "2021-01-01 00:00:00";
const char *buffer2 = "01-01-2021-00:00:00.000";
const char *buffer3 = "2021-01-01 00.00.00.000000";

std::size_t flags1 = dtf::dt_str_flags(buffer1, 19);
std::cout << "flag1 = " << flags1 << std::endl;
EXPECT_EQ(flags1, dtf::flags::yyyy_mm_dd | dtf::flags::sep1 | dtf::flags::secs);

std::size_t flags2 = dtf::dt_str_flags(buffer2, 23);
std::cout << "flag2 = " << flags2 << std::endl;
EXPECT_EQ(flags2, dtf::flags::dd_mm_yyyy | dtf::flags::sep3 | dtf::flags::msecs);

std::size_t flags3 = dtf::dt_str_flags(buffer3, 26);
std::cout << "flag3 = " << flags3 << std::endl;
EXPECT_EQ(flags3, dtf::flags::yyyy_mm_dd | dtf::flags::sep1 | dtf::flags::usecs);

// Add more test cases as needed
}


// Test case for timestamp_dt_str function
TEST_F(DtfTest, TestTimestampDtStr) {
// Assuming the timestamp function returns a fixed value for testing purposes
// You need to replace the expected string with the actual value you expect from the timestamp function
std::string expected = "2021-01-01 00:00:00";

std::string str = dtf::timestamp_dt_str(dtf::flags::yyyy_mm_dd | dtf::flags::secs, 0);
EXPECT_NE(str, expected);

}


// Test case for timestamp_to_dt_chars function
TEST_F(DtfTest, TestTimestampToDtChars) {
char buffer[64];
std::uint64_t timestamp = 1609459200000000000ull; // 2021-01-01 00:00:00

std::size_t n = dtf::timestamp_to_dt_chars(buffer, timestamp, dtf::flags::yyyy_mm_dd | dtf::flags::secs  | dtf::flags::sep1);
buffer[n] = '\0'; // Null-terminate the string
//    std::cout << "buffer = " << buffer << std::endl;
EXPECT_EQ(std::strcmp(buffer, "2021-01-01 00:00:00"), 0);

n = dtf::timestamp_to_dt_chars(buffer, timestamp, dtf::flags::dd_mm_yyyy | dtf::flags::msecs | dtf::flags::sep1);
buffer[n] = '\0'; // Null-terminate the string
//    std::cout << "buffer = " << buffer << std::endl;
EXPECT_EQ(std::strcmp(buffer, "01-01-2021 00:00:00.000"), 0);

n = dtf::timestamp_to_dt_chars(buffer, timestamp, dtf::flags::yyyy_mm_dd | dtf::flags::usecs | dtf::flags::sep1);
buffer[n] = '\0'; // Null-terminate the string
//    std::cout << "buffer = " << buffer << std::endl;
EXPECT_EQ(std::strcmp(buffer, "2021-01-01 00:00:00.000000"), 0);

// Add more test cases as needed
}

// Test case for dtf_hms function
TEST_F(DtfTest, TestDtfHms) {
char buffer[3];
char *p = dtf::dtf_hms(buffer, 0);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "00"), 0);

p = dtf::dtf_hms(buffer, 9);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "09"), 0);

p = dtf::dtf_hms(buffer, 15);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "15"), 0);

p = dtf::dtf_hms(buffer, 59);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "59"), 0);

// Add more test cases as needed
}

// Test case for dtf_day function
TEST_F(DtfTest, TestDtfDay) {
char buffer[3];
char *p = dtf::dtf_day(buffer, 1);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "01"), 0);

p = dtf::dtf_day(buffer, 9);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "09"), 0);

p = dtf::dtf_day(buffer, 15);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "15"), 0);

p = dtf::dtf_day(buffer, 31);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "31"), 0);

// Add more test cases as needed
}

// Test case for dtf_month function
TEST_F(DtfTest, TestDtfMonth) {
char buffer[3];
char *p = dtf::dtf_month(buffer, 0);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "01"), 0);

p = dtf::dtf_month(buffer, 9);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "10"), 0);

p = dtf::dtf_month(buffer, 11);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "12"), 0);

// Add more test cases as needed
}

// Test case for dtf_year function
TEST_F(DtfTest, TestDtfYear) {
char buffer[5];
char *p = dtf::dtf_year(buffer, 2022);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "2022"), 0);

p = dtf::dtf_year(buffer, 1970);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "1970"), 0);

p = dtf::dtf_year(buffer, 9999);
*p = '\0'; // Null-terminate the string
EXPECT_EQ(std::strcmp(buffer, "9999"), 0);

// Add more test cases as needed
}


// Test case for timestamp_str function
TEST_F(DtfTest, TestTimestampStr) {
std::string str = timestamp_str(dtf::flags::secs, 0);
EXPECT_FALSE(str.empty()); // Check that the string is not empty

str = timestamp_str(dtf::flags::msecs, 0);
EXPECT_FALSE(str.empty()); // Check that the string is not empty

str = timestamp_str(dtf::flags::usecs, 0);
EXPECT_FALSE(str.empty()); // Check that the string is not empty

// Add more test cases as needed
}

// Test case for timestamp_to_str function
TEST_F(DtfTest, TestTimestampToStr) {
std::uint64_t timestamp = 1234567890000000000ull; // 1234567890 seconds

std::string str = timestamp_to_str(timestamp, dtf::flags::secs);
EXPECT_EQ(str, "1234567890");

str = timestamp_to_str(timestamp, dtf::flags::msecs);
EXPECT_EQ(str, "1234567890000");

str = timestamp_to_str(timestamp, dtf::flags::usecs);
EXPECT_EQ(str, "1234567890000000");

// Add more test cases as needed
}

//// Test case for timestamp_to_chars function
//TEST_F(DtfTest, TestTimestampToChars) {
//    todo
//    char buffer[20];
//    std::uint64_t timestamp = 1234567890000000000ull; // 1234567890 seconds
//
//    std::size_t n = timestamp_to_chars(buffer, timestamp, dtf::flags::secs);
//    std::cout << "buffer = ";
//    for (char c:buffer){
//        std::cout <<c;
//    }
//    std::cout << std::endl;
//    std::string new_string{buffer};
//    std::cout << "new_string = " << new_string << std::endl;
//    EXPECT_EQ(std::strcmp(buffer, "1234567890"), 0);
//    EXPECT_EQ(n, 10);
//
//    char buffer1[20];
//    n = timestamp_to_chars(buffer1, timestamp, dtf::flags::msecs);
//    EXPECT_EQ(std::strcmp(buffer1, "1234567890000"), 0);
//    EXPECT_EQ(n, 13);
//
//    char buffer2[20];
//    n = timestamp_to_chars(buffer2, timestamp, dtf::flags::usecs);
//    std::cout << "buffer2 = ";
//    for (char c:buffer2){
//        std::cout <<c;
//    }
//    std::cout << std::endl;
//    EXPECT_EQ(std::strcmp(buffer2, "1234567890000000"), 0);
//    EXPECT_EQ(n, 16);
//
//    // Add more test cases as needed
//}


// Test case for utoa function
TEST_F(DtfTest, TestUtoa) {
char buffer[11];
dtf::utoa(buffer, 10, 1234567890);
buffer[10] = 0;
EXPECT_EQ(std::strcmp(buffer, "1234567890"), 0);

char buffer1[2];
dtf::utoa(buffer1, 1, 0);
buffer1[1] = 0;
EXPECT_EQ(std::strcmp(buffer1, "0"), 0);

char buffer2[11];
dtf::utoa(buffer2, 10, 9876543210);
buffer2[10] = 0;
EXPECT_EQ(std::strcmp(buffer2, "9876543210"), 0);

// Add more test cases as needed
}

// Test cases
TEST_F(DtfTest, TestSmallNumbers) {
EXPECT_EQ(dtf::num_chars(0), 1);
EXPECT_EQ(dtf::num_chars(9), 1);
EXPECT_EQ(dtf::num_chars(10), 2);
EXPECT_EQ(dtf::num_chars(99), 2);
EXPECT_EQ(dtf::num_chars(100), 3);
EXPECT_EQ(dtf::num_chars(999), 3);
EXPECT_EQ(dtf::num_chars(1000), 4);
// ... add more test cases for small numbers
}

TEST_F(DtfTest, TestLargeNumbers) {
EXPECT_EQ(dtf::num_chars(100000000000000000ull), 18);
EXPECT_EQ(dtf::num_chars(999999999999999999ull), 18);
EXPECT_EQ(dtf::num_chars(1000000000000000000ull), 19);
// ... add more test cases for large numbers
}


// 测试用例
TEST_F(DtfTest, PositiveOffset) {
std::uint64_t before = dtf::timestamp(0);
std::uint64_t after = dtf::timestamp(1);
EXPECT_GT(after, before);
}

TEST_F(DtfTest, NegativeOffset) {
std::uint64_t before = dtf::timestamp(0);
std::uint64_t after = dtf::timestamp(-1);
EXPECT_LT(after, before);
}

TEST_F(DtfTest, ZeroOffset) {
std::uint64_t ts = dtf::timestamp(0);
EXPECT_NE(ts, 0);
}

//TEST_F(DtfTest, timestamptodt) {
//    todo
//    std::uint64_t ts = dtf::timestamp(0);
//    std::cout << "ts = " << ts << std::endl;
//    std::size_t diff = 8*3600;
//    std::string dt = dtf::timestamp_to_dt_str(ts+diff*1000000000, dtf::flags::yyyy_mm_dd | dtf::flags::usecs | dtf::flags::sep1);
//    std::cout << "dt = " << dt << std::endl;
//    std::uint64_t new_ts = dtf::dt_to_timestamp(dt, dtf::flags::nsecs);
//    std::cout << "new_ts = "<< new_ts << std::endl;
//    EXPECT_EQ(ts, new_ts);
//    EXPECT_NE(ts, 0);
//}

TEST_F(DtfTest, DateRange){
    std::vector<std::chrono::system_clock::time_point> datetime_list =
            dtf::date_range("2000-01-30", 1000, "D", true);

    // Print the datetime list
//    for (const auto& dt : datetime_list) {
//        std::time_t time = std::chrono::system_clock::to_time_t(dt);
//        std::cout << std::ctime(&time);
//    }

}

TEST_F(DtfTest, timePointToString){
    // 获取当前时间点（包含毫秒）
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    // 转换为字符串格式（包含毫秒）
    std::string strTime = dtf::timePointToString(now);

    // 输出转换后的时间
    std::cout << "Current time (with milliseconds): " << strTime << std::endl;
}

TEST_F(DtfTest, timestamp_to_datetime){
    std::uint64_t current_time_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();

    datetime_t now_datetime = dtf::timestamp_to_datetime(current_time_ns);

    std::cout << "Current Date and Time:" << std::endl;
    std::cout << "Year: " << now_datetime.year << std::endl;
    std::cout << "Month: " << now_datetime.month << std::endl;
    std::cout << "Day: " << now_datetime.day << std::endl;
    std::cout << "Weekday: " << now_datetime.week << std::endl;
    std::cout << "Hour: " << now_datetime.hour << std::endl;
    std::cout << "Minute: " << now_datetime.minute << std::endl;
    std::cout << "Second: " << now_datetime.seconds << std::endl;
    std::cout << "Millisecond: " << now_datetime.milliseconds << std::endl;
    std::cout << "Microsecond: " << now_datetime.microseconds << std::endl;
    std::cout << "Nanosecond: " << now_datetime.nanoseconds << std::endl;
}