//
//  main.cpp
//  Branch and Bound
//
//  Created by Sichen Zhong on 3/18/15.
//  Copyright (c) 2015 Sichen Zhong. All rights reserved.
//

#include <iostream>
#include "fileIO.h"
#include "Run.h"



using namespace std;


int main(int argc, char* argv[])

{
    //Reading in Input Matrix and setting M,p Values
    //---------------------------------------------------------------------------------------------------------------------------------------------
    
    string file("/Users/sichenzhong/Desktop/Sichen/Graduate School/Algorithms/Code/Branch and Bound/Branch and Bound/matrix400.txt");
    vector<vector<double>> mat;
    loadMatrix(file,mat);
    cout<<endl;
    
    int p = 2;
    int M = 8;
    if(argc >= 2)
        p = atoi(argv[1]);
    if(argc >= 3)
        M = atoi(argv[2]); //Note that M=5 in the paper is M=4 in our algorithm since we do not take into account the root node. Hence, add 1
    vector<vector<double>> theta2 = mat;
    //Setting up Global instance
    //---------------------------------------------------------------------------------------------------------------------------------------------
    
    Global test;
    test.setM(M);
    test.setNorm(p);
    test.settheta(theta2);
    test.setNorm(p);
    //test.setbigtheta();
    test.setbigtheta2(theta2);
    
    //Run Branch and Bound algorithm
    //---------------------------------------------------------------------------------------------------------------------------------------------
    
     double epslon = 0.001;
     Run::BranchandBound(theta2,p,M,epslon);
     
    
    //---------------------------------------------------------------------------------------------------------------------------------------------
    
    
    //Other Testing
    //---------------------------------------------------------------------------------------------------------------------------------------------
    
    /*
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout<<endl;
    vector<double> w(test.getBigTheta().size(), 0.0);// binary vector w is from 0 to bigtheta.size() - 1
    w[384]=1;
    w[55]=1;
  
    double objective = CalcObj(w, test.getBigTheta());
    
    cout<<"The objective value for w is : "<<objective<<endl;
    
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout<<endl;
    cout<<endl;
    //-----------------------------------
    
    test.init();
    test.setglpbase();
    
    vector<int> w2 = {126,135};
  
    LP::ChangeWiConstraints(w2, test.getProb());
    glp_load_matrix(test.getProb(), test.getsize(), test.getia(), test.getja(), test.getA());
    glp_smcp param;
    glp_init_smcp(&param);
    param.tol_bnd = 1e-20;
    param.tol_dj = 1e-15;
    param.tol_piv = 1e-20;

    glp_scale_prob(test.getProb(), NULL);
    glp_simplex(test.getProb(), &param);
    
    cout<<"The corresponding solution is: ";
    
    for(int i = 1; i <= test.getBigTheta().size(); i++)
    {
        cout<<"w_"<<i<<" = "<<glp_get_col_prim(test.getProb(), i)<<"    ";
    }
    cout<<endl;
    cout<<"The objective value from Simplex is : "<<glp_get_obj_val(test.getProb())<<endl;
    
     //-----------------------------------
    
    Greedy test3;
    test3.setM(M);
    test3.setp(p);
    test3.settotalbus(test.getBigTheta().size());
    test3.Convert(w2);
    
    test3.GreedyAlg(test.getBigTheta());
    
    cout<<endl;
    cout<<"The objective value from GreedyAlg with w2 as the path is: "<<test3.getObjValue()<<endl;
    
    for(auto it = test3.getP().begin(); it!=test3.getP().end(); it++)
    {
        cout<<" "<<*it;
    }
    */
    //---------------------------------------------------------------------------------------------------------------------------------------------
    
}
