//
//  CalcObj.cpp
//  Branch and Bound
//
//  Created by Sichen Zhong on 5/13/15.
//  Copyright (c) 2015 Sichen Zhong. All rights reserved.
//

#include "CalcObj.h"



double CalcObj(vector<double> w, const vector<vector<double>> &bigtheta)
{
    double objective=0;
    
    if (w.size() != bigtheta.size())
    {
        cout<<"Dimensions do not match"<<endl;
        return 0;
    }
    
    double CurrentMin = numeric_limits<double>::max();
    
    for(int j = 0; j != bigtheta[0].size(); j++)
    {
        double PartialSum = 0;
        
        for(int i = 0; i < w.size(); i++)
        {
            PartialSum = PartialSum + w[i]*bigtheta[i][j];
        }
        
        if (CurrentMin > PartialSum)
        {
            CurrentMin = PartialSum;
        }
        
    }
    
    objective = CurrentMin;
    
    return objective;
}