#include "fileIO.h"

void loadMatrix(string file,vector< vector<double> >& mat){
    maxtrixLoader* tmp = new maxtrixLoader(file);
    assert(tmp);
    tmp->load(mat);
    delete tmp;
}
