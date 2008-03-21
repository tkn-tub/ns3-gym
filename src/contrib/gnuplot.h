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

/**
 * \brief store a dataset to be used by ns3::Gnuplot
 */
class GnuplotDataset
{
public:
  /**
   * The plotting style to use for this dataset.
   */
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
  /**
   * Whether errorbars should be used for this dataset.
   */
  enum ErrorBars {
    NONE,
    X,
    Y,
    XY
  };

  /**
   * Create an empty dataset without any title.
   */
  GnuplotDataset ();
  /**
   * \param title the title to be associated to this dataset.
   *
   * Create an empty dataset. Usually, the dataset's title is 
   * displayed in the legend box.
   */
  GnuplotDataset (std::string title);
  /**
   * \param style the style of plotting to use for this dataset.
   */
  void SetStyle (enum Style style);
  /**
   * \param errorBars the style of errorbars to display.
   *
   * If you use any style other than none, you need
   * to make sure you store the delta information in 
   * this dataset with the right GnuplotDataset::Add 
   * method.
   */
  void SetErrorBars (enum ErrorBars errorBars);
  /**
   * \param x x coord to new data point
   * \param y y coord to new data point
   *
   * Use this method with error bar style NONE.
   */
  void Add (double x, double y);
  /**
   * \param x x coord to new data point
   * \param y y coord to new data point
   * \param errorDelta
   *
   * Use this method with error bar style X or Y.
   */
  void Add (double x, double y, double errorDelta);
  /**
   * \param x x coord to new data point
   * \param y y coord to new data point
   * \param errorDeltaX x delta for the new data point
   * \param errorDeltaY y delta for the new data point
   *
   * Use this method with error bar style XY.
   */
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

/**
 * \brief a simple class to generate gnuplot-ready plotting commands
 *        from a set of datasets.
 *
 * This class really represents a single graph on which multiple datasets
 * can be plotted.
 */
class Gnuplot
{
public:
  /**
   * \param pngFilename the name of the file where the png rendering of the
   *        graph will be generated if you feed the command stream output by
   *        Gnuplot::GenerateOutput to the gnuplot program.
   */
  Gnuplot (std::string pngFilename);
  ~Gnuplot ();

  /**
   * \param xLegend the legend for the x horizontal axis
   * \param yLegend the legend for the y vertical axis
   */
  void SetLegend (std::string xLegend, std::string yLegend);

  /**
   * \param dataset add a dataset to the graph to be plotted.
   */
  void AddDataset (const GnuplotDataset &dataset);

  /**
   * \param os the output stream on which the relevant gnuplot
   *        commands should be generated.
   */
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
