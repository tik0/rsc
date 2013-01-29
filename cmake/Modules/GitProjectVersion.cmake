# - If the project is maintained in git, this macro extracts git version
# information using "git determine".
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

FIND_PROGRAM(GIT_EXECUTABLE "git")

FUNCTION(GIT_PROJECT_VERSION LATEST_TAG COMMIT_NUMBER COMMIT_ID)

    SET(${LATEST_TAG} "" PARENT_SCOPE)
    SET(${COMMIT_NUMBER} "" PARENT_SCOPE)
    SET(${COMMIT_ID} "" PARENT_SCOPE)

    IF(GIT_EXECUTABLE AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    
        MESSAGE(STATUS "This is a git repository")
        
        # first, define the last commit id using git log.
        # This is necessary, as git describe later might fail on shallow clones
        EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:g%h
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        RESULT_VARIABLE HASH_RESULT
                        OUTPUT_VARIABLE HASH_OUTPUT)
    
        IF(NOT HASH_RESULT EQUAL 0)
            MESSAGE(STATUS "Unable to extract the git commit hash.")
            RETURN()
        ENDIF()
    
        STRING(STRIP ${HASH_OUTPUT} ID)    
        SET(${COMMIT_ID} ${ID} PARENT_SCOPE)
      
        # After the commit hash was extracted safely we can try to use the full
        # git describe logic, which is now safe to fail. 
        EXECUTE_PROCESS(COMMAND ${GIT_EXECUTABLE} describe --tags --match *.*
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        RESULT_VARIABLE VERSION_RESULT
                        OUTPUT_VARIABLE VERSION_OUTPUT)
               
        # we should not fail if git execution had an error         
        IF(NOT VERSION_RESULT EQUAL 0)
            RETURN()
        ENDIF()
    
        STRING(STRIP ${VERSION_OUTPUT} VERSION_OUTPUT)
        STRING(REGEX REPLACE "(.+)-([0-9]+)-(.+)" "\\1;\\2;\\3" VERSION_MATCH ${VERSION_OUTPUT})
        
        LIST(LENGTH VERSION_MATCH MATCH_LENGTH)
        IF(NOT MATCH_LENGTH EQUAL 3)
            RETURN()
        ENDIF()
        
        LIST(GET VERSION_MATCH 0 TAG)
        LIST(GET VERSION_MATCH 1 NUMBER)
        LIST(GET VERSION_MATCH 2 ID)
        
        SET(${LATEST_TAG} ${TAG} PARENT_SCOPE)
        SET(${COMMIT_NUMBER} ${NUMBER} PARENT_SCOPE)

    ENDIF()

ENDFUNCTION()
