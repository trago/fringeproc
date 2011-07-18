/*
** fringeproc.h
** 
** Made by Julio Cesar Estrada Rico
** Login   <julio@localhost>
** 
** Started on  Mon Aug 21 14:20:57 2006 Julio Cesar Estrada Rico
** Last update Thu Oct 19 10:53:05 2006 Julio Cesar Estrada Rico
*/

/**
 *@file fringeproc.h
 *Aqui se definen los metodos y operaciones para el procesamiento de franjas.
 */

#ifndef   	FRINGEPROC_H_
# define   	FRINGEPROC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 *Realiza un filtrado pasabajas usando un kernel gaussiano.
 *@param f variable de salida. Aqui se pone el filtrado.
 *@param g es la señal a filtrar.
 *@param sigma es el ancho del filtro gaussiano
 *@param nr numero de renglones de la señal a filtrar
 *@param nc numero de columnas de la señal a filtrar
 *@return 0 si se pudo procesar, 1 en otro caso. Cuando no se puede
 *procesar se debe a falta de memoria.
 */
int filter_sgaussian(float* f, float *g, 
		     const float sigma, const int nr, const int nc);
/**
 *Realiza un filtrado pasabajas usando un kernel gaussiano.
 *@param f variable de salida. Aqui se pone el filtrado.
 *@param g es la señal a filtrar.
 *@param sigma es el ancho del filtro gaussiano
 *@param nr numero de renglones de la señal a filtrar
 *@param nc numero de columnas de la señal a filtrar
 *@return 0 si se pudo procesar, 1 en otro caso. Cuando no se puede
 *procesar se debe a falta de memoria.
 */
int filter_dgaussian(double *f, double *g, 
		      const double sigma, const int nr, const int nc);

/**
 *Realiza un filtro pasabajas robusto usando potencial de
 *regularización de primer orden.
 *@param f aqui se guarda la señal filtrada
 *@param g es la señal a filtrar
 *@param lambda es el parámetro de regularización. Indica la fuerza
 *del filtrado pasabaja.
 *@param max_error Variable de referencia. Es el error máximo de
 *convergencia. Esto ya que se utiliza un algoritmo de optimización
 *(gradiente conjugado). Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param max_iters Variable de referencia. Número máximo de
 *interaciones a realizar. Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param nr es el número de renglones del patrón de franjas de
 *entrada.
 *@param nc es el número de columnas del patrón de franjas de entrada.
 *@return 0 si se concluyo el proceso exitosamente, 1 en caso
 *contrario. Los errores pueden deberse a falta de memoria en el sistema.
 */
int filter_sreg1(float *f, float *g,
		 const float lambda, float* max_error,
		 float* max_iters, const int nr, const int nc);

/**
 *Realiza un filtro pasabajas robusto usando potencial de
 *regularización de primer orden.
 *@param f aqui se guarda la señal filtrada
 *@param g es la señal a filtrar
 *@param lambda es el parámetro de regularización. Indica la fuerza
 *del filtrado pasabaja.
 *@param max_error Variable de referencia. Es el error máximo de
 *convergencia. Esto ya que se utiliza un algoritmo de optimización
 *(gradiente conjugado). Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param max_iters Variable de referencia. Número máximo de
 *interaciones a realizar. Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param nr es el número de renglones del patrón de franjas de
 *entrada.
 *@param nc es el número de columnas del patrón de franjas de entrada.
 *@return 0 si se concluyo el proceso exitosamente, 1 en caso
 *contrario. Los errores pueden deberse a falta de memoria en el sistema.
 */
int filter_dreg1(double *f, double *g,
		 const double lambda, double* max_error,
		 double* max_iters, const int nr, const int nc);

/**
 *Realiza un filtro pasabajas robusto usando potencial de
 *regularización de segundo orden.
 *@param f aqui se guarda la señal filtrada
 *@param g es la señal a filtrar
 *@param lambda es el parámetro de regularización. Indica la fuerza
 *del filtrado pasabaja.
 *@param max_error Variable de referencia. Es el error máximo de
 *convergencia. Esto ya que se utiliza un algoritmo de optimización
 *(gradiente conjugado). Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param max_iters Variable de referencia. Número máximo de
 *interaciones a realizar. Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param nr es el número de renglones del patrón de franjas de
 *entrada.
 *@param nc es el número de columnas del patrón de franjas de entrada.
 *@return 0 si se concluyo el proceso exitosamente, 1 en caso
 *contrario. Los errores pueden deberse a falta de memoria en el
 *sistema.
 *@note El filtrado de segundo orden no funciona correctamente. Hay
 *que rebisar su implementación.
 */
int filter_sreg2(float *f, float *g,
		 const float lambda, float* max_error,
		 float* max_iters, const int nr, const int nc);

/**
 *Realiza un filtro pasabajas robusto usando potencial de
 *regularización de segundo orden.
 *@param f aqui se guarda la señal filtrada
 *@param g es la señal a filtrar
 *@param lambda es el parámetro de regularización. Indica la fuerza
 *del filtrado pasabaja.
 *@param max_error Variable de referencia. Es el error máximo de
 *convergencia. Esto ya que se utiliza un algoritmo de optimización
 *(gradiente conjugado). Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param max_iters Variable de referencia. Número máximo de
 *interaciones a realizar. Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param nr es el número de renglones del patrón de franjas de
 *entrada.
 *@param nc es el número de columnas del patrón de franjas de entrada.
 *@return 0 si se concluyo el proceso exitosamente, 1 en caso
 *contrario. Los errores pueden deberse a falta de memoria en el sistema.
 *@note El filtrado de segundo orden no funciona correctamente. Hay
 *que rebisar su implementación.
 */
int filter_dreg2(double *f, double *g,
		 const double lambda, double* max_error,
		 double* max_iters, const int nr, const int nc);

/**
 *Demodula un patrón de franjas usando el método Frequency Tracker
 *(FT).
 *@param f variable de salida. Aqui se regresa la fase demodulada.
 *@param g es el patrón de franjas a demodular.
 *@param ir es el numero de renglon donde se inicia la demodulación.
 *@param ic es el número de columna donde se inicia la demodulación.
 *@param nr es el número de renglones del patrón de franjas de
 *entrada.
 *@param nc es el número de columnas del patrón de franjas de entrada.
 *@return 0 si se termino el procesado correctamente. 1 en caso contrario.
 */
int demod_sft(float *f, float *g, int ir, int ic,
	      int nr, int nc);

/**
 *Demodula un patrón de franjas usando el método Frequency Tracker
 *(FT).
 *@param f variable de salida. Aqui se regresa la fase demodulada.
 *@param g es el patrón de franjas a demodular.
 *@param ir es el numero de renglon donde se inicia la demodulación.
 *@param ic es el número de columna donde se inicia la demodulación.
 *@param nr es el número de renglones del patrón de franjas de
 *entrada.
 *@param nc es el número de columnas del patrón de franjas de entrada.
 *@return 0 si se termino el procesado correctamente. 1 en caso contrario.
 */
int demod_dft(double *f, double *g, int ir, int ic,
	      int nr, int nc);

/**
 *Realiza un filtro de cuadratura robusto usando potencial de
 *regularización de primer orden.
 *@param fi aqui se guarda la señal filtrada. Parte real
 *@param psi aqui se guarda la señal filtrada. Parte imaginaria
 *@param g es la señal a filtrar
 *@param lambda es el parámetro de regularización. Indica la fuerza
 *del filtrado pasabaja.
 *@param wx frecuencia de entonamiento en dirección @a x
 *@param wy frecuencia de entonamiento en dirección @a y
 *@param max_error Variable de referencia. Es el error máximo de
 *convergencia. Esto ya que se utiliza un algoritmo de optimización
 *(gradiente conjugado). Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param max_iters Variable de referencia. Número máximo de
 *interaciones a realizar. Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param nr es el número de renglones del patrón de franjas de
 *entrada.
 *@param nc es el número de columnas del patrón de franjas de entrada.
 *@return 0 si se concluyo el proceso exitosamente, 1 en caso
 *contrario. Los errores pueden deberse a falta de memoria en el sistema.
 */
int filter_sqreg1(float *fi, float* psi, float *g, const float lambda, 
		  const float wx, const float wy,
		  float* max_error, int* max_iters, 
		  const int nr, const int nc);

/**
 *Realiza un filtro de cuadratura robusto usando potencial de
 *regularización de primer orden.
 *@param fi aqui se guarda la señal filtrada. Parte real
 *@param psi aqui se guarda la señal filtrada. Parte imaginaria
 *@param g es la señal a filtrar
 *@param lambda es el parámetro de regularización. Indica la fuerza
 *del filtrado pasabaja.
 *@param wx frecuencia de entonamiento en dirección @a x
 *@param wy frecuencia de entonamiento en dirección @a y
 *@param max_error Variable de referencia. Es el error máximo de
 *convergencia. Esto ya que se utiliza un algoritmo de optimización
 *(gradiente conjugado). Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param max_iters Variable de referencia. Número máximo de
 *interaciones a realizar. Cuando
 *termina el proceso, aqui se guarda el error de convergencia
 *generado.
 *@param method vale 1 si se utiliza el metodo gauss-seidel, 0 si se
 *utiliza método de gradiente conjugado.
 *@param nr es el número de renglones del patrón de franjas de
 *entrada.
 *@param nc es el número de columnas del patrón de franjas de entrada.
 *@return 0 si se concluyo el proceso exitosamente, 1 en caso
 *contrario. Los errores pueden deberse a falta de memoria en el
 *sistema.
 *@param method indica que metodo usar para la operacion de
 *filtrado. 0 es gradiente conjugado, 1 es gauss-seidel.
 */
int filter_dqreg1(double *fi, double* psi, double *g, const double lambda, 
		  const double wx, const double wy,
		  double* max_error, int* max_iters, const int method,
		  const int nr, const int nc);

/**
 *Realiza un filtro local de cuadratura robusto usando potencial de
 *regularización de primer orden.
 *@param fi aqui se guarda la señal filtrada. Parte real
 *@param psi aqui se guarda la señal filtrada. Parte imaginaria
 *@param g es la señal a filtrar
 *@param lambda es el parámetro de regularización. Indica la fuerza
 *del filtrado pasabaja.
 *@param wx frecuencia de entonamiento en dirección @a x
 *@param wy frecuencia de entonamiento en dirección @a y
 *@param max_error Variable de referencia. Es el error máximo de
 *convergencia. Esto ya que se utiliza un algoritmo de optimización
 *(gradiente conjugado). Cuando
 *termina el proceso, aqui se guarda el error de convergencia generado.
 *@param max_iters Variable de referencia. Número máximo de
 *interaciones a realizar. Cuando
 *termina el proceso, aqui se guarda el error de convergencia
 *generado.
 *@param N es la dimensión de la vecindad local donde se aplicara el
 *filtro.
 *@param ir numero de renglon donde se filtrara localmente
 *@param ic numero de columna donde se filtrara localmente
 *@param nr es el número de renglones del patrón de franjas de
 *entrada.
 *@param nc es el número de columnas del patrón de franjas de entrada.
 *@return 0 si se concluyo el proceso exitosamente, 1 en caso
 *contrario. Los errores pueden deberse a falta de memoria en el
 *sistema.
 */
int filter_local_dqreg1(double *fi, double* psi, double *g,
			const double lambda, 
			const double wx, const double wy, 
			double* max_error, int* max_iters,
			const int N, const int ir, const int ic,
			const int nr, const int nc);
/**
 *Demodula un patrón de franjas usando el método Frequency Tracker
 *(FT) en la vecindad de dimension @a N al rededor del punto dado por
 *@a ir y @a ic.
 *@param f variable de salida. Aqui se regresa la fase demodulada.
 *@param g es el patrón de franjas a demodular.
 *@param ir es el numero de renglon donde se inicia la demodulación.
 *@param ic es el número de columna donde se inicia la demodulación.
 *@param nr es el número de renglones del patrón de franjas de
 *entrada.
 *@param nc es el número de columnas del patrón de franjas de entrada.
 *@param N es la dimension de la vensidad a demodular
 *@return 0 si se termino el procesado correctamente. 1 en caso contrario.
 */
int demod_local_dft(double *f, double *g, double* sp, 
		    const int ir, const int ic,
		    const int nr, const int nc, const int N);

/**
 *Demodula un interferograma usando el metodo frequency tracker con
 *prefiltrado de cuadratura robusto.
 *@param p aqui se guarda la fase demodulada
 *@param g es el interferograma
 *@param lambda es el parametro de regularización para el filtro de
 *cuadratura robusto.
 *@param error es el error de convergencia en el proceso de filtrado
 *@param ir numero de renglon inicial
 *@param ic numero de columna inicial
 *@param N dimension de la vesidad local donde se hace el prefiltrado
 *de cuadratura
 *@param nr numero de renglones
 *@param nc numero de columnas
 */
int demod_robust_dft(const double *p, const double *g, double wx, double wy,
		     const double lambda, const double mu, const double error,
		     const int ir, const int ic, 
		     const int N, const int nr, const int nc);

/**
 * Tima el indice del menor elemento.
 * @param g es la matriz de valores
 * @param ir es el indice del renglon donde esta el elemento menor
 * @param ic es el indice de la columna donde esta el elemento menor
 */
void fringeproc_get_dmin(double* g, int* ir, int* ic, int nr, int nc);

/**
 * Toma el indice del mayor elemento.
 * @param g es la matriz de valores
 * @param ir es el indice del renglon donde esta el elemento mayor
 * @param ic es el indice de la columna donde este el elemento mayor
 */
void fringeproc_get_dmax(double* g, int* ir, int* ic, int nr, int nc);




#ifdef __cplusplus
}
#endif

#endif 	    /* !FRINGEPROC_H_ */
