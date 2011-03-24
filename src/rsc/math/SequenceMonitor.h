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

#pragma once

#include <boost/shared_ptr.hpp>
#include <iostream>

#include <math.h>
#include <sstream>
#include <stdexcept>

namespace rsc {
namespace math {

///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////// METRICS ////////////////////////////////////////////////////////

class Metric;
typedef boost::shared_ptr<Metric> MetricPtr;

/**
 * Defines interface for vector metrics providing a calc-method that
 * calculates the metric of two vectors.
 */
class Metric {
public:

    Metric() {
    }

    /**
     * calculates the distance between v1 and v2
     * @param v1 vector
     * @param v2 another vector
     * @return distance between the two vectors
     */
    virtual double calc(const double* v1, const double* v2,
            const unsigned int& dim) = 0;
};

/**
 * Euclidean distance between two vectors.
 */
class EuclidDist: public Metric {
public:

    EuclidDist() {
    }

    /**
     * calculates the euclidean distance between v1 and v2
     * @param v1 vector
     * @param v2 another vector
     * @return euclidean distance between the two vectors
     */
    double calc(const double* v1, const double* v2, const unsigned int& dim) {
        double sum = 0;
        for (int i = dim - 1; i >= 0; i--)
            sum += pow(v1[i] - v2[i], 2);
        return sqrt(sum);
    }
};

/**
 * Euclidean distance between two vectors.
 */
class MaximumDist: public Metric {
public:

    MaximumDist() {
    }

    /**
     * distance between two vectors is defined as the maximum absolute componentwise value
     * @param v1 vector
     * @param v2 another vector
     * @return maximum absolut distance between the two vectors
     */
    double calc(const double* v1, const double* v2, const unsigned int& dim) {
        double currValue, maxValue = 0.0;
        for (int i = dim - 1; i >= 0; i--) {
            currValue = fabs(v1[i] - v2[i]);
            if (currValue > maxValue) {
                maxValue = currValue;
            }
        }
        return maxValue;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////// METRIC CONDITIONS /////////////////////////////////////////////////

class MetricCondition;
typedef boost::shared_ptr<MetricCondition> MetricConditionPtr;

/**
 * Defines a interface for metric conditions. Provides a method isfulfilled.
 */
class MetricCondition {
public:

    MetricCondition(MetricPtr m) :
        metric(m) {
    }

    /**
     *  Tests whether the metric condition is fulfilled for two vectors
     * @param v1 a vector
     * @param v2 another vector
     * @param dim dimension of the two vectors v1 and v2
     * @return true if v1 and v2 fulfill the metric condition
     */
    virtual bool isfulfilled(const double* v1, const double* v2,
            const unsigned int& dim) = 0;

protected:
    const MetricPtr metric;
};

/**
 * The BelowThreshold - condition tests whether a given metric of two vectors stays
 * below a given upper threshold
 */
class BelowThreshold: public MetricCondition {
public:

    BelowThreshold(const MetricPtr m, const double threshold) :
        MetricCondition(m), threshold(threshold) {
    }

    /**
     * tests whether two vectors fulfill the BelowUpperThreshold - condition
     * @param v1 a vector
     * @param v2 another vector
     * @param dim dimension of both given vectors
     * @return true, if metric(v1, v2) < threshold
     */
    bool isfulfilled(const double* v1, const double* v2,
            const unsigned int& dim) {
        return (this->metric->calc(v1, v2, dim) < this->threshold);
    }

protected:
    const double threshold;
};

/**
 * The AboveThreshold - condition tests whether a given metric of two vectors stays
 * above a given threshold
 */
class AboveThreshold: public MetricCondition {
public:

    AboveThreshold(const MetricPtr m, const double threshold) :
        MetricCondition(m), threshold(threshold) {
    }

    /**
     * tests whether two vectors fulfill the AboveThreshold - condition
     * @param v1 a vector
     * @param v2 another vector
     * @param dim dimension of both given vectors
     * @return true, if metric(v1, v2) > threshold
     */
    bool isfulfilled(const double* v1, const double* v2,
            const unsigned int& dim) {
        return (this->metric->calc(v1, v2, dim) > this->threshold);
    }

protected:
    const double threshold;
};

//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////// SEQUENCE MONITOR //////////////////////////////////////////////////

/**
 * A monitor for (vector-) sequences. Tests whether the difference of consecutive members of a sequence
 * fulfills a certain metric condition for a certain number of steps or not.
 *
 * A simple application could be convergence testing: Test whether the difference between the current vector
 * and the predecessor is less than a certain threshold value. If this holds for a certain number of
 * consecutive sequence members (window size + 1), the sequence is said to be converged.
 *
 *
 * @author cemmeric
 */
class SequenceMonitor {

public:

    /**
     * Constructor 
     *
     * @param dim dimension of the vectors of the sequence
     * @param window number of consecutive members for which the condition should hold
     * @param condition the condition that should be fulfilled for consecutive sequence members
     * 
     */
    SequenceMonitor(const unsigned int dim, const unsigned int window,
            MetricConditionPtr condition) :
        dim(dim), windowSize(window), metricCondition(condition) {
        prev_v = new double[dim];
        for (unsigned int i = 0; i < dim; i++)
            prev_v[i] = 0;
        resetCnt();
    }

    ~SequenceMonitor() {
        delete[] prev_v;
    }

    /**
     * Test whether the difference of consecutive sequence members fulfills
     * the condition for the given 'time window' or not.
     *
     * @param v next sequence member
     */
    bool condition_fulfilled(double* new_v, const unsigned int& dim) {
        if (dim != this->dim) {
            std::stringstream s;
            s << "Given dimension " << dim
                    << " of argument vector does not match preliminarily defined dimension "
                    << this->dim;
            throw std::domain_error(s.str());
        }

        // test if condition is fulfilled for this element and copy current vector for next call
        bool currfulfilled = this->metricCondition->isfulfilled(new_v,
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


protected:

    void resetCnt() {
        cnt = windowSize;
    }

    const unsigned int dim;
    const unsigned int windowSize;
    MetricConditionPtr metricCondition;

    double* prev_v; // previous sequence element
    int cnt; // counts whether the condition is fulfilled long enough

};

}
}

