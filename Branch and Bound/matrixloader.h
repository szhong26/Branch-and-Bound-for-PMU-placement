#ifndef MAXTRIXLOADER_H
#define MAXTRIXLOADER_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <stdexcept>
using std::ifstream;
using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::cerr;
using std::runtime_error;
class maxtrixLoader
{
public:
    maxtrixLoader(string file);
    void    load(vector< vector<double> >& m);
    ~maxtrixLoader(){
        this->_fhandler.close();
    }
private:
    ifstream                    _fhandler;
};

#endif // MAXTRIXLOADER_H
