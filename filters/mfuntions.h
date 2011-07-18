/*
** mfuntions.h
** 
** Made by Julio Cesar Estrada Rico
** Login   <julio@localhost>
** 
** Started on  Tue Aug 22 21:22:50 2006 Julio Cesar Estrada Rico
** Last update Tue Sep 12 17:54:08 2006 Julio Cesar Estrada Rico
*/

/**
 *@file mfuntions.h
 *Definicion de operadores para generar funciones matematicas.
 */

#ifndef   	MFUNTIONS_H_
# define   	MFUNTIONS_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 *Evalua la funcion exponencial.
 *La señal de salida corresponde a los valores de la función
 *esponencial.
 *@param f Variable de salida. Aqui se guardan los valores obtenidos
 *al aplicar la función exponencial.
 *@param g Son los valores a evaluar con la funcion esponencial.
 *@param N es la dimención de la señal.
 */
void math_sexp(float *__restrict__ f, float *__restrict__ g, const int N);

/**
 *Evalua la funcion exponencial.
 *La señal de salida corresponde a los valores de la función
 *esponencial.
 *@param f Variable de salida. Aqui se guardan los valores obtenidos
 *al aplicar la función exponencial.
 *@param g Son los valores a evaluar con la funcion esponencial.
 *@param N es la dimención de la señal.
 */
void math_dexp(double *__restrict__ f, double *__restrict__ g, const int N);


/**
 *Igual que @a atan2 de la libreria estandar per actua sobre una
 *matriz en una vecindad local. El argumento del atangente es 
 *@f$ \frac{y}{x} @f$.
 *@param p es el atangente calculado
 *@param y numerador
 *@param x denominador
 *@param ir numero de renglon donde se situa la vecindad a calcular
 *@param ic numero de columna donde se situa la vecindad a calcular
 *@param N dimension de la vecindad
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void math_local_datan2(double* restrict p, double* restrict s,
		       double* restrict y, double* restrict x, 
		       const int ir, const int ic,
		       const int N, const int nr, const int nc);

/**
 *Igual que @a cos de la libreria estandar per actua sobre una
 *matriz en una vecindad local.
 *@param I es el coseno calculado
 *@param p es la fase
 *@param ir numero de renglon donde se situa la vecindad a calcular
 *@param ic numero de columna donde se situa la vecindad a calcular
 *@param N dimension de la vecindad
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
void math_local_dcos(double* restrict I, double *restrict p, const int ir,
		    const int ic, const int N, const int nr, const int nc);


void math_sub_dcopy(double* restrict s, double* restrict sc, 
		    double* restrict mask, const int ir,
		    const int ic, const int nr, const int nc, const int N);

double math_sub_ddot(double* restrict v1, double *restrict v2,
         double* restrict s,
		     const int ir, const int ic, const int nr, const int nc,
		     const int N);

#ifdef __cplusplus
}
#endif

#endif 	    /* !MFUNTIONS_H_ */
