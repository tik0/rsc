/* ============================================================
 *
 * This file is a part of the RSC project.
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

#include "rsc/math/MathUtils.h"

#include <cmath>

using namespace std;
using namespace rsc;
using namespace rsc::math;

TEST(MathUtilsTest, testNormalizeAngle)
{

    EXPECT_NEAR(0.1f, MathUtils::normalizeAngle(2.0 * M_PI + 0.1), MathUtils::getDefaultAnglePrecision());
    EXPECT_NEAR(0.2f, MathUtils::normalizeAngle(4.0 * M_PI + 0.2), MathUtils::getDefaultAnglePrecision());
    EXPECT_NEAR(0.3f, MathUtils::normalizeAngle(6.0 * M_PI + 0.3), MathUtils::getDefaultAnglePrecision());
    EXPECT_NEAR(2.0 * M_PI - 0.3, MathUtils::normalizeAngle(-0.3), MathUtils::getDefaultAnglePrecision());
    EXPECT_NEAR(2.0 * M_PI - 0.6, MathUtils::normalizeAngle(-0.6), MathUtils::getDefaultAnglePrecision());

}

TEST(MathUtilsTest, testIsSameAngle)
{

    EXPECT_TRUE(MathUtils::isSameAngle(0, 0));
    EXPECT_TRUE(MathUtils::isSameAngle(0, 2.0 * M_PI));
    EXPECT_TRUE(MathUtils::isSameAngle(0, -2.0 * M_PI));
    EXPECT_TRUE(MathUtils::isSameAngle(M_PI, 3.0 * M_PI));
    EXPECT_TRUE(MathUtils::isSameAngle(M_PI, 3.0 * M_PI - 0.5 * MathUtils::getDefaultAnglePrecision()));
    EXPECT_TRUE(MathUtils::isSameAngle(-0.2 * MathUtils::getDefaultAnglePrecision(), 0.2 * MathUtils::getDefaultAnglePrecision()));

    // ensure that precision is used
    EXPECT_TRUE(MathUtils::isSameAngle(0.1, 0.4, 0.5));
    EXPECT_TRUE(MathUtils::isSameAngle(-0.1, 0.1, 0.3));

    EXPECT_FALSE(MathUtils::isSameAngle(0, 1));

    EXPECT_TRUE(MathUtils::isSameAngle(2.3561944961547852, 2.3561945339037349));

}

TEST(MathUtilsTest, testAngleConversion)
{

    EXPECT_NEAR(0.0, MathUtils::degreeFromRadian(MathUtils::radianFromDegree(0.0)), 0.0001);

    EXPECT_NEAR(0.0, MathUtils::degreeFromRadian(0.0), 0.0001);
    EXPECT_NEAR(180.0, MathUtils::degreeFromRadian(M_PI), 0.0001);
    EXPECT_NEAR(360.0, MathUtils::degreeFromRadian(2.0 * M_PI), 0.0001);

    EXPECT_NEAR(0.0, MathUtils::radianFromDegree(0.0), 0.0001);
    EXPECT_NEAR(M_PI, MathUtils::radianFromDegree(180.0), 0.0001);
    EXPECT_NEAR(2.0 * M_PI, MathUtils::radianFromDegree(360.0), 0.0001);

}
