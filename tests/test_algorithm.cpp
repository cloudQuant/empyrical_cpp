#include "gtest/gtest.h"
#include <algorithm>
#include "../include/empyrical/algorithm.h"
#include "../include/empyrical/utils.h"
#include "../include/empyrical/datetime.h"  // 包含要测试的头文件
class QSortDTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize test data
        input1 = {3.0, 1.0, 4.0, 1.5, 2.0};
        input2 = {5.0, 3.0, 8.0, 1.0, 7.0, 2.0, 6.0, 4.0};
        input3 = {1.0, 2.0, 3.0, 4.0, 5.0};
        input4 = {5.0, 4.0, 3.0, 2.0, 1.0};
        input5 = {};
    }

    std::vector<double> input1;
    std::vector<double> input2;
    std::vector<double> input3;
    std::vector<double> input4;
    std::vector<double> input5;
};

// Test cases
TEST_F(QSortDTest, SortsArray) {

    quick_sorted(0, static_cast<std::int64_t>(input1.size() - 1), input1.data());
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end()));


    quick_sorted(0, static_cast<std::int64_t>(input2.size() - 1), input2.data());
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end()));

    quick_sorted(0, static_cast<std::int64_t>(input3.size() - 1), input3.data());
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end()));

    quick_sorted(0, static_cast<std::int64_t>(input4.size() - 1), input4.data());
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end()));

    quick_sorted(0, static_cast<std::int64_t>(input5.size() - 1), input5.data());
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end()));
    }
}

TEST_F(QSortDTest, SortsVector) {

    quick_sorted(input1);
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end()));


    quick_sorted(input2);
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end()));

    quick_sorted(input3);
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end()));

    quick_sorted(input4);
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end()));

    quick_sorted(input5);
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end()));
    }
}

TEST_F(QSortDTest, bubble_sort_vector) {

    bubble_sort(input1);
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end()));


    bubble_sort(input2);
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end()));

    bubble_sort(input3);
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end()));

    bubble_sort(input4);
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end()));

    bubble_sort(input5);
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end()));
    }
}

TEST_F(QSortDTest, bubble_sort_array) {
    double *arr1 = input1.data();
    bubble_sort(arr1, input1.size());
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end()));


    double *arr2 = input2.data();
    bubble_sort(arr2, input2.size());
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end()));

    double *arr3 = input3.data();
    bubble_sort(arr3, input3.size());
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end()));

    double *arr4 = input4.data();
    bubble_sort(arr4, input4.size());
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end()));

    double *arr5 = input5.data();
    bubble_sort(arr5, input5.size());
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end()));
    }
}

TEST_F(QSortDTest, bubble_sort_vector_reverse) {

    bubble_sort(input1,true);
    //cal_func::print_vector(input1);
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end(), std::greater<double>()));
    EXPECT_GE(input1[0],input1[input1.size()-1]);

    bubble_sort(input2,true);
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end(), std::greater<double>()));
    EXPECT_GE(input2[0],input2[input2.size()-1]);

    bubble_sort(input3,true);
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end(), std::greater<double>()));
    EXPECT_GE(input3[0],input3[input3.size()-1]);

    bubble_sort(input4,true);
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end(), std::greater<double>()));
    EXPECT_GE(input4[0],input4[input4.size()-1]);

    bubble_sort(input5,true);
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end(), std::greater<double>()));
        EXPECT_GE(input4[0],input4[input4.size()-1]);
    }
}

TEST_F(QSortDTest, bubble_sort_array_reverse) {
    double *arr1 = input1.data();
    bubble_sort(arr1, input1.size(),true);
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end(), std::greater<double>()));
    EXPECT_GE(input1[0],input1[input1.size()-1]);


    double *arr2 = input2.data();
    bubble_sort(arr2, input2.size(),true);
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end(), std::greater<double>()));
    EXPECT_GE(input2[0],input2[input2.size()-1]);

    double *arr3 = input3.data();
    bubble_sort(arr3, input3.size(),true);
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end(), std::greater<double>()));
    EXPECT_GE(input3[0],input3[input3.size()-1]);

    double *arr4 = input4.data();
    bubble_sort(arr4, input4.size(),true);
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end(), std::greater<double>()));
    EXPECT_GE(input4[0],input4[input4.size()-1]);

    double *arr5 = input5.data();
    bubble_sort(arr5, input5.size(),true);
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end(), std::greater<double>()));
        EXPECT_GE(input5[0],input5[input5.size()-1]);
    }
}

TEST_F(QSortDTest, selection_sort_vector) {

    selection_sort(input1);
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end()));


    selection_sort(input2);
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end()));

    selection_sort(input3);
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end()));

    selection_sort(input4);
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end()));
    //std::cout << "run 1 success" << std::endl;

    selection_sort(input5);
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end()));
    }
}

TEST_F(QSortDTest, selection_sort_array) {
    double *arr1 = input1.data();
    selection_sort(arr1, input1.size());
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end()));


    double *arr2 = input2.data();
    selection_sort(arr2, input2.size());
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end()));

    double *arr3 = input3.data();
    selection_sort(arr3, input3.size());
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end()));

    double *arr4 = input4.data();
    selection_sort(arr4, input4.size());
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end()));

    double *arr5 = input5.data();
    selection_sort(arr5, input5.size());
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end()));
    }
}

TEST_F(QSortDTest, selection_sort_vector_reverse) {

    selection_sort(input1,true);
    //cal_func::print_vector(input1);
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end(), std::greater<double>()));
    EXPECT_GE(input1[0],input1[input1.size()-1]);

    selection_sort(input2,true);
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end(), std::greater<double>()));
    EXPECT_GE(input2[0],input2[input2.size()-1]);

    selection_sort(input3,true);
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end(), std::greater<double>()));
    EXPECT_GE(input3[0],input3[input3.size()-1]);

    selection_sort(input4,true);
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end(), std::greater<double>()));
    EXPECT_GE(input4[0],input4[input4.size()-1]);

    selection_sort(input5,true);
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end(), std::greater<double>()));
        EXPECT_GE(input4[0],input4[input4.size()-1]);
    }
}

TEST_F(QSortDTest, selection_sort_array_reverse) {
    double *arr1 = input1.data();
    selection_sort(arr1, input1.size(),true);
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end(), std::greater<double>()));
    EXPECT_GE(input1[0],input1[input1.size()-1]);


    double *arr2 = input2.data();
    selection_sort(arr2, input2.size(),true);
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end(), std::greater<double>()));
    EXPECT_GE(input2[0],input2[input2.size()-1]);

    double *arr3 = input3.data();
    selection_sort(arr3, input3.size(),true);
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end(), std::greater<double>()));
    EXPECT_GE(input3[0],input3[input3.size()-1]);

    double *arr4 = input4.data();
    selection_sort(arr4, input4.size(),true);
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end(), std::greater<double>()));
    EXPECT_GE(input4[0],input4[input4.size()-1]);

    double *arr5 = input5.data();
    selection_sort(arr5, input5.size(),true);
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end(), std::greater<double>()));
        EXPECT_GE(input5[0],input5[input5.size()-1]);
    }
}

TEST_F(QSortDTest, insertion_sort_vector) {

    insertion_sort(input1);
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end()));


    insertion_sort(input2);
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end()));

    insertion_sort(input3);
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end()));

    insertion_sort(input4);
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end()));
    //std::cout << "run 1 success" << std::endl;

    insertion_sort(input5);
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end()));
    }
}

TEST_F(QSortDTest, insertion_sort_array) {
    double *arr1 = input1.data();
    insertion_sort(arr1, input1.size());
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end()));


    double *arr2 = input2.data();
    insertion_sort(arr2, input2.size());
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end()));

    double *arr3 = input3.data();
    insertion_sort(arr3, input3.size());
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end()));

    double *arr4 = input4.data();
    insertion_sort(arr4, input4.size());
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end()));

    double *arr5 = input5.data();
    insertion_sort(arr5, input5.size());
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end()));
    }
}

TEST_F(QSortDTest, insertion_sort_vector_reverse) {

    insertion_sort(input1,true);
    //cal_func::print_vector(input1);
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end(), std::greater<double>()));
    EXPECT_GE(input1[0],input1[input1.size()-1]);

    insertion_sort(input2,true);
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end(), std::greater<double>()));
    EXPECT_GE(input2[0],input2[input2.size()-1]);

    insertion_sort(input3,true);
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end(), std::greater<double>()));
    EXPECT_GE(input3[0],input3[input3.size()-1]);

    insertion_sort(input4,true);
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end(), std::greater<double>()));
    EXPECT_GE(input4[0],input4[input4.size()-1]);

    insertion_sort(input5,true);
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end(), std::greater<double>()));
        EXPECT_GE(input4[0],input4[input4.size()-1]);
    }
}

TEST_F(QSortDTest, insertion_sort_array_reverse) {
    double *arr1 = input1.data();
    insertion_sort(arr1, input1.size(),true);
    EXPECT_TRUE(std::is_sorted(input1.begin(), input1.end(), std::greater<double>()));
    EXPECT_GE(input1[0],input1[input1.size()-1]);


    double *arr2 = input2.data();
    insertion_sort(arr2, input2.size(),true);
    EXPECT_TRUE(std::is_sorted(input2.begin(), input2.end(), std::greater<double>()));
    EXPECT_GE(input2[0],input2[input2.size()-1]);

    double *arr3 = input3.data();
    insertion_sort(arr3, input3.size(),true);
    EXPECT_TRUE(std::is_sorted(input3.begin(), input3.end(), std::greater<double>()));
    EXPECT_GE(input3[0],input3[input3.size()-1]);

    double *arr4 = input4.data();
    insertion_sort(arr4, input4.size(),true);
    EXPECT_TRUE(std::is_sorted(input4.begin(), input4.end(), std::greater<double>()));
    EXPECT_GE(input4[0],input4[input4.size()-1]);

    double *arr5 = input5.data();
    insertion_sort(arr5, input5.size(),true);
    if (!input5.empty()){
        EXPECT_TRUE(std::is_sorted(input5.begin(), input5.end(), std::greater<double>()));
        EXPECT_GE(input5[0],input5[input5.size()-1]);
    }
}
