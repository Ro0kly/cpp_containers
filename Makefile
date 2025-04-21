all: build_vector_test build_queue_test build_map_test
build_vector_test: 
	@g++ -std=c++20 -fprofile-arcs -ftest-coverage  tests/test_vector.cc \
	-L$(shell dirname $(shell which gcov))/../lib \
	-I/opt/homebrew/opt/googletest/include \
	-L/opt/homebrew/opt/googletest/lib \
	-lgtest -lgtest_main -lpthread \
	-o vector_test.out
	./vector_test.out

build_queue_test: 
	@g++ -std=c++20 -fprofile-arcs -ftest-coverage  tests/test_queue.cc \
	-L$(shell dirname $(shell which gcov))/../lib \
	-I/opt/homebrew/opt/googletest/include \
	-L/opt/homebrew/opt/googletest/lib \
	-lgtest -lgtest_main -lpthread \
	-o queue_test.out
	./queue_test.out

build_map_test: 
	@g++ -std=c++20 -fprofile-arcs -ftest-coverage  tests/test_map.cc \
	-L$(shell dirname $(shell which gcov))/../lib \
	-I/opt/homebrew/opt/googletest/include \
	-L/opt/homebrew/opt/googletest/lib \
	-lgtest -lgtest_main -lpthread \
	-o map_test.out
	./map_test.out

lcov:
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info \
	'/usr/*' \
	'/opt/*' \
	'*/gtest/*' \
	'*/gmock/*' \
	'*v1*' \
	-o coverage.info
	genhtml coverage.info --output-directory coverage_report
	open coverage_report/index.html       
	
clean:
	rm -f output *.gcda *.gcno coverage.info coverage_filtered.info *.out
	rm -rf coverage_report