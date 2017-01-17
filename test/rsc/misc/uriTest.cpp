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

#include <stdexcept>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "rsc/misc/uri.h"

using namespace std;
using namespace testing;
using namespace rsc::misc;

TEST(uriTest, invalidURI)
{
	EXPECT_THROW(uri("spread///foo"), invalid_argument);
	EXPECT_THROW(uri("spread:///foo#12"), invalid_argument);
}

TEST(uriTest, validURIs)
{
	uri u("spread://localhost:42/foo/bar");
	EXPECT_EQ(u.scheme(), "spread");
	EXPECT_EQ(u.host(), "localhost");
	EXPECT_EQ(u.port(), "42");
	EXPECT_EQ(u.path(), "/foo/bar");

	u = uri("spread:/host//foo//bar");
	EXPECT_EQ(u.path(), "/host//foo//bar");
}

TEST(uriTest, emptyParts)
{
	uri u("/");
	EXPECT_TRUE(u.scheme().empty());
	EXPECT_TRUE(u.host().empty());
	EXPECT_TRUE(u.port().empty());
	EXPECT_EQ(u.path(), "/");

	u = uri (":4711/foo/bar");
	EXPECT_TRUE(u.scheme().empty());
	EXPECT_TRUE(u.host().empty());
	EXPECT_EQ(u.port(), "4711");
	EXPECT_EQ(u.path(), "/foo/bar");
}

TEST(uriTest, queryURI)
{
	uri u("?key1=value1&key3=value3");
	u = uri("spread://localhost:42/foo/bar?host=foo&port=17");
	EXPECT_EQ(u.scheme(), "spread");
	EXPECT_EQ(u.host(), "localhost");
	EXPECT_EQ(u.port(), "42");
	EXPECT_EQ(u.path(), "/foo/bar");
}

TEST(uriTest, ipv4)
{
#define IP "292.168.0.1"
	uri u("//" IP ":13");
	EXPECT_EQ(u.host(), IP);
	EXPECT_EQ(u.port(), "13");
}
