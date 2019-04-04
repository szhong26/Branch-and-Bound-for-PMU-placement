//
//  Greedy.cpp
//  Branch and Bound
//
//  Created by Sichen Zhong on 3/18/15.
//  Copyright (c) 2015 Sichen Zhong. All rights reserved.
//

#include "Greedy.h"



//DEFAULT CONSTRUCTOR
//-------------------------------------------------------------------------------------------------------------
Greedy::Greedy ():ObjVal(0){}

//-------------------------------------------------------------------------------------------------------------

//-Greedy function takes in number of buses, size of subset we are allowed to take, and a n by m square matrix where each column is a theta vector denoting the phase angle readings at each of the n buses.
//-entry (i,j) in matrix theta represents the phase angle reading of bus i for line break event j
//-M is the size of the number of buses we pick
//-float p is the type of norm

//SET PRIVATE MEMBERS U,P,ObjVal
//-------------------------------------------------------------------------------------------------------------
//Convert the path vector from a Node to 2 sets U and P

void Greedy::Convert(const vector<int>& path) //Note that every value in the path of a node is between 1 and bigtheta.size. However, Greedy calculates from 0 to bigtheta.size - 1. Hence, we need to subtract 1 from every bus in the path of a node.
{
    this->DefaultU(); //U initially contains all buses from 0 to bigtheta.size()-1 
    
    
    if(path.empty()==false)
    {
        for(auto it = path.begin(); it != path.end(); it++)
        {
            if(*it>0)
            {
                P.insert(*it-1);
            }
            U.erase(abs(*it) -1);
        }
    }
}

void Greedy::setP(unordered_set<int>& manualP)
{
    P = manualP;
}


void Greedy::setU(unordered_set<int>& manualU)
{
    U = manualU;
}

void Greedy::DefaultU()
{
    U.clear();
    for(int i=0; i<totalbus; i++)
    {
        U.insert(i);
    }
    P.clear();
    
}

void Greedy::setp(int norm)
{
    p = norm;
}


void Greedy::setM(int bussize)
{
    M = bussize;
}

void Greedy::settotalbus(unsigned long totalbussize)
{
    totalbus = totalbussize;
}

void Greedy::CalcObjVal(const vector<vector<double>> &bigtheta)
{
    double CurrentMin=0;
    
    for(int j=0; j!=bigtheta[0].size(); j++)
    {
        double PartialSum=0;
        
        for(unordered_set<int>::iterator it3 = P.begin(); it3 !=P.end(); it3++)
        {
            PartialSum = PartialSum + bigtheta[*it3][j];
        }
        
        if (j==0)
        {
            CurrentMin = PartialSum;
        }
        
        if(CurrentMin>PartialSum)
        {
            CurrentMin = PartialSum;
        }
    }
    
    ObjVal = CurrentMin;
}
//-------------------------------------------------------------------------------------------------------------

//GET PRIVATE MEMBERS
//-------------------------------------------------------------------------------------------------------------
double Greedy::getObjValue()
{
    return ObjVal;
}
const unordered_set<int>& Greedy::getU()const
{
    return U;
}
const unordered_set<int>& Greedy::getP()const 
{
    return P;
}
//-------------------------------------------------------------------------------------------------------------


//GREEDY ALGORITHM CALCULATIONS
//-------------------------------------------------------------------------------------------------------------

int Greedy::GreedyAlg(const vector<vector<double>> &bigtheta)

{
    
    int b;
    vector<double> mediateSum(bigtheta[0].size(),0.0);
    for(auto it = P.begin(); it != P.end(); it++){
        for(size_t i = 0; i < bigtheta[0].size(); i++){
            mediateSum[i] += bigtheta[*it][i];
        }
    }
    int first = -1;
    while(P.size() < M)
    {
        //cout<<"****p size "<<P.size()<<endl;
        b=Nextbus(bigtheta,mediateSum);
        
        assert(b >=0);
        if(first < 0)
            first = b;
        P.insert(b);
        
        U.erase(b);
        
    }
    
    this->CalcObjVal(bigtheta);
    return first;
}

int Greedy::Nextbus(const vector<vector<double>> &bigtheta, vector<double>& mediateSum)

{
    double CurrentMin;
    double CurrentMax(numeric_limits<double>::min());
    //double PartialSum;
    double temp(0);
    int bus=-1;
    
    CurrentMax=0;
    for(unordered_set<int>::iterator it = U.begin(); it != U.end(); it++)
    {
        CurrentMin=numeric_limits<double>::max();
        
        for(int i=0; i!=mediateSum.size(); i++)
        {
            temp = mediateSum[i] + bigtheta[*it][i];
            
            if(CurrentMin>temp)
            {
                CurrentMin = temp;
            }
            
        }
        
        
        if (CurrentMax <= CurrentMin)
        {
            CurrentMax = CurrentMin;
            
            bus=*it;
        }
    }
    for(int i = 0; i != mediateSum.size(); i++){
        mediateSum[i] += bigtheta[bus][i];
    }
    
    //cout<<"The next bus chosen is: "<<'\t'<<bus+1<<endl;
    return bus;
}
//-------------------------------------------------------------------------------------------------------------


