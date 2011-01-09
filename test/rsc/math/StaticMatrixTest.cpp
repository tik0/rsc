/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "rsc/math/StaticMatrix.h"

using namespace std;
using namespace testing;
using namespace rsc;
using namespace rsc::math;

TEST(StaticMatrixTest, testConstruction)
{

    const unsigned int rows = 3;
    const unsigned int cols = 10;
    StaticMatrix<rows, cols> testMatrix;
    for (unsigned int row = 0; row < rows; ++row) {
        for (unsigned int col = 0; col < cols; ++col) {
            EXPECT_DOUBLE_EQ(0, testMatrix(row, col));
        }
    }

}

TEST(StaticMatrixTest, testAssignment)
{

    const unsigned int rows = 3;
    const unsigned int cols = 10;
    StaticMatrix<rows, cols> testMatrix;
    const unsigned int changedRow = 2;
    const unsigned int changedCol = 6;
    const double newValue = 10.345;
    testMatrix(changedRow, changedCol) = newValue;
    for (unsigned int row = 0; row < rows; ++row) {
        for (unsigned int col = 0; col < cols; ++col) {
            if (row == changedRow && col == changedCol) {
                EXPECT_DOUBLE_EQ(newValue, testMatrix(row, col));
            } else {
                EXPECT_DOUBLE_EQ(0, testMatrix(row, col));
            }
        }
    }

}

TEST(StaticMatrixTest, testIllegalAccess)
{
    StaticMatrix<1, 1> testMatrix;
    EXPECT_THROW(testMatrix(1, 0), std::domain_error);
    EXPECT_THROW(testMatrix(0, 1), std::domain_error);
    EXPECT_THROW(testMatrix(1, 1), std::domain_error);
    EXPECT_THROW(testMatrix(2, 1), std::domain_error);
}

TEST(StaticMatrixTest, testConstAccess)
{
    const StaticMatrix<1, 1> testMatrix;
    EXPECT_DOUBLE_EQ(0, testMatrix(0, 0));
}

TEST(StaticMatrixTest, testMult)
{

    StaticMatrix<1, 1> a;
    a(0, 0) = 2;
    StaticMatrix<1, 1> aSquare = a.mult(a);
    EXPECT_DOUBLE_EQ(4, aSquare(0, 0));

    StaticMatrix<2, 2> b;
    b(0, 0) = 2; // 2 3
    b(0, 1) = 3; // 4 5
    b(1, 0) = 4;
    b(1, 1) = 5;

    StaticMatrix<2, 4> c;
    c(0, 0) = 1; // 1 3 2 4
    c(0, 1) = 3; // 4 5 7 2
    c(0, 2) = 2;
    c(0, 3) = 4;
    c(1, 0) = 4;
    c(1, 1) = 5;
    c(1, 2) = 7;
    c(1, 3) = 2;

    StaticMatrix<2, 4> res = b.mult(c);
    EXPECT_DOUBLE_EQ(14, res(0, 0));
    EXPECT_DOUBLE_EQ(21, res(0, 1));
    EXPECT_DOUBLE_EQ(25, res(0, 2));
    EXPECT_DOUBLE_EQ(14, res(0, 3));
    EXPECT_DOUBLE_EQ(24, res(1, 0));
    EXPECT_DOUBLE_EQ(37, res(1, 1));
    EXPECT_DOUBLE_EQ(43, res(1, 2));
    EXPECT_DOUBLE_EQ(26, res(1, 3));

}

TEST(StaticMatrixTest, testMultOperator)
{

    StaticMatrix<2, 2> b;
    b(0, 0) = 2; // 2 3
    b(0, 1) = 3; // 4 5
    b(1, 0) = 4;
    b(1, 1) = 5;

    StaticMatrix<2, 4> c;
    c(0, 0) = 1; // 1 3 2 4
    c(0, 1) = 3; // 4 5 7 2
    c(0, 2) = 2;
    c(0, 3) = 4;
    c(1, 0) = 4;
    c(1, 1) = 5;
    c(1, 2) = 7;
    c(1, 3) = 2;

    StaticMatrix<2, 4> res = b * c;
    EXPECT_DOUBLE_EQ(14, res(0, 0));
    EXPECT_DOUBLE_EQ(21, res(0, 1));
    EXPECT_DOUBLE_EQ(25, res(0, 2));
    EXPECT_DOUBLE_EQ(14, res(0, 3));
    EXPECT_DOUBLE_EQ(24, res(1, 0));
    EXPECT_DOUBLE_EQ(37, res(1, 1));
    EXPECT_DOUBLE_EQ(43, res(1, 2));
    EXPECT_DOUBLE_EQ(26, res(1, 3));

}

TEST(StaticMatrixTest, testComparisonOperator)
{

    StaticMatrix<2, 2> a;
    StaticMatrix<2, 2> b;

    EXPECT_EQ(a, b);
    EXPECT_EQ(b, a);

    a(0, 0) = 0.5;

    EXPECT_NE(a, b);
    EXPECT_NE(b, a);

    b(0, 0) = 0.5;

    EXPECT_EQ(a, b);
    EXPECT_EQ(b, a);

    a(1, 1) = 0.7;

    EXPECT_NE(a, b);
    EXPECT_NE(b, a);

    b(1, 1) = 0.7;

    EXPECT_EQ(a, b);
    EXPECT_EQ(b, a);

}

TEST(StaticMatrixTest, testDataTypeConversion)
{

    StaticMatrix<2, 2> doubleMatrix;
    StaticMatrix<2, 2, float> floatMatrix;
    EXPECT_EQ(doubleMatrix, floatMatrix);

    StaticMatrix<1, 1> start;
    start(0, 0) = 1.7;

    StaticMatrix<1, 1, int> stop = start;

    EXPECT_EQ(1, stop(0, 0));

}

TEST(StaticMatrixTest, testSet)
{

    StaticMatrix<2, 2> m;
    const double value = 3.5;
    m.set(value);

    EXPECT_DOUBLE_EQ(value, m(0, 0));
    EXPECT_DOUBLE_EQ(value, m(0, 1));
    EXPECT_DOUBLE_EQ(value, m(1, 0));
    EXPECT_DOUBLE_EQ(value, m(1, 1));

}

TEST(StaticMatrixTest, testScalarMult)
{

    StaticMatrix<2, 2> matrix;
    matrix.set(1.5);
    matrix.mult(2);

    EXPECT_DOUBLE_EQ(3, matrix(0, 0));
    EXPECT_DOUBLE_EQ(3, matrix(0, 1));
    EXPECT_DOUBLE_EQ(3, matrix(1, 0));
    EXPECT_DOUBLE_EQ(3, matrix(1, 1));

}

TEST(StaticMatrixTest, testScalarAdd)
{

    StaticMatrix<2, 2, int> m;
    m.add(3);
    StaticMatrix<2, 2, int> expected;
    expected.set(3);
    EXPECT_EQ(expected, m);

}

TEST(StaticMatrixTest, testAdd)
{

    StaticMatrix<2, 2> a;
    StaticMatrix<2, 2> b;
    b.set(5);
    EXPECT_EQ(b, a.add(b));
    EXPECT_EQ(b, a);

}

TEST(StaticMatrixTest, testSetDiagonal)
{

    StaticMatrix<3, 5> m1;
    m1.setDiagonal(1.5);
    EXPECT_DOUBLE_EQ(1.5, m1(0, 0));
    EXPECT_DOUBLE_EQ(0, m1(0, 1));
    EXPECT_DOUBLE_EQ(0, m1(0, 2));
    EXPECT_DOUBLE_EQ(0, m1(0, 3));
    EXPECT_DOUBLE_EQ(0, m1(0, 4));
    EXPECT_DOUBLE_EQ(0, m1(1, 0));
    EXPECT_DOUBLE_EQ(1.5, m1(1, 1));
    EXPECT_DOUBLE_EQ(0, m1(1, 2));
    EXPECT_DOUBLE_EQ(0, m1(1, 3));
    EXPECT_DOUBLE_EQ(0, m1(1, 4));
    EXPECT_DOUBLE_EQ(0, m1(2, 0));
    EXPECT_DOUBLE_EQ(0, m1(2, 1));
    EXPECT_DOUBLE_EQ(1.5, m1(2, 2));
    EXPECT_DOUBLE_EQ(0, m1(2, 3));
    EXPECT_DOUBLE_EQ(0, m1(2, 4));

}
