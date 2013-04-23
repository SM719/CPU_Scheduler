scheduler:
	# Compile each cpp file with -c
	g++ -c -std=c++0x fileReader.cpp
	g++ -c algorithm.cpp
	g++ -c main.cpp

	# Link the object files together with -o and call them "scheduler"
	g++ -o scheduler fileReader.o algorithm.o main.o

	#You can call me Nigel "TheMakefile" Rahkola.
