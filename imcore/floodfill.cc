/*
 * floodfill.cpp
 *
 *  Created on: Aug 19, 2010
 *      Author: julio
 */

#include "floodfill.h"

FloodFill::FloodFill(int start_x, int start_y, int M, int N) :
  m_queue(), m_mask(M, N), m_visited(M, N), m_queued(M, N)
{
  m_M= M;
  m_N= N;
  node point(start_x, start_y);
  m_queue.push_back(point);
  m_mask.setVal(true);
  m_visited.setVal(false);
  m_queued.setVal(false);
  m_queued(start_x, start_y)= true;
}

bool FloodFill::next(int& out_x, int& out_y)
{
  if (!m_queue.empty()) {
    node point= m_queue.back();
    out_x= point.m_x;
    out_y= point.m_y;
    m_visited(out_x, out_y)= true;
    m_queue.pop_back();

    insert_neighbors(point);

    return true;
  }
  int M= m_visited.rows(), N= m_visited.cols();
  bool*__restrict visited = m_visited.getData();
  for (int i= out_y; i < M; i++){
    bool* row = visited + i*N;
    for (int j= out_x; j < N; j++)
      if (!row[j]) {
        node point(j, i);
        out_x= j;
        out_y= i;
        row[j]= true;
        insert_neighbors(point);

        return true;
      }
  }

  return false;
}

FloodFill::~FloodFill()
{
}

inline
void FloodFill::insert_neighbors(const node& pixel)
{

  int weights[]= {-1, -1, -1, -1};

  node point= pixel.east();
  if (isvalid(point))
    if (!m_visited(point.m_x, point.m_y))
      weights[0]= neighbor_weight(point);
  point= pixel.weast();
  if (isvalid(point))
    if (!m_visited(point.m_x, point.m_y))
      weights[1]= neighbor_weight(point);
  point= pixel.north();
  if (isvalid(point))
    if (!m_visited(point.m_x, point.m_y))
      weights[2]= neighbor_weight(point);
  point= pixel.south();
  if (isvalid(point))
    if (!m_visited(point.m_x, point.m_y))
      weights[3]= neighbor_weight(point);

  switch (max(weights))
  {
  case 0:
    point= pixel.east();
    if (isvalid(point))
      m_queue.push_front(point);
    break;
  case 1:
    point= pixel.weast();
    if (isvalid(point))
      m_queue.push_front(point);
    break;
  case 2:
    point= pixel.north();
    if (isvalid(point))
      m_queue.push_front(point);
    break;
  case 3:
    point= pixel.south();
    if (isvalid(point))
      m_queue.push_front(point);
  }

}
inline
int FloodFill::neighbor_weight(const node& pixel)
{
  int weight= 0;
  weight= isvalid_and_visited(pixel.east()) ? weight + 1 : weight;
  weight= isvalid_and_visited(pixel.weast()) ? weight + 1 : weight;
  weight= isvalid_and_visited(pixel.north()) ? weight + 1 : weight;
  weight= isvalid_and_visited(pixel.south()) ? weight + 1 : weight;
  weight= isvalid_and_visited(pixel.north_east()) ? weight + 1 : weight;
  weight= isvalid_and_visited(pixel.north_weast()) ? weight + 1 : weight;
  weight= isvalid_and_visited(pixel.south_east()) ? weight + 1 : weight;
  weight= isvalid_and_visited(pixel.south_weast()) ? weight + 1 : weight;

  return weight;
}
inline
int FloodFill::max(int *weights)
{
  int mayor= weights[0];
  int neighbor= 0;
  for (int i= 1; i < 4; i++)
    if (mayor < weights[i]) {
      mayor= weights[i];
      neighbor= i;
    }

  return (neighbor < 0) ? 0 : neighbor;
}
inline
bool FloodFill::isvalid_and_visited(const node& point)
{
  return isvalid(point) && m_visited(point.m_x, point.m_y);
}
inline
bool FloodFill::isvalid(const node& point) const
{
  if (point.m_x < m_N && point.m_x >= 0 && point.m_y < m_M && point.m_y >= 0)
    if (m_mask(point.m_x, point.m_y) == true)
      return true;
  return false;
}
inline FloodFill::node FloodFill::node::east() const
{
  return node(m_x + 1, m_y);
}
inline FloodFill::node FloodFill::node::weast() const
{
  return node(m_x - 1, m_y);
}
inline FloodFill::node FloodFill::node::north() const
{
  return node(m_x, m_y - 1);
}
inline FloodFill::node FloodFill::node::south() const
{
  return node(m_x, m_y + 1);
}
inline FloodFill::node FloodFill::node::north_east() const
{
  return node(m_x + 1, m_y - 1);
}
inline FloodFill::node FloodFill::node::south_east() const
{
  return node(m_x + 1, m_y + 1);
}
inline FloodFill::node FloodFill::node::north_weast() const
{
  return node(m_x - 1, m_y - 1);
}
inline FloodFill::node FloodFill::node::south_weast() const
{
  return node(m_x - 1, m_y + 1);
}
