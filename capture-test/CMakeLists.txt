project(capture-test)

include_directories("./")

set(capture-test_SOURCES
	capture-func.cpp
	capture-test.cpp
	dxgi-sample.cpp
	)

add_executable(capture-test
	${capture-test_SOURCES})