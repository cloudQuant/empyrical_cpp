#include "gtest/gtest.h"
#include "../include/empyrical/stats.h"

/*
 * 1. don't test omega_ratio as the original test because fix the bug of omega ratio
 * 2. sharpe_ratio don't test risk-free rate's value is an array
 * 3. don't test the dataframe data's downside_risk
 */

class OriginalStatsTest : public ::testing::Test {
protected:
    // You can add setup and teardown code here if needed
    std::vector<std::string> simple_benchmark_index = {"2000-01-30",
                                                       "2000-01-31",
                                                       "2000-02-01",
                                                       "2000-02-02",
                                                        "2000-02-03",
                                                        "2000-02-04",
                                                        "2000-02-05",
                                                        "2000-02-06",
                                                        "2000-02-07"};
    std::vector<double> simple_benchmark_value = {0.  ,
                                                  0.01,
                                                  0.  ,
                                                  0.01,
                                                  0.  ,
                                                  0.01,
                                                  0.  ,
                                                  0.01,
                                                  0.};
    std::vector<std::string> positive_returns_index = {
            "2000-01-30", "2000-01-31", "2000-02-01", "2000-02-02",
            "2000-02-03", "2000-02-04", "2000-02-05", "2000-02-06",
            "2000-02-07"
    };
    std::vector<double> positive_returns_value = {0.01, 0.02, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};

    std::vector<std::string> negative_returns_index = {
            "2000-01-30", "2000-01-31", "2000-02-01", "2000-02-02",
            "2000-02-03", "2000-02-04", "2000-02-05", "2000-02-06",
            "2000-02-07"
    };
    std::vector<double> negative_returns_value = {0.  , -0.06, -0.07, -0.01, -0.09, -0.02, -0.06, -0.08, -0.05};

    std::vector<std::string> all_negative_returns_index = {
            "2000-01-30", "2000-01-31", "2000-02-01", "2000-02-02",
            "2000-02-03", "2000-02-04", "2000-02-05", "2000-02-06",
            "2000-02-07"
    };
    std::vector<double> all_negative_returns_value = {-0.02, -0.06, -0.07, -0.01, -0.09, -0.02, -0.06, -0.08, -0.05};

    std::vector<std::string> mixed_returns_index = {
            "2000-01-30", "2000-01-31", "2000-02-01", "2000-02-02",
            "2000-02-03", "2000-02-04", "2000-02-05", "2000-02-06",
            "2000-02-07"
    };
    std::vector<double> mixed_returns_value = {NAN,  0.01,  0.1 , -0.04,  0.02,  0.03,  0.02,  0.01, -0.1};


    std::vector<std::string> flat_line_1_index = {
            "2000-01-30", "2000-01-31", "2000-02-01", "2000-02-02",
            "2000-02-03", "2000-02-04", "2000-02-05", "2000-02-06",
            "2000-02-07"
    };
    std::vector<double> flat_line_1_value = {0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};

    std::vector<std::string> weekly_returns_index = {
            "2000-01-30", "2000-02-06", "2000-02-13", "2000-02-20",
            "2000-02-27", "2000-03-05", "2000-03-12", "2000-03-19",
            "2000-03-26"
    };
    std::vector<double> weekly_returns_value = {0.  ,  0.01,  0.1 , -0.04,  0.02,  0.03,  0.02,  0.01, -0.1};

    std::vector<std::string> monthly_returns_index = {
            "2000-01-31", "2000-02-29", "2000-03-31", "2000-04-30",
            "2000-05-31", "2000-06-30", "2000-07-31", "2000-08-31",
            "2000-09-30"
    };
    std::vector<double> monthly_returns_value = {0.  ,  0.01,  0.1 , -0.04,  0.02,  0.03,  0.02,  0.01, -0.1};

    std::vector<std::string> one_returns_index = {
            "2000-01-30"
    };
    std::vector<double> one_returns_value = {0.01};

    std::vector<std::string> empty_returns_index = {};
    std::vector<double> empty_returns_value = {};

    std::vector<std::string> noise_index = dtf::date_range("2000-01-30",1000,"D",0);

    std::vector<double> noise_value = cal_func::generateRandomNormal(1000, 0, 0.001);

    std::vector<std::string> noise_uniform_index = dtf::date_range("2000-01-30",1000,"D",0);
    std::vector<double> noise_uniform_value = cal_func::generateUniformRandom(-0.01, 0.01, 1000);

    std::vector<std::string> inverse_noise_index{noise_uniform_index};
    std::vector<double> inverse_noise_value = cal_func::inverseVector(noise_value);

    std::vector<std::string> flat_line_0_index = dtf::date_range("2000-01-30",1000,"D",0);
    std::vector<double> flat_line_0_value = cal_func::generateLinspace(0,0,1000);

    std::vector<std::string> flat_line1_tz_index = dtf::date_range("2000-01-30",1000,"D",0);
    std::vector<double> flat_line1_tz_value = cal_func::generateLinspace(0.01,0.01,1000);

    std::vector<std::string> pos_line_index = dtf::date_range("2000-01-30",1000,"D",0);
    std::vector<double> pos_line_value = cal_func::generateLinspace(0,1,1000);

    std::vector<std::string> neg_line_index = dtf::date_range("2000-01-30",1000,"D",0);
    std::vector<double> neg_line_value = cal_func::generateLinspace(0.01,0.01,1000);

    std::vector<double> spase_noise_value = cal_func::replaceOneRandomValueWithNan(noise_value);

    std::vector<double> sparse_flat_line_1_value = cal_func::replaceOneRandomValueWithNan(flat_line_1_value);

    std::vector<double> one = {-0.00171614, 0.01322056, 0.03063862, -0.01422057, -0.00489779,
                               0.01268925, -0.03357711, 0.01797036};
    std::vector<double> two = {0.01846232, 0.00793951, -0.01448395, 0.00422537, -0.00339611,
                               0.03756813, 0.0151531, 0.03549769};

    std::vector<std::string> df_index_simple = {"2000-01-30", "2000-01-31", "2000-02-01", "2000-02-02",
                                                "2000-02-03", "2000-02-04", "2000-02-05", "2000-02-06"};
    std::vector<std::string> df_index_week = {"2000-01-30", "2000-02-06", "2000-02-13", "2000-02-20",
                                              "2000-02-27", "2000-03-05", "2000-03-12", "2000-03-19"};
    std::vector<std::string> df_index_month = {"2000-01-31", "2000-02-29", "2000-03-31", "2000-04-30",
                                               "2000-05-31", "2000-06-30", "2000-07-31", "2000-08-31"};

    DataFrame dataframe_simple = {one, two};
    std::vector<std::string> simple_col = {"one", "two"};
    PdDataFrame df_simple{df_index_simple, simple_col, dataframe_simple};
    PdDataFrame df_week{df_index_week, simple_col, dataframe_simple};
    PdDataFrame df_month{df_index_month, simple_col, dataframe_simple};

};

TEST_F(OriginalStatsTest, test_simple_returns_1){
    std::vector<double> now_simple_returns = simple_returns(flat_line_1_value);
    std::vector<double> expect_simple_returns(flat_line_1_value.size()-1, 0);
    ASSERT_EQ(now_simple_returns, expect_simple_returns);
}

TEST_F(OriginalStatsTest, test_simple_returns_2){
    std::vector<double> now_simple_returns = simple_returns(pos_line_value);
    std::vector<double> expect_simple_returns{};
    for (int i=1; i<999; ++i){
        expect_simple_returns.push_back(1.0/i);
    }
    now_simple_returns.erase(now_simple_returns.begin());
    std::cout << "now_simple_returns = " << now_simple_returns.size() << std::endl;
    std::cout << "expect_simple_returns = " << expect_simple_returns.size() << std::endl;
    for (std::size_t i=0; i<now_simple_returns.size(); ++i){
        ASSERT_NEAR(now_simple_returns[i], expect_simple_returns[i], 0.0001);
    }

}

TEST_F(OriginalStatsTest, test_cum_returns_1){
    std::vector<double> now_cum_return = cum_returns(empty_returns_value,0);
    ASSERT_EQ(now_cum_return.size(), 0);
}

TEST_F(OriginalStatsTest, test_cum_returns_2){
    std::vector<double> now_cum_return = cum_returns(mixed_returns_value, 0);
    std::vector<double> expect_values = {0.0, 0.01, 0.111, 0.066559, 0.08789, 0.12052, 0.14293, 0.15436, 0.03893};
    for (std::size_t i=0; i<now_cum_return.size(); ++i){
        //std::cout << "now_cum_return[i] = " << now_cum_return[i] << " expect_values[i] " << expect_values[i] << std::endl;
        ASSERT_NEAR(now_cum_return[i], expect_values[i], 0.0001);
    }
}

TEST_F(OriginalStatsTest, test_cum_returns_3){
    std::vector<double> now_cum_return = cum_returns(mixed_returns_value, 100);
    std::vector<double> expect_values = {100.0, 101.0, 111.1, 106.65599, 108.78912,
                                         112.05279, 114.29384, 115.43678, 103.89310};
    for (std::size_t i=0; i<now_cum_return.size(); ++i){
        std::cout << "now_cum_return[i] = " << now_cum_return[i] << " expect_values[i] " << expect_values[i] << std::endl;
        ASSERT_NEAR(now_cum_return[i], expect_values[i], 0.0001);
    }
}

TEST_F(OriginalStatsTest, test_cum_returns_4){
    std::vector<double> now_cum_return = cum_returns(negative_returns_value, 0);
    std::vector<double> expect_values = {0.0, -0.06, -0.1258, -0.13454, -0.21243,
                                         -0.22818, -0.27449, -0.33253, -0.36590};
    for (std::size_t i=0; i<now_cum_return.size(); ++i){
        std::cout << "now_cum_return[i] = " << now_cum_return[i] << " expect_values[i] " << expect_values[i] << std::endl;
        ASSERT_NEAR(now_cum_return[i], expect_values[i], 0.0001);
    }
}

TEST_F(OriginalStatsTest, test_cum_final_1){
    double final_value = cum_returns_final(empty_returns_value, 0);
    double expect_value = NAN;
    ASSERT_TRUE(std::isnan(final_value));
    ASSERT_TRUE(std::isnan(expect_value));
}

TEST_F(OriginalStatsTest, test_cum_final_2){
    double final_value = cum_returns_final(one_returns_value, 0);
    double expect_value = one_returns_value[0];
    ASSERT_NEAR(final_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_cum_final_3){
    double final_value = cum_returns_final(mixed_returns_value, 0);
    double expect_value = 0.03893;
    ASSERT_NEAR(final_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_cum_final_4){
    double final_value = cum_returns_final(mixed_returns_value, 100);
    double expect_value = 103.89310;
    ASSERT_NEAR(final_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_cum_final_5){
    double final_value = cum_returns_final(negative_returns_value, 0);
    double expect_value = -0.36590;
    ASSERT_NEAR(final_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_max_drawdown_1){
    double actual_value = max_drawdown_from_simple_return(empty_returns_value);
    //double expect_value = -0.36590;
    //ASSERT_NEAR(final_value, expect_value, 0.0001);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_max_drawdown_2){
    double actual_value = max_drawdown_from_simple_return(one_returns_value);
    double expect_value = 0;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_max_drawdown_3){
    double actual_value = max_drawdown_from_simple_return(simple_benchmark_value);
    double expect_value = 0;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_max_drawdown_4){
    double actual_value = max_drawdown_from_simple_return(mixed_returns_value);
    double expect_value = -0.1;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_max_drawdown_5){
    double actual_value = max_drawdown_from_simple_return(positive_returns_value);
    double expect_value = -0.0;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_max_drawdown_6){
    double actual_value = max_drawdown_from_simple_return(negative_returns_value);
    double expect_value = cum_returns_final(negative_returns_value, 0);
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_max_drawdown_7){
    double actual_value = max_drawdown_from_simple_return(all_negative_returns_value);
    double expect_value = cum_returns_final(all_negative_returns_value, 0);
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_max_drawdown_8){
    std::vector<double> special_returns = {0.1, -0.1, 0.1};
    double actual_value = max_drawdown_from_simple_return(special_returns);
    double expect_value = -0.1;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_max_drawdown_9){
    std::vector<double> depressed_returns(noise_value);
    for (double &v: depressed_returns){
        v-=0.0001;
    }

    std::vector<double> raised_returns(noise_value);
    for (double &v: raised_returns){
        v+=0.0001;
    }
    double max_dd = max_drawdown_from_simple_return(noise_value);
    double depressed_dd = max_drawdown_from_simple_return(depressed_returns);
    double raised_dd = max_drawdown_from_simple_return(raised_returns);
    ASSERT_NE(max_dd, raised_dd);
    ASSERT_NE(depressed_dd, max_dd);
}

TEST_F(OriginalStatsTest, test_max_drawdown_10){
    std::vector<double> depressed_returns(noise_value);
    for (double &v: depressed_returns){
        v-=0.001;
    }

    std::vector<double> raised_returns(noise_value);
    for (double &v: raised_returns){
        v+=0.001;
    }
    double max_dd = max_drawdown_from_simple_return(noise_value);
    double depressed_dd = max_drawdown_from_simple_return(depressed_returns);
    double raised_dd = max_drawdown_from_simple_return(raised_returns);
    ASSERT_NE(max_dd, raised_dd);
    ASSERT_NE(depressed_dd, max_dd);
}

TEST_F(OriginalStatsTest, test_max_drawdown_11){
    std::vector<double> depressed_returns(noise_uniform_value);
    for (double &v: depressed_returns){
        v-=0.01;
    }

    std::vector<double> raised_returns(noise_uniform_value);
    for (double &v: raised_returns){
        v+=0.01;
    }
    double max_dd = max_drawdown_from_simple_return(noise_uniform_value);
    double depressed_dd = max_drawdown_from_simple_return(depressed_returns);
    double raised_dd = max_drawdown_from_simple_return(raised_returns);
    ASSERT_NE(max_dd, raised_dd);
    ASSERT_NE(depressed_dd, max_dd);
}

TEST_F(OriginalStatsTest, test_max_drawdown_12){
    std::vector<double> depressed_returns(noise_uniform_value);
    for (double &v: depressed_returns){
        v-=0.1;
    }

    std::vector<double> raised_returns(noise_uniform_value);
    for (double &v: raised_returns){
        v+=0.1;
    }
    double max_dd = max_drawdown_from_simple_return(noise_uniform_value);
    double depressed_dd = max_drawdown_from_simple_return(depressed_returns);
    double raised_dd = max_drawdown_from_simple_return(raised_returns);
    ASSERT_NE(max_dd, raised_dd);
    ASSERT_NE(depressed_dd, max_dd);
}

TEST_F(OriginalStatsTest, test_annual_ret_1){
    double actual_value = annual_return_from_simple_return(mixed_returns_value, DAILY);
    double expect_value = 1.9135925373194231;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_annual_ret_2){
    double actual_value = annual_return_from_simple_return(weekly_returns_value, WEEKLY);
    double expect_value = 0.24690830513998208;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_annual_ret_3){
    double actual_value = annual_return_from_simple_return(monthly_returns_value, MONTHLY);
    double expect_value = 0.052242061386048144;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_annual_volatility_1){
    double actual_value = annual_volatility_from_simple_return(flat_line1_tz_value, DAILY);
    double expect_value = 0;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_annual_volatility_2){
    double actual_value = annual_volatility_from_simple_return(mixed_returns_value, DAILY, 1);
    double expect_value = 0.9136465399704637;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_annual_volatility_3){
    double actual_value = annual_volatility_from_simple_return(weekly_returns_value, WEEKLY, 1);
    double expect_value = 0.38851569394870583;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_annual_volatility_4){
    double actual_value = annual_volatility_from_simple_return(monthly_returns_value, MONTHLY, 1);
    double expect_value = 0.18663690238892558;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_calmar_1){
    double actual_value = calmar_ratio_from_simple_return(empty_returns_value, DAILY);
//    double expect_value = 0;
//    ASSERT_NEAR(max_drawdown, expect_value, 0.0001);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_calmar_2){
    double actual_value = calmar_ratio_from_simple_return(one_returns_value, DAILY);
    ASSERT_TRUE(std::isnan(actual_value));
//    double expect_value = 0;
//    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_calmar_3){
    double actual_value = calmar_ratio_from_simple_return(mixed_returns_value, DAILY);
    double expect_value = 19.135925373194233;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_calmar_4){
    double actual_value = calmar_ratio_from_simple_return(weekly_returns_value, WEEKLY);
    double expect_value = 2.4690830513998208;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_calmar_5){
    double actual_value = calmar_ratio_from_simple_return(weekly_returns_value, MONTHLY);
    double expect_value = 0.52242061386048144;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_sharpe_ratio_1){
    double actual_value = calculate_annualized_sharpe_ratio(empty_returns_value, 252, 0);
    ASSERT_TRUE(std::isnan(actual_value));
//    double expect_value = 0.52242061386048144;
//    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_sharpe_ratio_2){
    double actual_value = calculate_annualized_sharpe_ratio(one_returns_value, 252, 0);
    ASSERT_TRUE(std::isnan(actual_value));
//    double expect_value = 0.52242061386048144;
//    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_sharpe_ratio_3){
    double actual_value = calculate_annualized_sharpe_ratio(mixed_returns_value, 252, 0);
    double expect_value = 1.7238613961706866;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_sharpe_ratio_4){
    double actual_value = calculate_annualized_sharpe_ratio(positive_returns_value, 252, 0);
    double expect_value = 52.915026221291804;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_sharpe_ratio_5){
    double actual_value = calculate_annualized_sharpe_ratio(negative_returns_value, 252, 0);
    double expect_value = -24.406808633910085;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_sharpe_ratio_6){
    double actual_value = calculate_annualized_sharpe_ratio(flat_line_1_value, 252, 0);
    ASSERT_TRUE(std::isinf(actual_value));
}

TEST_F(OriginalStatsTest, test_sharpe_ratio_7){
    double sr = calculate_annualized_sharpe_ratio(noise_uniform_value, 252, 0);
    std::vector<double> depressed_returns(noise_uniform_value);
    for (double &v: depressed_returns){
        v-=0.005;
    }

    std::vector<double> raised_returns(noise_uniform_value);
    for (double &v: raised_returns){
        v+=0.005;
    }
    double sr_depressed = calculate_annualized_sharpe_ratio(depressed_returns, 252, -0.005);
    double sr_raised = calculate_annualized_sharpe_ratio(raised_returns, 252, 0.005);
    ASSERT_NEAR(sr, sr_depressed, 0.0001);
    ASSERT_NEAR(sr, sr_raised, 0.0001);

}

TEST_F(OriginalStatsTest, test_sharpe_ratio_8){
    double sr = calculate_annualized_sharpe_ratio(noise_uniform_value, 252, 0.005);
    std::vector<double> depressed_returns(noise_uniform_value);
    for (double &v: depressed_returns){
        v-=0.005;
    }

    std::vector<double> raised_returns(noise_uniform_value);
    for (double &v: raised_returns){
        v+=0.005;
    }
    double sr_depressed = calculate_annualized_sharpe_ratio(depressed_returns, 252, 0);
    double sr_raised = calculate_annualized_sharpe_ratio(raised_returns, 252, 0.01);
    ASSERT_NEAR(sr, sr_depressed, 0.0001);
    ASSERT_NEAR(sr, sr_raised, 0.0001);

}

TEST_F(OriginalStatsTest, test_sharpe_ratio_9){
    double sr = calculate_annualized_sharpe_ratio(noise_uniform_value, 252, 0);
    std::vector<double> depressed_returns(noise_uniform_value);
    for (double &v: depressed_returns){
        v-=0.0002;
    }

    std::vector<double> raised_returns(noise_uniform_value);
    for (double &v: raised_returns){
        v+=0.0002;
    }
    double sr_depressed = calculate_annualized_sharpe_ratio(depressed_returns, 252, -0.0001);
    double sr_raised = calculate_annualized_sharpe_ratio(raised_returns, 252, 0.0001);
//    ASSERT_NEAR(sr, sr_depressed, 0.0001);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
    ASSERT_NE(sr, sr_depressed);
    ASSERT_NE(sr, sr_raised);

}

TEST_F(OriginalStatsTest, test_sharpe_ratio_10){
    double sr = calculate_annualized_sharpe_ratio(noise_uniform_value, 252, 0.005);
    std::vector<double> depressed_returns(noise_uniform_value);
    for (double &v: depressed_returns){
        v-=0.0001;
    }

    std::vector<double> raised_returns(noise_uniform_value);
    for (double &v: raised_returns){
        v+=0.0001;
    }
    double sr_depressed = calculate_annualized_sharpe_ratio(depressed_returns, 252, 0.003);
    double sr_raised = calculate_annualized_sharpe_ratio(raised_returns, 252, 0.007);
    ASSERT_NE(sr, sr_depressed);
    ASSERT_NE(sr, sr_raised);

}

TEST_F(OriginalStatsTest, test_sharpe_ratio_11){
    double sr = calculate_annualized_sharpe_ratio(noise_uniform_value, 252, 0);
    std::vector<double> depressed_returns(noise_uniform_value);
    std::vector<double> raised_returns(noise_uniform_value);
    double sr_depressed = calculate_annualized_sharpe_ratio(depressed_returns, 252, -0.005);
    double sr_raised = calculate_annualized_sharpe_ratio(raised_returns, 252, 0.005);
//    ASSERT_NEAR(sr, sr_depressed, 0.0001);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
    ASSERT_GT(sr_depressed, sr);
    ASSERT_GT(sr, sr_raised);

}

TEST_F(OriginalStatsTest, test_sharpe_ratio_12){
    double sr = calculate_annualized_sharpe_ratio(noise_value, 252, 0);
    std::vector<double> depressed_returns(noise_value);
    std::vector<double> raised_returns(noise_value);
    double sr_depressed = calculate_annualized_sharpe_ratio(depressed_returns, 252, -0.005);
    double sr_raised = calculate_annualized_sharpe_ratio(raised_returns, 252, 0.005);
//    ASSERT_NEAR(sr, sr_depressed, 0.0001);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
    ASSERT_GT(sr_depressed, sr);
    ASSERT_GT(sr, sr_raised);

}

TEST_F(OriginalStatsTest, test_sharpe_ratio_13){
    std::vector<double> smaller_normal_returns = cal_func::generateRandomNormal(1000, 0.01, 0.001);
    std::vector<double> bigger_normal_returns = cal_func::generateRandomNormal(1000, 0.01, 0.002);
    double sr_depressed = calculate_annualized_sharpe_ratio(smaller_normal_returns, 252, 0.001);
    double sr_raised = calculate_annualized_sharpe_ratio(bigger_normal_returns, 252, 0.001);
//    ASSERT_NEAR(sr, sr_depressed, 0.0001);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
    ASSERT_GT(sr_depressed, sr_raised);
}

TEST_F(OriginalStatsTest, test_sharpe_ratio_14){
    std::vector<double> smaller_normal_returns = cal_func::generateRandomNormal(1000, 0.01, 0.01);
    std::vector<double> bigger_normal_returns = cal_func::generateRandomNormal(1000, 0.01, 0.02);
    double sr_depressed = calculate_annualized_sharpe_ratio(smaller_normal_returns, 252, 0.001);
    double sr_raised = calculate_annualized_sharpe_ratio(bigger_normal_returns, 252, 0.001);
//    ASSERT_NEAR(sr, sr_depressed, 0.0001);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
    ASSERT_GT(sr_depressed, sr_raised);
}

TEST_F(OriginalStatsTest, test_downside_risk_1){
    double actual_value = calculate_downside_risk(empty_returns_value, 0);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_downside_risk_2){
    double actual_value = calculate_downside_risk(one_returns_value, 0);
    double except_value = 0;
    ASSERT_NEAR(actual_value, except_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_downside_risk_3){
    double actual_value = calculate_downside_risk(mixed_returns_value, mixed_returns_value);
    double except_value = 0;
    ASSERT_NEAR(actual_value, except_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_downside_risk_4){
    double actual_value = calculate_downside_risk(mixed_returns_value, 0);
    double except_value = 0.60448325038829653;
    ASSERT_NEAR(actual_value*std::sqrt(252), except_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_downside_risk_5){
    double actual_value = calculate_downside_risk(mixed_returns_value, 0.1);
    double except_value = 1.7161730681956295;
    ASSERT_NEAR(actual_value*std::sqrt(252), except_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_downside_risk_6){
    double actual_value = calculate_downside_risk(weekly_returns_value, 0.0);
    double except_value = 0.25888650451930134;
    ASSERT_NEAR(actual_value*std::sqrt(WEEKS_PER_YEAR), except_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_downside_risk_7){
    double actual_value = calculate_downside_risk(weekly_returns_value, 0.1);
    double except_value = 0.7733045971672482;
    ASSERT_NEAR(actual_value*std::sqrt(WEEKS_PER_YEAR), except_value, 0.0001);
}


TEST_F(OriginalStatsTest, test_downside_risk_8){
    double actual_value = calculate_downside_risk(monthly_returns_value, 0.0);
    double except_value = 0.1243650540411842;
    ASSERT_NEAR(actual_value*std::sqrt(MONTHS_PER_YEAR), except_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_downside_risk_9){
    double actual_value = calculate_downside_risk(monthly_returns_value, 0.1);
    double except_value = 0.37148351242013422;
    ASSERT_NEAR(actual_value*std::sqrt(MONTHS_PER_YEAR), except_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_downside_risk_10){
    std::vector<double> noisy_returns_1;
    noisy_returns_1.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<250){
            noisy_returns_1.push_back(noise_value[i]);
        }else{
            noisy_returns_1.push_back(flat_line_0_value[i]);
        }
    }

    std::vector<double> noisy_returns_2;
    noisy_returns_2.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<500){
            noisy_returns_2.push_back(noise_value[i]);
        }else{
            noisy_returns_2.push_back(flat_line_0_value[i]);
        }
    }

    std::vector<double> noisy_returns_3;
    noisy_returns_3.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<750){
            noisy_returns_3.push_back(noise_value[i]);
        }else{
            noisy_returns_3.push_back(flat_line_0_value[i]);
        }
    }
    double dr_1 = calculate_downside_risk(noisy_returns_1, flat_line_0_value);
    double dr_2 = calculate_downside_risk(noisy_returns_2, flat_line_0_value);
    double dr_3 = calculate_downside_risk(noisy_returns_3, flat_line_0_value);
    ASSERT_NE(dr_1, dr_2);
    ASSERT_NE(dr_2, dr_3);
}

TEST_F(OriginalStatsTest, test_downside_risk_11){
    std::vector<double> noisy_returns_1;
    noisy_returns_1.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<250){
            noisy_returns_1.push_back(noise_uniform_value[i]);
        }else{
            noisy_returns_1.push_back(flat_line_0_value[i]);
        }
    }

    std::vector<double> noisy_returns_2;
    noisy_returns_2.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<500){
            noisy_returns_2.push_back(noise_uniform_value[i]);
        }else{
            noisy_returns_2.push_back(flat_line_0_value[i]);
        }
    }

    std::vector<double> noisy_returns_3;
    noisy_returns_3.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<750){
            noisy_returns_3.push_back(noise_uniform_value[i]);
        }else{
            noisy_returns_3.push_back(flat_line_0_value[i]);
        }
    }
    double dr_1 = calculate_downside_risk(noisy_returns_1, flat_line_0_value);
    double dr_2 = calculate_downside_risk(noisy_returns_2, flat_line_0_value);
    double dr_3 = calculate_downside_risk(noisy_returns_3, flat_line_0_value);
    ASSERT_NE(dr_1, dr_2);
    ASSERT_NE(dr_2, dr_3);
}

TEST_F(OriginalStatsTest, test_downside_risk_12){
    double dr_1 = calculate_downside_risk(noise_value, -0.005);
    double dr_2 = calculate_downside_risk(noise_value, 0);
    double dr_3 = calculate_downside_risk(noise_value, 0.005);
    ASSERT_NE(dr_1, dr_2);
    ASSERT_NE(dr_2, dr_3);
}

TEST_F(OriginalStatsTest, test_downside_risk_13){
    double dr_1 = calculate_downside_risk(noise_uniform_value, -0.005);
    double dr_2 = calculate_downside_risk(noise_uniform_value, 0);
    double dr_3 = calculate_downside_risk(noise_uniform_value, 0.005);
    ASSERT_NE(dr_1, dr_2);
    ASSERT_NE(dr_2, dr_3);
}

TEST_F(OriginalStatsTest, test_downside_risk_14){
    std::vector<double> less_noise = cal_func::generateRandomNormal(1000, 0, 0.001);
    std::vector<double> more_noise = cal_func::generateRandomNormal(1000, 0, 0.002);
    double less = calculate_downside_risk(less_noise, 0);
    double more = calculate_downside_risk(more_noise, 0);
    ASSERT_NE(less, more);
}

TEST_F(OriginalStatsTest, test_downside_risk_15){
    std::vector<double> less_noise = cal_func::generateRandomNormal(1000, 0, 0.001);
    std::vector<double> more_noise = cal_func::generateRandomNormal(1000, 0, 0.01);
    double less = calculate_downside_risk(less_noise, 0);
    double more = calculate_downside_risk(more_noise, 0);
    ASSERT_NE(less, more);
}

TEST_F(OriginalStatsTest, test_downside_risk_16){
    std::vector<double> less_noise(1000, 0);
    std::vector<double> more_noise = cal_func::generateRandomNormal(1000, 0, 0.001);
    double less = calculate_downside_risk(less_noise, 0);
    double more = calculate_downside_risk(more_noise, 0);
    ASSERT_NE(less, more);
}

TEST_F(OriginalStatsTest, test_sortino_ratio_01){
    double actual_value = calculate_annualized_sortino_ratio(empty_returns_value, 252);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_02){
    double actual_value = calculate_annualized_sortino_ratio(one_returns_value, 252);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_03){
    double actual_value = calculate_annualized_sortino_ratio(mixed_returns_value, 252, mixed_returns_value);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_04){
    double actual_value = calculate_annualized_sortino_ratio(mixed_returns_value, APPROX_BDAYS_PER_YEAR, 0);
    double expect_value = 2.605531251673693;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_sortino_ratio_05){
    double actual_value = calculate_annualized_sortino_ratio(mixed_returns_value, APPROX_BDAYS_PER_YEAR, flat_line_1_value);
    double expect_value = -1.3934779588919977;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_sortino_ratio_06){
    double actual_value = calculate_annualized_sortino_ratio(positive_returns_value, APPROX_BDAYS_PER_YEAR, 0.0);
    // ASSERT_NEAR(actual_value, expect_value, 0.0001);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_07){
    double actual_value = calculate_annualized_sortino_ratio(negative_returns_value, APPROX_BDAYS_PER_YEAR, 0);
    double expect_value = -13.532743075043401;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_sortino_ratio_08){
    double actual_value = calculate_annualized_sortino_ratio(simple_benchmark_value, APPROX_BDAYS_PER_YEAR, 0);
    //double expect_value = 0;
    // ASSERT_NEAR(actual_value, expect_value, 0.0001);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_09){
    double actual_value = calculate_annualized_sortino_ratio(weekly_returns_value, WEEKS_PER_YEAR, 0);
    double expect_value = 1.1158901056866439;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
    //ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_10){
    double actual_value = calculate_annualized_sortino_ratio(monthly_returns_value, MONTHS_PER_YEAR, 0);
    double expect_value = 0.53605626741889756;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
    //ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_11){
    std::vector<double> new_return(noise_uniform_value);
    double sr_1 = calculate_annualized_sortino_ratio(noise_uniform_value, APPROX_BDAYS_PER_YEAR, 0);
    std::vector<std::size_t> index;
    for (std::size_t i = 0; i< noise_uniform_value.size(); i++){
        if (noise_uniform_value[i]>0){
            index.push_back(i);
        }
    }
    std::size_t m = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    new_return[m]=-0.01;
    double sr_2 = calculate_annualized_sortino_ratio(new_return, APPROX_BDAYS_PER_YEAR, 0);
    std::size_t n = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    while (n==m){
        n = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    }
    new_return[n] = -0.01;
    double sr_3 = calculate_annualized_sortino_ratio(new_return, APPROX_BDAYS_PER_YEAR, 0);
    ASSERT_NE(sr_1, sr_2);
    ASSERT_NE(sr_2, sr_3);
    //ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_12){
    std::vector<double> new_return(noise_value);
    double sr_1 = calculate_annualized_sortino_ratio(noise_value, APPROX_BDAYS_PER_YEAR, 0);
    std::vector<std::size_t> index;
    for (std::size_t i = 0; i< noise_value.size(); i++){
        if (noise_value[i]>0){
            index.push_back(i);
        }
    }
    std::size_t m = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    new_return[m]=-0.01;
    double sr_2 = calculate_annualized_sortino_ratio(new_return, APPROX_BDAYS_PER_YEAR, 0);
    std::size_t n = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    while (n==m){
        n = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    }
    new_return[n] = -0.01;
    double sr_3 = calculate_annualized_sortino_ratio(new_return, APPROX_BDAYS_PER_YEAR, 0);
    ASSERT_NE(sr_1, sr_2);
    ASSERT_NE(sr_2, sr_3);
    //ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_13){
    std::vector<double> new_return(noise_uniform_value);
    double sr_1 = calculate_annualized_sortino_ratio(noise_uniform_value, APPROX_BDAYS_PER_YEAR, 0);
    std::vector<std::size_t> index;
    for (std::size_t i = 0; i< noise_uniform_value.size(); i++){
        if (noise_uniform_value[i]<0){
            index.push_back(i);
        }
    }
    std::size_t m = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    new_return[m]=0;
    double sr_2 = calculate_annualized_sortino_ratio(new_return, APPROX_BDAYS_PER_YEAR, 0);
    std::size_t n = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    while (n==m){
        n = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    }
    new_return[n] = 0;
    double sr_3 = calculate_annualized_sortino_ratio(new_return, APPROX_BDAYS_PER_YEAR, 0);
    ASSERT_NE(sr_3, sr_2);
    ASSERT_NE(sr_2, sr_1);
    //ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_14){
    std::vector<double> new_return(noise_value);
    double sr_1 = calculate_annualized_sortino_ratio(noise_value, APPROX_BDAYS_PER_YEAR, 0);
    std::vector<std::size_t> index;
    for (std::size_t i = 0; i< noise_value.size(); i++){
        if (noise_value[i]<0){
            index.push_back(i);
        }
    }
    std::size_t m = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    new_return[m]=0;
    double sr_2 = calculate_annualized_sortino_ratio(new_return, APPROX_BDAYS_PER_YEAR, 0);
    std::size_t n = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    while (n==m){
        n = cal_func::randomInt(0, static_cast<int>(index.size()-1));
    }
    new_return[n] = 0;
    double sr_3 = calculate_annualized_sortino_ratio(new_return, APPROX_BDAYS_PER_YEAR, 0);
    ASSERT_NE(sr_3, sr_2);
    ASSERT_NE(sr_2, sr_1);
    //ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_15){
    double sr = calculate_annualized_sortino_ratio(noise_uniform_value, 252, 0);
    std::vector<double> depressed_returns(noise_uniform_value);
    for (double &v: depressed_returns){
        v-=0.005;
    }

    std::vector<double> raised_returns(noise_uniform_value);
    for (double &v: raised_returns){
        v+=0.005;
    }
    double sr_depressed = calculate_annualized_sortino_ratio(depressed_returns, 252, -0.005);
    double sr_raised = calculate_annualized_sortino_ratio(raised_returns, 252, 0.005);
    ASSERT_NEAR(sr, sr_depressed, 0.0001);
    ASSERT_NEAR(sr, sr_raised, 0.0001);

}

TEST_F(OriginalStatsTest, test_sortino_ratio_16){
    double sr = calculate_annualized_sortino_ratio(noise_uniform_value, 252, 0.005);
    std::vector<double> depressed_returns(noise_uniform_value);
    for (double &v: depressed_returns){
        v-=0.005;
    }

    std::vector<double> raised_returns(noise_uniform_value);
    for (double &v: raised_returns){
        v+=0.005;
    }
    double sr_depressed = calculate_annualized_sortino_ratio(depressed_returns, 252, 0);
    double sr_raised = calculate_annualized_sortino_ratio(raised_returns, 252, 0.01);
    ASSERT_NEAR(sr, sr_depressed, 0.0001);
    ASSERT_NEAR(sr, sr_raised, 0.0001);

}

TEST_F(OriginalStatsTest, test_sortino_ratio_17){
    double sr = calculate_annualized_sortino_ratio(noise_uniform_value, 252, 0);
    std::vector<double> depressed_returns(noise_uniform_value);
    for (double &v: depressed_returns){
        v-=0.000;
    }

    std::vector<double> raised_returns(noise_uniform_value);
    for (double &v: raised_returns){
        v+=0.000;
    }
    double sr_depressed = calculate_annualized_sortino_ratio(depressed_returns, 252, -0.001);
    double sr_raised = calculate_annualized_sortino_ratio(raised_returns, 252, 0.001);
    ASSERT_FALSE(std::abs(sr-sr_depressed)<0.000001);
    ASSERT_FALSE(std::abs(sr-sr_raised)<0.000001);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
}

TEST_F(OriginalStatsTest, test_sortino_ratio_18){
    double sr = calculate_annualized_sortino_ratio(noise_uniform_value, 252, 0.005);
    std::vector<double> depressed_returns(noise_uniform_value);
    for (double &v: depressed_returns){
        v-=0.001;
    }

    std::vector<double> raised_returns(noise_uniform_value);
    for (double &v: raised_returns){
        v+=0.001;
    }
    double sr_depressed = calculate_annualized_sortino_ratio(depressed_returns, 252, 0.005);
    double sr_raised = calculate_annualized_sortino_ratio(raised_returns, 252, 0.005);
    ASSERT_FALSE(std::abs(sr-sr_depressed)<0.000001);
    ASSERT_FALSE(std::abs(sr-sr_raised)<0.000001);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
}

