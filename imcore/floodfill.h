/*
 * floodfill.h
 *
 *  Created on: Aug 19, 2010
 *      Author: julio
 */

#include <list>
#include "array.h"

#ifndef FLOODFILL_H_
#define FLOODFILL_H_

/** 
 * @brief Flood-fill image scanner. 
 *  
 * @details 
 * It scans an array using a flood fill algorithm. Used to visit 
 * sequentially all elements of an array. 
 *  
 * @author Julio C. Estrada-Rico 
 * @date 08/19/2010 
 */ 
class FloodFill {

public:
  /** 
   * @brief FloodFill Constructor. 
   *  
   * @param start_x sets the starting point in x direction. 
   * @param start_y sets the starting point in y direction. 
   *  
   * @author Julio C. Estrada-Rico 
   * @date 08/19/2010 
   */ 
  FloodFill(int start_x, int start_y, int M, int N);

  /** 
   * @brief Gives the next site to visit. 
   * @details 
   * It gives a valid position if there are sites not visited. 
   * When it gives a unvalid position it means that all sites has 
   * been visited, or that there are not sites to visit. 
   *  
   * @param out_x the position in x direction. 
   * @param out_y the position in y direction. 
   * @return true if it was a valid position, otherwise, false. 
   *  
   * @author Julio C. Estrada-Rico 
   * @date 08/19/2010 
   */ 
  bool next(int& out_x, int& out_y);

  /** 
   * FloodFill destructor. 
   *  
   * @author Julio C. Estrada-Rico 
   * @date 08/19/2010 
   */ 
  virtual ~FloodFill();

private:
  struct node;
  /** 
   * @brief The queue of sites to visit. 
   */ 
  std::list<node> m_queue;
  /** 
   * @brief A mask tagging the valid sites. 
   */ 
  Array<bool, UNKNOWN> m_mask;
  /** 
   * @brief A mask tagging the already visited sites. 
   */ 
  Array<bool, UNKNOWN> m_visited;
  /** 
   * @brief A mask tagging the queued sites. 
   */ 
  Array<bool, UNKNOWN> m_queued;
  /** 
   * @brief The dimensions of the square domain. 
   */ 
  int m_M, m_N;

  /** 
   * @brief Inserts the neighborhoods around a site to the queue. 
   *  
   * @param pixel is the site from which is taken the 
   *    	  neighborhood.
   *  
   * @author Julio C. Estrada-Rico 
   * @date 08/19/2010 
   */ 
  void insert_neighbors(const node& pixel);
  /** 
   * @brief Checks is a site direction is into the square domain 
   *        of array.
   *  
   * @param point is the site direcion in cartesian coordinates. 
   *  
   * @author Julio C. Estrada-Rico 
   * @date 08/19/2010 
   */ 
  bool isvalid(const node& point) const;
  /** 
   * @brief Calculates the weight of a neighborhood. 
   * @details 
   * The weight of a neighborhood is defined by the number of 
   * sites, in that neighborhood, that were previously visited. 
   *  
   * @param pixel is the site from which is taked the 
   *    	  neighborhood.
   *  
   * @author Julio C. Estrada-Rico 
   * @date 08/19/2010 
   */ 
  int neighbor_weight(const node& pixel);
  /** 
   * @brief returns the index having the maximum weight. 
   *  
   * @author Julio C. Estrada-Rico 
   * @date 08/19/2010 
   */ 
  int max(int *weights);
  /** 
   * @brief Checks if a site is valid and visited. 
   *  
   * @param point is the site. 
   *  
   * @author Julio C. Estrada-Rico 
   * @date 08/19/2010 
   */ 
  bool isvalid_and_visited(const node& point);

  /** 
   * @brief Represents a site or point in the array. 
   *  
   * @author Julio C. Estrada-Rico 
   * @date 08/19/2010 
   */ 
  struct node{
    /** 
     * @brief Default constructor. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010
     */ 
    node(){}
    /** 
     * @brief node constructor.
     * @details
     * Creates a node with the given cartesian point direction.
     * 
     * @param x is the x direction.
     * @param y is the y direction. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */ 
    node(int x, int y){
      m_x=x;
      m_y=y;
    }
    
    /** 
     * @brief Copy constructor. 
     *  
     * @param cp the node to be copied in this new node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */ 
    node(const node& cp){
      m_x=cp.m_x;
      m_y=cp.m_y;
    }

    /** 
     * @brief overloaded for asignament. 
     *  
     * @param cp the information node to be asigned in this node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */ 
    void operator=(const node& cp){
      m_x=cp.m_x;
      m_y=cp.m_y;
    }

    /** 
     * @brief The x direction of this node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */ 
    int m_x;
    /** 
     * @brief The y direction of this node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */ 
    int m_y;

    /** 
     * @brief Returns the next node position to the east.
     *  
     * @return the node located at the east of this node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */ 
    node east() const;
    /** 
     * @brief Returns the next node position to the east.
     *  
     * @return the node located at the east of this node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */    
    node weast() const;
    /** 
     * @brief Returns the next node position to the weast.
     *  
     * @return the node located at the weast of this node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */    
    node north() const;
    /** 
     * @brief Returns the next node position to the north.
     *  
     * @return the node located at the north of this node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */    
    node south() const;
    /** 
     * @brief Returns the next node position to the east.
     *  
     * @return the node located at the east of this node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */    
    node north_east() const;
    /** 
     * @brief Returns the next node position to the north-east.
     *  
     * @return the node located at the north-east of this node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */    
    node south_east() const;
    /** 
     * @brief Returns the next node position to the south-east.
     *  
     * @return the node located at the south-east of this node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */    
    node north_weast() const;
    /** 
     * @brief Returns the next node position to the north-weast.
     *  
     * @return the node located at the north-weast of this node. 
     *  
     * @author Julio C. Estrada-Rico 
     * @date 08/19/2010  
     */    
    node south_weast() const;

  };

};

#endif /* FLOODFILL_H_ */
