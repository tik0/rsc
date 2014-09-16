#include <iostream>

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
    rsc::misc::initSignalWaiter(rsc::misc::INTERRUPT_REQUESTED | rsc::misc::QUIT_REQUESTED);

    ResourceUser myScopeObject;

    return rsc::misc::suggestedExitCode(rsc::misc::waitForSignal());
}
