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

#ifndef ARRAY_H
#define ARRAY_H

#include <exception>
#include <iostream>
#include "exception.h"

#define RESTRICT_ptr *__restrict__

/**
 * @brief Array type
 * 
 * It indicates the data precision of the array
 * 
 * @author Julio C. Estrada-Rico
 * @date 2008-04-16
 */
enum array_type {

  /** Single precision data type. */
  SINGLE,
  /** Double precision data type. */
  DOUBLE,
  /** Integer precision 8 bits. */
  CHARACTER,
  /** Integer precision. */
  INTEGER,
  /** Unknown data presicion.  */
  UNKNOWN
};

namespace cv{
class Mat;
}

/**
 * @brief Generic data array (base class).
 * 
 * One can't create an array base class directly, it must be created through
 * its derived classes. This class implements a reference count and management
 * system. Then, derived data arrays use a reference system instead of a by
 * system of its data.
 * 
 * @author Julio C. Estrada-Rico
 * @date 25/02/2010
 */
class Adata {
public:
  /**
   * @brief Constructor.
   */
  Adata();
  /** 
   * @brief Regresa el tipo de datos que guarda este arreglo.
   *
   * @author Julio C. Estrada-Rico
   * @date 25/02/2010
   */
  virtual array_type type() const;
  /**
   * @brief Returns a raw pointer to data.
   */
  virtual void* data() const = 0;
 /** 
   * @brief Returns the number of columns.
   *  
   * @author Julio C. Estrada-Rico
   * @date 25/02/2010 
   */ 
  virtual int cols() const = 0;
  /** 
   * @brief Returns the number of rows. 
   *  
   * @author Julio C. Estrada-Rico
   * @date 25/02/2010 
   */ 
  virtual int rows() const = 0;
  /** 
   * Returns an OpenCV matrix. 
   *  
   * @author Julio C. Estrada-Rico
   * @date 25/02/2010 
   */ 
  virtual cv::Mat cvMat();
  /**
   * Data destructor. 
   *  
   * @author Julio C. Estrada-Rico
   * @date 25/02/2010 
   */
  virtual ~Adata();

protected:
  /**
   * It is used to add an object that refers to this data.
   *
   * @param data the data object that is added to the reference counter
   *
   * @author Julio C. Estrada-Rico
   * @date 11/06/2010
   */
  void add_ref(const Adata& data);
  /**
   * Creates the reference counter, and the data.
   *
   * @param shared set it true if the data memory is not to be created.
   *
   * @author Julio C. Estrada-Rico
   * @date 11/06/2010
   */
  void create_ref(bool shared = false);
  /**
   * Removes this object from the reference counter.
   *
   * @author Julio C. Estrada-Rico
   * @date 11/06/2010
   */
  void rem_ref();
  /**
   * Checks if there is no more objects referencing this data.
   *
   * @return true if there is at least on object, otherwise false.
   *
   * @author Julio C. Estrada-Rico
   * @date 11/06/2010
   */
  bool release_mem();
  /**
   * Returns the number of references to this data object.
   */
  int get_refcount();

  /**
   * Implements the data creation.
   *
   * @author Julio C. Estrada-Rico
   * @date 11/06/2010
   */
  virtual void createData()
  {
  }
  /**
   * Implements the data memory release.
   *
   * @author Julio C. Estrada-Rico
   * @date 11/06/2010
   */
  virtual void deleteData()
  {
  }
private:
  /**
   * The reference counter.
   *
   * @author Julio C. Estrada-Rico
   * @date 11/06/2010
   */
  int* m_refcount;
};

/**
 * @brief Matriz de datos
 * 
 * @author Julio C. Estrada-Rico
 * @date 2008-04-16
 */
template<class T, array_type PRECISION>
class Array: public Adata {
public:
  /**
   * @brief Crea una matriz con dimenciones dadas.
   * 
   * @param nr numero de renglones
   * @param nc numero de columnas
   * 
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  Array(int nr, int nc) throw (Exception)
  {
    m_free = false;
    m_nr = nr;
    m_nc = nc;
    m_data = (T*) 0;

    try {
      create_ref();
    } catch (std::exception e) {
      Exception te("Array: while creating array memory", e.what());
      throw(te);
    } catch (...) {
      Exception te("Array: while creating array memory", "Unexpected error");
    }
  }

  /**
   * Crea una matriz con datos vacios.
   * 
   * La matriz que se crea tiene dimenci�n 0.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  Array()
  {
    m_free = false;
    m_nr = 0;
    m_nc = 0;
    m_data = (T*) 0;
    create_ref(true);
  }

  /**
   * Crea una matriz copiando los datos de otra.
   * @param cpy matriz de donde se copian los datos.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  Array(const Array<T, PRECISION>& cpy) throw (Exception)
  {
    m_free = cpy.m_free;
    m_nr = cpy.m_nr;
    m_nc = cpy.m_nc;
    m_data = cpy.m_data;
    add_ref(cpy);
  }

  /**
   * Crea una matriz con una fuente de datos externa.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  Array(T* data, int nr, int nc)
  {
    m_free = false;
    m_nr = nr;
    m_nc = nc;
    m_data = data;
    create_ref(true);
  }
  
  /** 
   * @brief Makes an array copying the data of this array. 
   *  
   * @author Julio C. Estrada-Rico
   * @date 25/02/2010 
   */ 
  Array<T, PRECISION> copy() throw(Exception)
  {
    try {
      Array<T, PRECISION> cpy(m_nr, m_nc);
      copy_data(cpy.m_data, m_data, m_nr, m_nc);
      return cpy;
    } catch (std::bad_alloc e) {
      Exception except("Array: while creating array data", e.what());
      throw(except);
    } catch (...) {
      Exception except("Array: while copying data", "Unexpected error");
      throw(except);
    }
  }

  /**
   * Copia los valores de la matriz dada a esta.
   * 
   * @param mat el valor a asignar.
   *
   * @author Julio C Estrada-Rico
   * @date 27/02/210
   */
  void copy(const Array<T, PRECISION>& mat) throw (Exception)
  {
    checkDims(mat);
    copy_data(m_data, mat.m_data, m_nr, m_nc);
  }

  /**
   * Indexa la matriz usando paretesis.
   * @param c indice de la columna
   * @param r indice del renglon
   * @return a refernence to the indexed element.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  T& operator()(const int c, const int r) const throw (Exception)
  {
    return val(c, r);
  }

  /**
   * Regresa la referencia al elemento (c,r) del arreglo de datos.
   * @param c indice de la columna
   * @param r indice del renglon
   * @return a refernence to the indexed element.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  T& val(const int c, const int r) const throw (Exception)
  {
    int idx = c + r * m_nc;
    if (idx < m_nr * m_nc)
      return m_data[idx];
    else {
      Exception e("Array: while indexing array data",
          "the given index exceeds the index range of array");
      throw(e);
    }
  }

  /**
   * Indexa la matriz usando la notaci�n matriz[col][row].
   * @param c el n�mero de columna.
   * @return a reference to the given column. 
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  T* operator[](int r) const throw (Exception)
  {
    if (r < m_nr)
      return m_data + r * m_nc;
    else {
      Exception e("Array: while indexing array data",
          "the given index exceeds the index range of array");
      throw(e);
    }
  }
  /**
   * Establece con el valor val a todos los elementos del arreglo.
   * @param val the value to be set.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  virtual void setVal(T val)
  {
    set_value(m_data, m_nr, m_nc, val);
  }

  /**
   * Asigna el valor de val a todos los elementos del arrglo.
   * 
   * @param val el valor a asignar.
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  void operator=(const T val)
  {
    setVal(val);
  }

  /**
   * Copia los valores de una matriz a esta.
   * 
   * @param mat el valor a asignar.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  void operator=(const Array<T, PRECISION>& cpy) throw (Exception)
  {
    rem_ref();
    m_free = cpy.m_free;
    m_nr = cpy.m_nr;
    m_nc = cpy.m_nc;
    m_data = cpy.m_data;
    add_ref(cpy);
  }

  /**
   * Devuelve la referencia a los datos de la matriz.
   * @return a refernece to the chunk of data.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  T* getData() const
  {
    return m_data;
  }

  /** 
   * @brief Returns the data reference as a general pointer. 
   *  
   * @return a general pointer to de data. 
   * @author Julio C. Estrada-Rico
   * @date 25/02/2010 
   */ 
  virtual void* data() const
  {
    return m_data;
  }

  /**
   * Devuelve el numero de columnas de la matriz.
   * @return the number of columns
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  virtual int cols() const
  {
    return m_nc;
  }
  /**
   * Devuelve el numero de renglones de la matriz.
   * @return the number of rows
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  virtual int rows() const
  {
    return m_nr;
  }

  /**
   * Redimenciona los datos de la matrix. 
   * 
   * Los valores que se tenian se pierden.
   * 
   * @param nr numero de renglones
   * @param nc numero de columnas
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  void resize(int nr, int nc) throw (Exception)
  {
    m_nr = nr;
    m_nc = nc;
    try {
      create_ref();
    } catch (std::exception e) {
      Exception te("Array: while redimentioning array data", e.what());
      throw(e);
    }
  }

  /**
   * Returns the type of this array.
   * Possible types are 'float32', 'float64', 'int8', and 'int32',
   * for data types of float single, float souble, character, and integer,
   * respectively.
   * 
   * @return the type string
   * @author Julio C. Estrada-Rico
   * @date 15/03/2010
   */
  virtual array_type type() const
  {
    return PRECISION;
  }

  /**
   * Destructor.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  virtual ~Array()
  {
    rem_ref();
  }

protected:
  /**
   * Referencia a los datos de la matriz.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  T* m_data;
  /**
   * Numero de renglones.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  int m_nr;
  /**
   * Numero de columnas.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  int m_nc;
  /**
   * Indica si los recursos que ocupan los datos pueden ser liberados.
   * 
   * Por ejemplo, que la memoria de los datos es administrada por la matriz.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  bool m_free;

  virtual void createData()
  {
    create();
  }

  virtual void deleteData()
  {
    destroy();
  }
private:
  /**
   * Reserva recursos para los datos de esta matriz.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  inline
  void create()
  {
    rem_ref();
    m_data = new T[m_nr * m_nc];
    m_free = true;
  }
  /**
   * Libera los recursos de los datos de esta matriz.
   *
   * @author Julio C Estrada-Rico
   * @date 25/02/210
   */
  inline
  void destroy()
  {
    if (m_free && release_mem())
      if (m_data != (T*) 0) {
        delete[] m_data;
        m_data = (T*) 0x0;
      }
  }

  /**
   * @brief Sets the given value to all elements of array.
   *
   * @param data is the data pointer
   * @param nr in the number of rows
   * @param nc is the number of columns
   * @param val is the value that will be assigned to the array data
   */
  inline
  void set_value(T RESTRICT_ptr data, const int nr, 
		 const int nc, const T val)
  {
    const int N = nr * nc;
    int i;
    for (i = 0; i < N; i++)
      data[i] = val;
  }

  /**
   * @brief copies data falues from one array to other.
   *
   * Here it is assumed that both data arrays has the same dimensions.
   *
   * @param data is the pointer array where data will be copied
   * @param cpydata is the pointer array that has the data to be copied
   * @param nr number of rows
   * @param nc number of columns
   */
  inline
  void copy_data(T RESTRICT_ptr data, const T RESTRICT_ptr cpydata, 
		 const int nr, const int nc)
  {
    int i;
    const int N = nr * nc;
    for (i = 0; i < N; i++)
      data[i] = cpydata[i];
  }

  /**
   * @brief Chechs if the given array has the same dimensions than this array
   *
   * @param arr is the array whose dimensions will be compared
   *
   * @throw Exception if the dimencions ara not the sames.
   */
  inline
  void checkDims(const Array<T, PRECISION>& arr) throw (Exception)
  {
    if (m_nr != arr.m_nr && m_nc != arr.m_nc) {
      Exception e("Array: while checking array dimensions",
          "mismatch array dimensions.");
      throw(e);
    }
  }
};

/** 
 * @brief an array of single presicion data. 
 */
typedef Array<float, SINGLE> sArray;
/** 
 * @brief an array of double presicion data. 
 */
typedef Array<double, DOUBLE> dArray;
/** 
 * @brief an array of 8-bit data. 
 */
typedef Array<char, CHARACTER> cArray;
/** 
 * @brief an array of integer data. 
 */
typedef Array<int, INTEGER> nArray;


// Definition of aritmetic operators

sArray operator+(const sArray op1, const sArray op2) throw(Exception);
dArray operator+(const sArray op1, const dArray op2) throw(Exception);
sArray operator+(const sArray op1, const nArray op2) throw(Exception);
sArray operator+(const sArray op1, const cArray op2) throw(Exception);
dArray operator+(const dArray op1, const sArray op2) throw(Exception);
dArray operator+(const dArray op1, const dArray op2) throw(Exception);
dArray operator+(const dArray op1, const nArray op2) throw(Exception);
dArray operator+(const dArray op1, const cArray op2) throw(Exception);
sArray operator+(const cArray op1, const sArray op2) throw(Exception);
dArray operator+(const cArray op1, const dArray op2) throw(Exception);
nArray operator+(const cArray op1, const nArray op2) throw(Exception);
cArray operator+(const cArray op1, const cArray op2) throw(Exception);
sArray operator+(const nArray op1, const sArray op2) throw(Exception);
dArray operator+(const nArray op1, const dArray op2) throw(Exception);
nArray operator+(const nArray op1, const nArray op2) throw(Exception);
nArray operator+(const nArray op1, const cArray op2) throw(Exception);

sArray operator-(const sArray op1, const sArray op2) throw(Exception);
dArray operator-(const sArray op1, const dArray op2) throw(Exception);
sArray operator-(const sArray op1, const nArray op2) throw(Exception);
sArray operator-(const sArray op1, const cArray op2) throw(Exception);
dArray operator-(const dArray op1, const sArray op2) throw(Exception);
dArray operator-(const dArray op1, const dArray op2) throw(Exception);
dArray operator-(const dArray op1, const nArray op2) throw(Exception);
dArray operator-(const dArray op1, const cArray op2) throw(Exception);
sArray operator-(const cArray op1, const sArray op2) throw(Exception);
dArray operator-(const cArray op1, const dArray op2) throw(Exception);
nArray operator-(const cArray op1, const nArray op2) throw(Exception);
cArray operator-(const cArray op1, const cArray op2) throw(Exception);
sArray operator-(const nArray op1, const sArray op2) throw(Exception);
dArray operator-(const nArray op1, const dArray op2) throw(Exception);
nArray operator-(const nArray op1, const nArray op2) throw(Exception);
nArray operator-(const nArray op1, const cArray op2) throw(Exception);

sArray operator*(const sArray op1, const sArray op2) throw(Exception);
dArray operator*(const sArray op1, const dArray op2) throw(Exception);
sArray operator*(const sArray op1, const nArray op2) throw(Exception);
sArray operator*(const sArray op1, const cArray op2) throw(Exception);
dArray operator*(const dArray op1, const sArray op2) throw(Exception);
dArray operator*(const dArray op1, const dArray op2) throw(Exception);
dArray operator*(const dArray op1, const nArray op2) throw(Exception);
dArray operator*(const dArray op1, const cArray op2) throw(Exception);
sArray operator*(const cArray op1, const sArray op2) throw(Exception);
dArray operator*(const cArray op1, const dArray op2) throw(Exception);
nArray operator*(const cArray op1, const nArray op2) throw(Exception);
cArray operator*(const cArray op1, const cArray op2) throw(Exception);
sArray operator*(const nArray op1, const sArray op2) throw(Exception);
dArray operator*(const nArray op1, const dArray op2) throw(Exception);
nArray operator*(const nArray op1, const nArray op2) throw(Exception);
nArray operator*(const nArray op1, const cArray op2) throw(Exception);

sArray operator/(const sArray op1, const sArray op2) throw(Exception);
dArray operator/(const sArray op1, const dArray op2) throw(Exception);
sArray operator/(const sArray op1, const nArray op2) throw(Exception);
sArray operator/(const sArray op1, const cArray op2) throw(Exception);
dArray operator/(const dArray op1, const sArray op2) throw(Exception);
dArray operator/(const dArray op1, const dArray op2) throw(Exception);
dArray operator/(const dArray op1, const nArray op2) throw(Exception);
dArray operator/(const dArray op1, const cArray op2) throw(Exception);
sArray operator/(const cArray op1, const sArray op2) throw(Exception);
dArray operator/(const cArray op1, const dArray op2) throw(Exception);
nArray operator/(const cArray op1, const nArray op2) throw(Exception);
cArray operator/(const cArray op1, const cArray op2) throw(Exception);
sArray operator/(const nArray op1, const sArray op2) throw(Exception);
dArray operator/(const nArray op1, const dArray op2) throw(Exception);
nArray operator/(const nArray op1, const nArray op2) throw(Exception);
nArray operator/(const nArray op1, const cArray op2) throw(Exception);

#endif // ARRAY_H
