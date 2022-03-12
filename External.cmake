

######################################################################
# EXTERNAL LIBRARIES
######################################################################
if (NOT EXTERNAL_INSTALL_LOCATION)
	set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external)
endif()
if (NOT IS_DIRECTORY ${EXTERNAL_INSTALL_LOCATION})
	file(MAKE_DIRECTORY ${EXTERNAL_INSTALL_LOCATION})
endif()


include(ExternalProject)
# External include directory
include_directories(${EXTERNAL_INSTALL_LOCATION})
add_custom_target(update)



#ExternalProject_Add(library
#  GIT_REPOSITORY GitHub CLONE LINK
#  SOURCE_DIR ${EXTERNAL_INSTALL_LOCATION}/library
#  GIT_TAG main
#  UPDATE_DISCONNECTED 1
#  STEP_TARGETS update
#  BUILD_COMMAND ""
#  CONFIGURE_COMMAND ""
#  INSTALL_COMMAND ""
#)
#add_dependencies(update library)



#include_directories(${EXTERNAL_INSTALL_LOCATION}/library)