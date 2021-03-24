Benjamin Stanelle   1001534907
c++11; g++ 9.3.0
structure is selection. Functions are at top and main is at bottom.
Make input1.txt, Makefile, and main.cpp are all in the same directory.
Compile in a linux command line, using Makefile by typing "make main" press enter then type "./main".

<image src="graph.png">

Assignment 1 is implementing a state space search algorithm that can a find a route between any two cities. The program will be called find_route, and will take exactly commandline aruments as follows:

./find_route input1.txt Bremen Kassel (For doing Uninformed Search)

or

./find_route input1.txt Bremen Kassel h_kassel.txt (For doing Informed Search)

Argument input_filename is the name of a text file such as input1.txt, that describes road connections between cities in some part of the world. For example, the road system described by file input1.txt can be visualized with the photo below: