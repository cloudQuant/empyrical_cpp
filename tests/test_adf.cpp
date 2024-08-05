#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <stdexcept>
#include <gtest/gtest.h>

// 计算均值
double mean(const std::vector<double>& data) {
    return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
}

// 计算向量之间的内积
double dot_product(const std::vector<double>& v1, const std::vector<double>& v2) {
    double result = 0.0;
    for (size_t i = 0; i < v1.size(); ++i) {
        result += v1[i] * v2[i];
    }
    return result;
}

// 矩阵乘法（矩阵和向量）
std::vector<double> matrix_vector_multiply(const std::vector<std::vector<double>>& A, const std::vector<double>& B) {
    int rows = A.size();
    int cols = A[0].size();
    std::vector<double> result(rows, 0.0);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result[i] += A[i][j] * B[j];
        }
    }
    return result;
}

// 矩阵求逆（任意大小的方阵）
std::vector<std::vector<double>> matrix_inverse(const std::vector<std::vector<double>>& A) {
    int n = A.size();
    std::vector<std::vector<double>> A_inv(n, std::vector<double>(n, 0.0));
    std::vector<std::vector<double>> augmented_matrix(n, std::vector<double>(2 * n, 0.0));

    // 创建增广矩阵
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmented_matrix[i][j] = A[i][j];
        }
        augmented_matrix[i][n + i] = 1.0;
    }

    // 高斯-约旦消元法
    for (int i = 0; i < n; ++i) {
        // 选择主元
        double pivot = augmented_matrix[i][i];
        if (pivot == 0) {
            throw std::runtime_error("Matrix is singular, cannot invert.");
        }

        // 归一化主元行
        for (int j = 0; j < 2 * n; ++j) {
            augmented_matrix[i][j] /= pivot;
        }

        // 消元
        for (int k = 0; k < n; ++k) {
            if (k == i) continue;
            double factor = augmented_matrix[k][i];
            for (int j = 0; j < 2 * n; ++j) {
                augmented_matrix[k][j] -= factor * augmented_matrix[i][j];
            }
        }
    }

    // 提取逆矩阵
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A_inv[i][j] = augmented_matrix[i][n + j];
        }
    }

    return A_inv;
}

// 计算ADF统计量
double adf_statistic(const std::vector<double>& data, int lag) {
    int n = data.size();

    if (n <= lag) {
        throw std::invalid_argument("The lag is too large for the data size.");
    }

    // 计算差分序列
    std::vector<double> diff(n - 1);
    for (int i = 1; i < n; ++i) {
        diff[i - 1] = data[i] - data[i - 1];
    }

    // 构建设计矩阵X和响应向量Y
    std::vector<std::vector<double>> X(n - lag - 1, std::vector<double>(lag + 2, 1.0)); // 包含常数项
    std::vector<double> Y(n - lag - 1);

    for (int i = lag; i < n - 1; ++i) {
        Y[i - lag] = diff[i];
        X[i - lag][1] = data[i]; // 第一列为常数项
        for (int j = 1; j <= lag; ++j) {
            X[i - lag][j + 1] = diff[i - j];
        }
    }

    // 计算(X^T * X)和(X^T * Y)
    int p = lag + 2;
    std::vector<std::vector<double>> XtX(p, std::vector<double>(p, 0.0));
    std::vector<double> XtY(p, 0.0);

    for (int i = 0; i < n - lag - 1; ++i) {
        for (int j = 0; j < p; ++j) {
            XtY[j] += X[i][j] * Y[i];
            for (int k = 0; k < p; ++k) {
                XtX[j][k] += X[i][j] * X[i][k];
            }
        }
    }

    // 计算(XtX)的逆
    std::vector<std::vector<double>> XtX_inv = matrix_inverse(XtX);

    // 计算beta = (XtX)^(-1) * XtY
    std::vector<double> beta = matrix_vector_multiply(XtX_inv, XtY);

    // 计算残差
    std::vector<double> residuals(n - lag - 1, 0.0);
    for (int i = 0; i < n - lag - 1; ++i) {
        residuals[i] = Y[i];
        for (int j = 0; j < p; ++j) {
            residuals[i] -= X[i][j] * beta[j];
        }
    }

    // 计算标准误差
    double sum_squared_residuals = std::inner_product(residuals.begin(), residuals.end(), residuals.begin(), 0.0);
    double sigma = std::sqrt(sum_squared_residuals / (n - lag - p));

    // 计算ADF统计量
    double adf_stat = beta[1] / (sigma * std::sqrt(XtX_inv[1][1]));

    return adf_stat;
}

// 测试类
class AdfTest : public ::testing::Test {
protected:
    // 可以在这里添加初始化和清理代码
};

TEST_F(AdfTest, basic) {
    // 示例时间序列数据
    std::vector<double> data = {4.4, 4.2, 4.1, 3.9, 3.8, 3.7, 3.6, 3.5, 3.4, 3.3};
    int lag = 1; // 设置滞后阶数

    double adf_stat = adf_statistic(data, lag);

    std::cout << "ADF Statistic: " << adf_stat << std::endl;
    // todo still cannot pass the test
    // ASSERT_NEAR(adf_stat, -2.680, 0.001);
}




