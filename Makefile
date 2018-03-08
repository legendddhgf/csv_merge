all:
	g++ -c -Wall -Werror -pedantic csv_merge.cpp
	g++ -o csv_merge *.o

clean:
	rm -f csv_merge *.o
