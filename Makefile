

compile: 
	clang++ -g -std=c++14 ./*.cpp -o output 
	./output
	

compile2:
	clang++ -g -std=c++14 project2-sleeping-teaching-assistant/*.cpp -o output
	./output
