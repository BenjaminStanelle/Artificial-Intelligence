#include "find_route.h"

Node::Node(Node* parent, std::string state, float pathCost, float totalPathCost, float evaluationCost, int depth, bool uSearch):
    _parent{ parent }, _state{ state }, _pathCost{ pathCost }, _totalPathCost{ totalPathCost }, _evaluationCost{ evaluationCost }, _depth{ depth }, _uSearch{ uSearch }{
    // Constructor for Node 
}

Node::Node(): _parent{NULL}, _state{"NULL"}, _pathCost{0.0}, _totalPathCost{0.0}, _evaluationCost{0.0}, _depth{0}, _uSearch{false}{
    // Empty Constructor for Node 
}

Node::Node(const Node &node): _parent{node._parent}, _state{node._state}, _pathCost{node._pathCost}, _totalPathCost{node._totalPathCost}, 
_evaluationCost{node._evaluationCost}, _depth{node._depth}, _uSearch{node._uSearch}{
    // Copy Constructor for Node
}

Node& Node::operator=(const Node &node){
    // Copy Assignment Constructor for Node
    if (&node == this)
        return *this;
    
    this->_parent         = node._parent;
    this->_state          = node._state;
    this->_pathCost       = node._pathCost;
    this->_totalPathCost  = node._totalPathCost;
    this->_evaluationCost = node._evaluationCost;
    this->_depth          = node._depth;
    this->_uSearch        = node._uSearch;
    return *this;
}

// SETTERS
void Node::setParent(Node *newParent){ this->_parent = newParent; }

// GETTERS 
int Node::getNodeDepth()    { return this->_depth; }

float Node::getPathCost()   { return this->_pathCost; }
        
bool  Node::getUSearch()    { return this->_uSearch; }
        
Node* Node::getParentNode() { return this->_parent; }
        
float Node::totalPathCost() { return this->_totalPathCost; }
        
float Node::evaluationCost(){ return this->_evaluationCost; }
        
std::string Node::getState(){ return this->_state; }


/*
Name        : opeartor()
Parmaeters  : Two Node pointers
Description : Overloading the '>' opertor two sort/compare two Node Pointers (helps create a min heap)
Returns     : a boolean (in this case false) value if the left > right
*/
bool CompareMyNodePtr::operator() (Node* left, Node *right){
    if (left->getUSearch()) {
        return left->totalPathCost() > right->totalPathCost();
    }
    else {
        return (left->evaluationCost() ) > (right->evaluationCost());
    }
}


/*
Name        : readHeuristicFile
Parameters  : A reference to a map to be populated with a key that is type string and value for heuristic value for that key
Description : Populates the map and brings the heuristic values into code to be used for informed searches
Returns     : Void
*/
void readHeuristicFile(std::string filename, std::map<std::string, float> &h){
    std::ifstream ifs {filename};
    if (!ifs)
        std::cerr << "Could not read in file: " << filename << std::endl;
    
    std::string city;
    float cost;
    while (ifs){
        std::string line;
        std::getline(ifs, line);
        std::stringstream ss(line);
        
        ss >> city >> cost;
        if (city != "END")
            h.insert({city, cost});
    }
}


/*
Name        : readInputFile
Parameters  : Reference multimap that uses a string as a key to hold all possible actions for the current state and the filename where the data is store 
Description : Reads in the problem data into a multimap to be used in informed and uninformed search
Returns     : Void
*/
void readInputFile(std::string filename, std::multimap<std::string, std::pair<std::string, float> > &data){
    std::ifstream ifs {filename};
    if (!ifs)
        std::cerr << "Could not read in file: " << filename << std::endl;
    
    std::string src;
    std::string dest;
    std::string cost;
    float pathCost;

    while (ifs){
        std::string line;
        std::getline(ifs, line);
        std::stringstream ss (line);
        ss >> src >> dest >> pathCost;
        
        if (src != "END")
            data.insert({src, std::make_pair(dest, pathCost)});
        
    }   
}


/*
Name        : isExplored
Parameters  : A vector of strings that represent the nodes that have been explored and the successors of the current state
Description : Check to see if a city has been explored
Returns     : Returns a bool (true if it has been explored)
*/
bool isExplored(std::vector<std::string> explored, std::string state){
    for (int i = 0; i < explored.size(); ++i){
        if (explored.at(i) == state)
            return true;
    }
    return false;
}



/*
Name        : reverse
Parameters  : The head of the linked list
Description : Reverse a linkedlist
Returns     : The head of the new linkedlist
*/
Node* reverse(Node* head){
    Node *current_head = head;
    Node *prev = NULL;
    Node *next = NULL;

    while(current_head != NULL){
        next = current_head->getParentNode();

        current_head->setParent(prev);

        prev = current_head;
        current_head = next;
    }

    head = prev;
    return head;
}

void expand(Node* n, std::priority_queue<Node*, std::vector<Node*>, CompareMyNodePtr>& f, std::multimap<std::string, std::pair<std::string, float>> a, int& n_gen, std::map<std::string, float> hmap) {
    //Goes through multimap(contains a string that is the start city, then a pair of a string that is the end city and float is the destance between start and end city)
    for (auto i = a.begin(); i != a.end(); ++i) {
        //if the first column is start state
        if (i->first == n->getState()) {    //if the start states are the same, get every end state and its path cost starting from that start state, and make a successor node for it
            if (n->getUSearch()) {
                Node* successorNode = new Node(&(*n), i->second.first, i->second.second, i->second.second + n->totalPathCost(), 0, n->getNodeDepth() + 1, n->getUSearch());
                // std::cout << "New Successor: " << successorNode->getState() << " Path cost: " << successorNode->getPathCost() << " Total Path Cost: " << successorNode->totalPathCost() << std::endl;
                f.push(successorNode);
            }
            else {
                Node* successorNode = new Node(&(*n), i->second.first, i->second.second, i->second.second + n->totalPathCost(), hmap[i->second.first] + n->totalPathCost() + i->second.second, n->getNodeDepth() + 1, n->getUSearch());
                f.push(successorNode);
            }
            n_gen++;
        }
        //if second column is start state we need if and if else because text file is undirected representation of a graph
        else if (i->second.first == n->getState()) {
            //if the start states are the same, get every end state and its path cost starting from that start state, and make a successor node for it
            if(n->getUSearch()) {
                Node* successorNode = new Node(&(*n), i->first, i->second.second, i->second.second + n->totalPathCost(), 0, n->getNodeDepth() + 1, n->getUSearch());
                // std::cout << "New Successor: " << successorNode->getState() << " Path cost: " << successorNode->getPathCost() << " Total Path Cost: " << successorNode->totalPathCost() << std::endl;
                f.push(successorNode);
            }
            else {
                Node* successorNode = new Node(&(*n), i->first, i->second.second, i->second.second + n->totalPathCost(), hmap[i->first] + n->totalPathCost() + i->second.second, n->getNodeDepth() + 1, n->getUSearch()); 
                f.push(successorNode);
            }
            n_gen++;
        }
    }
}

int main(int argc, char** argv){
    std::priority_queue<Node*, std::vector<Node*>, CompareMyNodePtr> fringe;        // Priority queue for UCS
    std::multimap<std::string, std::pair<std::string, float>> data;                 // Multimap to describe the problem set
    std::map<std::string, float> hValues;                                           // Map that describes the heuristic values
    int nGenerated = 1, nExpanded = 0;                                              // Counters for nodes generated and expanded
    std::vector<std::string> closed;                                                // Vector to hold all the states that we have visited
    std::vector<Node*> holdAll;                                                     // Hold all nodes to be deleted at the end (memory management)
    Node *headGoal = NULL;                                                          // Keeps track of the head (goal state node pointer)
    bool uSearch = false;
    Node* head = NULL;
      
    std::string goal_node= argv[3];

    // Figure out if we are doing uninformed or informed search
    if (argc == 4)
        uSearch = true;
    else if (argc == 5)
        uSearch = false;
    else
        return EXIT_FAILURE;
    

    // Read and store input file as a multimap 
    // If you are doing informed search then read in heuristic files
    readInputFile(argv[1], data);
    if (!uSearch)
        readHeuristicFile(argv[4], hValues);
    

    // Initialize fringe (priority_queue) with start node and closed as empty
    if (uSearch)
        fringe.push( new Node(NULL, argv[2], 0.0, 0.0, 0.0, 0, uSearch) ); 
    else
        fringe.push( new Node(NULL, argv[2], 0.0, 0.0, hValues[argv[2]], 0, uSearch) );
    
    
    // Perform search
    while (!fringe.empty()){
        // Pop node with the cheapest value
        Node *popNode = fringe.top();
        fringe.pop();
        // Increment the expanded nodes counter
        nExpanded++;
        // Check to see if it is a goal state

        if (popNode->getState() == goal_node){
            std::cout << "Nodes Expanded: " << nExpanded << std::endl;
            std::cout << "Nodes Generated: " << nGenerated << std::endl;
            std::cout << "Distance: " << popNode->totalPathCost() << " km" << std::endl;
            std::cout << "Route: " << std::endl;

            // Reverse linked list and display results
            head = reverse(popNode);

            // Print out linked list (path to goal)
            while (head->getParentNode() != NULL) {
                std::cout << head->getState() << " to ";
                head = head->getParentNode();
                std::cout << head->getState() << ", " << head->getPathCost() << " km" << std::endl;
            }

            // TODO: Add code to free memory
            return EXIT_SUCCESS;
        }
        else {
            //if the popped node is not in the closed set expand, else don't expand and go to next node in fringe
            if ( !isExplored(closed, popNode->getState()) ){       
                // TODO: Add code for node to be expanded because it has not yet been expanded
                closed.push_back(popNode->getState());
                if (uSearch) {                                 //Uniform Cost search expanding
                    expand(popNode, fringe, data, nGenerated, hValues);
                }
                else {                                  //A* search expanding
                    expand(popNode, fringe, data, nGenerated, hValues);
                }
                    
            }
            if (fringe.empty() && popNode->getState() != goal_node) {
                std::cout << "Nodes Expanded: " << nExpanded << std::endl;
                std::cout << "Nodes Generated: " << nGenerated << std::endl;
                std::cout << "Distance: Infinity" << std::endl;
                std::cout << "Route: \nNone" << std::endl;
            }

        }
    }


    // TODO: Add code to print out linked list
    

    return EXIT_SUCCESS;
}
