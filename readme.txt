Benjamin Stanelle   1001534907

Compilation Instructions:

The user needs a g++ compiler to compile the code c++17, g++ 9.3.0

Uninformed Search
g++ -c find_route.cpp -o find_route ./find_route input1.txt Bremen Kassel

Informed Search
g++ -c find_route.cpp -o find_route ./find_route input1.txt Bremen Kassel h_kassel.txt


Assignment 1 is implementing a state space search algorithm that can a find a route between any two cities. The program will be called find_route, and will take exactly commandline aruments as follows:

./find_route input1.txt Bremen Kassel (For doing Uninformed Search)

or

./find_route input1.txt Bremen Kassel h_kassel.txt (For doing Informed Search)

Argument input_filename is the name of a text file such as input1.txt, that describes road connections between cities in some part of the world. For example, the road system described by file input1.txt can be visualized with the photo named graph.