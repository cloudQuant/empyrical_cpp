#include "gtest/gtest.h"
#include "../include/empyrical/utils.h"


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