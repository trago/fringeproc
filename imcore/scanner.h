/**************************************************************************
Copyright (c) 2012, Julio C. Estrada
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

+ Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

+ Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************/

#ifndef SCANNER_H
#define SCANNER_H

#ifndef SWIG

#include <Eigen/Dense>
#include <list>
#include <vector>

#endif

/**
 * Scans sequentially the matrix pixels folloging the magnitude of the gradient.
 *
 * It recives the differences in x- and y-direction and gives the pixel
 * sequence that scans the data matrix following the magnitude of the vector
 * space build with the components.
 *
 * Here we have the reference to differences in x- and y-direction. It gives
 * the flexibility to update these fields outside the class. For example, we
 * can ask a pixel to objects of this class, process data, update
 * the x- and y-direction fields and in the following pixel asking it will
 * use the updated fields automatically.
 *
 * @author Julio C. Estrada
 */
class Scanner
{
public:
  /**
   * Default constructor.
   */
  Scanner();
#ifndef SWIG
  /**
   * Builds the scanner with the given differences in x- and y-direction.
   *
   * @param mat_u the differences in x-direction
   * @param mat_v the differences in y-direction
   */
  Scanner(const Eigen::ArrayXXf* mat_u, const Eigen::ArrayXXf* mat_v);
  /**
   * Builds the scanner with the given differences in x- and y-direction,
   * starting at the given pixel point.
   *
   * @param mat_u the differences in x-direction
   * @param mat_v the differences in y-direction
   * @param pixel the starting pixel point
   */
  Scanner(const Eigen::ArrayXXf* mat_u,
          const Eigen::ArrayXXf* mat_v, Eigen::Array2i pixel);

  void setMask(Eigen::ArrayXXf mask);
#endif
  /**
   * Determines the next pixel point in the sequence.
   *
   * @return true if there was a next pixel, false in other case.
   */
  bool next();
  /**
   * Gets the actual pixel position.
   *
   * @return the actual pixel point
   */
  Eigen::Array2i getPosition();
   /**
   * Sets the starting pixel point.
   *
   * @param pixel the starting pixel point
   */
  void setInitPosition(Eigen::Array2i pixel);
  /**
   * Sets the minimum magnitude (frequency magnitude) to scan.
   *
   * The minimum frequency magnitude is the minimum magnitude that will be
   * considered for scanning. That is, it will not visit any pixel whose
   * magnitude is lower than this frequency magnitude.
   *
   * @param freq the minimum frequency magnitude.
   */
  void setFreqMin(double freq);

  /**
   * Spesifies if the minimum frequency to scan can be updated.
   *
   * The minimum freqeuncy to update can be updated automatically when all
   * points having less than the minimum frequency were scanned. When this
   * is the case, the minimum freqeuncy to scan is updated to the first
   * frequency that belongs to the first border point found. A border point is
   * in the edge of the scannen and not scanned points.
   *
   * @param update true if the minimum frequency to scann should
   *        be updated.
   */
  void updateFreqMin(bool update);

private:
  /** The frequencies or differences in x-direction */
  const Eigen::ArrayXXf* m_matu;
  /** The frequencies or differences in y-direction */
  const Eigen::ArrayXXf* m_matv;
  /** Mask for the data */
  Eigen::ArrayXXf m_mask;
  /** Label field that marks whith true the already visited pixels. */
  Eigen::ArrayXXf m_visited;
  /** The current pixel in the scanning sequence */
  Eigen::Array2i m_pixel;
  /** Stores the pixel sequence */
  std::vector<Eigen::Array2i, Eigen::aligned_allocator<Eigen::Array2i> > m_path;
  /** The minimum frequency to scan*/
  double m_freqmin;
  /** Indicates if the minimum frequency to scan is updated*/
  bool m_updateMinFreq;

  /** Inserts the pixel to the path and marks it as visited*/
  void insertPixelToPath(const Eigen::Array2i& pixel);
  /** }
   * Moves to the next pixel in the path.
   *
   * @param magn the minimum frequency magnitud to scan
   */
  bool next(double magn);
  /**
   * @todo In development.
   */
  Eigen::Array2i findPixel();
  /**
   * @todo In development.
   */
  bool checkNeighbor(Eigen::Array2i pixel);

};

#endif // SCANNER_H
