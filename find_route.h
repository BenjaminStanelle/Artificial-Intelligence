#ifndef __FIND_H
#define __FIND_H


#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <queue>
#include <iomanip>

/*
Node*   : represents the parent node
string  : represents the current state
float   : represents the step path cost to the city
float   : represents the actual cumulative path cost to this node (eliminates any heuristic cost with it)
float   : represents the cost evaluation (cost to the city + heuritic function)
int     : represents the depth
bool    : represents if we are doing uninformed search or informed 
*/
class Node{
    public:
        Node(Node*, std::string, float, float, float, int, bool);
        
        Node();
        
        Node(const Node &node);

        Node& operator=(const Node& node);

        ~Node();

        // setters (comes in handy when reversing linkedlist)
        void setParent(Node*);

        // getters
        float       getPathCost();
        
        bool        getUSearch();
        
        Node*       getParentNode();
        
        float       totalPathCost();
        
        float       evaluationCost();

        int         getNodeDepth();
        
        std::string getState();

    
    private:
        Node*       _parent;
        std::string _state;
        float       _pathCost;
        float       _totalPathCost;
        float       _evaluationCost;
        int         _depth;
        bool        _uSearch;
};


// Compare Node*'s in the priority queue
class CompareMyNodePtr{
    public:
        bool operator() (Node* left, Node* right);
};

void expand(Node* n, std::priority_queue<Node*, std::vector<Node*>, CompareMyNodePtr>& f, std::multimap<std::string, std::pair<std::string, float>> a, int& n_gen, std::map<std::string, float> hmap);

void readInputFile(std::string filename, std::multimap<std::string, std::pair<std::string, float>> &data);

void readHeuristicFile(std::string filename, std::map<std::string, float> &heuristic);

bool isExplored(std::vector<std::string> explored, std::string state);

Node* reverse(Node* head);
#endif
