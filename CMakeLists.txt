project(slogproj)
cmake_minimum_required(VERSION 3.0)

FILE(GLOB SLOG_SRC src/*.c src/*.h)

IF(WIN32)
	SET(SLOG_DPEND Dbghelp)
ELSEIF(UNIX)
	SET(SLOG_DPEND )
ENDIF()

SET(LIB_NAME slog)
add_library(${LIB_NAME} STATIC ${SLOG_SRC})
target_link_libraries(${LIB_NAME} ${SLOG_DPEND})

add_subdirectory(test)