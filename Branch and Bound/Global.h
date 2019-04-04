//
//  Global.h
//  Branch and Bound
//
//  Created by Sichen Zhong on 4/1/15.
//  Copyright (c) 2015 Sichen Zhong. All rights reserved.
//

#ifndef __Branch_and_Bound__Global__
#define __Branch_and_Bound__Global__

#include <stdio.h>
#include <string.h>
#include "Node.h"
#include <vector>
#include "glpk.h"
#include <algorithm>

using namespace std;

class Global

{
private:
    
    //ORDER SHOULD BE THE SAME AS DEFAULT CONSTRUCTOR
    //private members dealing with Node data structure
    Node* MaxUpper;
    Node* MaxLower;
    
    //private members dealing with the input phase angle matrix
    vector<vector<double>> theta; //input matrix theta
    vector<vector<double>> bigtheta; //big theta matrix from the paper needed for linear program
    
    //private members dealing with setting up the lp problem before calling glpk solver
    int size; //contains the number of non-zero entries in the constraints of the upper bound linear program
    glp_prob* baseProb;
    int *ia;
    int *ja;
    double *A;
    
    //Other private members
    int norm; //norm value
    int M;  //desired bus subset size in solution
    double minUpper;
    
    
    
public:
    //RETRIVE GLOBAL PRIVATE MEMBERS
    //-----------------------------------------------------------------------------------------------------------
    Node* getMaxUpper(){return this->MaxUpper;}
    Node* getMaxLower(){return this->MaxLower;}
    vector<vector<double>> getTheta(){return this->theta;}
    vector<vector<double>> getBigTheta(){return this->bigtheta;}
    int getsize(){return this->size;}
    int getnorm(){return this->norm;}
    int* getia(){return this->ia;}
    int* getja(){return this->ja;}
    double* getA(){return this->A;}
    glp_prob* getProb(){return this->baseProb;}
    int getM(){return this->M;}
    bool isDone()const;
    double getminUpper(){return this->minUpper;}
    
    //-----------------------------------------------------------------------------------------------------------
    //PREPROCESS THETA MATRIX
    //-----------------------------------------------------------------------------------------------------------
    void setbigtheta(); //need to set Theta before setting big theta
    void setbigtheta2(vector<vector<double>>); //manually set bigtheta
    //-----------------------------------------------------------------------------------------------------------
    
    
    //DEFINE GLOBAL PRIVATE MEMBERS
    //-----------------------------------------------------------------------------------------------------------
    Global();
    void init();
    double Objective(); //Current Uk-Lk
    void settheta(vector<vector<double>> mat);
    void setMaxUpper(Node* temp);
    void setMaxLower(Node* temp);
    void setSecondMax();
    void setNorm(int p);
    void setM(int M);
    void setglpbase();
    void setminUpper(double);
    //-----------------------------------------------------------------------------------------------------------
    
    
    
    //NODE OPERATIONS
    //-----------------------------------------------------------------------------------------------------------
    void Expand(Node*);
    //-----------------------------------------------------------------------------------------------------------
    ~Global(){this->clean();}
    
private:
    //clean all pointers
    void clean()
    {
        if(ia)
            delete []ia;
        ia = NULL;
        if(ja)
            delete []ja;
        ja = NULL;
        if(A)
            delete []A;
        A = NULL;
        this->destroyList();
    }
    
    void destroyList()
    {
    
    }

};

#endif /* defined(__Branch_and_Bound__Global__) */