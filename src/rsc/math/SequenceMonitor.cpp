/* ============================================================
 *
 * This file is a part of RSC project
 *
 * Copyright (C) 2011 by Christian Emmerich <cemmeric at techfak dot uni-bielefeld dot de>
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

#include "SequenceMonitor.h"

using namespace std;

namespace rsc {
namespace math {

Metric::~Metric() {
}

double EuclidDist::calc(const double* v1, const double* v2,
        const unsigned int& dim) {
    double sum = 0;
    for (int i = dim - 1; i >= 0; i--)
        sum += pow(v1[i] - v2[i], 2);
    return sqrt(sum);
}

double MaximumDist::calc(const double* v1, const double* v2,
        const unsigned int& dim) {
    double currValue, maxValue = 0.0;
    for (int i = dim - 1; i >= 0; i--) {
        currValue = fabs(v1[i] - v2[i]);
        if (currValue > maxValue) {
            maxValue = currValue;
        }
    }
    return maxValue;
}

MetricCondition::MetricCondition(MetricPtr m) :
    metric(m) {
}

MetricCondition::~MetricCondition() {
}

BelowThreshold::BelowThreshold(const MetricPtr m, const double threshold) :
    MetricCondition(m), threshold(threshold) {
}

bool BelowThreshold::isFulfilled(const double* v1, const double* v2,
        const unsigned int& dim) {
    return (this->metric->calc(v1, v2, dim) < this->threshold);
}

AboveThreshold::AboveThreshold(const MetricPtr m, const double threshold) :
    MetricCondition(m), threshold(threshold) {
}

bool AboveThreshold::isFulfilled(const double* v1, const double* v2,
        const unsigned int& dim) {
    return (this->metric->calc(v1, v2, dim) > this->threshold);
}

SequenceMonitor::SequenceMonitor(const unsigned int dim,
        const unsigned int window, MetricConditionPtr condition) :
    dim(dim), windowSize(window), metricCondition(condition) {
    prev_v = new double[dim];
    for (unsigned int i = 0; i < dim; i++)
        prev_v[i] = 0;
    resetCnt();
}

SequenceMonitor::~SequenceMonitor() {
    delete[] prev_v;
}

bool SequenceMonitor::isConditionFulfilled(double* new_v,
        const unsigned int& dim) {
    if (dim != this->dim) {
        std::stringstream s;
        s << "Given dimension " << dim
                << " of argument vector does not match preliminarily defined dimension "
                << this->dim;
        throw std::domain_error(s.str());
    }

    // test if condition is fulfilled for this element and copy current vector for next call
    bool currfulfilled = this->metricCondition->isFulfilled(new_v,
            this->prev_v, dim);
    for (unsigned int i = 0; i < this->dim; i++)
        this->prev_v[i] = new_v[i];

    // if condition is not fulfilled currently, start counter again from window size
    if (!currfulfilled) {
        resetCnt();
        return false;
    }

    // test if cnt reaches zero, otherwise condition is not fulfilled long enough
    this->cnt--;
    if (this->cnt > 0)
        return false;

    // now the sequence fulfilled the condition long enough
    return true;
}

void SequenceMonitor::resetCnt() {
    cnt = windowSize;
}

}
}
