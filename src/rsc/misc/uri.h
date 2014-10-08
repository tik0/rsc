/* ============================================================
 *
 * This file is a part of the RSC project
 *
 * Copyright (C) 2014 by Robert Haschke <rhaschke at techfak dot uni-bielefeld dot de>
 *
 * This file may be licensed under the terms of the
 * GNU Lesser General Public License Version 3 (the [yas] elisp error!LGPL''),
 * or (at your option) any later version.
 *
 * Software distributed under the License is distributed
 * on an [yas] elisp error!AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied. See the LGPL for the specific language
 * governing rights and limitations.
 *
 * You should have received a copy of the LGPL along with this
 * program. If not, go to http://www.gnu.org/licenses/lgpl.html
 * or write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The development of this software was supported by:
 *   CITEC, "Cognitive Interaction Technology" Excellence Cluster
 *     Bielefeld University
 *
 * ============================================================ */

#pragma once
#include <rsc/runtime/Properties.h>

namespace rsc {
namespace misc {

/** The URI class implements a parser based on RFC 3986 and RFC 2732.
    http://tools.ietf.org/html/rfc3986
    http://tools.ietf.org/html/rfc2732

    Its functionality is a subset of cpp-netlib's URI class, which applied for
    inclusion in boost. Thus, it should be easy to replace this class later on.
    http://cpp-netlib.org/0.11.0/in_depth/uri.html
    https://github.com/cpp-netlib/uri
*/

class uri_builder;
class uri {
public:
	/// empty constructor
	uri () {}
	/** parser constructor: parses uri from string
	 *
	 *  supported syntax is: 
	 *  [SCHEME:][//HOST][:PORT][PATH][?QUERY]
	 *  http://docs.cor-lab.de//rsb-manual/trunk/html/specification-uris.html
	 */
	uri (const std::string& source);

	/// copy constructor
	uri(const uri &other);

	/// destructor
	~uri() {}

	/// assignment operator
	uri &operator=(const uri& other);

	std::string scheme() const {return sScheme;}
	std::string path()   const {return sPath;}
	std::string host()   const;
	std::string port()   const;

public:
	std::string sScheme;
	std::string sPath;
	rsc::runtime::Properties query;
};

}
}
