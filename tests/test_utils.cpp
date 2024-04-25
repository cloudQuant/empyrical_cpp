#include "gtest/gtest.h"
#include "../include/empyrical/utils.hpp"

TEST_F(){
    std::string input_dt = "2024-04-10 12:00:00";  // 输入的时间字符串
    std::string utc_dt = get_utc_timestamp(input_dt);
    std::cout << "UTC Time: " << utc_dt << std::endl;
}