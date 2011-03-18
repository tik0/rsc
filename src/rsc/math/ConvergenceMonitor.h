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

#include <iostream> //TODO remove this after debuggin
#include <sstream>
#include <stdexcept>


namespace rsc {
namespace math {


/**
 * A monitor for (vector-) sequences. Tests whether a sequence is converged or not.
 * The metric is simple: Test whether the difference between the current vector
 * and the predecessor is less than a certain threshold value. If this holds for a certain number of
 * subsequent sequence members (window size + 1), the sequence is said to be converged.
 * 
 *
 * @author cemmeric
 */
template<class DataType = double>
class ConvergenceMonitor{
    
public:
    
    /**
     * Constructor 
     *
     * @param dim dimension of the vectors of the sequence
     * @param window number of subsequent members for which the difference falls below the threshold
     * @param threshold 
     * @param metric the metric which is to be used for determining the distance of subsequent members
     * 
     */
    ConvergenceMonitor(const unsigned int dim, const unsigned int window, const DataType threshold)
    : dim(dim), windowSize(window), threshold(threshold) {
        prev_v = new DataType[dim];
        reset();
    }

    ~ConvergenceMonitor() {
        delete [] prev_v;
    }

    /**
     * Test whether the sequence is converged or not. Once a sequence is converged,
     * this method returns true regardless of new elements. For further usage this object
     * it has to be reset -> reset().
     *
     * @param v next sequence member
     */
    bool isConverged(DataType* new_v, unsigned int dim) {
        if (dim!=this->dim) {
            std::stringstream s;
            s << "Given dimension " << dim
              << " of argument vector does not match preliminarily defined dimension "
              << this->dim;
            throw std::domain_error(s.str());
        }

        // if already converged in previous steps, return true again
        if (converged)  return true;

        // calculate metric and store new vector
        bool lowerThanTheshold = true;
        for (unsigned int i=0; i<this->dim; i++) {
            // test the threshold in each dimension
            lowerThanTheshold = lowerThanTheshold && ( abs(prev_v[i] - new_v[i]) < threshold );
            // copy current vector for next call
            prev_v[i] = new_v[i];
        }

        // test if condition fulfilled
        if (!lowerThanTheshold) {
            resetCnt();
            return false;
        }

        // test if cnt reaches zero
        cnt--;
        if (cnt>0)  return false;

        // now sequence is converged, remember for following steps
        return (converged = true);
    }
    
    /**
     * Reset monitor for new convergence test
     */
    void reset() {
        converged = false;
        resetCnt();
        for (unsigned int i=0; i<dim; i++)  prev_v[i] = 0;
    }

protected:

    void resetCnt() {
        cnt = windowSize;
    }


    const unsigned int dim;
    const unsigned int windowSize;
    const DataType threshold;
    DataType* prev_v;

    int cnt;   // counts how long the condition is fulfilled
    bool converged;

};

}
}


