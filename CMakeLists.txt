include("config.cmake")

cmake_minimum_required(VERSION 3.0.0)
project(ColorSpace VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(${PROJECT_NAME} WIN32 ${SRC_LISTS})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${DEF_LISTS})
target_include_directories(${PROJECT_NAME} PUBLIC ${INC_PATHS})
target_link_directories(${PROJECT_NAME} PUBLIC ${LIB_PATHS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIB_LISTS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
