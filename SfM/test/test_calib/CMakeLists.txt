# set project's name
PROJECT( test_calib )

###############################################################################
# CMake settings
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

FILE(GLOB_RECURSE SRCS_FILES "./*.h" "./*.cc")

include_directories(${PROJECT_ROOT_DIR}/src)

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/input.txt

ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS_FILES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${OpenCV_LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${TEST_ROOT_FOLDER})

