#ifndef FRINGEMATRIX_H
#define FRINGEMATRIX_H


namespace fringe {

/**
Es un arreglo cuadrado de datos.

Una Matriz se define por sis dimenciones dadas por numero de renglones
y numero de columnas. Los datos de la Matriz son almacenados en un
vector de memoria.

@author Julio Cesar Estrada Rico
*/
class Matrix{
public:
  /**
     Genera una matriz de datos nula.
  */
  Matrix();

  /**
     Genera una matriz a partir de un bloque de datos.
     @param dat es el bloque de datos
     @param nr es el número de renglones
     @param nc es el número de columnas
  */
  Matrix(double* dat, int nr, int nc);

  Matrix(const Matrix& mcp);

  /**
  Genera una matriz de datos de dimensión rowsXcols
  */
  Matrix(int rows, int cols);
  ~Matrix();

  /**
  Redimensiona la matriz*/
  void resize(int rows, int cols);
  /**
  Devuelve el número de columnas
  */
  int cols() const;
  /**
  Devuelve el número de renglones
  */
  int rows() const;
  /**
  Operador para indexar a los elementos de la matriz
  */
  double* operator[](int index);

  /**Operador para asignar matrices por valor.*/
  Matrix& operator=(const Matrix& mat);

  /**Asigna un escalar a cada elemento de la matriz.*/
  Matrix& operator=(const double val);
  /**
  Indica si esta matriz es una matriz nula.
  Esto es cuando las dimenciones de la matriz son cero.
  */
  bool isNull() const;
  /**
  Regresa el apuntador al bloque de datos
  */
  double* getData() const;
  /**
  Regresa el apuntador al bloque de datos en forma de matriz.
  La memoria es continua.
  */
  double** getMat() const;

  /**Calcula la norma euclidiana al cuadrado*/
  double norm2();
  /**
    Calcula el producto punto entre esta matriz y la dada por parámetro.
    */
  double dot(Matrix& r);

  Matrix operator-(Matrix& r);
  Matrix& operator*=(double k);
  Matrix& operator+=(Matrix& r);
  Matrix& operator-=(Matrix& r);
  Matrix operator*(double k);
protected:
  /**
  Mantiene el numero de columnas
  */
  int _cols;
  /**
  Mantiene el número de renglones
  */
  int _rows;
  /**
  Apuntal al bloque de datos de la matriz.
  La memoria es continua
   */
  double* data;
  /**
  Conveniente para manejar el bloque de datos como un vector de vectores
   */
  double** mat;
  /**
    Indica si la memoria ocupada por el bloque de datos es manejada por
    la matriz.
    */
  bool _ismanaged;
};

}

#endif
