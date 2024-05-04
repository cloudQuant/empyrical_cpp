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
}

TEST_F(OriginalStatsTest, test_sharpe_ratio_2){
    double actual_value = calculate_annualized_sharpe_ratio(one_returns_value, 252, 0);
    ASSERT_TRUE(std::isnan(actual_value));
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
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_sortino_ratio_07){
    double actual_value = calculate_annualized_sortino_ratio(negative_returns_value, APPROX_BDAYS_PER_YEAR, 0);
    double expect_value = -13.532743075043401;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
}

TEST_F(OriginalStatsTest, test_sortino_ratio_08){
    double actual_value = calculate_annualized_sortino_ratio(simple_benchmark_value, APPROX_BDAYS_PER_YEAR, 0);
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

TEST_F(OriginalStatsTest, test_excess_sharpe_1){
    double actual_value = calculate_excess_sharpe_ratio(empty_returns_value, 0, 0);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_excess_sharpe_2){
    double actual_value = calculate_excess_sharpe_ratio(one_returns_value, 0, 0);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_excess_sharpe_3){
    double actual_value = calculate_excess_sharpe_ratio(pos_line_value, pos_line_value, 0);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_excess_sharpe_4){
    double actual_value = calculate_excess_sharpe_ratio(mixed_returns_value, 0, 1);
    double expect_value = 0.10859306069076737;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
//    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_excess_sharpe_5){
    double actual_value = calculate_excess_sharpe_ratio(mixed_returns_value, flat_line_1_value, 1);
    double expect_value = -0.06515583641446039;
    ASSERT_NEAR(actual_value, expect_value, 0.0001);
//    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_excess_sharpe_6){
    std::vector<double> noisy_returns_1;
    noisy_returns_1.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<250){
            noisy_returns_1.push_back(flat_line_0_value[i]);
        }else{
            noisy_returns_1.push_back(pos_line_value[i]);
        }
    }

    std::vector<double> noisy_returns_2;
    noisy_returns_2.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<500){
            noisy_returns_1.push_back(flat_line_0_value[i]);
        }else{
            noisy_returns_1.push_back(pos_line_value[i]);
        }
    }

    std::vector<double> noisy_returns_3;
    noisy_returns_3.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<750){
            noisy_returns_1.push_back(flat_line_0_value[i]);
        }else{
            noisy_returns_1.push_back(pos_line_value[i]);
        }
    }
    double ir_1 = calculate_excess_sharpe_ratio(noisy_returns_1, flat_line_0_value,1);
    double ir_2 = calculate_excess_sharpe_ratio(noisy_returns_2, flat_line_0_value,1);
    double ir_3 = calculate_excess_sharpe_ratio(noisy_returns_3, flat_line_0_value,1);
    ASSERT_NE(ir_1, ir_2);
    ASSERT_NE(ir_2, ir_3);
}

TEST_F(OriginalStatsTest, test_excess_sharpe_7){
    std::vector<double> noisy_returns_1;
    noisy_returns_1.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<250){
            noisy_returns_1.push_back(flat_line1_tz_value[i]);
        }else{
            noisy_returns_1.push_back(pos_line_value[i]);
        }
    }

    std::vector<double> noisy_returns_2;
    noisy_returns_2.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<500){
            noisy_returns_1.push_back(flat_line1_tz_value[i]);
        }else{
            noisy_returns_1.push_back(pos_line_value[i]);
        }
    }

    std::vector<double> noisy_returns_3;
    noisy_returns_3.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<750){
            noisy_returns_1.push_back(flat_line1_tz_value[i]);
        }else{
            noisy_returns_1.push_back(pos_line_value[i]);
        }
    }
    double ir_1 = calculate_excess_sharpe_ratio(noisy_returns_1, flat_line_0_value,1);
    double ir_2 = calculate_excess_sharpe_ratio(noisy_returns_2, flat_line_0_value,1);
    double ir_3 = calculate_excess_sharpe_ratio(noisy_returns_3, flat_line_0_value,1);
    ASSERT_NE(ir_1, ir_2);
    ASSERT_NE(ir_2, ir_3);
}

TEST_F(OriginalStatsTest, test_excess_sharpe_8){
    std::vector<double> noisy_returns_1;
    noisy_returns_1.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<250){
            noisy_returns_1.push_back(noise_value[i]);
        }else{
            noisy_returns_1.push_back(pos_line_value[i]);
        }
    }

    std::vector<double> noisy_returns_2;
    noisy_returns_2.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<500){
            noisy_returns_1.push_back(noise_value[i]);
        }else{
            noisy_returns_1.push_back(pos_line_value[i]);
        }
    }

    std::vector<double> noisy_returns_3;
    noisy_returns_3.reserve(1000);
    for (int i=0; i<1000; ++i){
        if (i<750){
            noisy_returns_1.push_back(noise_value[i]);
        }else{
            noisy_returns_1.push_back(pos_line_value[i]);
        }
    }
    double ir_1 = calculate_excess_sharpe_ratio(noisy_returns_1, flat_line_0_value,1);
    double ir_2 = calculate_excess_sharpe_ratio(noisy_returns_2, flat_line_0_value,1);
    double ir_3 = calculate_excess_sharpe_ratio(noisy_returns_3, flat_line_0_value,1);
    ASSERT_NE(ir_1, ir_2);
    ASSERT_NE(ir_2, ir_3);
}

TEST_F(OriginalStatsTest, test_excess_sharpe_9){
    std::vector<double> new_returns(pos_line_value);
    for (std::size_t i=0; i<new_returns.size(); ++i){
        new_returns[i]+=inverse_noise_value[i];
    }
    double sr = calculate_excess_sharpe_ratio(new_returns, pos_line_value, 1);
    std::vector<double> depressed_returns(pos_line_value);
    for (std::size_t i=0; i<depressed_returns.size(); ++i){
        depressed_returns[i]+=(inverse_noise_value[i]-flat_line1_tz_value[i]);
    }

    std::vector<double> raised_returns(pos_line_value);
    for (std::size_t i=0; i<raised_returns.size(); ++i){
        raised_returns[i]+=(inverse_noise_value[i]+flat_line1_tz_value[i]);
    }
    double sr_depressed = calculate_excess_sharpe_ratio(depressed_returns, pos_line_value, 1);
    double sr_raised = calculate_excess_sharpe_ratio(raised_returns, pos_line_value, 1);
    ASSERT_FALSE(sr>sr_raised);
    ASSERT_FALSE(sr_depressed>sr);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
}

TEST_F(OriginalStatsTest, test_excess_sharpe_10){
    std::vector<double> new_returns(pos_line_value);
    for (std::size_t i=0; i<new_returns.size(); ++i){
        new_returns[i]+=noise_value[i];
    }
    double sr = calculate_excess_sharpe_ratio(new_returns, pos_line_value, 1);
    std::vector<double> depressed_returns(pos_line_value);
    for (std::size_t i=0; i<depressed_returns.size(); ++i){
        depressed_returns[i]+=(noise_value[i]-flat_line1_tz_value[i]);
    }

    std::vector<double> raised_returns(pos_line_value);
    for (std::size_t i=0; i<raised_returns.size(); ++i){
        raised_returns[i]+=(noise_value[i]+flat_line1_tz_value[i]);
    }
    double sr_depressed = calculate_excess_sharpe_ratio(depressed_returns, pos_line_value, 1);
    double sr_raised = calculate_excess_sharpe_ratio(raised_returns, pos_line_value, 1);
    ASSERT_FALSE(sr>sr_raised);
    ASSERT_FALSE(sr_depressed>sr);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
}

TEST_F(OriginalStatsTest, test_excess_sharpe_11){
    std::vector<double> new_returns(neg_line_value);
    for (std::size_t i=0; i<new_returns.size(); ++i){
        new_returns[i]+=noise_value[i];
    }
    double sr = calculate_excess_sharpe_ratio(new_returns, neg_line_value, 1);
    std::vector<double> depressed_returns(neg_line_value);
    for (std::size_t i=0; i<depressed_returns.size(); ++i){
        depressed_returns[i]+=(noise_value[i]-flat_line1_tz_value[i]);
    }

    std::vector<double> raised_returns(neg_line_value);
    for (std::size_t i=0; i<raised_returns.size(); ++i){
        raised_returns[i]+=(noise_value[i]+flat_line1_tz_value[i]);
    }
    double sr_depressed = calculate_excess_sharpe_ratio(depressed_returns, neg_line_value, 1);
    double sr_raised = calculate_excess_sharpe_ratio(raised_returns, neg_line_value, 1);
    ASSERT_FALSE(sr>sr_raised);
    ASSERT_FALSE(sr_depressed>sr);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
}

TEST_F(OriginalStatsTest, test_excess_sharpe_12){
    std::vector<double> new_returns(neg_line_value);
    for (std::size_t i=0; i<new_returns.size(); ++i){
        new_returns[i]+=inverse_noise_value[i];
    }
    double sr = calculate_excess_sharpe_ratio(new_returns, neg_line_value, 1);
    std::vector<double> depressed_returns(neg_line_value);
    for (std::size_t i=0; i<depressed_returns.size(); ++i){
        depressed_returns[i]+=(inverse_noise_value[i]-flat_line1_tz_value[i]);
    }

    std::vector<double> raised_returns(neg_line_value);
    for (std::size_t i=0; i<raised_returns.size(); ++i){
        raised_returns[i]+=(inverse_noise_value[i]+flat_line1_tz_value[i]);
    }
    double sr_depressed = calculate_excess_sharpe_ratio(depressed_returns, neg_line_value, 1);
    double sr_raised = calculate_excess_sharpe_ratio(raised_returns, neg_line_value, 1);
    ASSERT_FALSE(sr>sr_raised);
    ASSERT_FALSE(sr_depressed>sr);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
}

TEST_F(OriginalStatsTest, test_alpha_beta_1){
    std::pair<double,double> p = calculate_alpha_beta(empty_returns_value,
                                     simple_benchmark_value,
                                     0);
    double alpha_value = p.first;
    double beta_value = p.second;
    ASSERT_TRUE(std::isnan(alpha_value));
    ASSERT_TRUE(std::isnan(beta_value));
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
}

TEST_F(OriginalStatsTest, test_alpha_beta_2){
    std::pair<double,double> p = calculate_alpha_beta(one_returns_value,
                                     one_returns_value,
                                     0);
    double alpha_value = p.first;
    double beta_value = p.second;
    ASSERT_TRUE(std::isnan(alpha_value));
    ASSERT_TRUE(std::isnan(beta_value));
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
}

TEST_F(OriginalStatsTest, test_alpha_beta_3){
    std::vector<double> y(mixed_returns_value);
    y.erase(y.begin());
    std::vector<double> x(negative_returns_value);
    x.erase(x.begin());
    std::pair<double,double> p = calculate_alpha_beta(y,x,0);
    double alpha_value = p.first;
    double beta_value = p.second;
    std::cout << "alpha_value = " << alpha_value << std::endl;
    std::cout << "beta_value = " << beta_value << std::endl;
    ASSERT_NEAR(alpha_value, -0.9997853834885004, 0.000001);
    ASSERT_NEAR(beta_value, -0.71296296296296313, 0.000001);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
}

TEST_F(OriginalStatsTest, test_alpha_beta_4){
    std::vector<double> y(mixed_returns_value);
    y.erase(y.begin());
    std::vector<double> x(mixed_returns_value);
    x.erase(x.begin());
    std::pair<double,double> p = calculate_alpha_beta(y,x,0);
    double alpha_value = p.first;
    double beta_value = p.second;
    std::cout << "alpha_value = " << alpha_value << std::endl;
    std::cout << "beta_value = " << beta_value << std::endl;
    ASSERT_NEAR(alpha_value, 0, 0.000001);
    ASSERT_NEAR(beta_value, 1, 0.000001);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
}

TEST_F(OriginalStatsTest, test_alpha_beta_5){
    std::vector<double> y(mixed_returns_value);
    y.erase(y.begin());
    for (double v:y){
        std::cout << v << " ";
    }
    std::cout << std::endl;
    std::vector<double> x(mixed_returns_value);
    x.erase(x.begin());
    for (double &v:x){
        v=v*-1.0;
    }
    for (double v:x){
        std::cout << v << " ";
    }
    std::pair<double,double> p = calculate_alpha_beta(y,x,0);
    double alpha_value = p.first;
    double beta_value = p.second;
    std::cout << "alpha_value = " << alpha_value << std::endl;
    std::cout << "beta_value = " << beta_value << std::endl;
    ASSERT_NEAR(alpha_value, 0, 0.000001);
    ASSERT_NEAR(beta_value, -1.0, 0.000001);
//    ASSERT_NEAR(sr, sr_raised, 0.0001);
}

TEST_F(OriginalStatsTest, test_alpha_1){

    double alpha = calculate_alpha(empty_returns_value,
                                   simple_benchmark_value,
                                   0);

    ASSERT_TRUE(std::isnan(alpha));
}

TEST_F(OriginalStatsTest, test_alpha_2){

    double alpha = calculate_alpha(one_returns_value,
                                   one_returns_value,
                                   0);

    ASSERT_TRUE(std::isnan(alpha));
}

TEST_F(OriginalStatsTest, test_alpha_3){

    double alpha = calculate_alpha(mixed_returns_value,
                                   flat_line_1_value,
                                   0);

    ASSERT_TRUE(std::isnan(alpha));
}

TEST_F(OriginalStatsTest, test_alpha_4){

    double alpha = calculate_alpha(mixed_returns_value,
                                   flat_line_1_value,
                                   0);

    ASSERT_TRUE(std::isnan(alpha));
}

TEST_F(OriginalStatsTest, test_alpha_5){
    std::vector<double> new_returns(mixed_returns_value);
    new_returns.erase(new_returns.begin());
    double alpha = calculate_alpha(new_returns,
                                   new_returns,
                                   0);

    ASSERT_DOUBLE_EQ(alpha, 0);
}

TEST_F(OriginalStatsTest, test_alpha_6){
    std::vector<double> new_returns_0(mixed_returns_value);
    new_returns_0.erase(new_returns_0.begin());
    std::vector<double> new_returns(mixed_returns_value);
    new_returns.erase(new_returns.begin());
    for (double& v:mixed_returns_value){
        v*=-1;
    }
    double alpha = calculate_alpha(new_returns_0,
                                   new_returns,
                                   0);

    ASSERT_DOUBLE_EQ(alpha, 0);
}

TEST_F(OriginalStatsTest, test_alpha_beta_6){
    std::pair<double, double> p = calculate_alpha_beta(spase_noise_value,
                                   spase_noise_value,
                                   0);
    double alpha_value = p.first;
    double beta_value = p.second;

    ASSERT_TRUE(std::isnan(alpha_value));
    ASSERT_TRUE(std::isnan(beta_value));
}

TEST_F(OriginalStatsTest, test_beta_1){
    double beta = calculate_beta(empty_returns_value,
                                 simple_benchmark_value);

    ASSERT_TRUE(std::isnan(beta));
}

TEST_F(OriginalStatsTest, test_beta_2){
    double beta = calculate_beta(one_returns_value,
                                 one_returns_value);

    ASSERT_TRUE(std::isnan(beta));
}

TEST_F(OriginalStatsTest, test_beta_3){
    double beta = calculate_beta(mixed_returns_value,
                                 flat_line_1_value);

    ASSERT_TRUE(std::isnan(beta));
}

TEST_F(OriginalStatsTest, test_beta_4){
    double beta = calculate_beta(noise_value,
                                 noise_value);

    ASSERT_NEAR(beta, 1, 0.000001);
}

TEST_F(OriginalStatsTest, test_beta_5){
    std::vector<double> new_returns(noise_value);
    for(double &v:new_returns){
        v*=2.0;
    }
    double beta = calculate_beta(new_returns,
                                 noise_value);

    ASSERT_NEAR(beta, 2, 0.000001);
}

TEST_F(OriginalStatsTest, test_beta_6){
    std::vector<double> new_returns(noise_value);
    for(double &v:new_returns){
        v*=2.0;
    }
    double beta = calculate_beta(noise_value,
                                 inverse_noise_value);

    ASSERT_NEAR(beta, -1, 0.000001);
}

TEST_F(OriginalStatsTest, test_beta_7){
    std::vector<double> new_returns(noise_value);
    for(double &v:new_returns){
        v*=2.0;
    }
    double beta = calculate_beta(new_returns,
                                 inverse_noise_value);

    ASSERT_NEAR(beta, -2, 0.000001);
}

TEST_F(OriginalStatsTest, test_beta_8){
    std::vector<double> new_returns(spase_noise_value);
    for (std::size_t i = 0; i< new_returns.size();++i){
       new_returns[i]*=flat_line1_tz_value[i];
    }
    double beta = calculate_beta(new_returns,
                                 sparse_flat_line_1_value);

    ASSERT_TRUE(std::isnan(beta));
}

TEST_F(OriginalStatsTest, test_stability_of_timeseries_1){

    double actual_value = stability_of_timeseries(empty_returns_value);

    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_stability_of_timeseries_2){

    double actual_value = stability_of_timeseries(one_returns_value);

    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_stability_of_timeseries_3){
    std::vector<double> new_returns(mixed_returns_value);
    new_returns.erase(new_returns.begin());
    double actual_value = stability_of_timeseries(new_returns);
    double expect_value = 0.1529973665111273;
    ASSERT_NEAR(actual_value, expect_value, 0.000001);
}

TEST_F(OriginalStatsTest, test_stability_of_timeseries_4){

    double actual_value = stability_of_timeseries(flat_line1_tz_value);
    double expect_value = 1;
    ASSERT_NEAR(actual_value, expect_value, 0.000001);
}

TEST_F(OriginalStatsTest, test_tail_ratio_1){

    double actual_value = tail_ratio(empty_returns_value);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_tail_ratio_2){

    double actual_value = tail_ratio(one_returns_value);
    //ASSERT_TRUE(std::isnan(actual_value));
    double expect_value = 1;
    ASSERT_NEAR(actual_value, expect_value, 0.000001);
}

TEST_F(OriginalStatsTest, test_tail_ratio_3){
    std::vector<double> new_returns(mixed_returns_value);
    new_returns.erase(new_returns.begin());
    for (double v:new_returns){
        std::cout <<v << " ";
    }
    double actual_value = tail_ratio(new_returns);
    //ASSERT_TRUE(std::isnan(actual_value));
    double expect_value = 0.955696202531645;
    ASSERT_NEAR(actual_value, expect_value, 0.000001);
}

TEST_F(OriginalStatsTest, test_tail_ratio_4){
    std::vector<double> new_returns = cal_func::generateRandomNormal(10000,0,1);
    double actual_value = tail_ratio(new_returns);
    //ASSERT_TRUE(std::isnan(actual_value));
    double expect_value = 0.9473684210526313;
    ASSERT_NEAR(actual_value, expect_value, 0.1);
}

TEST_F(OriginalStatsTest, test_annual_return_from_simple_return_1){
    double actual_value = annual_return_from_simple_return(empty_returns_value);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_cagr_2){
    double actual_value = annual_return_from_simple_return(one_returns_value, "daily", APPROX_BDAYS_PER_YEAR);
    //ASSERT_TRUE(std::isnan(actual_value));
    double expect_value = 11.274002099240244;
    ASSERT_NEAR(actual_value, expect_value, 0.000001);
}

TEST_F(OriginalStatsTest, test_cagr_3){
    std::vector<double> new_returns(mixed_returns_value);
    new_returns.erase(new_returns.begin());
    double actual_value = annual_return_from_simple_return(new_returns, "daily", APPROX_BDAYS_PER_YEAR);
    //ASSERT_TRUE(std::isnan(actual_value));
    double expect_value = 2.330292797300439;
    ASSERT_NEAR(actual_value, expect_value, 0.000001);
}

TEST_F(OriginalStatsTest, test_cagr_4){
    double actual_value = annual_return_from_simple_return(flat_line1_tz_value, "daily", APPROX_BDAYS_PER_YEAR);
    //ASSERT_TRUE(std::isnan(actual_value));
    double expect_value = 11.274002099240256;
    ASSERT_NEAR(actual_value, expect_value, 0.000001);
}

TEST_F(OriginalStatsTest, test_cagr_5){
    double cagr_depressed = annual_return_from_simple_return(
            cal_func::vector_add_value(noise_value, -0.01),
            "daily", APPROX_BDAYS_PER_YEAR);
    double cagr_raised = annual_return_from_simple_return(
            cal_func::vector_add_value(noise_value, 0.01),
            "daily", APPROX_BDAYS_PER_YEAR);
    double cagr_unchanged = annual_return_from_simple_return(
            noise_value,
            "daily", APPROX_BDAYS_PER_YEAR);
    //double expect_value = 11.274002099240256;
    ASSERT_TRUE(cagr_depressed < cagr_unchanged);
    ASSERT_TRUE(cagr_unchanged < cagr_raised);
}

TEST_F(OriginalStatsTest, test_cagr_6){
    double cagr_depressed = annual_return_from_simple_return(
            cal_func::vector_add_value(noise_uniform_value, -0.01),
            "daily", APPROX_BDAYS_PER_YEAR);
    double cagr_raised = annual_return_from_simple_return(
            cal_func::vector_add_value(noise_uniform_value, 0.01),
            "daily", APPROX_BDAYS_PER_YEAR);
    double cagr_unchanged = annual_return_from_simple_return(
            noise_uniform_value,
            "daily", APPROX_BDAYS_PER_YEAR);
    //double expect_value = 11.274002099240256;
    ASSERT_TRUE(cagr_depressed < cagr_unchanged);
    ASSERT_TRUE(cagr_unchanged < cagr_raised);

}

TEST_F(OriginalStatsTest, test_cagr_7){
    double actual_value = annual_return_from_simple_return(spase_noise_value, "daily", APPROX_BDAYS_PER_YEAR);
    std::cout << "actual_value = " << actual_value << std::endl;
//    for (double v: spase_noise_value){
//        if (std::isnan(v)){
//            std::cout << " v is nan " << v << std::endl;
//        }
//    }
    ASSERT_FALSE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_cagr_8){
    double cagr = annual_return_from_simple_return(pos_line_value, "daily", APPROX_BDAYS_PER_YEAR);
    double noisy_cagr_1 = annual_return_from_simple_return(cal_func::vector_add_vector(pos_line_value,noise_value),
                                                           "daily", APPROX_BDAYS_PER_YEAR);

    double noisy_cagr_2 = annual_return_from_simple_return(cal_func::vector_minus_vector(pos_line_value,noise_value),
                                                           "daily", APPROX_BDAYS_PER_YEAR);


    ASSERT_NEAR(cagr/noisy_cagr_1, 1, 0.1);
    ASSERT_NEAR(cagr/noisy_cagr_2, 1, 0.1);
}

TEST_F(OriginalStatsTest, test_cagr_9){
    double cagr = annual_return_from_simple_return(pos_line_value, "daily", APPROX_BDAYS_PER_YEAR);
    double noisy_cagr_1 = annual_return_from_simple_return(cal_func::vector_add_vector(pos_line_value,noise_uniform_value),
                                                           "daily", APPROX_BDAYS_PER_YEAR);

    double noisy_cagr_2 = annual_return_from_simple_return(cal_func::vector_minus_vector(pos_line_value,noise_uniform_value),
                                                           "daily", APPROX_BDAYS_PER_YEAR);


    ASSERT_NEAR(cagr/noisy_cagr_1, 1, 0.1);
    ASSERT_NEAR(cagr/noisy_cagr_2, 1, 0.1);
}

TEST_F(OriginalStatsTest, test_cagr_10){
    double cagr = annual_return_from_simple_return(flat_line1_tz_value, "daily", APPROX_BDAYS_PER_YEAR);
    std::vector<double> v1 = cal_func::vector_add_vector(flat_line1_tz_value,noise_value);
    std::cout << "v1 size = " << v1.size() << std::endl;
    double noisy_cagr_1 = annual_return_from_simple_return(v1,
                                                           "daily", APPROX_BDAYS_PER_YEAR);

    double noisy_cagr_2 = annual_return_from_simple_return(cal_func::vector_minus_vector(flat_line1_tz_value,noise_value),
                                                           "daily", APPROX_BDAYS_PER_YEAR);


    ASSERT_NEAR(cagr/noisy_cagr_1, 1, 0.1);
    ASSERT_NEAR(cagr/noisy_cagr_2, 1, 0.1);
}

TEST_F(OriginalStatsTest, test_beta_fragility_heuristic_1){
    double actual_value = calculate_beta_fragility_heuristic(
            one_returns_value, one_returns_value);
    ASSERT_TRUE(std::isnan(actual_value));
}

TEST_F(OriginalStatsTest, test_beta_fragility_heuristic_2){
    double actual_value = calculate_beta_fragility_heuristic(
            positive_returns_value, simple_benchmark_value);
    //ASSERT_TRUE(std::isnan(actual_value));
    double expect_value = 0;
    ASSERT_NEAR(actual_value, expect_value, 0.000001);
}

TEST_F(OriginalStatsTest, test_beta_fragility_heuristic_3){
    double actual_value = calculate_beta_fragility_heuristic(
            mixed_returns_value, simple_benchmark_value);
    //ASSERT_TRUE(std::isnan(actual_value));
    double expect_value = 0.09;
    ASSERT_NEAR(actual_value, expect_value, 0.000001);
}

TEST_F(OriginalStatsTest, test_beta_fragility_heuristic_4){
    double actual_value = calculate_beta_fragility_heuristic(
            negative_returns_value, simple_benchmark_value);
    //ASSERT_TRUE(std::isnan(actual_value));
    double expect_value = -0.029999999999999999;
    ASSERT_NEAR(actual_value, expect_value, 0.000001);
}



