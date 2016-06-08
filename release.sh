#!/bin/bash

# Copyright (C) 2012, 2013, 2014 Jan Moringen <jmoringe@techfak.uni-bielefeld.de>
# Copyright (C) 2016 Johannes Wienke <jwienke@techfak.uni-bielefeld.de>
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
#     Bielefeld University

# Utility functions

function patch_file {
  file="$1"
  substitutions="$2"

  echo "Patching ${file}"
  perl -p -i -e "${substitutions}" "${file}"
}

function set_version_cmake {
  file="$1/CMakeLists.txt"
  major=$2
  minor=$3
  revision=$4

  patch_file "${file}"                                                                                  \
             "s#(SET|set)\((.*)VERSION_MAJOR([ ]+)\"([0-9]+)\"#\\1(\\2VERSION_MAJOR\\3\"${major}\"#;    \
              s#(SET|set)\((.*)VERSION_MINOR([ ]+)\"([0-9]+)\"#\\1(\\2VERSION_MINOR\\3\"${minor}\"#;    \
              s#(SET|set)\((.*)VERSION_PATCH([ ]+)\"([0-9]+)\"#\\1(\\2VERSION_PATCH\\3\"${revision}\"#"
}

# Entry point

function usage {
  echo -e "usage:
\t$ . release.sh
\t$ release MAJOR.MINOR.REVISION ISSUE

\t    where MINOR - 1 will be the released version and ISSUE is the
\t    number of the redmine issue to reference"
}

function release {

  if [ -z "$1" ] || [ -z "$2" ] ; then
    usage
    return 1
  fi

  version=$1
  issue=$2

  export major=$(echo ${version} | cut -d '.' -f 1)
  if [ -z "${major}" ] ; then
    echo "missing major version component"
    usage
    return 1
  fi

  export minor=$(echo ${version} | cut -d '.' -f 2)
  if [ -z "${minor}" ] ; then
    echo "missing minor version component"
    usage
    return 1
  fi

  export revision=$(echo ${version} | cut -d '.' -f 3)
  if [ -z "${revision}" ] ; then
    echo "missing revision version component"
    usage
    return 1
  fi

  echo "Version ${major}.${minor}.${revision}"

  message="Version bump to ${version}, refs #${issue}"

  this="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

  minor_minus_one=$(( ${minor} - 1 ))

  # Create a tag
  git checkout master
  git pull --ff-only
  git tag --annotate                                              \
          --message="Tag for ${major}.${minor_minus_one} release" \
          "release-${major}.${minor_minus_one}"

  # Create a branch
  git checkout master
  git branch "${major}.${minor_minus_one}"

  # Version bump
  git checkout master
  if ! set_version_cmake "${this}" ${major} ${minor} ${revision} ; then
    echo "Could not patch the CMake file"
    return 1
  fi
  git commit -a -m "${message}" || true

}
