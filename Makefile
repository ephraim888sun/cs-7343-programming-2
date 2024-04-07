

compile2:
	clang++ -g -std=c++14 project2-sleeping-teaching-assistant/*.cpp -o output
	./output

compile3:
	clang++ -g -std=c++14 project3-dining-philosophers/*.cpp -o output
	./output