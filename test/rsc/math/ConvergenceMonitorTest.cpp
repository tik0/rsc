/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2011 by Christian Emmerich <cemmeric at techfak dot uni-bielefeld dot de>
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

#include "rsc/math/ConvergenceMonitor.h"

using namespace std;
using namespace testing;
using namespace rsc;
using namespace rsc::math;


TEST(ConvergenceMonitorTest, testIllegalDimension)
{
    ConvergenceMonitor<double> monitor(5, 10, 0.1);
    double v[5] = {0, 1, 2, 3, 4};
    EXPECT_THROW(monitor.isConverged(v, 6), std::domain_error);
    EXPECT_THROW(monitor.isConverged(v, 4), std::domain_error);
}


TEST(ConvergenceMonitorTest, testConvergence)
{
    ConvergenceMonitor<double> monitor(5, 5, 0.01);

    // different vectors
    double v[5] = {1, 0, 2.5, 3, 4.5};
    double w[5] = {0.6, 1, 1, 3, 4.5};
    double x[5] = {0, 1, 7, 3, 4.5};
    double y[5] = {0, 1, 2, 3, 10.2};

    // similar vectors
    double v1[5] = {0, 1, 2, 3, 4};
    double v2[5] = {0.009, 1, 2, 3, 4};
    double v3[5] = {0, 1.009, 2, 3, 4};
    double v4[5] = {0, 1, 1.991, 3, 4};
    double v5[5] = {0, 1, 2, 2.991, 4};
    double v6[5] = {0, 1, 2, 3, 4.009};

    // test different vectors
    EXPECT_FALSE(monitor.isConverged(v, 5));
    EXPECT_FALSE(monitor.isConverged(y, 5));
    EXPECT_FALSE(monitor.isConverged(v, 5));
    EXPECT_FALSE(monitor.isConverged(x, 5));
    EXPECT_FALSE(monitor.isConverged(w, 5));
    EXPECT_FALSE(monitor.isConverged(y, 5));
    EXPECT_FALSE(monitor.isConverged(v, 5));

    // test similar vectors
    EXPECT_FALSE(monitor.isConverged(v3, 5));
    EXPECT_FALSE(monitor.isConverged(v1, 5));
    EXPECT_FALSE(monitor.isConverged(v4, 5));
    EXPECT_FALSE(monitor.isConverged(v5, 5));
    EXPECT_FALSE(monitor.isConverged(v2, 5));
    EXPECT_TRUE(monitor.isConverged(v6, 5));
}

TEST(ConvergenceMonitorTest, testStayConverged)
{
    double v[5] = {0, 1, 2, 3, 4.5};
    double w[5] = {7, 3.2, 6, 3, 0};
    ConvergenceMonitor<double> monitor(5, 5, 0.01);

    // let converge
    for (int i=1; i<=5; i++)    monitor.isConverged(v, 5);
    EXPECT_TRUE(monitor.isConverged(v, 5));

    // stay converged regardless of input
    EXPECT_TRUE(monitor.isConverged(v, 5));
    EXPECT_TRUE(monitor.isConverged(w, 5));
    EXPECT_TRUE(monitor.isConverged(w, 5));
    EXPECT_TRUE(monitor.isConverged(v, 5));
    EXPECT_TRUE(monitor.isConverged(w, 5));
}


TEST(ConvergenceMonitorTest, testReset)
{
    double v[5] = {0, 1, 2, 3, 4.5};
    ConvergenceMonitor<double> monitor(5, 5, 0.01);

    // let converge
    for (int i=1; i<=5; i++)    monitor.isConverged(v, 5);
    EXPECT_TRUE(monitor.isConverged(v, 5));

    // reset and test again convergence
    monitor.reset();
    for (int i=1; i<=5; i++)    EXPECT_FALSE(monitor.isConverged(v, 5));
    EXPECT_TRUE(monitor.isConverged(v, 5));
}



