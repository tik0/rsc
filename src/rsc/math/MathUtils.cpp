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

#include "MathUtils.h"

#include <cmath>

using namespace std;

namespace rsc {
namespace math {

double MathUtils::getTwoPi() {
    return 2.0 * M_PI;
}

double MathUtils::getDefaultAnglePrecision() {
    return 0.001;
}

double MathUtils::normalizeAngle(const double &angleInRad) {

    double mod = fmod(angleInRad, getTwoPi());

    if (mod < 0) {
        return getTwoPi() + mod;
    } else {
        return mod;
    }

}

bool MathUtils::isSameAngle(const double &firstInRad,
        const double &secondInRad, const double &precision) {

    return cos(normalizeAngle(firstInRad) - normalizeAngle(secondInRad)) > cos(
            precision);

}

double MathUtils::radianFromDegree(const double &degree) {
    return degree * (M_PI / 180.0f);
}

double MathUtils::degreeFromRadian(const double &radian) {
    return radian * (180.0f / M_PI);
}

double MathUtils::getDefaultClosePrecision() {
    return 0.000001;
}

bool MathUtils::isClose(const double &a, const double &b,
        const double &precision) {
    return abs(a - b) < precision;
}

}
}
