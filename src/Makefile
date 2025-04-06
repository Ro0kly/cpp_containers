all:
	g++ -fno-elide-constructors -std=c++20 main.cc 
	./a.out

leaks:
	leaks -atExit -- ./a.out
