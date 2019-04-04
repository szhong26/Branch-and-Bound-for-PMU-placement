#include "matrixloader.h"

maxtrixLoader::maxtrixLoader(string file):_fhandler(file.c_str(),std::ifstream::in)
{
    if(!this->_fhandler.good())
        throw runtime_error("can not open matrix file " + file);
}
/*
void    maxtrixLoader::load(vector<vector<double> >& m){
    size_t row = 0;
    size_t col = 0;
    this->_fhandler>>row;
    this->_fhandler>>col;
    assert(row);
    assert(col);
    double val = 0;
    for(size_t i = 0; i < row; i++){
        m.push_back(vector<double>());
        for(size_t j = 0; j < col; j++){
            if(this->_fhandler.eof())
                throw runtime_error("There is not enough elements in input matrix file\n");
            this->_fhandler>>val;
            m.back().push_back(val);
        }
    }
    cout<<"The matrix has "<<row << " rows and "<< col << " columns"<<endl;
    if(!this->_fhandler.eof())
        cerr<<"There some extra numbers after the specified matrix!"<<endl;
}
*/

void    maxtrixLoader::load(vector<vector<double> >& m){
    size_t row = 0;
    size_t col = 0;
    this->_fhandler>>row;
    this->_fhandler>>col;
    assert(row);
    assert(col);
    double val = 0;
    m.clear();
    for(size_t i = 0; i < col; i++){
        m.push_back(vector<double>());
    }
    for(size_t j = 0; j < row ; j++){
        for(size_t i = 0; i < col; i++){
            if(this->_fhandler.eof())
                throw runtime_error("There is not enough elements in input matrix file\n");
            this->_fhandler>>val;
            m[i].push_back(val);
        }
    }
    cout<<"The matrix has "<<row << " rows and "<< col << " columns"<<endl;
    if(!this->_fhandler.eof())
        cerr<<"There some extra numbers after the specified matrix!"<<endl;
}

