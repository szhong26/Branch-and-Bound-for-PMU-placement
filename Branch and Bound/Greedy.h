//
//  Greedy.h
//  Branch and Bound
//
//  Created by Sichen Zhong on 3/18/15.
//  Copyright (c) 2015 Sichen Zhong. All rights reserved.
//

#ifndef Greedy_h
#define Greedy_h

#include <iostream>
#include "stdio.h"
#include "vector"
#include "math.h"
#include "unordered_set"
#include <cassert>
#include <limits>
using namespace std;



class Greedy

{
    
private:
    
    size_t M; //The number of buses we ultimately want in our solution
    int p; //the value of the norm
    unordered_set<int> U;  //set of unprocessed buses (buses we have not looked at yet)
    unordered_set<int> P;  //set of processed buses (buses we must take)
    unsigned long totalbus; //total number of buses. Used for Node::DefaultU function
    
    
    double ObjVal;
    
public:
    //DEFAULT CONSTRUCTOR
    //-------------------------------------------------------------------------------------------------------------
    Greedy();
    //-------------------------------------------------------------------------------------------------------------
    
    
    //SET PRIVATE MEMBERS U,P,ObjVal
    //-------------------------------------------------------------------------------------------------------------
    //Converts path from a node to 2 sets U and P. We only need a copy of this path
    void Convert(const vector<int>& path);
    //set U and P manually. Generally not used in branch and bound algorithm. only need to use convert
    void setU(unordered_set<int>& manualU);
    void setP(unordered_set<int>& manualP);
    void setp(int norm);
    void setM(int bussize);
    void DefaultU();
    void settotalbus(unsigned long totalbussize);
    void CalcObjVal(const vector<vector<double>> &bigtheta);
    //-------------------------------------------------------------------------------------------------------------
    
    
    
    //GET PRIVATE MEMBERS U,P,ObjVal
    //-------------------------------------------------------------------------------------------------------------
    double getObjValue();
    const unordered_set<int>& getU()const;
    const unordered_set<int>& getP()const;
    
    //-------------------------------------------------------------------------------------------------------------
    
    
    
    //GREEDY ALGORITHM CALCULATIONS
    //-------------------------------------------------------------------------------------------------------------
    //GreedyAlg will output a solution that is an array with the buses we pick
    int GreedyAlg(const vector<vector<double>> &bigtheta);
    
    int Nextbus(const vector<vector<double>> &bigtheta, vector<double>&);
    //-------------------------------------------------------------------------------------------------------------


};

#endif