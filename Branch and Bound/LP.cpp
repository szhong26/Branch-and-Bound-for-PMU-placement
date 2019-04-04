//
//  LP.cpp
//  Branch and Bound
//
//  Created by Sichen Zhong on 4/11/15.
//  Copyright (c) 2015 Sichen Zhong. All rights reserved.
//

#include "LP.h"


using namespace std;

void LP::PreprocessConstraints(const vector<vector<double>> &bigtheta, glp_prob* temp, size_t M) //used in Global::setglpbase()
{
    size_t N = bigtheta.size();
    if(N == 0) return;
    size_t K = bigtheta[0].size();
    glp_set_obj_dir(temp, GLP_MAX); //sets the problem as a maximization problem
    
    //Add K+1 rows/constraints, K+1 columns/variables
    //---------------------------------------
     glp_add_rows(temp,K+1);
     glp_add_cols(temp,N+1);
    
    //---------------------------------------
    
    //Set up the constraint that the sum of all w_i's must equal M
    //---------------------------------------
    glp_set_row_name(temp, K+1, "M");
    glp_set_row_bnds(temp, K+1, GLP_FX, M,M);
    
    //setting up the bounds on the rest of the row constraints [w^T*bigtheta]-y = xi >=0]
    //---------------------------------------
    for(int i = 1; i <K+1; i++)
    {
        stringstream b;
        b << 'x' <<i;
        
        glp_set_row_name(temp, i, b.str().c_str());
        
        glp_set_row_bnds(temp, i, GLP_LO, 0.0, 0.0);
        
        b.clear();
    }
    
    //set the variable of y to be bounded by below and define of the coefficient of y in objective function
    //---------------------------------------
    glp_set_col_name(temp, N+1, "y");
    glp_set_obj_coef(temp, N+1, 1.0);
    glp_set_col_bnds(temp, N+1, GLP_LO, 0.0, 0.0);

    
    //Set ALL wi variables to be in between 0 and 1
    //---------------------------------------
    for(int i = 1; i <= N; i++)
    {
        stringstream a;
        a << 'w'<<i;
        glp_set_col_name(temp, i, a.str().c_str());
        
        glp_set_col_bnds(temp, i, GLP_DB, 0.0, 1.0);
        
        a.clear();
    }
}



void LP::ChangeWiConstraints(const vector<int> &path, glp_prob* temp) //Used in Node::setUpper()
{
    //Now set all w_j's in our path to be either 0 or 1 depending on whether they are negative or positive in our path
    //Also, the path saved in a node ranges from values 1 to bigtheta.size() (inclusive)
    if(path.empty()==false)
    {
        for(vector<int>::const_iterator it = path.begin(); it != path.end(); it++)
        {
            assert(*it != 0);
            if (*it<0)
            {
                glp_set_col_bnds(temp, (-*it), GLP_FX, 0.0, 0.0);
            }
        
            else if(*it>0)
            {
                glp_set_col_bnds(temp, *it, GLP_FX, 1.0, 1.0);
            }
        }
    }

}



void LP::PreprocessCoefficientA(const vector<vector<double>> &bigtheta, int* ia, int* ja, double* A) //used in Global::setglpbase()
{
    assert(ia);
    assert(ja);
    assert(A);
    
    //sets up the index vectors ia, ja, and the non-zero constraint matrix A for use in GLPK
    int l = 1;
    
    for(int i=0; i<bigtheta[0].size(); i++) //bigtheta[0].size = 666 in base example.
    {
        
        for(int j=0; j<bigtheta.size(); j++) //bigtheta.size = 29 in base example
        {
            
            if(bigtheta[j][i] != 0.0) //WE MAY HAVE TO SET AN EPSILON VALUE, SO IF BIGTHETA IS LESS THAN THIS VALUE, WE VIEW IT AS ZERO!!
                                      //1)MAYBE WE CAN DIVIDE EACH VALUE IN BIGTHETA BY THE SMALLEST VALUE IN BIGTHETA. THIS IS THE PROBLEM!!!!!
                                      //2)OR WE CAN CHANGE THE PRECISION ACCURACY!!!!! MEANING IF BIGTHETA[J][I} is not greater than some threshold, we just treat that value as 0!!!!!!!!!
            {
                ia[l]=i+1;  //equals *(ia+l)
                ja[l]=j+1;
                A[l]=bigtheta[j][i];
                l++;
            }
            
        }
        
    }
    
    //set y's coefficients to -1 for each row from 1 to K
    
    int K= bigtheta[0].size();
    int N= bigtheta.size();
    
    for(int i=0; i<K; i++)
        
    {
        ia[l]=i+1;
        ja[l]=N+1;
        A[l]=-1;
        l++;
    }
    
    //set w_i's coefficients to 1 in the constraint sum(wi)=M;
    for(int j=0; j<N; j++)
    {
        ia[l]=K+1;
        ja[l]=j+1;
        A[l]=1;
        l++;
    }
    
}



