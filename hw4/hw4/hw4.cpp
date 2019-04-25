
#include <iostream>
#include <fstream>
using namespace std;

/************************************
*  Note that in general using global
*  variables is a bad habit.
*  I use globals here because I don't 
*  want to scare you with "double***"
*************************************/

int m;
int n;
double **v;  // n*(m+1) 2D-array
double **h;  // (n+1)*m 2D-array

// For internal use only. Do not really use them directly
double *real_v;
double *real_h;


void readParameters()
{
    ifstream ifs("input1", ifstream::binary);

    ifs.read((char*)&m, sizeof(int));
    ifs.read((char*)&n, sizeof(int));
	
	
    real_v = new double[n*(m+1)];
    real_h = new double[(n+1)*m];

    ifs.read((char*)real_v, sizeof(double)*n*(m+1));
    ifs.read((char*)real_h, sizeof(double)*(n+1)*m);

    v = new double*[n];
    for (int i=0; i<n; ++i){
        v[i] = &(real_v[i*(m+1)]);
		
	}
    h = new double*[n+1];
    for (int i=0; i<n+1; ++i){
        h[i] = &(real_h[i*m]);
	}

    ifs.close();
	
	
}

void release()
{

    delete []v;
    delete []h;

    delete []real_v;
    delete []real_h;
}

int main()
{
    readParameters();
    /***************************
     *
     *   your code/functions here
     *
     ***************************/

    release();
    return 0;
}

