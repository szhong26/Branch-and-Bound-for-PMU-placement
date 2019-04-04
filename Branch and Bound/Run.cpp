//
//  Run.cpp
//  Branch and Bound
//
//  Created by Sichen Zhong on 4/20/15.
//  Copyright (c) 2015 Sichen Zhong. All rights reserved.
//

#include "Run.h"

//RUN THE ALGORITHM
//-----------------------------------------------------------------------------------------------------------

void Run::BranchandBound(const vector<vector<double>>& theta, int norm, int M, double epsilon)


{
    clock_t tStart = clock();
    ofstream outputfile("/Users/sichenzhong/Desktop/Sichen/Graduate School/Algorithms/Code/Branch and Bound/Branch and Bound/mat400 result output/mat400_sim_output_M8.txt");
    ofstream output;
    output.open("/Users/sichenzhong/Desktop/Sichen/Graduate School/Algorithms/Code/Branch and Bound/Branch and Bound/mat400 result output/mat400_sim_output_M8.txt");
    
    output<<"--------------------------------------------------------------------------"<<endl;
    output<<"Initializing Algorithm..."<<endl;
    output<<endl;
    //-----------------------------------------------------------------------------------------------------------
    Global temp; //Note that when we declare object temp, init() is run, so size ia, ja, and A are set
    
    //set all private members of temp
    //first define the given inputs
    temp.setM(M);
    temp.setNorm(norm);
    temp.settheta(theta);
    //temp.setbigtheta();
    temp.setbigtheta2(theta);
    //-line 1 below initializes the sizes of arrays ia, ja, and A.
    //-line 2 below initializes and creates the problem baseProb along with all the fixed constraints. For the base case, we assume all w_i's are between 0 and 1.
    temp.init();
    temp.setglpbase();
    //-----------------------------------------------------------------------------------------------------------
    output<<"Constructing Dummy root Node"<<endl;
    output<<endl;
    //-----------------------------------------------------------------------------------------------------------
    //Initialize root and declare variables before constructing Branch and Bound Tree. We run Greedy first to find the first bus picked. This is our root node.
    //The path of the root node is empty. We calculate Lower and Upper bound values for root node. Upper bound has no variables restricted.
    //Find the first bus for the root
    
    
    Greedy begin;
    
    begin.setM(1);
    begin.setp(temp.getnorm());
    begin.settotalbus(temp.getBigTheta().size());
    begin.DefaultU();
    unordered_set<int> P_init;
    P_init.clear();
    begin.setP(P_init);
    
    
    int startbus=(begin.GreedyAlg(temp.getBigTheta()))+1;  //The +1 is because the buses chosen by greedy start from bus 0 to N-1 instead of 1 to N, and firstbus private member ranges from 1 to N.
    
    Node* root = new Node();
     root->setfirstbus2({startbus});
     root->setUpper(temp.getBigTheta(), temp.getProb(), temp.getia(), temp.getja(), temp.getA(), temp.getsize(), temp.getM());
     root->setLower(temp.getBigTheta(), temp.getnorm(), temp.getM());
    
    //-----------------------------------------------------------------------------------------------------------
    //Define our maxU and maxL priority queues. In other words, we initiate the heap/priority queue here. Note that the set of lower bounds is only used in testing U_k -L_k. Priority queues are initially empty.
    
    priority_queue<Node*, vector<Node*>, compareUpper> maxU;
    
    maxU.push(root);
    
    temp.setMaxUpper(maxU.top());
    temp.setMaxLower(maxU.top());
    
    
    output<<"Starting U_0 is: "<<temp.getMaxUpper()->getUpper()<<endl;
    output<<"Starting L_0 is: "<<temp.getMaxLower()->getLower()<<endl;
    output<<"Starting (U_0 - L_0)/L_0 is: "<<abs(temp.getMaxUpper()->getUpper()-temp.getMaxLower()->getLower())/temp.getMaxLower()->getLower()<<endl;
    output<<"Initializiation Complete"<<endl;
    output<<"--------------------------------------------------------------------------"<<endl;
    
    //-----------------------------------------------------------------------------------------------------------
    output<<endl;
    output<<"Beginning main algorithm loop..."<<endl;
    output<<endl;
    
    //ALGORITHM MAIN LOOP
    //-----------------------------------------------------------------------------------------------------------
    
    int iteration = 1;
    
    while((temp.getMaxUpper()->getUpper()-temp.getMaxLower()->getLower())/temp.getMaxLower()->getLower()>epsilon)
    {
        assert(maxU.size()); // make sure there are elements in the priority queue
        //Make a copy of the node in maxU with the largest upper bound
        //With this copy, expand it into 2 new nodes. Update copy as the left node and create a new node that is the successor to copy
        //insert copy and copy->next()) back into maxU
        //------------------------------------------------
        Node* copy = (maxU.top());
        maxU.pop();
        assert(copy);  // make sure the maxupper node is valid
        
        
        //Test if the current Node we are going to expand (copy) already has M solutions.
        //------------------------------------------------
        
        vector<int> path = copy->getPath();
        int solutionsize=0;
        
        output<<"--------------------------------------------------------------------------"<<endl;
        output<<"Beginning iteration: "<<iteration<<endl;
        output<<"The node with the largest upper bound's path is: root";
        for (auto it=path.begin(); it!=path.end(); it++)
        {
            if(*it>0)
            {
                output<<" "<<*it;
                solutionsize++;
            }
        }
        output<<endl;
    
        if (solutionsize == M)
        {
            output<<"We ended on iteration: "<<iteration<<endl;
            output<<"The objective value is: "<<temp.getMaxLower()->getLower()<<endl;
            break;
        }
        
        //------------------------------------------------
        
        temp.Expand(copy);
        maxU.push(copy);
        maxU.push(copy->next());
        
        //------------------------------------------------
        //Update MaxUpper and MaxLower for Global temp object
        //1)first update MaxUpper
        //2)next update MaxLower by making 2 comparisons with the two new nodes from using expand
        //------------------------------------------------
        
        temp.setMaxUpper(maxU.top());
        
        if(temp.getMaxLower()->getLower() < copy->getLower())
        {
            temp.setMaxLower(copy);
        }
        
        if(temp.getMaxLower()->getLower() < copy->next()->getLower())
        {
            temp.setMaxLower(copy->next());
        }
        
        
        //------------------------------------------------
        output<<endl;
        output<<"The current U_k is: "<<temp.getMaxUpper()->getUpper()<<endl;
        output<<"The current L_k is: "<<temp.getMaxLower()->getLower()<<endl;
        
        double t = abs(temp.getMaxUpper()->getUpper()-temp.getMaxLower()->getLower())/temp.getMaxLower()->getLower();
        
        output<<"The current (U_k - L_k)/L_k is: "<<t<<endl;
        output<<endl;
        output<<"The buses we pick from the max lower solution are(LBpath): "<<endl;
        for(int i : temp.getMaxLower()->getLBpath())
        {
            output<<i+1<<endl;
        }
        
        output<<"End of iteration: "<<iteration<<endl;
        output<<"--------------------------------------------------------------------------"<<endl;
        output<<endl;
        
        iteration++;
    }
    
    //-----------------------------------------------------------------------------------------------------------
    
    
    output<<"Algorithm complete"<<endl;
    //-----------------------------------------------------------------------------------------------------------
    
    output.close();
    
    //GARBAGE COLLECTION
    //-----------------------------------------------------------------------------------------------------------
    while(root)
    {
        Node* temp = root->next();
        delete root;
        root = temp;
    }
    //-----------------------------------------------------------------------------------------------------------
    
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}
//---------------------------------------------------------------------------------------------------------------------------------------------



