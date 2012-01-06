/* ============================================================
 *
 * This file is a part of the RSC project.
 *
 * Copyright (C) 2010 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
 *
 * This file may be licensed under the terms of of the
 * GNU Lesser General Public License Version 3 (the ``LGPL''),
 * or (at your option) any later version.
 *
 * Software distributed under the License is distributed
 * on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied. See the LGPL for the specific language
 * governing rights and limitations.
 *
 * You should have received a copy of the LGPL along with this
 * program. If not, go to http://www.gnu.org/licenses/lgpl.html
 * or write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The development of this software was supported by:
 *   CoR-Lab, Research Institute for Cognition and Robotics
 *     Bielefeld University
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

double MathUtils::normalizeAngle(const double& angleInRad) {

    double mod = fmod(angleInRad, getTwoPi());

    if (mod < 0) {
        return getTwoPi() + mod;
    } else {
        return mod;
    }

}

bool MathUtils::isSameAngle(const double& firstInRad,
        const double& secondInRad, const double& precision) {

    return cos(normalizeAngle(firstInRad) - normalizeAngle(secondInRad)) > cos(
            precision);

}

double MathUtils::radianFromDegree(const double& degree) {
    return degree * (M_PI / 180.0f);
}

double MathUtils::degreeFromRadian(const double& radian) {
    return radian * (180.0f / M_PI);
}

double MathUtils::getDefaultClosePrecision() {
    return 0.000001;
}

bool MathUtils::isClose(const double& a, const double& b,
        const double& precision) {
    return abs(a - b) < precision;
}

}
}
