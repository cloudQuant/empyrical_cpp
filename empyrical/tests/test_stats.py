from __future__ import division

from copy import copy
from operator import attrgetter
from unittest import TestCase, SkipTest

from parameterized import parameterized
import numpy as np
from numpy.testing import assert_almost_equal, assert_allclose
import pandas as pd
from pandas.core.generic import NDFrame
from scipy import stats
from six import iteritems, wraps

try:
    from pandas.testing import assert_index_equal
except ImportError:
    # This moved in pandas 0.20.
    from pandas.util.testing import assert_index_equal

import empyrical
import empyrical.utils as emutils

DECIMAL_PLACES = 8


rand = np.random.RandomState(1337)


class BaseTestCase(TestCase):
    # def assert_indexes_match(self, result, expected):
    #     """
    #     Assert that two pandas objects have the same indices.
    #
    #     This is a method instead of a free function so that we can override it
    #     to be a no-op in suites like TestStatsArrays that unwrap pandas objects
    #     into ndarrays.
    #     """
    #     assert_index_equal(result.index, expected.index)
    #
    #     if isinstance(result, pd.DataFrame) and \
    #        isinstance(expected, pd.DataFrame):
    #         assert_index_equal(result.columns, expected.columns)


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


    # Regressive tests for information ratio
    @parameterized.expand([
        (empty_returns, 0.0, np.nan),
        (one_return, 0.0, np.nan),
        (pos_line, pos_line, np.nan),
        (mixed_returns, 0.0, 0.10859306069076737),
        (mixed_returns, flat_line_1, -0.06515583641446039),
    ])
    def test_excess_sharpe(self, returns, factor_returns, expected):
        assert_almost_equal(
            self.empyrical.excess_sharpe(returns, factor_returns),
            expected,
            DECIMAL_PLACES)

    # The magnitude of the information ratio increases as a higher
    # proportion of returns are uncorrelated with the benchmark.
    @parameterized.expand([
        (flat_line_0, pos_line),
        (flat_line_1_tz, pos_line),
        (noise, pos_line)
    ])
    def test_excess_sharpe_noisy(self, noise_line, benchmark):
        noisy_returns_1 = noise_line[0:250].add(benchmark[250:], fill_value=0)
        noisy_returns_2 = noise_line[0:500].add(benchmark[500:], fill_value=0)
        noisy_returns_3 = noise_line[0:750].add(benchmark[750:], fill_value=0)
        ir_1 = self.empyrical.excess_sharpe(noisy_returns_1, benchmark)
        ir_2 = self.empyrical.excess_sharpe(noisy_returns_2, benchmark)
        ir_3 = self.empyrical.excess_sharpe(noisy_returns_3, benchmark)
        assert abs(ir_1) < abs(ir_2)
        assert abs(ir_2) < abs(ir_3)

    # Vertical translations change the information ratio in the
    # direction of the translation.
    @parameterized.expand([
        (pos_line, noise, flat_line_1_tz),
        (pos_line, inv_noise, flat_line_1_tz),
        (neg_line, noise, flat_line_1_tz),
        (neg_line, inv_noise, flat_line_1_tz)
    ])
    def test_excess_sharpe_trans(self, returns, add_noise, translation):
        ir = self.empyrical.excess_sharpe(
            returns+add_noise,
            returns
        )
        raised_ir = self.empyrical.excess_sharpe(
            returns+add_noise+translation,
            returns
        )
        depressed_ir = self.empyrical.excess_sharpe(
            returns+add_noise-translation,
            returns
        )
        assert ir < raised_ir
        assert depressed_ir < ir

    @parameterized.expand([
        (empty_returns, simple_benchmark, (np.nan, np.nan)),
        (one_return, one_return, (np.nan, np.nan)),
        (mixed_returns, negative_returns[1:], (-0.9997853834885004,
                                               -0.71296296296296313)),
        (mixed_returns, mixed_returns, (0.0, 1.0)),
        (mixed_returns, -mixed_returns, (0.0, -1.0)),
    ])
    def test_alpha_beta(self, returns, benchmark, expected):
        alpha, beta = self.empyrical(
            pandas_only=len(returns) != len(benchmark),
            return_types=np.ndarray,
        ).alpha_beta(returns, benchmark)
        assert_almost_equal(
            alpha,
            expected[0],
            DECIMAL_PLACES)
        assert_almost_equal(
            beta,
            expected[1],
            DECIMAL_PLACES)

    # Regression tests for alpha
    @parameterized.expand([
        (empty_returns, simple_benchmark, np.nan),
        (one_return, one_return, np.nan),
        (mixed_returns, flat_line_1, np.nan),
        (mixed_returns, mixed_returns, 0.0),
        (mixed_returns, -mixed_returns, 0.0),
    ])
    def test_alpha(self, returns, benchmark, expected):
        observed = self.empyrical.alpha(returns, benchmark)
        assert_almost_equal(
            observed,
            expected,
            DECIMAL_PLACES)

        if len(returns) == len(benchmark):
            # Compare to scipy linregress
            returns_arr = returns.values
            benchmark_arr = benchmark.values
            mask = ~np.isnan(returns_arr) & ~np.isnan(benchmark_arr)
            slope, intercept, _, _, _ = stats.linregress(benchmark_arr[mask],
                                                         returns_arr[mask])

            assert_almost_equal(
                observed,
                intercept * 252,
                DECIMAL_PLACES
            )

    # Alpha/beta translation tests.
    @parameterized.expand([
        (0, .001),
        (0.01, .001),
    ])
    def test_alpha_beta_translation(self, mean_returns, translation):
        # Generate correlated returns and benchmark.
        std_returns = 0.01
        correlation = 0.8
        std_bench = .001
        means = [mean_returns, .001]
        covs = [[std_returns**2, std_returns*std_bench*correlation],
                [std_returns*std_bench*correlation, std_bench**2]]
        (ret, bench) = rand.multivariate_normal(means, covs, 1000).T
        returns = pd.Series(
            ret,
            index=pd.date_range('2000-1-30', periods=1000, freq='D'))
        benchmark = pd.Series(
            bench,
            index=pd.date_range('2000-1-30', periods=1000, freq='D'))

        # Translate returns and generate alphas and betas.
        returns_depressed = returns-translation
        returns_raised = returns+translation
        alpha_beta = self.empyrical(return_types=np.ndarray).alpha_beta
        (alpha_depressed, beta_depressed) = alpha_beta(
            returns_depressed, benchmark)
        (alpha_standard, beta_standard) = alpha_beta(
            returns, benchmark)
        (alpha_raised, beta_raised) = alpha_beta(
            returns_raised, benchmark)
        # Alpha should change proportionally to how much returns were
        # translated.
        assert_almost_equal(
            ((alpha_standard + 1) ** (1/252)) -
            ((alpha_depressed + 1) ** (1/252)),
            translation,
            DECIMAL_PLACES)
        assert_almost_equal(
            ((alpha_raised + 1) ** (1/252)) -
            ((alpha_standard + 1) ** (1/252)),
            translation,
            DECIMAL_PLACES)
        # Beta remains constant.
        assert_almost_equal(
            beta_standard,
            beta_depressed,
            DECIMAL_PLACES)
        assert_almost_equal(
            beta_standard,
            beta_raised,
            DECIMAL_PLACES)

    # Test alpha/beta with a smaller and larger correlation values.
    @parameterized.expand([
        (.1, .9)
    ])
    def test_alpha_beta_correlation(self, corr_less, corr_more):
        mean_returns = 0.01
        mean_bench = .001
        std_returns = 0.01
        std_bench = .001
        index = pd.date_range('2000-1-30', periods=1000, freq='D')
        # Generate less correlated returns
        means_less = [mean_returns, mean_bench]
        covs_less = [[std_returns**2, std_returns*std_bench*corr_less],
                     [std_returns*std_bench*corr_less, std_bench**2]]
        (ret_less, bench_less) = rand.multivariate_normal(
            means_less, covs_less, 1000).T
        returns_less = pd.Series(ret_less, index=index)
        benchmark_less = pd.Series(bench_less, index=index)
        # Genereate more highly correlated returns
        means_more = [mean_returns, mean_bench]
        covs_more = [[std_returns**2, std_returns*std_bench*corr_more],
                     [std_returns*std_bench*corr_more, std_bench**2]]
        (ret_more, bench_more) = rand.multivariate_normal(
            means_more, covs_more, 1000).T
        returns_more = pd.Series(ret_more, index=index)
        benchmark_more = pd.Series(bench_more, index=index)
        # Calculate alpha/beta values
        alpha_beta = self.empyrical(return_types=np.ndarray).alpha_beta
        alpha_less, beta_less = alpha_beta(returns_less, benchmark_less)
        alpha_more, beta_more = alpha_beta(returns_more, benchmark_more)
        # Alpha determines by how much returns vary from the benchmark return.
        # A lower correlation leads to higher alpha.
        assert alpha_less > alpha_more
        # Beta measures the volatility of returns against benchmark returns.
        # Beta increases proportionally to correlation.
        assert beta_less < beta_more

    # When faced with data containing np.nan, do not return np.nan. Calculate
    # alpha and beta using dates containing both.
    @parameterized.expand([
        (sparse_noise, sparse_noise),
    ])
    def test_alpha_beta_with_nan_inputs(self, returns, benchmark):
        alpha, beta = self.empyrical(return_types=np.ndarray).alpha_beta(
            returns,
            benchmark,
        )
        self.assertFalse(np.isnan(alpha))
        self.assertFalse(np.isnan(beta))

    @parameterized.expand([
        (empty_returns, simple_benchmark, np.nan),
        (one_return, one_return,  np.nan),
        (mixed_returns, flat_line_1, np.nan),
        (noise, noise, 1.0),
        (2 * noise, noise, 2.0),
        (noise, inv_noise, -1.0),
        (2 * noise, inv_noise, -2.0),
        (sparse_noise * flat_line_1_tz, sparse_flat_line_1_tz, np.nan),
        (
            simple_benchmark + rand.normal(0, 0.001, len(simple_benchmark)),
            pd.DataFrame({'returns': simple_benchmark}),
            1.0,
            2,
        ),
    ])
    def test_beta(self,
                  returns,
                  benchmark,
                  expected,
                  decimal_places=DECIMAL_PLACES):
        observed = self.empyrical.beta(returns, benchmark)
        assert_almost_equal(
            observed,
            expected,
            decimal_places,
        )

        if len(returns) == len(benchmark):
            # Compare to scipy linregress

            if isinstance(benchmark, pd.DataFrame):
                benchmark = benchmark['returns']

            returns_arr = returns.values
            benchmark_arr = benchmark.values
            mask = ~np.isnan(returns_arr) & ~np.isnan(benchmark_arr)
            slope, intercept, _, _, _ = stats.linregress(benchmark_arr[mask],
                                                         returns_arr[mask])

            assert_almost_equal(
                observed,
                slope
            )

    @parameterized.expand([
        (empty_returns, simple_benchmark),
        (one_return, one_return),
        (mixed_returns, simple_benchmark[1:]),
        (mixed_returns, negative_returns[1:]),
        (mixed_returns, mixed_returns),
        (mixed_returns, -mixed_returns),
    ])
    def test_alpha_beta_equality(self, returns, benchmark):
        alpha, beta = self.empyrical(
            pandas_only=len(returns) != len(benchmark),
            return_types=np.ndarray,
        ).alpha_beta(returns, benchmark)
        assert_almost_equal(
            alpha,
            self.empyrical.alpha(returns, benchmark),
            DECIMAL_PLACES)
        assert_almost_equal(
            beta,
            self.empyrical.beta(returns, benchmark),
            DECIMAL_PLACES
        )

        if len(returns) == len(benchmark):
            # Compare to scipy linregress
            returns_arr = returns.values
            benchmark_arr = benchmark.values
            mask = ~np.isnan(returns_arr) & ~np.isnan(benchmark_arr)
            slope, intercept, _, _, _ = stats.linregress(returns_arr[mask],
                                                         benchmark_arr[mask])

            assert_almost_equal(
                alpha,
                intercept
            )
            assert_almost_equal(
                beta,
                slope
            )

    @parameterized.expand([
        (empty_returns, np.nan),
        (one_return, np.nan),
        (mixed_returns, 0.1529973665111273),
        (flat_line_1_tz, 1.0),
    ])
    def test_stability_of_timeseries(self, returns, expected):
        assert_almost_equal(
            self.empyrical.stability_of_timeseries(returns),
            expected,
            DECIMAL_PLACES)

    @parameterized.expand([
        (empty_returns, np.nan),
        (one_return, 1.0),
        (mixed_returns, 0.9473684210526313),
        (pd.Series(rand.randn(100000)), 1.),
    ])
    def test_tail_ratio(self, returns, expected):
        assert_almost_equal(
            self.empyrical.tail_ratio(returns),
            expected,
            1)

    # Regression tests for CAGR.
    @parameterized.expand([
        (empty_returns, empyrical.DAILY, np.nan),
        (one_return, empyrical.DAILY, 11.274002099240244),
        (mixed_returns, empyrical.DAILY, 1.9135925373194231),
        (flat_line_1_tz, empyrical.DAILY, 11.274002099240256),
        (pd.Series(np.array(
            [3., 3., 3.])/100,
            index=pd.date_range('2000-1-30', periods=3, freq='A')
        ), 'yearly', 0.03)
    ])
    def test_cagr(self, returns, period, expected):
        assert_almost_equal(
            self.empyrical.cagr(returns, period=period),
            expected,
            DECIMAL_PLACES)

    # CAGR is calculated by the starting and ending value of returns,
    # translating returns by a constant will change cagr in the same
    # direction of translation.
    @parameterized.expand([
        (noise, .01),
        (noise_uniform, .01)
    ])
    def test_cagr_translation(self, returns, constant):
        cagr_depressed = self.empyrical.cagr(returns-constant)
        cagr_unchanged = self.empyrical.cagr(returns)
        cagr_raised = self.empyrical.cagr(returns+constant)
        self.assertTrue(cagr_depressed < cagr_unchanged)
        self.assertTrue(cagr_unchanged < cagr_raised)

    # Function does not return np.nan when inputs contain np.nan.
    @parameterized.expand([
        (sparse_noise,)
    ])
    def test_cagr_with_nan_inputs(self, returns):
        self.assertFalse(np.isnan(self.empyrical.cagr(returns)))

    # Adding noise to returns should not significantly alter the cagr values.
    # Confirm that adding noise does not change cagr values to one
    # significant digit
    @parameterized.expand([
        (pos_line, noise),
        (pos_line, noise_uniform),
        (flat_line_1_tz, noise)
    ])
    def test_cagr_noisy(self, returns, add_noise):
        cagr = self.empyrical.cagr(returns)
        noisy_cagr_1 = self.empyrical.cagr(returns+add_noise)
        noisy_cagr_2 = self.empyrical.cagr(returns-add_noise)
        np.testing.assert_approx_equal(
            cagr,
            noisy_cagr_1,
            1)
        np.testing.assert_approx_equal(
            cagr,
            noisy_cagr_2,
            1)

    # regression tests for beta_fragility_heuristic
    @parameterized.expand([
        (one_return, one_return, np.nan),
        (positive_returns, simple_benchmark, 0.0),
        (mixed_returns, simple_benchmark, 0.09),
        (negative_returns, simple_benchmark, -0.029999999999999999),
    ])
    def test_beta_fragility_heuristic(self, returns, factor_returns, expected):
        assert_almost_equal(
            self.empyrical.beta_fragility_heuristic(returns, factor_returns),
            expected,
            DECIMAL_PLACES)

    mixed_returns_expected_gpd_risk_result = [0.1,
                                              0.10001255835838491,
                                              1.5657360018514067e-06,
                                              0.4912526273742347,
                                              0.59126595492541179]

    negative_returns_expected_gpd_risk_result = [0.05,
                                                 0.068353586736348199,
                                                 9.4304947982121171e-07,
                                                 0.34511639904932639,
                                                 0.41347032855617882]

    # regression tests for gpd_risk_estimates
    @parameterized.expand([
        (one_return, [0, 0, 0, 0, 0]),
        (empty_returns, [0, 0, 0, 0, 0]),
        (simple_benchmark, [0, 0, 0, 0, 0]),
        (positive_returns, [0, 0, 0, 0, 0]),
        (negative_returns, negative_returns_expected_gpd_risk_result),
        (mixed_returns, mixed_returns_expected_gpd_risk_result),
        (flat_line_1, [0, 0, 0, 0]),
        (weekly_returns, mixed_returns_expected_gpd_risk_result),
        (monthly_returns, mixed_returns_expected_gpd_risk_result),
    ])
    def test_gpd_risk_estimates(self, returns, expected):
        result = self.empyrical.gpd_risk_estimates_aligned(returns)
        for result_item, expected_item in zip(result, expected):
            assert_almost_equal(
                result_item,
                expected_item,
                DECIMAL_PLACES)

    @parameterized.expand([
        (empty_returns, 6, []),
        (negative_returns, 6, [-0.2282, -0.2745, -0.2899, -0.2747])
    ])
    def test_roll_max_drawdown(self, returns, window, expected):
        test = self.empyrical.roll_max_drawdown(returns, window=window)
        assert_almost_equal(
            np.asarray(test),
            np.asarray(expected),
            4)

        self.assert_indexes_match(test, returns[-len(expected):])

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


class Test2DStats(BaseTestCase):
    """
    Tests for functions that are capable of outputting a DataFrame.
    """

    input_one = [np.nan, 0.01322056, 0.03063862, -0.01422057,
                 -0.00489779, 0.01268925, -0.03357711, 0.01797036]
    input_two = [0.01846232, 0.00793951, -0.01448395, 0.00422537,
                 -0.00339611, 0.03756813, 0.0151531, np.nan]

    expected_0_one = [0.000000, 0.013221, 0.044264, 0.029414, 0.024372,
                      0.037371, 0.002539, 0.020555]
    expected_0_two = [0.018462, 0.026548, 0.011680, 0.015955, 0.012504,
                      0.050542, 0.066461, 0.066461]

    expected_100_one = [100.000000, 101.322056, 104.426424, 102.941421,
                        102.437235, 103.737087, 100.253895, 102.055494]
    expected_100_two = [101.846232, 102.654841, 101.167994, 101.595466,
                        101.250436, 105.054226, 106.646123, 106.646123]

    df_index = pd.date_range('2000-1-30', periods=8, freq='D')

    df_input = pd.DataFrame({
        'one': pd.Series(input_one, index=df_index),
        'two': pd.Series(input_two, index=df_index)})

    df_empty = pd.DataFrame()

    df_0_expected = pd.DataFrame({
        'one': pd.Series(expected_0_one, index=df_index),
        'two': pd.Series(expected_0_two, index=df_index)})

    df_100_expected = pd.DataFrame({
        'one': pd.Series(expected_100_one, index=df_index),
        'two': pd.Series(expected_100_two, index=df_index)})

    @parameterized.expand([
        (df_input, 0, df_0_expected),
        (df_input, 100, df_100_expected),
        (df_empty, 0, pd.DataFrame())
    ])
    def test_cum_returns_df(self, returns, starting_value, expected):
        cum_returns = self.empyrical.cum_returns(
            returns,
            starting_value=starting_value,
        )

        assert_almost_equal(
            np.asarray(cum_returns),
            np.asarray(expected),
            4,
        )

        self.assert_indexes_match(cum_returns, returns)

    @parameterized.expand([
        (df_input, 0, df_0_expected.iloc[-1]),
        (df_input, 100, df_100_expected.iloc[-1]),
    ])
    def test_cum_returns_final_df(self, returns, starting_value, expected):
        return_types = (pd.Series, np.ndarray)
        result = self.empyrical(return_types=return_types).cum_returns_final(
            returns,
            starting_value=starting_value,
        )
        assert_almost_equal(np.array(result), expected, 5)
        self.assert_indexes_match(result, expected)

    @property
    def empyrical(self):
        """
        Returns a wrapper around the empyrical module so tests can
        perform input conversions or return type checks on each call to an
        empyrical function. See full explanation in TestStats.

        Returns
        -------
        empyrical

        """

        return ReturnTypeEmpyricalProxy(self, pd.DataFrame)


class Test2DStatsArrays(Test2DStats):
    """
    Tests pass np.ndarray inputs to empyrical and assert that outputs are of
    type np.ndarray.

    """
    @property
    def empyrical(self):
        return PassArraysEmpyricalProxy(self, np.ndarray)

    def assert_indexes_match(self, result, expected):
        pass


class ReturnTypeEmpyricalProxy(object):
    """
    A wrapper around the empyrical module which, on each function call, asserts
    that the type of the return value is in a given set.

    Also asserts that inputs were not modified by the empyrical function call.

    Calling an instance with kwargs will return a new copy with those
    attributes overridden.

    """
    def __init__(self, test_case, return_types):
        self._test_case = test_case
        self._return_types = return_types

    def __call__(self, **kwargs):
        dupe = copy(self)

        for k, v in iteritems(kwargs):
            attr = '_' + k
            if hasattr(dupe, attr):
                setattr(dupe, attr, v)

        return dupe

    def __copy__(self):
        newone = type(self).__new__(type(self))
        newone.__dict__.update(self.__dict__)
        return newone

    def __getattr__(self, item):
        return self._check_input_not_mutated(
            self._check_return_type(
                getattr(empyrical, item)
            )
        )

    def _check_return_type(self, func):
        @wraps(func)
        def check_return_type(*args, **kwargs):
            result = func(*args, **kwargs)
            if isinstance(result, tuple):
                tuple_result = result
            else:
                tuple_result = (result,)

            for r in tuple_result:
                self._test_case.assertIsInstance(r, self._return_types)
            return result

        return check_return_type

    def _check_input_not_mutated(self, func):
        @wraps(func)
        def check_not_mutated(*args, **kwargs):
            # Copy inputs to compare them to originals later.
            arg_copies = [(i, arg.copy()) for i, arg in enumerate(args)
                          if isinstance(arg, (NDFrame, np.ndarray))]
            kwarg_copies = {
                k: v.copy() for k, v in iteritems(kwargs)
                if isinstance(v, (NDFrame, np.ndarray))
            }

            result = func(*args, **kwargs)

            # Check that inputs weren't mutated by func.
            for i, arg_copy in arg_copies:
                assert_allclose(
                    args[i],
                    arg_copy,
                    atol=0.5 * 10 ** (-DECIMAL_PLACES),
                    err_msg="Input 'arg %s' mutated by %s"
                            % (i, func.__name__),
                )
            for kwarg_name, kwarg_copy in iteritems(kwarg_copies):
                assert_allclose(
                    kwargs[kwarg_name],
                    kwarg_copy,
                    atol=0.5 * 10 ** (-DECIMAL_PLACES),
                    err_msg="Input '%s' mutated by %s"
                            % (kwarg_name, func.__name__),
                )

            return result

        return check_not_mutated


class ConvertPandasEmpyricalProxy(ReturnTypeEmpyricalProxy):
    """
    A ReturnTypeEmpyricalProxy which also converts pandas NDFrame inputs to
    empyrical functions according to the given conversion method.

    Calling an instance with a truthy pandas_only will return a new instance
    which will skip the test when an empyrical function is called.

    """
    def __init__(self, test_case, return_types, convert, pandas_only=False):
        super(ConvertPandasEmpyricalProxy, self).__init__(test_case,
                                                          return_types)
        self._convert = convert
        self._pandas_only = pandas_only

    def __getattr__(self, item):
        if self._pandas_only:
            raise SkipTest("empyrical.%s expects pandas-only inputs that have "
                           "dt indices/labels" % item)

        func = super(ConvertPandasEmpyricalProxy, self).__getattr__(item)

        @wraps(func)
        def convert_args(*args, **kwargs):
            args = [self._convert(arg) if isinstance(arg, NDFrame) else arg
                    for arg in args]
            kwargs = {
                k: self._convert(v) if isinstance(v, NDFrame) else v
                for k, v in iteritems(kwargs)
            }
            return func(*args, **kwargs)

        return convert_args


class PassArraysEmpyricalProxy(ConvertPandasEmpyricalProxy):
    """
    A ConvertPandasEmpyricalProxy which converts NDFrame inputs to empyrical
    functions to numpy arrays.

    Calls the underlying
    empyrical.[alpha|beta|alpha_beta]_aligned functions directly, instead of
    the wrappers which align Series first.

    """
    def __init__(self, test_case, return_types):
        super(PassArraysEmpyricalProxy, self).__init__(
            test_case, return_types, attrgetter('values'),
        )

    def __getattr__(self, item):
        if item in ('alpha', 'beta', 'alpha_beta', 'beta_fragility_heuristic',
                    'gpd_risk_estimates'):
            item += '_aligned'

        return super(PassArraysEmpyricalProxy, self).__getattr__(item)
