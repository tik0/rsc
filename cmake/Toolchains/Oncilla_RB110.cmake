# - Toolchain file for AMARSi Oncilla Platform (RoBoard RB 110) 
# Will adjust compiler flags and target architecture.
#
# Copyright (C) 2010 by Arne Nordmann <anordman at cor-lab dot de>
#
# This file may be licensed under the terms of the
# GNU Lesser General Public License Version 3 (the ``LGPL''),
# or (at your option) any later version.
#
# Software distributed under the License is distributed
# on an ``AS IS'' basis, WITHOUT WARRANTY OF ANY KIND, either
# express or implied. See the LGPL for the specific language
# governing rights and limitations.
#
# You should have received a copy of the LGPL along with this
# program. If not, go to http://www.gnu.org/licenses/lgpl.html
# or write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# The development of this software was supported by:
#   CoR-Lab, Research Institute for Cognition and Robotics
#     Bielefeld UniversityMESSAGE(STATUS "Loading toolchain for Oncilla system")

# System description
SET(CMAKE_SYSTEM_NAME "Linux")
SET(CMAKE_SYSTEM_PROCESSOR "i586")
SET(UNIX "True")
SET(CMAKE_COMPILER_IS_GNUCXX "True")

# CXX compiler flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=i586" CACHE STRING "" FORCE)
