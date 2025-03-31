all:
	g++ -fno-elide-constructors -std=c++17 main.cc 
	./a.out

leaks:
	leaks -atExit -- ./a.out
