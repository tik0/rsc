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

#include "rsc/math/SequenceMonitor.h"

using namespace std;
using namespace testing;
using namespace rsc;
using namespace rsc::math;


TEST(SequenceMonitorTest, testIllegalDimension)
{
    MetricPtr m(new EuclidDist());
    MetricCondition* mcond = new BelowThreshold(m, 0.01);

    MetricConditionPtr mc(mcond);
    SequenceMonitor monitor(5, 10, mc);
    double v[5] = {0, 1, 2, 3, 4};
    EXPECT_THROW(monitor.isConditionFulfilled(v, 6), std::domain_error);
    EXPECT_THROW(monitor.isConditionFulfilled(v, 4), std::domain_error);
}


TEST(SequenceMonitorTest, testConvergence)
{
    MetricPtr m(new MaximumDist());
    MetricConditionPtr mc(new BelowThreshold(m, 0.01));
    SequenceMonitor monitor(5, 5, mc);

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

    for (int t=1; t<=2; t++) {
        // test different vectors
        EXPECT_FALSE(monitor.isConditionFulfilled(v, 5));
        EXPECT_FALSE(monitor.isConditionFulfilled(y, 5));
        EXPECT_FALSE(monitor.isConditionFulfilled(v, 5));
        EXPECT_FALSE(monitor.isConditionFulfilled(x, 5));
        EXPECT_FALSE(monitor.isConditionFulfilled(w, 5));
        EXPECT_FALSE(monitor.isConditionFulfilled(y, 5));
        EXPECT_FALSE(monitor.isConditionFulfilled(v, 5));

        // test similar vectors
        EXPECT_FALSE(monitor.isConditionFulfilled(v3, 5));
        EXPECT_FALSE(monitor.isConditionFulfilled(v1, 5));
        EXPECT_FALSE(monitor.isConditionFulfilled(v4, 5));
        EXPECT_FALSE(monitor.isConditionFulfilled(v5, 5));
        EXPECT_FALSE(monitor.isConditionFulfilled(v2, 5));
        EXPECT_TRUE(monitor.isConditionFulfilled(v6, 5));
        EXPECT_TRUE(monitor.isConditionFulfilled(v1, 5));
        EXPECT_TRUE(monitor.isConditionFulfilled(v2, 5));
        EXPECT_TRUE(monitor.isConditionFulfilled(v3, 5));
    }


}

TEST(SequenceMonitorTest, testDivergence)
{
    MetricPtr m(new MaximumDist());
    MetricConditionPtr mc(new AboveThreshold(m, 0.01));
    SequenceMonitor monitor(5, 5, mc);

    // different vectors
    double v[5] = {1, 1, 2, 3, 4};
    double w[5] = {1.5, 1.5, 2, 3, 4};
    double x[5] = {3, 2, 2.5, 3.5, 4.5};
    double y[5] = {5, 4, 3, 4, 5};
    double z[5] = {10, 7, 5, 6, 7};

    // similar vectors
    double v1[5] = {0, 1, 2, 3, 4};
    double v2[5] = {0.009, 1, 2, 3, 4};
    double v3[5] = {0, 1.009, 2, 3, 4};
    double v4[5] = {0, 1, 1.991, 3, 4};
    double v5[5] = {0, 1, 2, 2.991, 4};
    double v6[5] = {0, 1, 2, 3, 4.009};

    // test similar vectors
    EXPECT_FALSE(monitor.isConditionFulfilled(v3, 5));
    EXPECT_FALSE(monitor.isConditionFulfilled(v1, 5));
    EXPECT_FALSE(monitor.isConditionFulfilled(v4, 5));
    EXPECT_FALSE(monitor.isConditionFulfilled(v5, 5));
    EXPECT_FALSE(monitor.isConditionFulfilled(v2, 5));
    EXPECT_FALSE(monitor.isConditionFulfilled(v6, 5));
    EXPECT_FALSE(monitor.isConditionFulfilled(v1, 5));
    EXPECT_FALSE(monitor.isConditionFulfilled(v6, 5));

    // test different vectors
    EXPECT_FALSE(monitor.isConditionFulfilled(v6, 5));
    EXPECT_FALSE(monitor.isConditionFulfilled(v, 5));
    EXPECT_FALSE(monitor.isConditionFulfilled(w, 5));
    EXPECT_FALSE(monitor.isConditionFulfilled(x, 5));
    EXPECT_FALSE(monitor.isConditionFulfilled(y, 5));
    EXPECT_TRUE(monitor.isConditionFulfilled(z, 5));


}

