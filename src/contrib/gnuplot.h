/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef GNUPLOT_H
#define GNUPLOT_H

#include <string>
#include <vector>
#include <utility>

namespace ns3 {

class GnuplotDataset
{
public:
  enum Style {
    LINES,
    POINTS,
    LINES_POINTS,
    DOTS,
    IMPULSES,
    STEPS,
    FSTEPS,
    HISTEPS,
  };
  enum ErrorBars {
    NONE,
    X,
    Y,
    XY
  };

  GnuplotDataset ();
  GnuplotDataset (std::string title);
  void SetStyle (enum Style style);
  void SetErrorBars (enum ErrorBars errorBars);
  void Add (double x, double y);
  void Add (double x, double y, double errorDelta);
  void Add (double x, double y, double errorDeltaX, double errorDeltaY);
private:
  friend class Gnuplot;
  struct Data {
    double x;
    double y;
    double dx;
    double dy;
  };
  typedef std::vector<struct Data> Dataset;
  Dataset m_dataset;
  std::string m_title;
  enum Style m_style;
  enum ErrorBars m_errorBars;
};

class Gnuplot
{
public:
  Gnuplot (std::string pngFilename);
  ~Gnuplot ();

  void SetLegend (std::string xLegend, std::string yLegend);

  void AddDataset (const GnuplotDataset &dataset);

  void GenerateOutput (std::ostream &os);
private:
  typedef std::vector<GnuplotDataset *> Datasets;
  Datasets m_datasets;
  std::string m_xLegend;
  std::string m_yLegend;
  std::string m_pngFilename;
};

} // namespace ns3

#endif /* GNUPLOT_H */
