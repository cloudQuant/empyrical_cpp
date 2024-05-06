
class TestStats(BaseTestCase):


    @parameterized.expand([
        (simple_benchmark, empyrical.WEEKLY, [0.0,
                                              0.040604010000000024,
                                              0.0]),
        (simple_benchmark, empyrical.MONTHLY, [0.01,
                                               0.03030099999999991]),
        (simple_benchmark, empyrical.QUARTERLY, [0.04060401]),
        (simple_benchmark, empyrical.YEARLY, [0.040604010000000024]),
        (weekly_returns, empyrical.MONTHLY, [0.0, 0.087891200000000058,
                                             -0.04500459999999995]),
        (weekly_returns, empyrical.YEARLY, [0.038931091700480147]),
        (monthly_returns, empyrical.YEARLY, [0.038931091700480147]),
        (monthly_returns, empyrical.QUARTERLY, [0.11100000000000021,
                                                0.008575999999999917,
                                                -0.072819999999999996])
    ])
    def test_aggregate_returns(self, returns, convert_to, expected):
        returns = self.empyrical(pandas_only=True).aggregate_returns(
            returns, convert_to
        ).values.tolist()
        for i, v in enumerate(returns):
            assert_almost_equal(
                v,
                expected[i],
                DECIMAL_PLACES)


    @parameterized.expand([
        (empty_returns, simple_benchmark, (np.nan, np.nan)),
        (one_return, one_return, (np.nan, np.nan)),
        (mixed_returns[1:], negative_returns[1:],
         (-0.9997853834885004, -0.71296296296296313)),
        (mixed_returns, mixed_returns, (0.0, 1.0)),
        (mixed_returns, -mixed_returns, (0.0, -1.0))
    ])
    def test_down_alpha_beta(self, returns, benchmark, expected):
        down_alpha, down_beta = self.empyrical(
            pandas_only=len(returns) != len(benchmark),
            return_types=np.ndarray,
        ).down_alpha_beta(returns, benchmark)
        assert_almost_equal(
            down_alpha,
            expected[0],
            DECIMAL_PLACES)
        assert_almost_equal(
            down_beta,
            expected[1],
            DECIMAL_PLACES)

    @parameterized.expand([
        (empty_returns, simple_benchmark, (np.nan, np.nan)),
        (one_return, one_return, (np.nan, np.nan)),
        (mixed_returns[1:], positive_returns[1:],
         (0.432961242076658, 0.4285714285)),
        (mixed_returns, mixed_returns, (0.0, 1.0)),
        (mixed_returns, -mixed_returns, (0.0, -1.0))
    ])
    def test_up_alpha_beta(self, returns, benchmark, expected):
        up_alpha, up_beta = self.empyrical(
            pandas_only=len(returns) != len(benchmark),
            return_types=np.ndarray,
        ).up_alpha_beta(returns, benchmark)
        assert_almost_equal(
            up_alpha,
            expected[0],
            DECIMAL_PLACES)
        assert_almost_equal(
            up_beta,
            expected[1],
            DECIMAL_PLACES)

    @parameterized.expand([
        (empty_returns, empty_returns, np.nan),
        (one_return, one_return, np.nan),
        (mixed_returns, mixed_returns, 1.),
        (positive_returns, mixed_returns, -0.0006756053495),
        (all_negative_returns, mixed_returns, -0.0004338236)
    ])
    def test_up_down_capture(self, returns, factor_returns, expected):
        assert_almost_equal(
            self.empyrical.up_down_capture(returns, factor_returns),
            expected,
            DECIMAL_PLACES)

    @parameterized.expand([
        (empty_returns, empty_returns, np.nan),
        (one_return, one_return, 1.),
        (mixed_returns, mixed_returns, 1.),
        (positive_returns, mixed_returns, 0.0076167762),
        (all_negative_returns, mixed_returns, -0.0004336328)
    ])
    def test_up_capture(self, returns, factor_returns, expected):
        assert_almost_equal(
            self.empyrical.up_capture(returns, factor_returns),
            expected,
            DECIMAL_PLACES)

    def test_value_at_risk(self):
        value_at_risk = self.empyrical.value_at_risk

        returns = [1.0, 2.0]
        assert_almost_equal(value_at_risk(returns, cutoff=0.0), 1.0)
        assert_almost_equal(value_at_risk(returns, cutoff=0.3), 1.3)
        assert_almost_equal(value_at_risk(returns, cutoff=1.0), 2.0)

        returns = [1, 81, 82, 83, 84, 85]
        assert_almost_equal(value_at_risk(returns, cutoff=0.1), 41)
        assert_almost_equal(value_at_risk(returns, cutoff=0.2), 81)
        assert_almost_equal(value_at_risk(returns, cutoff=0.3), 81.5)

        # Test a returns stream of 21 data points at different cutoffs.
        returns = rand.normal(0, 0.02, 21)
        for cutoff in (0, 0.0499, 0.05, 0.20, 0.999, 1):
            assert_almost_equal(
                value_at_risk(returns, cutoff),
                np.percentile(returns, cutoff * 100),
            )

    def test_conditional_value_at_risk(self):
        value_at_risk = self.empyrical.value_at_risk
        conditional_value_at_risk = self.empyrical.conditional_value_at_risk

        # A single-valued array will always just have a CVaR of its only value.
        returns = rand.normal(0, 0.02, 1)
        expected_cvar = returns[0]
        assert_almost_equal(
            conditional_value_at_risk(returns, cutoff=0), expected_cvar,
        )
        assert_almost_equal(
            conditional_value_at_risk(returns, cutoff=1), expected_cvar,
        )

        # Test a returns stream of 21 data points at different cutoffs.
        returns = rand.normal(0, 0.02, 21)

        for cutoff in (0, 0.0499, 0.05, 0.20, 0.999, 1):
            # Find the VaR based on our cutoff, then take the average of all
            # values at or below the VaR.
            var = value_at_risk(returns, cutoff)
            expected_cvar = np.mean(returns[returns <= var])

            assert_almost_equal(
                conditional_value_at_risk(returns, cutoff), expected_cvar,
            )




