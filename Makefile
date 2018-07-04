build:
	clang++ -std=c++11 -stdlib=libc++ -l ncurses  main.cpp -o game && ./game