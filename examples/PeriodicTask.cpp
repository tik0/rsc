/* ============================================================
 *
 * This file is part of the RSC project
 *
 * Copyright (C) 2010, 2011 Arne Nordmann
 *
 * This file may be licensed under the terms of the
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
