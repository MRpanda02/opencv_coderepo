project(splitchannel)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

add_executable(splitchannel
    ${DIR_SRCS})

target_link_libraries(splitchannel ${OpenCV_LIBS})