IF(UNIX)

	# Search Path
	SET(XENOMAI_SEARCH_PATH /usr/include/xenomai /usr/local/xenomai /usr/xenomai /usr)
	
	# Try to find xeno-config.h
	FIND_PATH(XENOMAI_DIR xeno_config.h ${XENOMAI_SEARCH_PATH})

	IF(XENOMAI_DIR)
	
		SET(XENOMAI_FOUND TRUE)

		# Set include directory
		IF(XENOMAI_DIR STREQUAL "/usr/include/xenomai")
			# We are on ubuntu
			SET(XENOMAI_INCLUDE_DIR ${XENOMAI_DIR})
			SET(XENOMAI_INCLUDE_POSIX_DIR ${XENOMAI_DIR}/posix)
		ELSE(XENOMAI_DIR STREQUAL "/usr/include/xenomai")
			SET(XENOMAI_INCLUDE_DIR ${XENOMAI_DIR}/include)
			SET(XENOMAI_INCLUDE_POSIX_DIR ${XENOMAI_DIR}/include/posix)
		ENDIF(XENOMAI_DIR STREQUAL "/usr/include/xenomai")

		# Find xenomai pthread
		FIND_LIBRARY(XENOMAI_LIBRARY_NATIVE native ${XENOMAI_DIR}/lib)
		FIND_LIBRARY(XENOMAI_LIBRARY_XENOMAI xenomai ${XENOMAI_DIR}/lib)
		FIND_LIBRARY(XENOMAI_LIBRARY_PTHREAD_RT pthread_rt rtdm ${XENOMAI_DIR}/lib)
		FIND_LIBRARY(XENOMAI_LIBRARY_RTDM rtdm ${XENOMAI_DIR}/lib)

		# Find posix wrappers
		FIND_FILE(XENOMAI_POSIX_WRAPPERS lib/posix.wrappers ${XENOMAI_SEARCH_PATH})

		# Set the linker flags
		SET(XENOMAI_EXE_LINKER_FLAGS "-Wl,@${XENOMAI_POSIX_WRAPPERS}")

		# Add compile/preprocess options
		SET(XENOMAI_DEFINITIONS "-D_GNU_SOURCE -D_REENTRANT -Wall -pipe -D__XENO__")
	
	ENDIF(XENOMAI_DIR)
	
	INCLUDE(FindPackageHandleStandardArgs)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(Xenomai DEFAULT_MSG XENOMAI_DIR)

ENDIF(UNIX)
