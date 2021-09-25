INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/)                 加入文件到cmake工程里

AUX_SOURCE_DIRECTORY(./ DIR_SRCS)                                      把文件夹下的原文件赋给变量

ADD_EXECUTABLE(PROJECTNAME ${DIR_SRCS})                      生成可执行文件

TARGET_LINK_LIBRARIES(PROJECTNAME 静态库名称)             加入静态库/动态库  只能在加入到cmake工程里的文件夹里的静态库(由文件夹里的CMakeLists.txt生成)

ADD_LIBRARY(PROJECTNAME STATIC/SHARED ${DIR_LIB_SRCS})        参数3为AUX_SOURCE_DIRECTORY赋值后的变量

CMAKE_MINIMUM_REQUIRED(VERSION X.X)                             声明CMAKE的版本

PROJECT(PROJECTNAME)                                                               PROJECT名字

SET(LIBRABY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)             把生成的库放到指定的路径下

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)      把可执行的文件放到指定的路径下

 ADD_DEFINATIONS(-std=c++11)                                          设置C++编译器版本

