//
//  Node.h
//  Branch and Bound
//
//  Created by Sichen Zhong on 4/1/15.
//  Copyright (c) 2015 Sichen Zhong. All rights reserved.
//

#ifndef Node_h
#define Node_h
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include "glpk.h"
#include "Greedy.h"
#include "LP.h"
#include "CalcObj.h"


using namespace std;
bool compareFunc(std::pair<int, double> v1,std::pair<int,double> v2);

class Node
{
 
    
private:
    Node*           _prev;
    Node*           _next;
    
    
    int firstbus; //range of values from 1 to bigtheta.size()
    double upper;
    double lower;
    vector<int> path; //keeps track of which vertices are fixed or not fixed for current node. If s denotes a node, then +s implies the node is taken, and -s imples the node is not taken. Indices are from 1 to bigtheta.size()
    unordered_set<int> LBpath; // unordered_set is from P in Greedy instance we set up in Node::setLower, so indices are from 0 to bigtheta.size()-1
    vector<double> UBpath; //vector indices is from 0 to bigtheta.size() - 1. Saves nonintegral solutions to the upper bound LP
    
    
public:
    
    //DEFAULT CONSTRUCTOR
    //-------------------------------------------------------------------------------------------------------------
    Node(); //default constructor ex. Node test1(); does not call default constructor. Node test2; calls the default constructor
    //-------------------------------------------------------------------------------------------------------------
    
    
    
    //set successor and predecessor to current object. Also includes functions which returns _prev and _next
    //-------------------------------------------------------------------------------------------------------------
    void setNext(Node*);
    void setPrevious(Node*);
    
    Node* next(){return this->_next;}
    Node* prev()const{return this->_prev;}
    //-------------------------------------------------------------------------------------------------------------
    
    
    
    //return member parameters of class Node
    //-------------------------------------------------------------------------------------------------------------
    int getfirstbus();
    double getUpper();
    double getLower();
    vector<int> getPath();
    unordered_set<int> getLBpath();
    vector<double> getUBpath();      
    //-------------------------------------------------------------------------------------------------------------
    
    
    
    //set member parameters of class Node
    //-------------------------------------------------------------------------------------------------------------
    void setPath(vector<int>); //sets the path of a Node
    void setUpper(const vector<vector <double>> &bigtheta, glp_prob* temp, int* ia, int* ja, double* A, int size, size_t M);        //set upper to current object by taking PATH and theta as input
    void setLower(const vector<vector <double>> &bigtheta, int norm, size_t M);        //set lower to current object by taking PATH and theta as input
    void setfirstbus(const vector<vector <double>> &bigtheta, int norm);     //set first bus to current object by taking path and theta as input
    void setfirstbus2(int temp);
    //-------------------------------------------------------------------------------------------------------------
    
    
    
    //overloading the boolean operator (<) for priority queue used in Global::BranchandBound
    //-------------------------------------------------------------------------------------------------------------
    

    //-------------------------------------------------------------------------------------------------------------
  
};

class compareUpper
{
    public:
    bool operator () (Node* N1, Node* N2); //note that "&" means the address of N1, N2
};

class compareLower
{
    public:
    bool operator () (Node* N1, Node* N2); //note that "&" means the address of N1, N2
};

#endif