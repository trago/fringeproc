#include "matrix.h"

#  ifndef NULL
#    define NULL 0
#  endif

namespace fringe {

Matrix::Matrix()
{
  mat=NULL;
  data=NULL;
  _rows=_cols=0;
  _ismanaged=true;
}

Matrix::Matrix(double* dat, int nr, int nc)
{
  int i;
  data=dat;
  mat=new double*[nc];
  if(mat!=NULL)
    for(i=0; i<nc; i++)
      mat[i]=data+(i*nr);

  _rows=nr;
  _cols=nc;
  _ismanaged=false;
}

Matrix::Matrix(int rows, int cols)
{
  int i;
  data=new double[rows*cols];
  mat=new double*[cols];
  if(mat!=NULL)
    for(i=0; i<cols; i++)
      mat[i]=data+(i*rows);

  _rows=rows;
  _cols=cols;
  _ismanaged=true;
}

Matrix::~Matrix()
{
  if(mat!=NULL)
    delete [] mat;
  if(data!=NULL)
    if(_ismanaged)
      delete [] data;
}

double* Matrix::operator[](int index)
{
  return mat[index];
}

int Matrix::cols() const
{
  return _cols;
}

int Matrix::rows() const
{
  return _rows;
}

void Matrix::resize(int rows, int cols)
{
  int i;
  if(mat!=NULL)
    delete [] mat;
  if(data!=NULL)
    delete [] data;

  data=new double[rows*cols];
  mat=new double*[cols];
  if(mat!=NULL)
    for(i=0; i<cols; i++)
      mat[i]=data+(i*rows);

  _rows=rows;
  _cols=cols;
}

Matrix& Matrix::operator=(const Matrix& mat)
{
  if(mat._cols*mat._rows!=_cols*_rows)
    resize(mat._rows,mat._cols);

  for(int i=0; i<_rows; i++)
    for(int j=0; j<_cols; j++)
      this->mat[j][i]=mat.mat[j][i];

  return *this;
}

Matrix& Matrix::operator=(const double val)
{
  for(int i=0; i<_rows; i++)
    for(int j=0; j<_cols; j++)
      mat[j][i]=val;

  return *this;
}

Matrix::Matrix(const Matrix& mat)
{
  data=NULL;
  this->mat=NULL;
  resize(mat._rows,mat._cols);
  _ismanaged=true;

  for(int i=0; i<_rows; i++)
    for(int j=0; j<_cols; j++)
      this->mat[j][i]=mat.mat[j][i];
}

bool Matrix::isNull() const
{
  return _cols*_rows == 0;
}

double* Matrix::getData() const
{
  return data;
}

double** Matrix::getMat() const
{
  return mat;
}

double Matrix::norm2()
{
  double sum=0;

  for(int i=0; i<_rows; i++)
    for(int j=0; j<_cols; j++)
      sum+=mat[j][i]*mat[j][i];

  return sum;
}

double Matrix::dot(Matrix& r)
{
  double sum=0;

  for(int i=0; i<_rows; i++)
    for(int j=0; j<_cols; j++)
      sum+=mat[j][i]*r.mat[j][i];

  return sum;
}

Matrix Matrix::operator-(Matrix& r)
{
  Matrix b(_rows,_cols);

  for(int i=0; i<_rows; i++)
    for(int j=0; j<_cols; j++)
      b.mat[j][i]=mat[j][i]-r.mat[j][i];

  return b;
}

Matrix Matrix::operator*(double k)
{
  Matrix b(_rows,_cols);
  for(int i=0; i<_rows; i++)
    for(int j=0; j<_cols; j++)
    b.mat[j][i]=mat[j][i]*k;

  return b;
}

Matrix& Matrix::operator*=(double k)
{
  for(int i=0; i<_rows; i++)
    for(int j=0; j<_cols; j++)
      mat[j][i]*=k;
  return *this;
}

Matrix& Matrix::operator+=(Matrix& r)
{
  for(int i=0; i<_rows; i++)
    for(int j=0; j<_cols; j++)
      mat[j][i]+=r.mat[j][i];
  return *this;
}

Matrix& Matrix::operator-=(Matrix& r)
{
  for(int i=0; i<_rows; i++)
    for(int j=0; j<_cols; j++)
      mat[j][i]-=r.mat[j][i];
  return *this;
}
}
