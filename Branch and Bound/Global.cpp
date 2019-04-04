//
//  Global.cpp
//  Branch and Bound
//
//  Created by Sichen Zhong on 4/1/15.
//  Copyright (c) 2015 Sichen Zhong. All rights reserved.
//

#include "Global.h"


//DEFAULT CONSTRUCTOR/INITIALIZE VALUES OF DEFAULT CONSTRUCTOR
//-----------------------------------------------------------------------------------------------------------
Global::Global():MaxUpper(NULL),MaxLower(NULL),size(0),baseProb(),ia(NULL),ja(NULL),A(NULL),norm(0),M(0),minUpper(numeric_limits<double>::max()){}


//SET VALUES OF PRIVATE MEMBERS
//-----------------------------------------------------------------------------------------------------------

void Global::init(){
    //preprocess to find what the size is. Remember the size of ia, ja, and A depends on the number of non-zero entries in constraints of LP
    
    size = 0;
    
    for(int i=0; i<bigtheta.size(); i++)
    {
        for(int j=0; j<bigtheta[0].size(); j++)
        {
            if (bigtheta[i][j] > 0)
            {
                size = size+1;
            }
        }
    }
    //there are bigtheta.size() number of nonzero coefficients in the constraint sum(wi)=M, and there are bigtheta[0].size() number of -1's for each variable y in each constraint.
    
    size = size + bigtheta[0].size() + bigtheta.size();
    
    ia = new int[size+1];
    memset(ia,0,(size+1)*sizeof(int));
    ja = new int[size+1];
    memset(ja,0,(size+1)*sizeof(int));
    A = new double[size+1];
    memset(A,0,(size+1)*sizeof(double));
    
}

double Global::Objective()
{
    return MaxUpper->getUpper() - MaxLower->getLower();
}


void Global::setMaxUpper(Node* temp)
{
    MaxUpper=temp;
}


void Global::setMaxLower(Node* temp)
{
    MaxLower=temp;
}


void Global::setNorm(int p)
{
    norm = p;
}


void Global::setM(int m)
{
    M = m;
}

void Global::setminUpper(double value)
{
    this->minUpper = value;
}

void Global::setbigtheta()
{
    vector<double> temp;
    double temp2;
    
    for(int n=0; n<theta[0].size(); n++)
    {
        for(int i=0; i<theta.size()-1; i++)
        {
            for(int j=i+1; j<theta.size(); j++)
            {
                temp2 = pow(fabs(theta[i][n] - theta[j][n]),norm);
                temp.push_back(temp2); //WE MAY MULTIPLY BY 100 HERE TO ACCOUNT FOR PRECISION ERRORS. THE VALUE OF 100 IS ARBITRARY. WE CAN CHANGE THIS TO DIVIDING BY THE SMALLEST NUMBER IN THETA AS WELL. MULTIPLYING BY A CONSTANT MAY DECREASE NUMBER OF ITERATIONS (AT LEAST FOR SIMPLEX METHOD). MULTIPLYING BY A CONSTANT DOES NOT CHANGE THE OUTCOME OF THE GREEDY ALGORITHM USED FOR THE LOWER BOUND.
            }
        }
        
        bigtheta.push_back(temp);
        temp.clear();
    }
}

void Global::setbigtheta2(vector<vector<double>> theta)
{
    //take the transpose of theta and set it to bigtheta
    vector<double> temp;
    double temp2;
    
    for(int n=0; n<theta[0].size(); n++)
    {
        for(int i=0; i<theta.size(); i++)
        {
            temp2 = theta[i][n];
            temp.push_back(temp2); ////WE MAY MULTIPLY BY 100 HERE TO ACCOUNT FOR PRECISION ERRORS. THE VALUE OF 100 IS ARBITRARY. WE CAN CHANGE THIS TO DIVIDING BY THE SMALLEST NUMBER IN THETA AS WELL. MULTIPLYING BY A CONSTANT MAY DECREASE NUMBER OF ITERATIONS (AT LEAST FOR SIMPLEX METHOD). MULTIPLYING BY A CONSTANT DOES NOT CHANGE THE OUTCOME OF THE GREEDY ALGORITHM USED FOR THE LOWER BOUND. NOTE THAT THE OBJECTIVE IS MAGNIFIED BY 100!!
        }
        
        bigtheta.push_back(temp);
        temp.clear();
    }
}


void Global::setglpbase() //When setbigtheta() has been run, and the sizes of ia, ja, and A are defined, we can then run this function to set up a base problem

{
    baseProb = glp_create_prob();
    LP::PreprocessConstraints(bigtheta, baseProb, M);
    LP::PreprocessCoefficientA(bigtheta, ia, ja, A); //Note that ia, ja, A are all pointers to the first element of some array with preallocated space
}


void Global::settheta(vector<vector<double>> mat)
{
    theta=mat;
}
//-----------------------------------------------------------------------------------------------------------




//NODE OPERATION: EXPAND A NODE INTO 2 DIFFERENT NODES
//-----------------------------------------------------------------------------------------------------------
void Global::Expand(Node* N)
{
    //1)Find the new bus we are adding to our tree given we already have the Node to start growing our tree (N)
    int newBus(0);
    vector<int> rightLeafPath;
    vector<int> leftLeafPath;
    
    newBus=N->getfirstbus();             //gets the first bus of N that was found in Greedy and assigns it to newBus
    
    //TESTING
    //-----------------------------------------
    //cout<<"newBus has value: "<<N->getfirstbus()<<endl;
    //-----------------------------------------
    
    leftLeafPath=N->getPath();
    leftLeafPath.push_back(-1*newBus);
    
    //TESTING
    //-----------------------------------------
    /*
    cout<<"leftLeafPath is: ";
    for (vector<int>::iterator it=leftLeafPath.begin(); it!=leftLeafPath.end(); it++)
    {
        cout<<" "<<*it;
    }
    cout<<endl;
     */
    //-----------------------------------------
    
    rightLeafPath=N->getPath();
    rightLeafPath.push_back(newBus);
    
    //TESTING
    //-----------------------------------------
    /*
    cout<<"rightLeafPath is: ";
    for (vector<int>::iterator it=rightLeafPath.begin(); it!=rightLeafPath.end(); it++)
    {
        cout<<" "<<*it;
    }
    cout<<endl;
    cout<<endl;
    cout<<"Constructing new paths complete"<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"Beginning Calculation of parameters for left node..."<<endl;
    */
    //-----------------------------------------
    
    //2)redefine N's private members.(IOW, redefine N as now the "left" leaf. Note that _next and _prev are not changed
    N->setPath(leftLeafPath);                             //update the path to NOT take the new bus
    N->setUpper(bigtheta,baseProb, ia, ja, A, size, M);   //using the updated path, redefine upper
    N->setLower(bigtheta, norm, M);     //set lower and firstbus for left node
    
    
    
    //FOR TESTING
    //-----------------------------------------
    /*
    cout<<"Left Node's lower bound value is: "<<N->getLower()<<endl;
    cout<<"Left Node's upper bound value is: "<<N->getUpper()<<endl;
    cout<<"Calculation of parameters for left node complete"<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"Beginning Calculation of parameters for right node..."<<endl;
     */
    //-----------------------------------------
    
    //3)Create new "right" leaf node and define path,firstbus,upper,lower in that order
    //---Note that one must delete rightLeaf manually since it is dynamically allocated.
    //---If we let the algorithm run 10 times, and then add delete, the previous 10
    //---times rightLeaf was generated will still NOT BE deleted!!!!
    
    
    Node* rightLeaf = new Node();                                   //allocate memory for a new Node and declare pointer to that memory
    rightLeaf->setPath(rightLeafPath);                              //set the path of the new Node to TAKE the new bus
    rightLeaf->setUpper(bigtheta, baseProb, ia, ja, A, size, M);    //using the new path, set upper for new Node
    rightLeaf->setLower(bigtheta, norm, M);                         //using the new path, set lower and firstbus for new Node
    
    
    //FOR TESTING
    //-----------------------------------------
    /*
    cout<<"Right Node's lower bound value is: "<<rightLeaf->getLower()<<endl;
    cout<<"Right Node's upper bound value is: "<<rightLeaf->getUpper()<<endl;
    cout<<"Calculation of parameters for right node complete"<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"Beginning linkage of 2 new nodes to existing doubly linked list"<<endl;
    */
     //-----------------------------------------
    
    //a)connect newly constructed node to N and N's successor.
    
    rightLeaf->setNext(N->next());
    if(N->next() != NULL)
    {
        N->next()->setPrevious(rightLeaf);
    }
    
    rightLeaf->setPrevious(N);
    
    N->setNext(rightLeaf);
    
    //FOR TESTING
    //-----------------------------------------
    /*
    cout<<"Node Linkage Complete"<<endl;
    cout<<endl;
    cout<<"Expand done"<<endl;
    cout<<endl;
    */
    //-----------------------------------------
    
}

bool Global::isDone()const{
    vector<int> path = this->MaxLower->getPath();
    int count = 0;
    for(const int& p: path){
        if(p > 0)
            count++;
    }
    return count >= this->M;
}