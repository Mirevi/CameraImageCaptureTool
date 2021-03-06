# top level cmake file
cmake_minimum_required (VERSION 2.6 FATAL_ERROR)
project (CameraImageCaptureTool)

include_directories("${CMAKE_SOURCE_DIR}/src")

# First for the generic no-config case (e.g. with mingw)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set(CMAKE_DEBUG_POSTFIX "d")

# CMake Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


####################################################################################################
######################################## DOWNLOAD OF DEPS ##########################################
####################################################################################################
# Only for developers who change the online dep files in this document:
# TODO for new downloads and updates deps:
# Check, if all files up to date (e.g. versions files such as /bin/bin-ver-100x or config.xml!)
# Zip it
# Upload it to the server
# Create Hash: Power shell: Get-FileHash XYZ.zip -Algorithm SHA256 + Short cut in Visual Studio to lower case: CTRL +U
# Add new Hash (in lowercase) to this document
# Check if the URL is still ok (Usually it will be incremented)
# Check if the CHECK_IF_DEPS_EXIST path and file is correct
# Commit and push changes
# At the End: Checkout and test (Best case: On a fresh windows 10 installation)
# Function for extraction of the name of the dependency in order to create appropriate directories
function(getsTheDepsNameFromURL "${URL}")
	string(FIND "${URL}" / FILENAME_STRING_POS REVERSE) # searches for the last / and returns the position in the string
	math(EXPR FILENAME_STRING_POS "${FILENAME_STRING_POS} + 1") # increments the string position since it would otherwise contain the /
	string(LENGTH "${URL}" FILENAME_STRING_LENGTH) # get the length of the whole string
	math(EXPR LENGTH_OF_DEPS_NAME "${FILENAME_STRING_LENGTH} - ${FILENAME_STRING_POS} - 4") # calculates the length of the deps name 
	string(SUBSTRING "${URL}" "${FILENAME_STRING_POS}" "${LENGTH_OF_DEPS_NAME}" DEPS_NAME) # gets the file name as string
	#message(STATUS "LENGTH_OF_DEPS_NAME ist ${LENGTH_OF_DEPS_NAME}")
	#message(STATUS "CURRENT_DEPS_PATH ist ${URL}")
	#message(STATUS "FILENAME_STRING_POS ist ${FILENAME_STRING_POS}")
	#message(STATUS "FILENAME_STRING_LENGTH ist ${FILENAME_STRING_LENGTH}")
	#message(STATUS "AUSGABE ist ${DEPS_NAME}")
	set(DEPS_NAME "${DEPS_NAME}" PARENT_SCOPE) #set the DEPS_NAME for the upper scope from where the function was called with PARENT_SCOPE
endfunction(getsTheDepsNameFromURL)

# OpenCV
set(EXPECTED_SHA256 bdc8e47b9de55f04a8bec30fb51fd38f990b875ab8d000954bad8a14e3fe4f7c )
set(CURRENT_DEPS_PATH ${CMAKE_BINARY_DIR}/../deps/opencv-4.20/)
set(URL https://claymore.mirevi.com/opencv-4.20.zip)
getsTheDepsNameFromURL("${URL}")
set(CHECK_IF_DEPS_EXIST "${CURRENT_DEPS_PATH}/include/opencv2/core.hpp")
FIND_PACKAGE( OpenCVCameraCalibrator REQUIRED)
file(COPY ${CURRENT_DEPS_PATH}/x64/vc15/bin/opencv_world420.dll DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY ${CURRENT_DEPS_PATH}/x64/vc15/bin/opencv_world420d.dll DESTINATION ${CMAKE_BINARY_DIR}/bin)

# REAL SENSE SDK
set(EXPECTED_SHA256 7d80f3d8633c9dd19a49ab840cabeda223ebc1e6b7011645aab3d35ea2a08aa3 )
set(CURRENT_DEPS_PATH ${CMAKE_BINARY_DIR}/../deps/realsense-2.34.0/)
set(URL https://claymore.mirevi.com/realsense-2.34.0.zip)
getsTheDepsNameFromURL("${URL}")
set(CHECK_IF_DEPS_EXIST "${CURRENT_DEPS_PATH}/include/h/rs_types.h")
FIND_PACKAGE( RealSense REQUIRED )
file(COPY ${CURRENT_DEPS_PATH}/bin/realsense2.dll DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY ${CURRENT_DEPS_PATH}/bin/realsense2d.dll DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY ${CURRENT_DEPS_PATH}/bin/Intel.RealSense.Viewer.exe DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY ${CURRENT_DEPS_PATH}/bin/Depth.Quality.Tool.exe DESTINATION ${CMAKE_BINARY_DIR}/bin)
# How-to-pack: Quite simple - nothing special.

# Add subdir with additional CMakeLists.txt and files
add_subdirectory("${CMAKE_SOURCE_DIR}/src")

set_target_properties(CameraImageCaptureTool PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
