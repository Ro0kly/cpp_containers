all:
	g++ -std=c++20 -arch arm64 tests/test_vector.cc \
  -I/opt/homebrew/opt/googletest/include \
  -L/opt/homebrew/opt/googletest/lib \
  -lgtest -lgtest_main -lpthread \
  -o output