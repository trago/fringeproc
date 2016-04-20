//
// Created by julio on 4/20/16.
//

#include <opencv2/core/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
  Mat_<float> M = Mat_<float>::zeros(5,5);

  cout<< "Hola mundo" << endl;

  M(0,4)=10;

  cout<< M.reshape(25,0) << endl;
  return 0;
}

