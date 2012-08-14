# - A macro returning the current date.
#
# Copyright (C) 2012 by Johannes Wienke <jwienke at techfak dot uni-bielefeld dot de>
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

FUNCTION(CURRENT_DATE RESULT)

    IF (WIN32)
        SET(ARGS "/T")
        SET(REPLACE "(..)/(..)/..(..).*" "\\3\\2\\1")
    ELSEIF(UNIX)
        SET(ARGS "+%d/%m/%Y")
        SET(REPLACE "(..)/(..)/..(..).*" "\\3\\2\\1")
    ELSE()
        MESSAGE(WARNING "date not implemented")
        SET(${RESULT} 000000 PARENT_SCOPE)
        RETURN()
    ENDIF()
    
    EXECUTE_PROCESS(COMMAND "date" ${ARGS}
                    OUTPUT_VARIABLE DATE_OUTPUT
                    RESULT_VARIABLE DATE_RESULT)

    IF(NOT DATE_RESULT EQUAL 0)
        MESSAGE(WARNING "error calling date command")
        SET(${RESULT} 000000 PARENT_SCOPE)
        RETURN()
    ENDIF()
                    
    STRING(REGEX REPLACE ${REPLACE} DATE_STRING ${DATE_OUTPUT})
    SET(${RESULT} ${DATE_STRING} PARENT_SCOPE)
    
ENDFUNCTION()
