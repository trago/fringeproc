/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include <cv.h>
#include "array.h"
#include "linalg.h"


array_type Adata::type() const
{
    return UNKNOWN;
}

Adata::Adata()
{
    m_refcount = NULL;
}
Adata::~Adata()
{
}

void Adata::add_ref(const Adata& data)
{
    m_refcount = data.m_refcount;
    if (m_refcount!=NULL)
        (*m_refcount)++;
}

void Adata::create_ref(bool shared)
{
    if (!shared) createData();

    m_refcount = new int;
    *m_refcount = 1;
}

void Adata::rem_ref()
{
    if (m_refcount!=NULL) {
        (*m_refcount)--;
        if (release_mem()) {
            deleteData();
            delete m_refcount;
            m_refcount = NULL;
        }
    }
}

bool Adata::release_mem()
{
    if (m_refcount!=NULL)
        return (*m_refcount)==0;
    else
        true;
}

int Adata::get_refcount()
{
    return (*m_refcount);
}

cv::Mat Adata::cvMat()
{
  cv::Mat output;

  switch(this->type()){
  case SINGLE:
    output = cv::Mat(this->rows(), this->cols(), CV_32FC1, this->data());
    return output;
  case DOUBLE:
    output = cv::Mat(this->rows(), this->cols(), CV_64FC1, this->data());
    return output;
  case CHARACTER:
    output = cv::Mat(this->rows(), this->cols(), CV_8SC1, this->data());
    return output;
  case INTEGER:
    output = cv::Mat(this->rows(), this->cols(), CV_16SC1, this->data());
    return output;
  }
  return output;
}

sArray operator+(sArray op1, sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  sum_ss(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
} 
dArray operator+(const sArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  sum_sd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator+(const sArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  sum_sn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator+(const sArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  sum_sc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator+(const dArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  sum_ds(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator+(const dArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  sum_dd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator+(const dArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  sum_dn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator+(const dArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  sum_dc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator+(const cArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  sum_cs(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator+(const cArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  sum_cd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator+(const cArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  sum_cn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
cArray operator+(const cArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  cArray res(op1.rows(), op1.cols());
  sum_cc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator+(const nArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  sum_ns(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator+(const nArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  sum_nd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator+(const nArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  sum_nn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator+(const nArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  sum_nc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator-(const sArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  diff_ss(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator-(const sArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  diff_sd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator-(const sArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  diff_sn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator-(const sArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  diff_sc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator-(const dArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  diff_ds(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator-(const dArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  diff_dd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator-(const dArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  diff_dn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator-(const dArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  diff_dc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator-(const cArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  diff_cs(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator-(const cArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  diff_cd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator-(const cArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  diff_cn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
cArray operator-(const cArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  cArray res(op1.rows(), op1.cols());
  diff_cc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator-(const nArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  diff_ns(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator-(const nArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  diff_nd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator-(const nArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  diff_nn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator-(const nArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  diff_nc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator*(const sArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  prod_ss(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator*(const sArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  prod_sd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator*(const sArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  prod_sn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator*(const sArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  prod_sc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator*(const dArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  prod_ds(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator*(const dArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  prod_dd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator*(const dArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  prod_dn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator*(const dArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  prod_dc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator*(const cArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  prod_cs(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator*(const cArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  prod_cd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator*(const cArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  prod_cn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
cArray operator*(const cArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  cArray res(op1.rows(), op1.cols());
  prod_cc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator*(const nArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  prod_ns(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator*(const nArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  prod_nd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator*(const nArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  prod_nn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator*(const nArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  prod_nc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator/(const sArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  div_ss(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator/(const sArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  div_sd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator/(const sArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  div_sn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator/(const sArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  div_sc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator/(const dArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  div_ds(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator/(const dArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  div_dd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator/(const dArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  div_dn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator/(const dArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  div_dc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator/(const cArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  div_cs(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator/(const cArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  div_cd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator/(const cArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  div_cn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
cArray operator/(const cArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  cArray res(op1.rows(), op1.cols());
  div_cc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
sArray operator/(const nArray op1, const sArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  sArray res(op1.rows(), op1.cols());
  div_ns(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
dArray operator/(const nArray op1, const dArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  dArray res(op1.rows(), op1.cols());
  div_nd(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator/(const nArray op1, const nArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  div_nn(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
nArray operator/(const nArray op1, const cArray op2) throw(Exception)
{
  if(op1.cols()!=op2.cols() || op1.rows()!=op2.rows()) {
    Exception e("operator+: attempting to sum two arrays", 
		"Array dimensions are not the same");
    throw(e);
  }
  nArray res(op1.rows(), op1.cols());
  div_nc(op1.getData(), op2.getData(), res.getData(), op1.cols()*op1.rows());

  return res;
}
