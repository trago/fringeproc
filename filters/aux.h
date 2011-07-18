/*
** aux.h
** 
** Made by Julio Cesar Estrada Rico
** Login   <julio@localhost>
** 
** Started on  Mon Aug 21 15:31:39 2006 Julio Cesar Estrada Rico
** Last update Sat Nov 18 00:38:56 2006 Julio Cesar Estrada Rico
*/

#ifndef   	AUX_H_
# define   	AUX_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 *@ingroup fringeproc
 *@def MIN(a,b)
 *Regresa el valor minimo entre a y b.
 */
#define MIN(a,b) (((a)<(b))? (a):(b))
/**
 *@ingroup fringeproc
 *@def MAX(a,b)
 *Regresa el valor maximo entre a y b.
 */
#define MAX(a,b) (((a)>(b))? (a):(b))
/**
 *@ingroup fringeproc
 *@def IDX(i,j)
 *Traduce el indice (i,j) al un indice en una dimencion. Esto es para
 *indexar una matriz que esta almecenada en un arreglo
 *unidimensional. nr es el numero de renglones en la matriz. i es la
 *columna, j es el renglon.
 */
#define IDX(i,j,nr) ((i)*(nr) + (j))


#ifdef __cplusplus
}
#endif

#endif 	    /* !AUX_H_ */
