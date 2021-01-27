#   CmakeLists for CameraImageCaptureTool

include_directories(${CMAKE_BINARY_DIR}/src ${OpenCV_INCLUDE_DIRS} ${REALSENSE_SDK_INCLUDE_DIR})
link_directories(${CMAKE_BINARY_DIR}/lib "${CMAKE_BINARY_DIR}/lib" ${REALSENSE_SDK_LIBRARIES_DIR})
file(GLOB_RECURSE CAMERAIMAGECAPTURETOOL_HEADER "${CMAKE_CURRENT_SRC_DIR}" *.h)
file(GLOB_RECURSE CAMERAIMAGECAPTURETOOL_SRC "${CMAKE_CURRENT_SRC_DIR}" *.cpp)
add_executable(CameraImageCaptureTool ${CAMERAIMAGECAPTURETOOL_HEADER} ${CAMERAIMAGECAPTURETOOL_SRC})

target_link_libraries(CameraImageCaptureTool
	${OpenCV_LIBS}
	debug realsense2${CMAKE_DEBUG_POSTFIX} optimized realsense2
)