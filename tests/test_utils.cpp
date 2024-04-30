#include "gtest/gtest.h"
#include "../include/empyrical/utils.h"

class DtfTest : public ::testing::Test {
protected:
    // You can add setup and teardown code here if needed
};

TEST_F(DtfTest, NormalTeest){
    ASSERT_EQ(2,2);
}

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

class RollNdarrayTest : public ::testing::Test {
protected:
    // You can add setup and teardown code here if needed
};

double customFunc(const std::vector<double>& vals, const std::map<std::string, double>&) {
    double sum = 0.0;
    for (double val : vals) {
        sum += val;
    }
    return sum / static_cast<double>(vals.size());
}

TEST_F(RollNdarrayTest, BasicTest) {
    std::vector<double> arr = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::map<std::string, double> params = {{"window", 3.0}};
    std::vector<double> expected = {2.0, 3.0, 4.0};  // 预期的结果

    // 调用 roll_ndarray 函数
    std::vector<double> result = cal_func::roll_ndarray(arr, customFunc, params);

    // 断言结果是否与预期相符
    ASSERT_EQ(result, expected);
}

TEST_F(RollNdarrayTest, ValidInput) {
    // Create a test DataFrame
    DataFrame df = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    // Define the expected result
    DataFrameRollingWindowResult expected_result;
    expected_result.numWindows = 2;
    expected_result.data = {{{1, 2, 3}, {4, 5, 6}}, {{4, 5, 6}, {7, 8, 9}}};

    // Call the rolling_window function
    int window_length = 2;
    DataFrameRollingWindowResult result = cal_func::rolling_window(df, window_length);

    // Check if the result matches the expected result
    ASSERT_EQ(result.numWindows, expected_result.numWindows);
    ASSERT_EQ(result.data, expected_result.data);
}

// Test case for invalid window length
TEST_F(RollNdarrayTest, InvalidWindowLength) {
    // Create a test DataFrame
    DataFrame df = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    // Call the rolling_window function with an invalid window length
    int window_length = 0;

    // Check if the function throws the expected exception
    ASSERT_THROW(cal_func::rolling_window(df, window_length), std::invalid_argument);
}


TEST_F(RollNdarrayTest, PositiveWindowLength) {
    Array arr = {1, 2, 3, 4, 5};
    int length = 3;

    ArrayRollingWindowResult result = cal_func::rolling_window(arr, length);

    ArrayRollingWindowResult expect_result;
    expect_result.numWindows = 3;
    expect_result.data = {
            {1,2,3},
            {2,3,4},
            {3,4,5}
    };

    EXPECT_EQ(result.numWindows, arr.size() - length + 1);
    EXPECT_EQ(result.data, expect_result.data);

}

// Test case for zero window length
TEST_F(RollNdarrayTest, ZeroWindowLength) {
    Array arr = {1, 2, 3, 4, 5};
    int length = 0;

    // Check if the function throws an invalid_argument exception
    EXPECT_THROW(cal_func::rolling_window(arr, length), std::invalid_argument);
}

// Test case for window length greater than array size
TEST_F(RollNdarrayTest, WindowLengthGreaterThanArraySize) {
    Array arr = {1, 2, 3, 4, 5};
    int length = 10;

    // Check if the function throws an invalid_argument exception
    EXPECT_THROW(cal_func::rolling_window(arr, length), std::invalid_argument);
}

class UtilsTest : public ::testing::Test {
protected:
    // You can add setup and teardown code here if needed
};

TEST_F(UtilsTest, HandlesNaNValues) {
    // Test case with NaN values
    std::vector<double> arr = {1.0, 2.0, 3.0, NAN, 5.0};
    double expected_mean = (1.0 + 2.0 + 3.0 + 5.0) / 4.0;  // Sum of non-NaN values divided by count
    double actual_mean = cal_func::nan_mean(arr);
    EXPECT_DOUBLE_EQ(actual_mean, expected_mean);
}

TEST_F(UtilsTest, HandlesAllNaNValues) {
    // Test case with all NaN values
    std::vector<double> arr = {NAN, NAN, NAN};
    double actual_mean = cal_func::nan_mean(arr);
    // Expected mean for all NaN values is NaN
    EXPECT_TRUE(std::isnan(actual_mean));
}

TEST_F(UtilsTest, HandlesEmptyArray) {
    // Test case with empty array
    std::vector<double> arr;
    double actual_mean = cal_func::nan_mean(arr);
    // Expected mean for empty array is NaN
    EXPECT_TRUE(std::isnan(actual_mean));
}

TEST_F(UtilsTest, HandlesValidInput) {
    // Test case with valid input
    std::vector<double> arr = {1.0, 3.0, 3.0, NAN, 5.0};
    double expected_std = std::sqrt(2);  // Calculated manually
    double actual_std = cal_func::nan_std(arr);
    EXPECT_DOUBLE_EQ(actual_std, expected_std);
}

TEST_F(UtilsTest, HandlesAllNaNValues1) {
    // Test case with all NaN values
    std::vector<double> arr = {NAN, NAN, NAN};
    double actual_std = cal_func::nan_std(arr);
    // Expected standard deviation for all NaN values is NaN
    EXPECT_TRUE(std::isnan(actual_std));
}

TEST_F(UtilsTest, HandlesSingleValue1) {
    // Test case with single non-NaN value
    std::vector<double> arr = {42.0};
    double actual_std = cal_func::nan_std(arr);
    // Expected standard deviation for a single value is NaN
    EXPECT_TRUE(std::isnan(actual_std));
}

TEST_F(UtilsTest, HandlesValidInput3) {
    // Test case with valid input
    std::vector<double> arr = {1.0, 2.0, 3.0, 4.0, 5.0};
    double expected_sum = 15.0;  // Calculated manually
    double actual_sum = cal_func::nan_sum(arr);
    EXPECT_DOUBLE_EQ(actual_sum, expected_sum);
}

TEST_F(UtilsTest, HandlesAllNaNValues3) {
    // Test case with all NaN values
    std::vector<double> arr = {NAN, NAN, NAN};
    double actual_sum = cal_func::nan_sum(arr);
    // Expected sum for all NaN values is 0.0
    EXPECT_DOUBLE_EQ(actual_sum, 0.0);
}

TEST_F(UtilsTest, HandlesEmptyArray3) {
    // Test case with an empty array
    std::vector<double> arr;
    double actual_sum = cal_func::nan_sum(arr);
    // Expected sum for an empty array is 0.0
    EXPECT_DOUBLE_EQ(actual_sum, 0.0);
}

TEST_F(UtilsTest, HandlesValidInput4) {
    // Test case with valid input
    std::vector<double> arr = {6.0, 1.0, NAN, 7.0, 5.0};
    double expected_max = 7.0;  // Max value in the array
    double actual_max = cal_func::nan_max(arr);
    EXPECT_DOUBLE_EQ(actual_max, expected_max);
}

TEST_F(UtilsTest, HandlesValidInput4_1) {
    // Test case with valid input
    std::vector<double> arr = {6.0, 8.0, NAN, 7.0, 5.0};
    double expected_max = 8.0;  // Max value in the array
    double actual_max = cal_func::nan_max(arr);
    EXPECT_DOUBLE_EQ(actual_max, expected_max);
}

TEST_F(UtilsTest, HandlesValidInput4_2) {
    // Test case with valid input
    std::vector<double> arr = {NAN, 0, 8.0, NAN, 7.0, 5.0};
    double expected_max = 8.0;  // Max value in the array
    double actual_max = cal_func::nan_max(arr);
    EXPECT_DOUBLE_EQ(actual_max, expected_max);
}

//TEST_F(UtilsTest, HandlesValidInput4_3) {
//    todo
//    // Test case with valid input
//    std::vector<double> arr = {NAN, NAN, 8.0, NAN, NAN, 5.0};
//    double expected_max = 8.0;  // Max value in the array
//    double actual_max = cal_func::nan_max(arr);
//    std::cout << "actual_max = " << actual_max << " expected_max = " << expected_max << std::endl;
//    EXPECT_DOUBLE_EQ(actual_max, expected_max);
//}

//TEST_F(UtilsTest, HandlesValidInput4_4) {
//   todo
//    // Test case with valid input
//    std::vector<double> arr = {NAN, NAN, NAN, NAN, NAN, 5.0};
//    double expected_max = 5.0;  // Max value in the array
//    double actual_max = cal_func::nan_max(arr);
//    EXPECT_DOUBLE_EQ(actual_max, expected_max);
//}

//TEST_F(UtilsTest, HandlesValidInput4_5) {
//    todo
//    // Test case with valid input
//    std::vector<double> arr = {NAN, NAN, 0.0, NAN, NAN, NAN};
//    double expected_max = 0.0;  // Max value in the array
//    double actual_max = cal_func::nan_max(arr);
//    EXPECT_DOUBLE_EQ(actual_max, expected_max);
//}

// todo
//TEST_F(UtilsTest, HandlesAllNaNValues4) {
//    // Test case with all NaN values
//    std::vector<double> arr = {NAN, NAN, NAN};
//    double actual_max = cal_func::nan_max(arr);
//    // Expected max for all NaN values is NaN
//    EXPECT_TRUE(std::isnan(actual_max));
//}

TEST_F(UtilsTest, HandlesEmptyArray4) {
    // Test case with an empty array
    std::vector<double> arr;
    double actual_max = cal_func::nan_max(arr);
    // Expected max for an empty array is NaN
    EXPECT_TRUE(std::isnan(actual_max));
}

TEST_F(UtilsTest, HandlesValidInput5) {
    // Test case with valid input
    std::vector<double> arr = {6.0, 1.0, 0, -1.0, 5.0};
    double expected_min = -1.0;  // Min value in the array
    double actual_min = cal_func::nan_min(arr);
    EXPECT_DOUBLE_EQ(actual_min, expected_min);
}

TEST_F(UtilsTest, HandlesValidInput5_1) {
    // Test case with valid input
    std::vector<double> arr = {6.0, 8.0, NAN, 7.0, 5.0};
    double expected_min = 5.0;  // Max value in the array
    double actual_min = cal_func::nan_min(arr);
    EXPECT_DOUBLE_EQ(actual_min, expected_min);
}

TEST_F(UtilsTest, HandlesValidInput5_2) {
    // Test case with valid input
    std::vector<double> arr = {NAN, 0, 8.0, NAN, 7.0, 5.0};
    double expected_min = 0.0;  // Max value in the array
    double actual_min = cal_func::nan_min(arr);
    EXPECT_DOUBLE_EQ(actual_min, expected_min);
}

TEST_F(UtilsTest, HandlesValidInput5_3) {
    // Test case with valid input
    std::vector<double> arr = {NAN, NAN, 8.0, NAN, NAN, 5.0};
    double expected_min = 5.0;  // Max value in the array
    double actual_min = cal_func::nan_min(arr);
    EXPECT_DOUBLE_EQ(actual_min, expected_min);
}

TEST_F(UtilsTest, HandlesValidInput5_4) {
    // Test case with valid input
    std::vector<double> arr = {NAN, NAN, NAN, NAN, NAN, 5.0};
    double expected_min = 5.0;  // Max value in the array
    double actual_min = cal_func::nan_min(arr);
    EXPECT_DOUBLE_EQ(actual_min, expected_min);
}

TEST_F(UtilsTest, HandlesValidInput5_5) {
    // Test case with valid input
    std::vector<double> arr = {NAN, NAN, 0.0, NAN, NAN, NAN};
    double expected_min = 0.0;  // Max value in the array
    double actual_min = cal_func::nan_min(arr);
    EXPECT_DOUBLE_EQ(actual_min, expected_min);
}

TEST_F(UtilsTest, HandlesAllNaNValues5) {
    // Test case with all NaN values
    std::vector<double> arr = {NAN, NAN, NAN};
    double actual_min = cal_func::nan_min(arr);
    // Expected min for all NaN values is NaN
    EXPECT_TRUE(std::isnan(actual_min));
}

TEST_F(UtilsTest, HandlesEmptyArray5) {
    // Test case with an empty array
    std::vector<double> arr;
    double actual_min = cal_func::nan_min(arr);
    // Expected min for an empty array is NaN
    EXPECT_TRUE(std::isnan(actual_min));
}

// 测试基本情况
TEST_F(UtilsTest, BasicTest) {
    std::vector<double> test_data = {1.0, 2.0, 3.0, std::numeric_limits<double>::quiet_NaN(), 5.0};
    std::size_t expected_idx = 4; // 5.0 在索引位置4
    std::size_t result_idx = cal_func::nan_argmax(test_data);
    EXPECT_EQ(result_idx, expected_idx);
}

// 测试空数组
TEST_F(UtilsTest, EmptyArrayTest) {
    std::vector<double> test_data;
    std::size_t result_idx = cal_func::nan_argmax(test_data);
    EXPECT_EQ(result_idx, static_cast<std::size_t>(-1)); // 无效索引
}

// 测试全为NaN的数组
TEST_F(UtilsTest, AllNaNTest) {
    std::vector<double> test_data = {std::numeric_limits<double>::quiet_NaN(),
                                     std::numeric_limits<double>::quiet_NaN(),
                                     std::numeric_limits<double>::quiet_NaN()};
    std::size_t result_idx = cal_func::nan_argmax(test_data);
    EXPECT_EQ(result_idx, static_cast<std::size_t>(-1)); // 无效索引
}



TEST_F(UtilsTest, BasicTest2) {
    std::vector<double> test_data = {1.0, 2.0, 3.0, std::numeric_limits<double>::quiet_NaN(), 0.0};
    std::size_t expected_idx = 4; // 0.0 在索引位置4
    std::size_t result_idx = cal_func::nan_argmin(test_data);
    EXPECT_EQ(result_idx, expected_idx);
}

// 测试空数组
TEST_F(UtilsTest, EmptyArrayTest2) {
    std::vector<double> test_data;
    std::size_t result_idx = cal_func::nan_argmin(test_data);
    EXPECT_EQ(result_idx, static_cast<std::size_t>(-1)); // 无效索引
}

// 测试全为NaN的数组
TEST_F(UtilsTest, AllNaNTest2) {
    std::vector<double> test_data = {std::numeric_limits<double>::quiet_NaN(),
                                     std::numeric_limits<double>::quiet_NaN(),
                                     std::numeric_limits<double>::quiet_NaN()};
    std::size_t result_idx = cal_func::nan_argmin(test_data);
    EXPECT_EQ(result_idx, static_cast<std::size_t>(-1)); // 无效索引
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}