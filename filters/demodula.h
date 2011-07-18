/*
** demodula.h
** 
** Made by Julio Cesar Estrada Rico
** Login   <julio@localhost>
** 
** Started on  Tue Aug 29 16:02:51 2006 Julio Cesar Estrada Rico
** Last update Thu Oct 19 10:33:56 2006 Julio Cesar Estrada Rico
*/
/**
 * @file demodula.h
 * Declaración de operaciones para la demodulación de interferogramas.
 *
 * @defgroup Demodula Operaciones y métodos para la demodulación.
 * @ingroup fringeproc
 * Aqui están reunidos métodos y operaciones para la demodulación de imágenes
 * con patrones de franjas. 
 */


#ifndef   	DEMODULA_H_
# define   	DEMODULA_H_

#ifdef __cplusplus
extern "C" {
#endif

//@{
/**
 * @ingroup Demodula
 * Calcula el potencial de segundo orden y regresa el valor (@c float).
 * Un potencial de segundo orden es definido de la siguiente manera (caso
 * discreto:
 * @f[
 * V[f(x)]=[f(x-1)-2f(x)+f(x+1)]^2.
 * @f]
 * Esta función realiza el cálculo de este potencial, resiviendo un vector con
 * los tres valores que se requieren para calcular este potencial. 
 * @param cliq aqui van los tres valores para calcular el potencial. Los
 * valores deben de ir en orden. Se asume que la dimensión de este arreglo es
 * de 3. 
 * @return el valor del potencial calculado.
 */
float demod_potential_s2ord(float *restrict cliq);

/**
 * Calcula el potencial de segundo orden y regresa el valor (@c double).
 * Un potencial de segundo orden es definido de la siguiente manera (caso
 * discreto:
 * @f[
 * V[f(x)]=[f(x-1)-2f(x)+f(x+1)]^2.
 * @f]
 * Esta función realiza el cálculo de este potencial, resiviendo un vector con
 * los tres valores que se requieren para calcular este potencial. 
 * @param cliq aqui van los tres valores para calcular el potencial. Los
 * valores deben de ir en orden. Se asume que la dimensión de este arreglo es
 * de 3. 
 * @return el valor del potencial calculado.
 */
double demod_potential_d2ord(double *restrict cliq);

/**
 * Estima la frecuencia dado un valor de fase y un valor cosenoidal (@c float).
 * Esta función es útil para estimar la frecuencia (derivada de la fase) cuando
 * conocemos solo un valor de fase en un punto y el valor cosenoidal en un
 * punto siguiente (o anterior). Aqui estamos suponiendo entoces que el valor
 * coseniodal viene dado de la siguiente manera:
 * @f[
 * g(x)=cos[\phi(x)],
 * @f]
 * donde @f$x@f$ es el punto y @f$\phi(x)@f$ es la fase. Entonces se 
 * estima la frecuencia dado los valores @f$ \phi(x) @f$ y @f$ g(x_+) @f$,
 * donde @f$ x_+ @f$ puede tomar los valores @f$ x+1 @f$ o @f$ x-1 @f$.
 * <p>
 * Esta función calcula dos valores de frecuencias. Ambas frecuencias son
 * válidas en la vecindad del punto @f$ x @f$ ya que la función coseniodal es
 * una función par.
 *
 * @param g es el valor cosenoidal.
 * @param p es el valor de fase.
 * @param freqs aquí se guardan los valores de frecuencia estimados. 
 * @param dir Indica si se estima la frecuencia hacia atrás (valor -1) o hacia 
 * adelante (valor 1). 
 */
void demod_calc_sfrequencies(const float g, const float p, 
			     float *restrict freqs, const int dir);

/**
 * Estima la frecuencia dado un valor de fase y un valor cosenoidal (@c double).
 * Esta función es útil para estimar la frecuencia (derivada de la fase) cuando
 * conocemos solo un valor de fase en un punto y el valor cosenoidal en un
 * punto siguiente (o anterior). Aqui estamos suponiendo entoces que el valor
 * coseniodal viene dado de la siguiente manera:
 * @f[
 * g(x)=cos[\phi(x)],
 * @f]
 * donde @f$ x @f$ es el punto y @f$ \phi(x) @f$ es la fase. Entonces se 
 * estima la frecuencia dado los valores @f$ \phi(x) @f$ y @f$ g(x_+) @f$,
 * donde @f$ x_+ @f$ puede tomar los valores @f$ x+1 @f$ o @f$ x-1 @f$.
 * <p>
 * Esta función calcula dos valores de frecuencias. Ambas frecuencias son
 * válidas en la vecindad del punto @f$ x @f$ ya que la función coseniodal es
 * una función par.
 *
 * @param g es el valor cosenoidal.
 * @param p es el valor de fase.
 * @param freqs aquí se guardan los valores de frecuencia estimados. 
 * @param dir Indica si se estima la frecuencia hacia atrás (valor -1) o hacia 
 * adelante (valor 1). 
 */
void demod_calc_dfrequencies(const double g, const double p, 
			     double *restrict freqs, const int dir);

/**
 * Calcula el arcocoseno validando su valor (@c float).
 * Esta función encapsula la función @c acos de la librería estandar de "C".
 * Antes de calcular el arcocoseno primero valida que el argumento se encuentre
 * en el intervalo @f$[-1,1]@f$. Si el argumento queda fuera del intervalor, se
 * toma el límite del intervalo mas sercano al valor del argumento.
 *
 * @param g es el valor del argumento del arcocoseno a calcular.
 * @return 0 si el argumento es mayor o igual que cero, @f$\pi@f$ si el 
 * argumento es menor o igual que -1. En otro caso se devuelve el valor del
 * arcocoseno del argumento.
 */
float demod_calc_sacos(float g);

/**
 * Calcula el arcocoseno validando su valor (@c double).
 * Esta función encapsula la función @c acos de la librería estandar de "C".
 * Antes de calcular el arcocoseno primero valida que el argumento se encuentre
 * en el intervalo @f$[-1,1]@f$. Si el argumento queda fuera del intervalor, se
 * toma el límite del intervalo mas sercano al valor del argumento.
 *
 * @param g es el valor del argumento del arcocoseno a calcular.
 * @return 0 si el argumento es mayor o igual que cero, @f$\pi@f$ si el 
 * argumento es menor o igual que -1. En otro caso se devuelve el valor del
 * arcocoseno del argumento.
 */
double demod_calc_dacos(double g);

/**
 * Estima la fase inicial en la vecindad de la semilla dada (@c float).
 * Esta es una función de utilería para ser usada en métodos de demodulación
 * como el Frequency Tracker (FT) que requieren una vecindad de 3 elementos (en
 * 1 dimención) inicializada. Entonces, dado el valor de fase de la semilla 
 * inicial, se estima la fase en los vecinos adyacentes. Para esto, se utiliza 
 * un vector de 3 elementos, donde el elemento central tiene el valor de de fase
 * de la semilla inicial. Otro vector de 3 elementos contienen los valores
 * cosenoidales en esa vecindad, o en otras palabra, la información de donde se
 * estimara la fase. Una vez ejecutado este proceso, el vector que guarda la
 * semilla inicial contendra los valores de fase estimados en las posiciones
 * adyacentes de la semilla inicial.
 *
 * @param g son los valores cosenoidales de la vecindad de la semilla inicial.
 * Vector de 3 elementos.
 * @param p Vector de 3 elementos, donde el elemento central contiene el valor
 * de fase de la semilla inicial. En los elementos adyacentes se guardara la
 * fase estimada al rededor de la semilla inicial.
 */
void demod_set_sseed(float *restrict g, float *restrict p);

/**
 * Estima la fase inicial en la vecindad de la semilla dada (@c double).
 * Esta es una función de utilería para ser usada en métodos de demodulación
 * como el Frequency Tracker (FT) que requieren una vecindad de 3 elementos (en
 * 1 dimención) inicializada. Entonces, dado el valor de fase de la semilla 
 * inicial, se estima la fase en los vecinos adyacentes. Para esto, se utiliza 
 * un vector de 3 elementos, donde el elemento central tiene el valor de de fase
 * de la semilla inicial. Otro vector de 3 elementos contienen los valores
 * cosenoidales en esa vecindad, o en otras palabra, la información de donde se
 * estimara la fase. Una vez ejecutado este proceso, el vector que guarda la
 * semilla inicial contendra los valores de fase estimados en las posiciones
 * adyacentes de la semilla inicial.
 *
 * @param g son los valores cosenoidales de la vecindad de la semilla inicial.
 * Vector de 3 elementos.
 * @param p Vector de 3 elementos, donde el elemento central contiene el valor
 * de fase de la semilla inicial. En los elementos adyacentes se guardara la
 * fase estimada al rededor de la semilla inicial.
 */
void demod_set_dseed(double *restrict g, double *restrict p);

/**
 * Demodula un renglón dada la matriz de datos de un patrón de franjas 
 * (@c float).
 * Demodula un renglón tomando un punto inicial dado. Esta demodulación del
 * renglón se hace de acuerdo al método Frequency Tracker. El punto inicial a
 * demodular es la semilla, la cual debe estar inicializada previamente (ver
 * demod_set_dseed(double* restrict,double* restrict)).
 *
 * @param p es la fase demodulada. Aqui se van guardando los valores
 * demodulados del renglón dado.
 * @param g es la matriz de datos del patrón de franjas o interferograma.
 * @param row es el número de reglón a demodular
 * @param seed es el número de elemento del renglón con el cual se inicial la
 * demodulación.
 * @param N Dimención del renglón.
 * @param nr número de renglones del patrón de franjas.
 */
void demod_demodula_srow(float* restrict p, float* restrict g, 
			 const int row, const int seed, const int N, 
			 const int nr);

/**
 * Demodula un renglón dada la matriz de datos de un patrón de franjas 
 * (@c float).
 * Demodula un renglón tomando un punto inicial dado. Esta demodulación del
 * renglón se hace de acuerdo al método Frequency Tracker. El punto inicial a
 * demodular es la semilla, la cual debe estar inicializada previamente (ver
 * demod_set_dseed(double* restrict,double* restrict)).
 *
 * @param p es la fase demodulada. Aqui se van guardando los valores
 * demodulados del renglón dado.
 * @param g es la matriz de datos del patrón de franjas o interferograma.
 * @param row es el número de reglón a demodular
 * @param seed es el número de elemento del renglón con el cual se inicial la
 * demodulación.
 * @param N Dimención del renglón.
 * @param nr número de renglones del patrón de franjas.
 */
void demod_demodula_drow(double* restrict p, double* restrict g, 
			 const int row, const int seed, const int N, 
			 const int nr);

/**
 *Demodula el sub-renglon especificado en @a row y limitado por @a li
 *y @a ls, iniciando en el elemento @a seed dado.
 *@param p aqui se coloca la demodulaciÃ³n. @a p representa el
 *la fase demodulada del interferograma.
 *@param g son los datos del interoframa.
 *@param row es el nÃºmero de renglÃ³n a demodular.
 *@param seed es el sitio inicial donde comienza la demodulaciÃ³n. @a
 *seed ya debe estar inicializada antes con
 *demod_det_deed().
 *@param li limite inferior del subrenglon.
 *@param ls limite superior del subrenglon.
 *@param nr es el numero de renglones que tiene el interferograma.
 */
void demod_demodula_dsubrow(double* restrict p, double* restrict g, 
			    double* restrict s,
			    const int row, const int seed,
			    const int li, const int ls, 
			    const int nr);
/**
 *Demodula la columna especificada en @a column, iniciando en el elemento
 *@a seed dado.
 *@param p aqui se coloca la demodulaciÃ³n. @a p representa el
 *la fase demodulada del interferograma.
 *@param g son los datos del interoframa.
 *@param column es el nÃºmero de columna a demodular.
 *@param seed es el sitio inicial donde comienza la demodulaciÃ³n. @a
 *seed ya debe estar inicializada antes con
 *demod_set_seed(float*,float*).
 *@param N es la dimenciÃ³n de la columna.
 */
void demod_demodula_scolumn(float* restrict p, float* restrict g, 
			    const int column, const int seed, const int N);

/**
 *Demodula la columna especificada en @a column, iniciando en el elemento
 *@a seed dado.
 *@param p aqui se coloca la demodulaciÃ³n. @a p representa el
 *la fase demodulada del interferograma.
 *@param g son los datos del interoframa.
 *@param column es el nÃºmero de columna a demodular.
 *@param seed es el sitio inicial donde comienza la demodulaciÃ³n. @a
 *seed ya debe estar inicializada antes con
 *demod_det_deed(double*,double*).
 *@param N es la dimenciÃ³n de la columna.
 */
void demod_demodula_dcolumn(double* restrict p, double* restrict g, 
			    const int column, const int seed, const int N);

/**
 *Demodula la sub-columna especificada en @a column y limitado por @a li
 *y @a ls, iniciando en el elemento @a seed dado.
 *@param p aqui se coloca la demodulaciÃ³n. @a p representa el
 *la fase demodulada del interferograma.
 *@param g son los datos del interoframa.
 *@param row es el nÃºmero de renglÃ³n a demodular.
 *@param seed es el sitio inicial donde comienza la demodulaciÃ³n. @a
 *seed ya debe estar inicializada antes con
 *demod_det_deed().
 *@param li limite inferior del subrenglon.
 *@param ls limite superior del subrenglon.
 *@param nr numero de renglones de @a g
 */
void demod_demodula_dsubcolumn(double* restrict p, double* restrict g, 
			       double* restrict s,
			       const int column, const int seed, const int li,
			       const int ls, const int nr);

void demod_local_calc_dfreqx(double* restrict p, double* restrict s,
                             double* restrict fi, double* restrict psi,
                             double* wx, double* wy,
                             const int ir, const int ic,
                             const int nr, const int nc, const int N);

/**
 *Demodula un interferograma usando el mÃ©todo RPT.
 *@param p es la fase demodulada.
 *@param g es el interferograma.
 *@param lambda es el parÃ¡metro de regularizaciÃ³n.
 *@param N es el tamaÃ±o de la vecindad que usa el RPT. la dimension es
 *N X N.
 *@param initc es la columna inicial
 *@param initr es el renglÃ³n inicial.
 *@note funcion no implementada
 */
void demod_drpt(double* restrict p, double* restrict g, const double lambda,
		const int N, const int initc, const int initr);
//@}
#ifdef __cplusplus
}
#endif

#endif 	    /* !DEMODULA_H_ */
