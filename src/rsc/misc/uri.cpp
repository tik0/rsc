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

#include "uri.h"

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_as.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix.hpp>

/* This boost::fusion magic serves as a translator between parser attribute tuples
   and our class uri */
BOOST_FUSION_ADAPT_STRUCT(
	rsc::misc::uri,
	(std::string, sScheme)
	(rsc::runtime::Properties, query)
	(std::string, sPath)
	(rsc::runtime::Properties, query)
)

namespace rsc {
namespace misc {

using namespace rsc::runtime;
namespace qi = boost::spirit::qi;
namespace bp = boost::phoenix;

struct uri_builder : qi::grammar<std::string::const_iterator, uri()> {
	typedef typename std::string::const_iterator iterator;

	uri_builder() : uri_builder::base_type(start)
	{

		start = -scheme >> host_port >> -path >> -query;

		scheme    %= +qi::char_("a-z") >> ':';
		path      %= +(qi::char_('/') >> *qi::char_("a-zA-Z0-9_.-"));
		host_port %=  -host >> -port;
		host      %=  qi::lit("//") >> qi::attr("host") 
		                            >> qi::as_string [ ipv4address | reg_name ];
		port      %=  qi::lit(':')  >> qi::attr("port") >> qi::as_string [ qi::raw [ qi::uint_ ] ];

		// reg-name = *( unreserved / pct-encoded / sub-delims )
		reg_name  %= qi::raw[+(unreserved | pct_encoded | sub_delims)];

		// IPv4 = dec-octet "." dec-octet "." dec-octet "." dec-octet
		ipv4address %= qi::raw[dec_octet >> qi::repeat(3)[qi::char_('.') >> dec_octet]];
		// octet = DIGIT / %x31-39 DIGIT / "1" 2DIGIT / "2" %x30-34 DIGIT / "25" %x30-35
		dec_octet %= qi::raw[qi::uint_parser<boost::uint8_t, 10, 1, 3>()];

		// rules for query key-value-pairs (values only parsed as_string)
		query = -( '?' >> pair % '&' ); // '?' followed by a list of pairs, separated by '&'
		pair  =  key >> '=' >> value;
		key   =  qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");
		value =  qi::as_string [ +qi::char_("a-zA-Z_0-9") ];

		// char sets
		gen_delims %= qi::char_(":/?#[]@");
		sub_delims %= qi::char_("!$&'()*+,;=");
		reserved   %= gen_delims | sub_delims;
		unreserved %= qi::alnum | qi::char_("-._~");
		pct_encoded %= qi::char_("%") >> qi::repeat(2)[qi::xdigit];
	}

	qi::rule<iterator, uri()> start;

	qi::rule<iterator, std::string()> scheme;
	qi::rule<iterator, std::string()> path;

	qi::rule<iterator, std::string()> ipv4address, reg_name;

	// query content as key=value pairs
	qi::rule<iterator, Properties()> query, host_port;
	qi::rule<iterator, std::pair<Properties::key_type, Properties::mapped_type>()> pair, host, port;
	qi::rule<iterator, Properties::key_type()>     key;
	qi::rule<iterator, Properties::mapped_type()>  value;

	qi::rule<iterator, typename boost::iterator_range<iterator>::value_type()>
	gen_delims, sub_delims, reserved, unreserved, dec_octet;
	qi::rule<iterator, std::string()> pct_encoded;
};

#include <iterator>
using namespace std;
uri::uri (const std::string& source) {
	static uri_builder grammar;
	std::string::const_iterator first=source.begin(), last=source.end();
	bool is_valid = qi::parse(first, last, grammar, *this);

#if 0 // DEBUG CODE
	cout << "parsed: " << source << " : ";
	if (!is_valid) cout << "failed." << endl;
	else {
		cout << "OK";
		if (first != last) {
			cout << " incomplete: ";
			copy(first, last, ostream_iterator<char>(cout));
		}

		cout << " result: ";
		cout << scheme() << " : " << host() << ":" << port() << " : " 
		     << path() << " : " << query << endl;
	}
#endif

	if (!is_valid || first != last)
		throw std::invalid_argument ("invalid uri format: " + source);
}

uri::uri (const uri &other) {*this = other;}

uri& uri::operator=(const uri& other) {
	sScheme = other.sScheme;
	sPath   = other.sPath;
	query   = other.query;
	return *this;
}

std::string uri::host() const {
	return query.get("host", std::string());
}

std::string uri::port() const {
	return query.get("port", std::string());
}

} // namespace misc
} // namespace rsc
