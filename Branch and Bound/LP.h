//
//  LP.h
//  Branch and Bound
//
//  Created by Sichen Zhong on 4/11/15.
//  Copyright (c) 2015 Sichen Zhong. All rights reserved.
//

#ifndef __Branch_and_Bound__LP__
#define __Branch_and_Bound__LP__

#include <stdio.h>
#include "vector"
#include "glpk.h"
#include <cassert>
#include "sstream"
#include "iostream"
#include <math.h>
using namespace std;


class LP

{
public:
    
    static void PreprocessConstraints(const vector<vector<double>> &bigtheta, glp_prob* temp, size_t M); //Used to preprocess our skeleton/base LP
    
    static void ChangeWiConstraints(const vector<int> &path, glp_prob* temp); //Change the w_i constraints depending on the path of current node
    
    static void PreprocessCoefficientA(const vector<vector<double>> &bigtheta, int* ia, int* ja, double* A);

    
};

#endif /* defined(__Branch_and_Bound__LP__) */