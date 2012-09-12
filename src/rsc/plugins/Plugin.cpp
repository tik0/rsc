/* ============================================================
 *
 * This file is part of the RSB project.
 *
 * Copyright (C) 2012 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
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
 * ============================================================  */

#include "Plugin.h"

#include <errno.h>
#include <string.h>

#ifdef __linux__
#include <dlfcn.h>
#endif

#include <stdexcept>

#include <boost/format.hpp>

#include "../logging/Logger.h"
#include "../logging/LoggerFactory.h"

using namespace std;

using namespace boost;

namespace rsc {
namespace plugins {

const std::string PLUGIN_INIT_SYMBOL     = "rsc_plugin_init";
const std::string PLUGIN_SHUTDOWN_SYMBOL = "rsc_plugin_shutdown";

class Impl {
public:
    Impl(const std::string& name,
         const std::string& library)
        : logger(logging::LoggerFactory::getInstance()
                 .getLogger(str((format("rsc.plugins.Plugin[%1%]")
                                 % name)))),
          name(name), library(library),
          handle(NULL) {
    }

    const string& getName() const {
        return this->name;
    }

    void load() {
        RSCINFO(this->logger, "Trying to load library `" << this->library << "'");

        // Load the library containing the plugin.
        loadLibrary();

        // Lookup init and shutdown functions in the plugin library.
        this->init
            = reinterpret_cast<InitFunction>(resolveSymbol(PLUGIN_INIT_SYMBOL));
        this->shutdown
            = reinterpret_cast<ShutdownFunction>(resolveSymbol(PLUGIN_SHUTDOWN_SYMBOL));

        // Initialize the plugin.
        RSCINFO(this->logger, "Initializing");
        try {
            this->init();
        } catch (const std::exception& e) {
            throw runtime_error(str(format("Plugin `%1%' failed to initialize: %2%")
                                    % this->name
                                    % e.what()));
        } catch (...) {
            throw runtime_error(str(format("Plugin `%1%' failed to initialize due to unknown error.")
                                    % this->name));
        }
    }

    void unload() {
        // Shut the plugin down.
        RSCINFO(this->logger, "Shutting down");
        try {
            this->shutdown();
        } catch (const std::exception& e) {
            throw runtime_error(str(format("Plugin `%1%' failed to shutdown: %2%")
                                    % this->name
                                    % e.what()));
        } catch (...) {
            throw runtime_error(str(format("Plugin `%1%' failed to shutdown due to unknown error.")
                                    % this->name));
        }
    }
private:
    typedef void (*InitFunction)();
    typedef void (*ShutdownFunction)();

    rsc::logging::LoggerPtr logger;

    string name;
    string library;

    void*            handle;
    InitFunction     init;
    ShutdownFunction shutdown;

    void loadLibrary() {
#ifdef __linux__
        if (!(this->handle = dlopen(this->library.c_str(), RTLD_NOW))) {
            char buffer[4096];
            const char* result = strerror_r(errno, buffer, 4096);
            throw runtime_error(str(format("Failed to load plugin `%1%' from shared object `%2%': %3%.")
                                    % this->name
                                    % this->library
                                    % (result ? result : "<unknown error>")));
        }
#elif __APPLE__
        throw runtime_error("Plugins are not implemented for this platform.");
#elif _WIN32
        throw runtime_error("Plugins are not implemented for this platform.");
#endif
    }

    void* resolveSymbol(const string& name) {
        RSCINFO(this->logger, "Resolving symbol `"
                << name
                << "' in library `" << this->library << "'");

        assert(this->handle);

        void *address;
#ifdef __linux__
        if (!(address = dlsym(this->handle, name.c_str()))) {
            char buffer[4096];
            const char* result = strerror_r(errno, buffer, 4096);
            throw runtime_error(str(format("Plugin `%1%' failed to define function `%2%': %3%")
                                    % this->name
                                    % name
                                    % (result ? result : "<unknown error>")));
        }
#elif __APPLE__
        throw runtime_error("Plugins are not implemented for this platform.");
#elif _WIN32
        throw runtime_error("Plugins are not implemented for this platform.");
#endif
        return address;
    }
};

Plugin::Plugin(Impl* impl)
    : impl(impl) {
}

Plugin::~Plugin() {
}

const string& Plugin::getName() const {
    return this->impl->getName();
}

void Plugin::load() {
    this->impl->load();
}

PluginPtr Plugin::create(const std::string& name, const std::string& library) {
    return PluginPtr(new Plugin(new Impl(name, library)));
}

}
}
