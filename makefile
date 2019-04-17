all: test_algos.o page_replacement_algos.o
	g++ -o test_algos.exe test_algos.o page_replacement_algos.o

test_algos.o:
	g++ -c test_algos.cpp

page_replacement_algos.o:
	g++ -c page_replacement_algos.cpp

clean:
	rm -rf *.o
	rm test_algos.exe
