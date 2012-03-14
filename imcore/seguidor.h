#ifndef SEGUIDOR_H_
#define SEGUIDOR_H_

#include "array.h"
#include <list>

class Punto{
  friend class Seguidor;
  private:
   Punto(){r=0; c=0;}
   Punto(int row, int col);
   int r;
   int c;
};

/**
 * Esta clase implementa los mecanismos para seguir las franjas de una im?gen
 * con patrones de franjas dado.
 * 
 * <P>
 * El algoritmo empleado aqui para seguir las franjas viene de la siguiente
 * referencia:
 *
 * @li Str?bel, "Processing of interferometric phase maps as complex-valued
 * phasor images", Appl. Opt. 35, 2192-2198 (1996)
 *
 * Para implementar el seguidor de franjas uno obtiene el gradiente de la 
 im?gen
 *@f[
  \nabla I = \left[\frac{\partial I}{\partial x}, \frac{\partial I}{\partial y} 
  \right]^T  
 *@f]
 * donde @f$ I @f$ es la im?gen con patrones de franjas. Dado el gradiente de
 * la im?gen, el siguiente paso es el c?lculo de la magnitud en cada punto:
 *@f[
  M(x,y) = \|\nabla I(x,y)\|
 *@f]
 * de donde se cuantiza la magnitud en @@f$ n @@f$ niveles de la siguiente manera:
 @f[
  M_n(x,y) = \left\{ \begin{array}{ccl}
  1 & \mbox{si} & M(x,y)\in [0,b/n] \\
  2 & \mbox{si} & M(x,y)\in (b/n,2b/n] \\
  3 & \mbox{si} & M(x,y)\in (2b/n,3b/n] \\
    & \vdots    & \\
  n & \mbox{si} & M(x,y)\in (\{n-1\}b/n,b]
  \end{array} \right.
 *@f]
 * as? pues generamos @f$ n @f$ registros para cada uno de los @f$ n @f$ niveles
 * del gradiente generado.
 *
 * El procedimiento para seguir las franjas dada la magnitud cuantizada en 
 * @f$ n @f$ niveles, los @f$ n @f$ registros y un punto incial @f$ (x,y) @f$
 * es:
 * @li 1 Colocamos los vecionos del punto @f$ (x,y) @f$ a los registros
 * correspondientes deacuerdo al nivel dado en la magnitud cauntizada en 
 * @f$ M_n(x,y) @f$.
 * @li 2 Sacamos del primer registro no vacio el primer punto introducido y lo
 * procesamos como en el paso 1.
 * @li 3 procesamos el paso 2 hasta que todos los registros queden 
 * completamente
 * vacios.
 *
 * en cada iteraci?n de este proceso cada punto que hemos sacado autom?tica
 * mentemarca el camino de seguir las franjas.
 * @section uso_sec Uso de la clase Seguidor
 * El siguiente c?digo muestra un caso t?pico del uso de la clase seguidor:
 * @code
      .
      .
      .
   //Creamos un seguidor con 4 niveles de cuantizaci?n.
   //I es tipo double[][] y contiene el patron de franjas
   Seguidor seguidor=new Seguidor(I,4);
   int i,j;
   while(!seguidor.siguiente()){
     //se obtiene el punto i,j siguiente a seguir.
     i=seguidor.get_i();
     j=seguidor.get_j();
     //se procesa el punto i,j
        .
    .
    .
   }
 * @endcode
 * 
 * Observaci?nes: El punto inicial del seguidor es tomado como el ?ltimo punto
 * encontrado cuyo nivel de cauntizaci?n es m?ximo. Es posible establecer un
 * punto de inicio diferente usando las funciones que para esto dispone la 
 * clase.
 * </P>
 * @author Julio Cesar Estrada Rico
 * @version 0.1.1
 */
class Seguidor{
 private:
    /**Es la imagen de entrada*/
    sArray _I;
    /**Es la m?scara de valores que son v?lidos en la imagen.

       @note Actualmente esta variable es puesta para uso en el futuro
       @todo Implementar la aplicaci?n de m?scaras sobre la im?gen
     */
    sArray _m;
    /**Contiene etiquetas de los puntos que ya han sido caminados.

       El objetivo es para identificar aquellos puntos que ya han sido
       recorridos por el seguidor. Si un punto tiene un valor de 1 es que ya ha
       sido recorrido, 0 es que no.
     */
    sArray _caminado;
    /**Aqui se guarda el mapa de calidad de la im?gen.

       El mapa de calidad se refiere al mapa cauntozado de la magnitud del
       gradiente como se explico anteriormente.
     */
    sArray _qmap;
    /**Es un arrglo de registros para cada nivel de cauntizaci?n.*/
    std::list<Punto*>* _colas;
    /**Aqui se guarda la secuencia de puntos para ser eliminados al destruirse
     * la clase.
     */
    std::list<Punto*> _puntos;
    /**Es el punto que actualmente es recorrido.*/
    Punto* _punto;
    /**Numero de niveles del mapa de calidad*/
    int _levels;

    /**
     *Calcula el mapa de calidad de la imagen de acuerdo al gradiente.
     *El mapa de calidad se refiere a la magnitud del gradiente. Las 
     *operaciones realizadas aqui son:
      @f[
      M(x,y) = \|\nabla I(x,y)\|
      @f]
      y
      @f[
      M_n(x,y) = \left\{ \begin{array}{ccl}
      1 & \mbox{si} & M(x,y)\in [0,b/n] \\
      2 & \mbox{si} & M(x,y)\in (b/n,2b/n] \\
      3 & \mbox{si} & M(x,y)\in (2b/n,3b/n] \\
        &  \vdots   & \\
      n & \mbox{si} & M(x,y)\in (\{n-1\}b/n,b]
      \end{array} \right.
      @f]
     *
     *@param levels es el n?mero de niveles en el cual ser? cuantizado 
     *el mapa de calidad
     *@todo Considerar el uso de m?scara sobre el dominio.
     */
     void calcQualityMap(int levels);
     
     /**
     *Establece autom?ticamente el punto incial.
     *La l?gica que se toma para establecer el punto inicial autom?ticamente
     *es tomar el ?ltimo punto encontrado cuyo nivel de cauntizaci?n es m?ximo.
     *@note Esta funci?n es usada internamente al construir el objeto de esta
     *clase.
     *@todo Considerar el uso de m?scara sobre el dominio.
     **/
     void set_inicio();
     /**
     *Carga los vecinos m?s pr?ximos del punto actual que se est? recorriendo.
     *@todo Considerar el uso de m?scara sobre el dominio.
     */
     void cargaVecinos();

     /**
      * Calcula el numero de vecinos que ya han sido visitados al rededor
      * del punto (i,j).
      *
      * A esto se le define como el peso del punto (i,j)
      */
     int pesoVecinos(const int i, const int j);

 public:
    /** Crea una nueva instancia de Seguidor.
     * Al crearce esta nueva instancia, se calcula autom?ticamente el mapa de
     * calidad cauntizado a @f$ n @f$ niveles (cuantizaci?n de la magnitud
     * del gradiente) y se establece el punto de incio.
     * @param I es la im?gen con franjas a seguir
     * @param levels es el n?mero de niveles a los que se discretiza el mapa
     *        de calidad de la im?gen
     * @todo Implementar un constructor que permita pasar como par?metro la
     * m?scara que ser? plicada al dominio del seguidor.
     */
    Seguidor(const sArray& I,int levels);
    /** Crea una nueva instancia de Seguidor.
     * Al crearce esta nueva instancia, se calcula autom?ticamente el mapa de
     * calidad cauntizado a @f$ n @f$ niveles (cuantizaci?n de la magnitud
     * del gradiente) y se toma el punto dado como punto de inicio.
     * @param I es la im?gen con franjas a seguir
     * @param levels es el n?mero de niveles a los que se discretiza el mapa
     *        de calidad de la im?gen
     * @param r renglon del punto
     * @param c columna del punto
     * @todo Implementar un constructor que permita pasar como par?metro la
     * m?scara que ser? plicada al dominio del seguidor.
     */
    Seguidor(const sArray& I,int r, int c, int levels);
    
    ~Seguidor();
    /**
     *Regresa el renglon del ultimo punto enlistado a seguir
     *
     *@return numero de renglon
     **/
    int get_r();
    /**
     *Regresa la columna del ultimo punto enlistado a seguir
     *
     *@return numero de columna
     **/
    int get_c();
    /**
     *Regresa el mapa de calidad que se esta utilizando.
     *@return double[][] el mapa de calidad
     **/
    sArray get_qmap();
    /**
     *Extrae el siguiente punto a seguir de acuerdo al mapa de calidad.
     *
     *@return boolean false si ya no hay puntos a seguir.
     **/
    bool siguiente();
    
    /**
      Establece un mapa de calidad dado de forma externa y lo cuantiza en los
      @f$ n @f$ niveles dados al contruir el objeto.
      Aunque internamente se genera  un mapa de calidad a partir del patr?n de
      franjas dado, este m?todo ofrece la posibilidad de usar otro mapa de 
      forma expl?cita. Este m?todo se debe llamar antes de seguir las franjas,
      en caso contrario el resultado es desconocido.
      @param qmap es el mapa de calidad a utilizar
     *@todo Considerar el uso de m?scara sobre el dominio.
     */
    void setQMap(sArray& qmap);
};


#endif /*SEGUIDOR_H_*/
