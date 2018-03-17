include_directories(${CMAKE_SOURCE_DIR}/src)
FILE(GLOB SLOGTEST_SRC *.c *.h)
SET(SLOGTEST_DEND slog)

SET(EXE_NAME slogtest)
add_executable(${EXE_NAME} ${SLOGTEST_SRC})
target_link_libraries(${EXE_NAME} ${SLOGTEST_DEND}) 
