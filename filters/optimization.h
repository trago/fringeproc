/*
** optimization.h
** 
** Made by Julio Cesar Estrada Rico
** Login   <julio@localhost>
** 
** Started on  Sat Sep  2 13:05:33 2006 Julio Cesar Estrada Rico
** Last update Thu Sep 14 12:12:05 2006 Julio Cesar Estrada Rico
*/
/**
 *@file optimization.h
 *Declaración de funciones y operaciones para algoritmos de
 *optimización.
 */


#ifndef   	OPTIMIZATION_H_
# define   	OPTIMIZATION_H_

#ifdef __cplusplus
exter "C" {
#endif

/**
 *Realiza la miltiplicación de la matriz @f$ Ax=b @f$ para la
 *operación de gradiente conjugado del filtro de regularización de
 *primer orden.
 *@param x vector de entrada a multiplicar.
 *@param b vector de salida. Resultado de la multiplicación.
 *@param lambda constante de regularización.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg1_satimes(float* restrict b, float* restrict x,
			     const float lambda, const int nr, const int nc);

/**
 *Realiza la miltiplicación de la matriz @f$ Ax=b @f$ para la
 *operación de gradiente conjugado del filtro de regularización de
 *primer orden.
 *@param x vector de entrada a multiplicar.
 *@param b vector de salida. Resultado de la multiplicación.
 *@param lambda constante de regularización.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg1_datimes(double* restrict b, double* restrict x,
			     const double lambda, const int nr, const int nc);

/**
 *Realiza la miltiplicación de la matriz @f$ Ax=b @f$ para la
 *operación de gradiente conjugado del filtro de regularización de
 *segundo orden.
 *@param x vector de entrada a multiplicar.
 *@param b vector de salida. Resultado de la multiplicación.
 *@param lambda constante de regularización.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg2_satimes(float* restrict b, float* restrict x,
			     const float lambda, const int nr, const int nc);

/**
 *Realiza la miltiplicación de la matriz @f$ Ax=b @f$ para la
 *operación de gradiente conjugado del filtro de regularización de
 *segundo orden.
 *@param x vector de entrada a multiplicar.
 *@param b vector de salida. Resultado de la multiplicación.
 *@param lambda constante de regularización.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg2_datimes(double* restrict b, double* restrict x,
			     const double lambda, const int nr, const int nc);

/**
 *Resuelve sistema @f$ Ax=b @f$ para calcular vector de
 *precondicionamiento en operación de gradiente conjugado para filtro
 *de regularización de primer orden.
 *@param x vector de salida. Solución al sistema.
 *@param b vector esperado en la multiplicación.
 *@param l constante de regularización.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg1_sasolve(float* restrict x, float* restrict b,
			const float l, const int nr, const int nc);

/**
 *Resuelve sistema @f$ Ax=b @f$ para calcular vector de
 *precondicionamiento en operación de gradiente conjugado para filtro
 *de regularización de primer orden.
 *@param x vector de salida. Solución al sistema.
 *@param b vector esperado en la multiplicación.
 *@param l constante de regularización.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg1_dasolve(double* restrict x, double* restrict b,
			const double l, const int nr, const int nc);

/**
 *Resuelve sistema @f$ Ax=b @f$ para calcular vector de
 *precondicionamiento en operación de gradiente conjugado para filtro
 *de regularización de segundo orden.
 *@param x vector de salida. Solución al sistema.
 *@param b vector esperado en la multiplicación.
 *@param l constante de regularización.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg2_sasolve(float* restrict x, float* restrict b,
			const float l, const int nr, const int nc);

/**
 *Resuelve sistema @f$ Ax=b @f$ para calcular vector de
 *precondicionamiento en operación de gradiente conjugado para filtro
 *de regularización de segundo orden.
 *@param x vector de salida. Solución al sistema.
 *@param b vector esperado en la multiplicación.
 *@param l constante de regularización.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg2_dasolve(double* restrict x, double* restrict b,
			const double l, const int nr, const int nc);

/**
 *Minimiza función de costo para filtro de regularización de primer
 *orden.
 *@param x aqui se guarda la señal optimizada (filtrada).
 *@param b es la señal a filtrar
 *@param p variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param pp variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param r variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param rr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param z variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param zz variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param lambda parámetro de regularización. Es un valor que indica la
 *fuerza del filtrado.
 *@param tol es el error de toleracia. El error de convergencia máximo
 *en el proceso del gradiente conjugado.
 *@param itmax el numero de iteraciones máximas del gradiente
 *conjugado
 *@param iter variable de referencia que indica el numero de iteración
 *actual.
 *@param err variable de referencia que indica el error de convergencia
 *actual.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg1_slinbcg(float* restrict x, float* restrict b,
			     float* restrict p, float* restrict pp,
			     float* restrict r, float* restrict rr,
			     float* restrict z, float* restrict zz,
			     const float lambda, const float tol, 
			     const int itmax, float* restrict iter,
			     float* restrict err, 
			     const int nr, const int nc);

/**
 *Minimiza función de costo para filtro de regularización de primer
 *orden.
 *@param x aqui se guarda la señal optimizada (filtrada).
 *@param b es la señal a filtrar
 *@param p variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param pp variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param r variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param rr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param z variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param zz variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param lambda parámetro de regularización. Es un valor que indica la
 *fuerza del filtrado.
 *@param tol es el error de toleracia. El error de convergencia máximo
 *en el proceso del gradiente conjugado.
 *@param itmax el numero de iteraciones máximas del gradiente
 *conjugado
 *@param iter variable de referencia que indica el numero de iteración
 *actual.
 *@param err variable de referencia que indica el error de convergencia
 *actual.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg1_dlinbcg(double* restrict x, double* restrict b,
			     double* restrict p, double* restrict pp,
			     double* restrict r, double* restrict rr,
			     double* restrict z, double* restrict zz,
			     const double lambda, const double tol, 
			     const int itmax, double* restrict iter,
			     double* restrict err, 
			     const int nr, const int nc);

/**
 *Minimiza función de costo para filtro de regularización de segundo
 *orden.
 *@param x aqui se guarda la señal optimizada (filtrada).
 *@param b es la señal a filtrar
 *@param p variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param pp variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param r variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param rr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param z variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param zz variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param lambda parámetro de regularización. Es un valor que indica la
 *fuerza del filtrado.
 *@param tol es el error de toleracia. El error de convergencia máximo
 *en el proceso del gradiente conjugado.
 *@param itmax el numero de iteraciones máximas del gradiente
 *conjugado
 *@param iter variable de referencia que indica el numero de iteración
 *actual.
 *@param err variable de referencia que indica el error de convergencia
 *actual.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg2_slinbcg(float* restrict x, float* restrict b,
			     float* restrict p, float* restrict pp,
			     float* restrict r, float* restrict rr,
			     float* restrict z, float* restrict zz,
			     const float lambda, const float tol, 
			     const int itmax, float* restrict iter,
			     float* restrict err, 
			     const int nr, const int nc);

/**
 *Minimiza función de costo para filtro de regularización de segundo
 *orden.
 *@param x aqui se guarda la señal optimizada (filtrada).
 *@param b es la señal a filtrar
 *@param p variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param pp variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param r variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param rr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param z variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param zz variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado.
 *@param lambda parámetro de regularización. Es un valor que indica la
 *fuerza del filtrado.
 *@param tol es el error de toleracia. El error de convergencia máximo
 *en el proceso del gradiente conjugado.
 *@param itmax el numero de iteraciones máximas del gradiente
 *conjugado
 *@param iter variable de referencia que indica el numero de iteración
 *actual.
 *@param err variable de referencia que indica el error de convergencia
 *actual.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_reg2_dlinbcg(double* restrict x, double* restrict b,
			     double* restrict p, double* restrict pp,
			     double* restrict r, double* restrict rr,
			     double* restrict z, double* restrict zz,
			     const double lambda, const double tol, 
			     const int itmax, double* restrict iter,
			     double* restrict err, 
			     const int nr, const int nc);

/**
 *Realiza la miltiplicación de la matriz @f$ Ax=b @f$ para la
 *operación de gradiente conjugado del filtro de regularización de
 *cuadratura de primer orden.
 *@param xr vector de entrada a multiplicar. Parte real
 *@param xi vector de entrada a multiplicar. Parte imaginaria
 *@param br vector de salida. Resultado de la multiplicación. Parte real
 *@param bi vector de salida. Resultado de la multiplicación. Parte imaginaria
 *@param lambda constante de regularización.
 *@param cx es el coseno de la frecuencia de entonamiento Dirección x
 *@param cy es el coseno de la frecuencia de entonamiento.Dirección y
 *@param sx es el seno de la frecuencia de entonamiento Dirección x
 *@param sy es el seno de la frecuencia de entonamiento.Dirección y
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_qreg1_satimes(float* restrict br, float* restrict bi,
			      float* restrict xr, float* restrict xi,
			      const float lambda, 
			      const float cx, const float cy,
			      const float sx, const float sy,
			      const int nr, const int nc);

/**
 *Resuelve sistema @f$ Ax=b @f$ para calcular vector de
 *precondicionamiento en operación de gradiente conjugado para filtro
 *de regularización de cuadratura de primer orden.
 *@param xr vector de salida. Solución al sistema, parte real
 *@param xi vector de salida. Solución al sistema, parte imaginaria
 *@param br vector esperado en la multiplicación. Parte real
 *@param bi vector esperado en la multiplicación. Parte imaginaria
 *@param l constante de regularización.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_qreg1_sasolve(float* restrict xr, float* restrict xi,
			     float* restrict br, float* restrict bi,
			     const float l, const int nr, const int nc);

/**
 *Minimiza función de costo para filtro de regularización de
 *cuadratura de primer orden.
 *@param xr aqui se guarda la señal optimizada (filtrada). Parte real
 *@param xi aqui se guarda la señal optimizada (filtrada). Parte imaginaria
 *@param br es la señal a filtrar. Parter real
 *@param bi es la señal a filtrar. Parte imaginaria
 *@param pr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param pi variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param ppr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param ppi variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param rr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param ri variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param rrr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param rri variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param zr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param zi variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param zzr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param zzi variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param lambda parámetro de regularización. Es un valor que indica la
 *fuerza del filtrado.
 *@param wx es la frecuencia de entonamiento en dirección @a x.
 *@param wy es la frecuencia de entonamiento en diracción @a y.
 *@param tol es el error de toleracia. El error de convergencia máximo
 *en el proceso del gradiente conjugado.
 *@param itmax el numero de iteraciones máximas del gradiente
 *conjugado
 *@param iter variable de referencia que indica el numero de iteración
 *actual.
 *@param err variable de referencia que indica el error de convergencia
 *actual.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_qreg1_slinbcg(float* restrict xr, float* restrict xi,
			      float* restrict br, float* restrict bi,
			      float* restrict pr, float* restrict pi,
			      float* restrict ppr, float* restrict ppi,
			      float* restrict rr, float* restrict ri,
			      float* restrict rrr, float* restrict rri,
			      float* restrict zr, float* restrict zi,
			      float* restrict zzr, float* restrict zzi,
			      const float lambda, 
			      const float wx, const float wy,
			      const float tol,
			      const int itmax, int* restrict iter,
			      float* restrict err, 
			      const int nr, const int nc);

/**
 *Realiza la miltiplicación de la matriz @f$ Ax=b @f$ para la
 *operación de gradiente conjugado del filtro de regularización de
 *cuadratura de primer orden.
 *@param xr vector de entrada a multiplicar. Parte real
 *@param xi vector de entrada a multiplicar. Parte imaginaria
 *@param br vector de salida. Resultado de la multiplicación. Parte real
 *@param bi vector de salida. Resultado de la multiplicación. Parte imaginaria
 *@param lambda constante de regularización.
 *@param cx es el coseno de la frecuencia de entonamiento Dirección x
 *@param cy es el coseno de la frecuencia de entonamiento.Dirección y
 *@param sx es el seno de la frecuencia de entonamiento Dirección x
 *@param sy es el seno de la frecuencia de entonamiento.Dirección y
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_qreg1_datimes(double* restrict br, double* restrict bi,
			      double* restrict xr, double* restrict xi,
			      const double lambda, 
			      const double cx, const double cy,
			      const double sx, const double sy,
			      const int nr, const int nc);

/**
 *Resuelve sistema @f$ Ax=b @f$ para calcular vector de
 *precondicionamiento en operación de gradiente conjugado para filtro
 *de regularización de cuadratura de primer orden.
 *@param xr vector de salida. Solución al sistema, parte real
 *@param xi vector de salida. Solución al sistema, parte imaginaria
 *@param br vector esperado en la multiplicación. Parte real
 *@param bi vector esperado en la multiplicación. Parte imaginaria
 *@param l constante de regularización.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_qreg1_dasolve(double* restrict xr, double* restrict xi,
			     double* restrict br, double* restrict bi,
			     const double l, const int nr, const int nc);

/**
 *Minimiza función de costo para filtro de regularización de
 *cuadratura de primer orden.
 *@param xr aqui se guarda la señal optimizada (filtrada). Parte real
 *@param xi aqui se guarda la señal optimizada (filtrada). Parte imaginaria
 *@param br es la señal a filtrar. Parter real
 *@param bi es la señal a filtrar. Parte imaginaria
 *@param pr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param pi variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param ppr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param ppi variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param rr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param ri variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param rrr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param rri variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param zr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param zi variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param zzr variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte real
 *@param zzi variable auziliar de dimensiones igual a @a g para las
 *iteraciones del gradiente conjugado. Parte imaginaria
 *@param lambda parámetro de regularización. Es un valor que indica la
 *fuerza del filtrado.
 *@param wx es la frecuencia de entonamiento en dirección @a x.
 *@param wy es la frecuencia de entonamiento en diracción @a y.
 *@param tol es el error de toleracia. El error de convergencia máximo
 *en el proceso del gradiente conjugado.
 *@param itmax el numero de iteraciones máximas del gradiente
 *conjugado
 *@param iter variable de referencia que indica el numero de iteración
 *actual.
 *@param err variable de referencia que indica el error de convergencia
 *actual.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_qreg1_dlinbcg(double* restrict xr, double* restrict xi,
			      double* restrict br, double* restrict bi,
			      double* restrict pr, double* restrict pi,
			      double* restrict ppr, double* restrict ppi,
			      double* restrict rr, double* restrict ri,
			      double* restrict rrr, double* restrict rri,
			      double* restrict zr, double* restrict zi,
			      double* restrict zzr, double* restrict zzi,
			      const double lambda, 
			      const double wx, const double wy,
			      const double tol,
			      const int itmax, int* restrict iter,
			      double* restrict err, 
			      const int nr, const int nc);

/**
 *Minimiza función de costo para filtro de cuadratura robusto usando
 *Gauss-Seidel.
 *@param fi parte rel del resultado
 *@param psi parte imaginaria del resultado
 *@param g datos observados
 *@param lambda parametro de regularización
 *@param wx frecuencia de entonamiento en x
 *@param wy frecuencia de entonamiento en y
 *@param err es el error de convergence. Esta referencia al final del
 *proceso conserva el error de convergencia que finalmente se obtuvo.
 *@param iters referencia que indica el numero de iteraciones maximas
 *a realizar por el proces iterativo. Al final de del proceso, se
 *guarda el numero de iteraciones que se dieron.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_qreg1_sgs(float* restrict fi, float* restrict psi,
			  float* restrict g, const float lambda,
			  const float wx, const float wy,
			  float* restrict err, int* restrict iters,
			  const int nr, const int nc);

/**
 *Minimiza función de costo para filtro de cuadratura robusto usando
 *Gauss-Seidel.
 *@param fi parte rel del resultado
 *@param psi parte imaginaria del resultado
 *@param g datos observados
 *@param lambda parametro de regularización
 *@param wx frecuencia de entonamiento en x
 *@param wy frecuencia de entonamiento en y
 *@param err es el error de convergence. Esta referencia al final del
 *proceso conserva el error de convergencia que finalmente se obtuvo.
 *@param iters referencia que indica el numero de iteraciones maximas
 *a realizar por el proces iterativo. Al final de del proceso, se
 *guarda el numero de iteraciones que se dieron.
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void optim_filt_qreg1_dgs(double* restrict fi, double* restrict psi,
			  double* restrict g, const double lambda,
			  const double wx, const double wy,
			  double* restrict err, int* restrict iters,
			  const int nr, const int nc);


/**
 *Minimiza función de costo para filtro local de cuadratura robusto usando
 *Gauss-Seidel.
 *@param fi parte rel del resultado
 *@param psi parte imaginaria del resultado
 *@param g datos observados
 *@param s mascara que indica que sitios ya han sido procesados.
 *@param lambda parametro de regularización
 *@param wx frecuencia de entonamiento en x
 *@param wy frecuencia de entonamiento en y
 *@param err es el error de convergence. Esta referencia al final del
 *proceso conserva el error de convergencia que finalmente se obtuvo.
 *@param iters referencia que indica el numero de iteraciones maximas
 *a realizar por el proces iterativo. Al final de del proceso, se
 *guarda el numero de iteraciones que se dieron.
 *@param ir numero de renglon donde se hace el filtrado local
 *@param ic numero de coliumna donde se hace el filtrado local
 *@param nr numero de renglones
 *@param nc numero de columnas
 *@param N dimension de la vecindad local
 */
void optim_local_filt_qreg1_dgs(double* restrict fi, double* restrict psi,
				 double* restrict fia, double* restrict psia,
				 double* restrict g, 
				 double* restrict s, double* restrict sa,
				 const double lambda, const double mu, 
				 const double wx, const double wy,
				 double* restrict err, int* restrict iters,
				 const int ir, const int ic,
				 const int nr, const int nc, const int N);

void optim_local_filt_qreg1_gradient(
    double* restrict fi, double* restrict psi,
         double* restrict fia, double* restrict psia,
         double* restrict g, 
         double* restrict s, double* restrict sa,
         const double lambda, const double mu, const double alpha, 
         const double wx, const double wy,
         double* restrict err, int* restrict iters,
         const int ir, const int ic,
         const int nr, const int nc, const int N);

#ifdef __cplusplus
}
#endif

#endif 	    /* !OPTIMIZATION_H_ */
