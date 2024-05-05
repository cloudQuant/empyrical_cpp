
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
        (empty_returns, 6, []),
        (negative_returns, 6, [-18.09162052, -26.79897486, -26.69138263,
                               -25.72298838]),
        (mixed_returns, 6, [7.57445259, 8.22784105, 8.22784105, -3.1374751])
    ])
    def test_roll_sharpe_ratio(self, returns, window, expected):
        test = self.empyrical.roll_sharpe_ratio(returns, window=window)
        assert_almost_equal(
            np.asarray(test),
            np.asarray(expected),
            DECIMAL_PLACES)

        self.assert_indexes_match(test, returns[-len(expected):])

    @parameterized.expand([
        (empty_returns, empty_returns, np.nan),
        (one_return, one_return, 1.),
        (mixed_returns, mixed_returns, 1.),
        (all_negative_returns, mixed_returns, -0.52257643222960259)
    ])
    def test_capture_ratio(self, returns, factor_returns, expected):
        assert_almost_equal(
            self.empyrical.capture(returns, factor_returns),
            expected,
            DECIMAL_PLACES)

    @parameterized.expand([
        (empty_returns, empty_returns, np.nan),
        (one_return, one_return, np.nan),
        (mixed_returns, mixed_returns, 1.),
        (all_negative_returns, mixed_returns, 0.99956025703798634),
        (positive_returns, mixed_returns, -11.27400221)
    ])
    def test_down_capture(self, returns, factor_returns, expected):
        assert_almost_equal(
            self.empyrical.down_capture(returns, factor_returns),
            expected,
            DECIMAL_PLACES)

    @parameterized.expand([
        (empty_returns, simple_benchmark,
         1,
         [(np.nan, np.nan)] * len(simple_benchmark)),
        (one_return, one_return, 1, [(np.nan, np.nan)]),
        (mixed_returns, negative_returns,
         6, [(-0.97854954, -0.7826087), (-0.9828927, -0.76156584),
             (-0.93166924, -0.61682243), (-0.99967288, -0.41311475)]),
        (mixed_returns, mixed_returns,
         6, [(0.0, 1.0), (0.0, 1.0), (0.0, 1.0), (0.0, 1.0)]),
        (mixed_returns, -mixed_returns,
         6,
         [(0.0, -1.0), (0.0, -1.0), (0.0, -1.0), (0.0, -1.0)]),
    ])
    def test_roll_alpha_beta(self, returns, benchmark, window, expected):
        test = self.empyrical(
            return_types=(np.ndarray, pd.DataFrame),
        ).roll_alpha_beta(
            returns,
            benchmark,
            window,
        )
        if isinstance(test, pd.DataFrame):
            self.assert_indexes_match(test, benchmark[-len(expected):])
            test = test.values

        alpha_test = [t[0] for t in test]
        beta_test = [t[1] for t in test]

        alpha_expected = [t[0] for t in expected]
        beta_expected = [t[1] for t in expected]

        assert_almost_equal(
            np.asarray(alpha_test),
            np.asarray(alpha_expected),
            DECIMAL_PLACES,
        )

        assert_almost_equal(
            np.asarray(beta_test),
            np.asarray(beta_expected),
            DECIMAL_PLACES,
        )

    @parameterized.expand([
        (empty_returns, empty_returns, 1, []),
        (one_return, one_return, 1,  np.nan),
        (mixed_returns, mixed_returns, 6, [1., 1., 1., 1.]),
        (positive_returns, mixed_returns,
         6, [-0.00011389, -0.00025861, -0.00015211, -0.00689239]),
        (all_negative_returns, mixed_returns,
         6, [-6.38880246e-05, -1.65241701e-04, -1.65241719e-04,
             -6.89541957e-03])
    ])
    def test_roll_up_down_capture(self, returns, factor_returns, window,
                                  expected):
        test = self.empyrical.roll_up_down_capture(returns, factor_returns,
                                                   window=window)
        assert_almost_equal(
            np.asarray(test),
            np.asarray(expected),
            DECIMAL_PLACES)

    @parameterized.expand([
        (empty_returns, empty_returns, 1, []),
        (one_return, one_return, 1,  [np.nan]),
        (mixed_returns, mixed_returns, 6, [1., 1., 1., 1.]),
        (positive_returns, mixed_returns,
         6, [-11.2743862, -11.2743862, -11.2743862, -11.27400221]),
        (all_negative_returns, mixed_returns,
         6, [0.92058591, 0.92058591, 0.92058591, 0.99956026])
    ])
    def test_roll_down_capture(self, returns, factor_returns, window,
                               expected):
        test = self.empyrical.roll_down_capture(returns, factor_returns,
                                                window=window)
        assert_almost_equal(
            np.asarray(test),
            np.asarray(expected),
            DECIMAL_PLACES)

        self.assert_indexes_match(test, returns[-len(expected):])

    @parameterized.expand([
        (empty_returns, empty_returns, 1, []),
        (one_return, one_return, 1,  [1.]),
        (mixed_returns, mixed_returns, 6, [1., 1., 1., 1.]),
        (positive_returns, mixed_returns,
         6, [0.00128406, 0.00291564, 0.00171499, 0.0777048]),
        (all_negative_returns, mixed_returns,
         6, [-5.88144154e-05, -1.52119182e-04, -1.52119198e-04,
             -6.89238735e-03])
    ])
    def test_roll_up_capture(self, returns, factor_returns, window, expected):
        test = self.empyrical.roll_up_capture(returns, factor_returns,
                                              window=window)
        assert_almost_equal(
            np.asarray(test),
            np.asarray(expected),
            DECIMAL_PLACES)

        self.assert_indexes_match(test, returns[-len(expected):])

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

    @property
    def empyrical(self):
        """
        This returns a wrapper around the empyrical module, so tests can
        perform input conversions or return type checks on each call to an
        empyrical function.

        Each test case subclass can override this property, so that all the
        same tests are run, but with different function inputs or type checks.

        This was done as part of enabling empyrical functions to work with
        inputs of either pd.Series or np.ndarray, with the expectation that
        they will return the same type as their input.

        Returns
        -------
        empyrical

        Notes
        -----
        Since some parameterized test parameters refer to attributes on the
        real empyrical module at class body scope, this property must be
        defined later in the body than those references. That way, the
        attributes are looked up on the empyrical module, not this property.

        e.g. empyrical.DAILY
        """
        return ReturnTypeEmpyricalProxy(self, (pd.Series, float))


class TestStatsArrays(TestStats):
    """
    Tests pass np.ndarray inputs to empyrical and assert that outputs are of
    type np.ndarray or float.

    """
    @property
    def empyrical(self):
        return PassArraysEmpyricalProxy(self, (np.ndarray, float))

    def assert_indexes_match(self, result, expected):
        pass


class TestStatsIntIndex(TestStats):
    """
    Tests pass int-indexed pd.Series inputs to empyrical and assert that
    outputs are of type pd.Series or float.

    This prevents a regression where we're indexing with ints into a pd.Series
    to find the last item and get a KeyError when the series is int-indexed.

    """
    @property
    def empyrical(self):
        return ConvertPandasEmpyricalProxy(
            self,
            (pd.Series, float),
            lambda obj: type(obj)(obj.values, index=np.arange(len(obj))),
        )

    def assert_indexes_match(self, result, expected):
        pass


class TestHelpers(BaseTestCase):
    """
    Tests for helper methods and utils.
    """

    def setUp(self):

        self.ser_length = 120
        self.window = 12

        self.returns = pd.Series(
            rand.randn(1, 120)[0]/100.,
            index=pd.date_range('2000-1-30', periods=120, freq='M'))

        self.factor_returns = pd.Series(
            rand.randn(1, 120)[0]/100.,
            index=pd.date_range('2000-1-30', periods=120, freq='M'))

    def test_roll_pandas(self):
        res = emutils.roll(self.returns,
                           self.factor_returns,
                           window=12,
                           function=empyrical.alpha_aligned)

        self.assertEqual(res.size, self.ser_length - self.window + 1)

    def test_roll_ndarray(self):
        res = emutils.roll(self.returns.values,
                           self.factor_returns.values,
                           window=12,
                           function=empyrical.alpha_aligned)

        self.assertEqual(len(res), self.ser_length - self.window + 1)

    def test_down(self):
        pd_res = emutils.down(self.returns, self.factor_returns,
                              function=empyrical.capture)
        np_res = emutils.down(self.returns.values, self.factor_returns.values,
                              function=empyrical.capture)

        self.assertTrue(isinstance(pd_res, float))
        assert_almost_equal(pd_res, np_res, DECIMAL_PLACES)

    def test_up(self):
        pd_res = emutils.up(self.returns, self.factor_returns,
                            function=empyrical.capture)
        np_res = emutils.up(self.returns.values, self.factor_returns.values,
                            function=empyrical.capture)

        self.assertTrue(isinstance(pd_res, float))
        assert_almost_equal(pd_res, np_res, DECIMAL_PLACES)

    def test_roll_bad_types(self):
        with self.assertRaises(ValueError):
            emutils.roll(self.returns.values,
                         self.factor_returns,
                         window=12,
                         function=empyrical.max_drawdown)

    def test_roll_max_window(self):
        res = emutils.roll(self.returns,
                           self.factor_returns,
                           window=self.ser_length + 100,
                           function=empyrical.max_drawdown)
        self.assertTrue(res.size == 0)


