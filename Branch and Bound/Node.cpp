//
//  Node.cpp
//  Branch and Bound
//
//  Created by Sichen Zhong on 4/1/15.
//  Copyright (c) 2015 Sichen Zhong. All rights reserved.
//

#include "Node.h"



//DEFAULT CONSTRUCTOR
//------------------------------------------------------------------------------------------------------------
Node::Node ():_prev(NULL),_next(NULL),firstbus(-1),upper(0),lower(0),path(),LBpath(), UBpath(){} //placing the default values in brackets is faster than placing them in the body of the function!!!!
//-------------------------------------------------------------------------------------------------------------


//NODE POINTER OPERATIONS
//------------------------------------------------------------------------------------------------------------
//Note that function parameters are passed by copy or by direct reference. If the input parameter was a node, then the function setNext first makes a copy of the node, then assigns _next to the address of the COPY(This is an expensive operation if the input parameter is a large object) However, when the function ends, the copy is destroyed, so _next would be pointing to something was destroyed!!! Hence, we must make a copy of the pointer to the succeeding node, then assign the copy to _next.

void Node::setNext(Node* successor)

{
    _next = successor;
}

void Node::setPrevious(Node* predecessor)
{
    _prev = predecessor;
}
//-------------------------------------------------------------------------------------------------------------


//RETRIEVE NODE PRIVATE VALUES
//-------------------------------------------------------------------------------------------------------------
int Node::getfirstbus(void)
{
    return firstbus;
}

double Node::getUpper(void)
{
    return upper;
}

double Node::getLower(void)
{
    return lower;
}

vector<int> Node::getPath(void)
{
    return path;
}

unordered_set<int> Node::getLBpath(void)
{
    return LBpath;
}

vector<double> Node::getUBpath(void)
{
    return UBpath;
}
//--------------------------------------------------------------------------------------------------------------



//SET NODE PRIVATE VALUES. setPath SHOULD ALWAYS BE USED FIRST SINCE THE OTHERS ALL DEPEND ON PRIVATE MEMBER path
//--------------------------------------------------------------------------------------------------------------

void Node::setPath(vector<int> a)
{
    path = a;
}

void Node::setfirstbus2(int temp)
{
    firstbus = temp;
}

void Node::setfirstbus(const vector<vector<double>> &bigtheta, int norm) //This function is generally not run since Node::setLower automatically sets the firstbus. 
{
     Greedy temp;
     temp.Convert(path);
     temp.setp(norm);
     firstbus = temp.GreedyAlg(bigtheta)+1; //Note that when Nextbus is run, ObjValue is also set for temp
}

void Node::setUpper(const vector<vector<double>> &bigtheta, glp_prob* temp,int* ia,int *ja,double *A, int size, size_t M)
//Takes in the baseProb and fixes certain w_i's according to the current path on the node
{
    //add additional constraints which fix some of the w_i's according to the path saved.
    //make a copy of the basic problem from global and assign it to NodeProb. NodeProb comes with additional constraints which fix certain w_i's of the basic problem
    
    glp_prob* NodeProb;
    
    NodeProb = glp_create_prob();
    
    glp_copy_prob(NodeProb, temp, GLP_ON);
    
    LP::ChangeWiConstraints(path, NodeProb);
    
    glp_load_matrix(NodeProb, size, ia, ja, A);
    
    //Initializing LP Simplex parameters
    //-----------------------------------------------------------
    glp_smcp param;
    glp_init_smcp(&param);
    param.tol_bnd = 1e-20;
    param.tol_dj = 1e-15;
    param.tol_piv = 1e-20;
    //-----------------------------------------------------------
    glp_scale_prob(NodeProb, NULL);
    
    glp_simplex(NodeProb, NULL);
    
    upper = glp_get_obj_val(NodeProb);
    
    double Partialsum = 0;
    vector<double> w(bigtheta.size());
    
    //cout<<endl;
    //cout<<"The corresponding solution is: ";
    for(int i = 1; i <= bigtheta.size(); i++)
    {
        w[i-1]=glp_get_col_prim(NodeProb, i);
        //if(w[i-1]!=0)
        //{
        //cout<<"w_"<<i<<" = "<<glp_get_col_prim(NodeProb, i)<<"    ";
        //}
        
        Partialsum = Partialsum + glp_get_col_prim(NodeProb, i);
    }
    UBpath = w;
    
    //cout<<"The sum of the solutions is equal to: "<<Partialsum;
    //cout<<endl;
    //cout<<endl;
    
    
    glp_delete_prob(NodeProb);
}


void Node::setLower(const vector<vector<double>> &bigtheta, int norm, size_t M)
{
    //use Greedyalg class to set lower value(lower)
    //ex. lower = Greedy::ObjValue(path)
    //also finds the firstbus
    
    Greedy temp;
    temp.setM(M);
    temp.setp(norm);
    temp.settotalbus(bigtheta.size());
    temp.Convert(path);
    
    //The +1 is for the fact that path's and firstbus private variables are from 1 to bigtheta.size()
    firstbus = temp.GreedyAlg(bigtheta) + 1;
    lower = temp.getObjValue();
    
    //cout<<"The Greedy solution without pruning is: "<<lower<<endl;
    
    //LBpath = temp.getP();
    //Update lower by comparing with upper
    //NOTE THAT Node::setUpper MUST BE SET BEFORE Node::setLower is CALLED
    //1)Convert UBpath to a vector of pairs which saves bus index and corresponding solution
    //2)Sort based on the solution value
    //3)Convert back into vector double and extract first largest M solutions to plug into calculate objective function and compare with lower
    //4)Update LBpath if Msol is greater than the lower above
    //-------------------------------------------------------------------
    vector<pair<int, double>> temp2(bigtheta.size()); //a vector of pairs. first value saves the bus index, the second value saves the solution gotten from UB
    
    for(int i=0; i<bigtheta.size(); i++)
    {
        temp2[i].first = i;
        temp2[i].second = UBpath[i];
    }
    
    std::sort(temp2.begin(),temp2.end(),compareFunc);

    vector<double> w(bigtheta.size(),0);
    for(int i=0; i<M; i++)
    {
        w[temp2[i].first] = 1;
    }
    //cout<<endl;
    
    //-------------------------------------------------------------------
    /*
    cout<<"UBpath: ";
    for(int i = 0; i<bigtheta.size(); i++)
    {
        cout<<UBpath[i]<<'\t';
    }
    cout<<endl;
    
    cout<<"Rounded path: ";
    for(int i = 0; i<bigtheta.size(); i++)
    {
        cout<<w[i]<<'\t';
    }
    cout<<endl;
    cout<<endl;
    */
    //-------------------------------------------------------------------
    
    double Msol = CalcObj(w, bigtheta);
    
    
    //-------------------------------------------------------------------
    /*
    cout<<"The upper bound solution is: "<<upper<<endl;
    cout<<"Sorted upper bound solutions and rounding largest M solutions to 1: "<<Msol<<endl;
    cout<<"The ORIGINAL lower bound without pruning is: "<<lower<<endl;
    cout<<endl;
    */
    //-------------------------------------------------------------------
    
    //-------------------------------------------------------------------
    // Yan: Find max in LP
    double UBmax = 0;
    size_t UBindx = 0;
    if (path.size() > 0) {
        for (size_t i = 0; i < path.size(); i ++) {
            if (path[i] > 0) {
                UBpath[path[i] - 1] = 0;
            }
        }
        
        for (size_t i = 0; i < UBpath.size(); i ++) {
            //cout << UBpath[i] << endl;;
            if (UBpath[i] > UBmax) {
                UBmax = UBpath[i];
                UBindx = i;
            }
        }
    }
    //cout << UBindx << endl;
    //cout << endl;
    
    //-------------------------------------------------------------------
    
    if(Msol > lower && UBindx != 0)
    {
        lower = Msol;
        for(int i=0; i<M; i++)
        {
            LBpath.insert(temp2[i].first);
        }
        this->setfirstbus2((int) UBindx + 1);
    }
     
    if(Msol <= lower)
    {
         LBpath = temp.getP();
    }
    
    
    //-------------------------------------------------------------------
    /*
    cout<<"LBpath+1: ";
    for(auto i = LBpath.begin(); i != LBpath.end(); i++)
    {
        cout<<*i+1<<'\t';
    }
     */
    //-------------------------------------------------------------------
    
}

//--------------------------------------------------------------------------------------------------------------


//Overloading the boolean operator (<) for the priority queue in BranchandBound in Global
//--------------------------------------------------------------------------------------------------------------
bool compareUpper::operator()(Node* N1, Node* N2)
{
    if (N1->getUpper() < N2->getUpper())
    {
        return true;
    }
    
    return false;
}

bool compareLower::operator()(Node* N1, Node* N2)
{
    if (N1->getLower() < N2->getLower())
    {
        return true;
    }
    
    return false;
}

bool compareFunc(std::pair<int, double> v1,std::pair<int,double> v2)
{
    return v1.second > v2.second;
}