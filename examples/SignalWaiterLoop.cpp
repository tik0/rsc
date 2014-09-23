#include <iostream>

#include <boost/date_time.hpp>
#include <boost/thread.hpp>

#include <rsc/misc/SignalWaiter.h>

class ResourceUser {
public:
    ResourceUser() {
        std::cout << "Allocating resources" << std::endl;
    }

    virtual ~ResourceUser() {
        std::cout << "Freeing resources" << std::endl;
    }
};

int main() {
    rsc::misc::initSignalWaiter(
            rsc::misc::INTERRUPT_REQUESTED | rsc::misc::QUIT_REQUESTED);

    ResourceUser myScopeObject;

    // something like a main loop
    while (true) {

        // simulate some work
        std::cout << "... working ..." << std::endl;
        boost::this_thread::sleep(boost::posix_time::seconds(1));

        // check whether a signal arrived in the meantime and handle it if so
        rsc::misc::Signal signal = rsc::misc::lastArrivedSignal();
        if (signal != rsc::misc::NO_SIGNAL) {
            return rsc::misc::suggestedExitCode(signal);
        }

    }
}
