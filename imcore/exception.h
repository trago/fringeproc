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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

/**
 * Exception management.
 * 
 * @author Julio C. Estrada-Rico
 * 25/02/2010
 */
class Exception{
 public:
  /**
   * Constructor.
   *
   * Crea una excepción inisializando el parámetro de cuando sucedio, y el
   * mensaje de esta excepción
   * 
   * @param when describe cuando sucedio la excepción.
   * @param msg es el mensaje que se envia al crear la excepción.
   *
   * @author Julio C. Estrada-Rico
   * 25/02/2010
   */
  Exception(const std::string when, const std::string msg);
  /**
   * Regresa la description (mensaje) de la excepción generada.
   *
   * @return El mensaje que se envia en esta excepción.
   *
   * @author Julio C. Estrada-Rico
   * 25/02/2010
   */
  std::string description() const;
  /**
   * Regresa una descripción de la situación de cuando sucedio el error.
   *
   * @return La situación de cuando sucedio la excepción.
   *
   * @author Julio C. Estrada-Rico
   * 25/02/2010
   */
  std::string when() const;

 private:
  std::string m_desc;
  std::string m_when;
};

#endif // EXCEPTION_H
