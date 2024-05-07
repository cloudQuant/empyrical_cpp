#include "gtest/gtest.h"
#include "../include/empyrical/datetime.h"  // 包含要测试的头文件
#include <ctime>

TEST(DateTimeTest, StringConstructor) {
    DateTime dt("2024-04-13 10:00:00.123456789");
    EXPECT_EQ(dt.get_year(), 2024);
    EXPECT_EQ(dt.get_month(), 4);
    EXPECT_EQ(dt.get_day(), 13);
    EXPECT_EQ(dt.get_hour(), 10);
    EXPECT_EQ(dt.get_week(), 6);
    // Add more EXPECT_EQ statements to validate the datetime components
}



TEST(DateTimeTest, StringConstructor_1) {
    DateTime dt("2024-04-14 10:00:00.123456789");
    EXPECT_EQ(dt.get_year(), 2024);
    EXPECT_EQ(dt.get_month(), 4);
    EXPECT_EQ(dt.get_day(), 14);
    EXPECT_EQ(dt.get_hour(), 10);
    EXPECT_EQ(dt.get_week(), 0);// 0代表星期日
    // Add more EXPECT_EQ statements to validate the datetime components
}

TEST(DateTimeTest, TimestampConstructor) {
    std::uint64_t timestamp = 1691670000123456789; // Example timestamp
    DateTime dt(timestamp);
    EXPECT_EQ(dt.get_timestamp(), timestamp);
    // Add more EXPECT_EQ statements to validate other datetime components
}

TEST(DateTimeTest, DateTimeConversion) {
    datetime_t dt_struct{false, 2024, 4, 13, 4, 10, 00, 00, 123, 456, 789};
    DateTime dt(dt_struct);
    EXPECT_EQ(dt.get_year(), 2024);
    EXPECT_EQ(dt.get_month(), 4);
    EXPECT_EQ(dt.get_day(), 13);
    EXPECT_EQ(dt.get_week(), 4);
    EXPECT_EQ(dt.get_hour(), 10);
    EXPECT_EQ(dt.get_nanoseconds(),789);
    EXPECT_EQ(dt.get_timestamp(),1712973600123456789);
    // Add more EXPECT_EQ statements to validate other datetime components
}

TEST(DateTimeTest, TimestampToDatetime) {
    DateTime dt(1618036800000000000);
    //std::uint64_t timestamp = 1618036800000000000;  // 2021-04-10 14:40:00 in nanoseconds
    datetime_t dt_expected{false, 2021, 4, 10, 6, 14, 40, 0, 0, 0, 0};  // Assuming it's Saturday (week = 6)
    //datetime_t dt_actual = DateTime::timestamp_to_datetime(timestamp);
    datetime_t dt_actual = dt.get_datetime();

    EXPECT_EQ(dt_expected.year, dt_actual.year);
    EXPECT_EQ(dt_expected.month, dt_actual.month);
    EXPECT_EQ(dt_expected.day, dt_actual.day);
    EXPECT_EQ(dt_expected.week, dt_actual.week);
    EXPECT_EQ(dt_expected.hour, dt_actual.hour);
    EXPECT_EQ(dt_expected.minute, dt_actual.minute);
    EXPECT_EQ(dt_expected.seconds, dt_actual.seconds);
    EXPECT_EQ(dt_expected.milliseconds, dt_actual.milliseconds);
    EXPECT_EQ(dt_expected.microseconds, dt_actual.microseconds);
    EXPECT_EQ(dt_expected.nanoseconds, dt_actual.nanoseconds);
}

TEST(DateTimeTest, DatetimeToString) {
    datetime_t dt_input{false, 2021, 4, 10, 6, 0, 0, 0, 0, 0, 0};  // Assuming it's Saturday (week = 6)
    DateTime dt(dt_input);
    //std::uint64_t timestamp = 1618036800000000000;
    std::string dt_str = DateTime::datetime_to_string(dt_input, flags::nsecs);
    std::string dt_expected = "2021-04-10 00:00:00.000000000";
    EXPECT_EQ(dt_expected, dt_str);
}

TEST(DateTimeTest, StringToDatetime) {
    DateTime dt("2021-04-10 06:00:00.000000000");
    std::string dt_str = "2021-04-10 06:00:00.000000000";
    datetime_t dt_expected{false, 2021, 4, 10, 6, 6, 0, 0, 0, 0, 0};  // Assuming it's Saturday (week = 6)
    datetime_t dt_actual = DateTime::string_to_datetime(dt_str, flags::nsecs);

    EXPECT_EQ(dt_expected.year, dt_actual.year);
    EXPECT_EQ(dt_expected.month, dt_actual.month);
    EXPECT_EQ(dt_expected.day, dt_actual.day);
    EXPECT_EQ(dt_expected.week, dt_actual.week);
    EXPECT_EQ(dt_expected.hour, dt_actual.hour);
    EXPECT_EQ(dt_expected.minute, dt_actual.minute);
    EXPECT_EQ(dt_expected.seconds, dt_actual.seconds);
    EXPECT_EQ(dt_expected.milliseconds, dt_actual.milliseconds);
    EXPECT_EQ(dt_expected.microseconds, dt_actual.microseconds);
    EXPECT_EQ(dt_expected.nanoseconds, dt_actual.nanoseconds);
}

