/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2010, 2011 Arne Nordmann
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

#include <utility>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <time.h>

#include <iostream>

#include <rsc/misc/langutils.h>
#include <rsc/threading/PeriodicTask.h>

using namespace rsc;
using namespace rsc::misc;
using namespace rsc::threading;

/**
 * FibonacciCalculator inheriting from perdiodic task
 * and calculating random fibonacci numbers in each cycle.
 *
 * @author anordman
 */
class FibonacciCalculator: public PeriodicTask {
public:
    FibonacciCalculator(unsigned int cycleTimeMs) :
            PeriodicTask(cycleTimeMs), number(0), start(), lastStart(), cycleTime(
                    cycleTimeMs) {
        srand(1);
    }
    void execute() {
        start = currentTimeMicros();
        std::cout
                << "\t- jitter (us): "
                << fabs(
                        cycleTime * 1000
                                - fabs(static_cast<double>(start - lastStart)));
        number = (rand() % 20);
        std::cout << std::endl << number << ": " << fibonacciOf(number);
        lastStart = start;
    }
    int fibonacciOf(int n) {
        if (n == 0) {
            return 0;
        } else if (1 == n || 2 == n) {
            return 1;
        } else {
            return fibonacciOf(n - 1) + fibonacciOf(n - 2);
        }
    }
    unsigned int number;
    boost::uint64_t start, lastStart;
    unsigned int cycleTime;
};

/**
 * Calculating fibonacci numbers between 0 and 20 in a periodic
 * task each 50ms and plotting the jitter.
 */
int main() {
    std::cout << "Calculate random fibonacci numbers in a periodic task."
            << std::endl;

    FibonacciCalculator fib(50);
    fib.run();

    return EXIT_SUCCESS;
}
