/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA, 2008 Timo Bingmann
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
 * Original Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Enhancements: Timo Bingmann <timo.bingmann@student.kit.edu>
 */
#ifndef GNUPLOT_H
#define GNUPLOT_H

#include <string>
#include <vector>
#include <utility>

namespace ns3 {

/**
 * \ingroup tools
 *
 * \brief Abstract class to store a plot line to be used by ns3::Gnuplot.
 *
 * This class contains a reference counted data object in m_data. The data
 * object contains different structs derived from struct Data by subclasses.
 */
class GnuplotDataset
{
public:

  /**
   * Reference-counting copy constructor.
   */
  GnuplotDataset (const GnuplotDataset& original);

  /**
   * Reference-counting destructor.
   */
  ~GnuplotDataset();

  /**
   * Reference-counting assignment operator.
   */
  GnuplotDataset& operator= (const GnuplotDataset& original);

  /**
   * \brief Change line title.
   * \param title the new title string to use for this dataset.
   */
  void SetTitle (const std::string& title);

  /**
   * \brief Change extra formatting style parameters for newly created objects.
   * \param extra       extra formatting
   */
  static void SetDefaultExtra (const std::string& extra);

  /**
   * \brief Add extra formatting parameters to this dataset.
   * \param extra       extra formatting
   */
  void SetExtra (const std::string& extra);

protected:

  /// Friend because it accesses m_data and it's virtual functions directly in
  /// GenerateOutput().
  friend class Gnuplot;

  /**
   * \brief Extra gnuplot parameters set on every newly created dataset.
   */
  static std::string m_defaultExtra;

  /**
   * \brief Derived classes subclass this struct and add their own data fields.
   */
  struct Data;

  /**
   * Called by constructors of derived classes.
   * \param data the reference counted data object representing this dataset.
   */
  GnuplotDataset (struct Data* data);

  /**
   * Reference counted data object.
   */
  struct Data*  m_data;
};

/**
 * \brief Class to represent a 2D points plot. Set the line or points style
 * using SetStyle() and set points using Add().
 */
class Gnuplot2dDataset : public GnuplotDataset
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
   * \param title the title to be associated to this dataset.
   *
   * Create an empty dataset. Usually, the dataset's title is 
   * displayed in the legend box.
   */
  Gnuplot2dDataset (const std::string& title = "Untitled");

  /**
   * Change default style for all newly created objects.
   * \param style the style of plotting to use for newly created datasets.
   */
  static void SetDefaultStyle (enum Style style);

  /**
   * \param style the style of plotting to use for this dataset.
   */
  void SetStyle (enum Style style);

  /**
   * Change default errorbars style for all newly created objects.
   * \param errorBars the style of errorbars to use for newly created datasets.
   */
  static void SetDefaultErrorBars (enum ErrorBars errorBars);

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
   * \param errorDelta data point error range.
   *
   * Use this method with error bar style X or Y.
   */
  void Add (double x, double y, double errorDelta);

  /**
   * \param x x coord to new data point
   * \param y y coord to new data point
   * \param minY minimum error data point
   * \param maxY maximum error data point
   *
   * Use this method with error bar style X or Y.
   */
  void Add (double x, double y, double minY, double maxY);

  /**
   * Add an empty line in the data output sequence. Empty lines in the plot
   * data break continuous lines and do other things in the output.
   */
  void AddEmptyLine ();

private:

  struct Point {
    bool empty;
    double x;
    double y;
    double dx;
    double dy;
  };

  typedef std::vector<struct Point> PointSet;

  static enum Style m_defaultStyle;
  static enum ErrorBars m_defaultErrorBars;

  /// Forward declaration of the internal data class.
  struct Data2d;
};

/**
 * \brief Class to represent a 2D function expression plot.
 *
 * Since the function expression is not escaped, styles and extras could just
 * as well be included in the expression string.
 */
class Gnuplot2dFunction : public GnuplotDataset
{
public:
  /**
   * \param title the title to be associated to this dataset.
   * \param function function to plot
   *
   * Create an function dataset. Usually, the dataset's title is displayed in
   * the legend box.
   */
  Gnuplot2dFunction (const std::string& title = "Untitled", const std::string& function = "");

  /**
   * \param function new function string to set
   */
  void SetFunction (const std::string& function);

private:

  /// Forward declaration of the internal data class.
  struct Function2d;
};

/**
 * \brief Class to represent a 3D points plot. Set the line or points style
 * using SetStyle() and set points using Add().
 */
class Gnuplot3dDataset : public GnuplotDataset
{
public:
  /**
   * \param title the title to be associated to this dataset.
   *
   * Create an empty dataset. Usually, the dataset's title is 
   * displayed in the legend box.
   */
  Gnuplot3dDataset (const std::string& title = "Untitled");

  /**
   * Change default style for all newly created objects.
   * \param style the style of plotting to use for newly created datasets.
   */
  static void SetDefaultStyle (const std::string& style);

  /**
   * \param style the style of plotting to use for this dataset.
   */
  void SetStyle (const std::string& style);

  /**
   * \param x x coord to new data point
   * \param y y coord to new data point
   * \param z z coord to new data point
   *
   * Use this method to add a new 3D point
   */
  void Add (double x, double y, double z);

  /**
   * Add an empty line in the data output sequence. Empty lines in the plot
   * data break continuous lines and do other things in the output.
   */
  void AddEmptyLine ();

private:

  struct Point {
    bool empty;
    double x, y, z;
  };

  typedef std::vector<struct Point> PointSet;

  static std::string m_defaultStyle;

  /// Forward declaration of the internal data class.
  struct Data3d;
};

/**
 * \brief Class to represent a 3D function expression plot.
 *
 * Since the function expression is not escaped, styles and extras could just as
 * well be included in the expression string. The only difference to
 * Gnuplot2dFunction is the splot command string.
 */
class Gnuplot3dFunction : public GnuplotDataset
{
public:
  /**
   * \param title the title to be associated to this dataset.
   * \param function function to plot
   *
   * Create an function dataset. Usually, the dataset's title is displayed in
   * the legend box.
   */
  Gnuplot3dFunction (const std::string& title = "Untitled", const std::string& function = "");

  /**
   * \param function new function string to set
   */
  void SetFunction (const std::string& function);

private:

  /// Forward declaration of the internal data class.
  struct Function3d;
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
   * \param outputFilename the name of the file where the rendering of the
   *        graph will be generated if you feed the command stream output by
   *        Gnuplot::GenerateOutput to the gnuplot program.
   * \param title title line of the plot page 
   */
  Gnuplot (const std::string& outputFilename="", const std::string& title = "");

  /**
   * Crude attempt to auto-detect the correct terminal setting by inspecting
   * the filename's extension.
   * \param filename output file name
   */
  static std::string DetectTerminal (const std::string& filename);

  /**
   * \param terminal terminal setting string for output. The default terminal
   * string is "png"
   */
  void SetTerminal (const std::string& terminal);

  /**
   * \param title set new plot title string to use for this plot.
   */
  void SetTitle (const std::string& title);

  /**
   * \param xLegend the legend for the x horizontal axis
   * \param yLegend the legend for the y vertical axis
   */
  void SetLegend (const std::string& xLegend, const std::string& yLegend);

  /**
   * \param extra set extra gnuplot directive for output.
   */
  void SetExtra (const std::string& extra);

  /**
   * \param extra append extra gnuplot directive for output.
   */
  void AppendExtra (const std::string& extra);

  /**
   * \param dataset add a dataset to the graph to be plotted.
   */
  void AddDataset (const GnuplotDataset& dataset);

  /**
   * \param os the output stream on which the relevant gnuplot commands should
   * be generated. Including output file and terminal headers.
   */
  void GenerateOutput (std::ostream &os) const;

private:
  typedef std::vector<GnuplotDataset> Datasets;

  std::string m_outputFilename;
  std::string m_terminal;

  Datasets m_datasets;

  std::string m_title;
  std::string m_xLegend;
  std::string m_yLegend;
  std::string m_extra;
};

/**
 * \brief a simple class to group together multiple gnuplots into one file,
 * e.g. for PDF multi-page output terminals.
 */
class GnuplotCollection
{
public:
  /**
   * \param outputFilename the name of the file where the rendering of the
   *        graph will be generated if you feed the command stream output by
   *        GnuplotCollection::GenerateOutput to the gnuplot program.
   */
  GnuplotCollection (const std::string& outputFilename);

  /**
   * \param terminal terminal setting string for output. The default terminal
   * string is guessed from the output filename's extension.
   */
  void SetTerminal (const std::string& terminal);

  /**
   * \param plot add a plot to the collection to be plotted.
   */
  void AddPlot (const Gnuplot& plot);

  /**
   * Return a pointer to one of the added plots.
   * \param id  index of plot to return
   * \return    reference to plot, throws std::range_error if it does not exist.
   */
  Gnuplot& GetPlot (unsigned int id);

  /**
   * \param os the output stream on which the relevant gnuplot commands should
   * be generated.
   */
  void GenerateOutput (std::ostream &os) const;
 
private:
  typedef std::vector<Gnuplot> Plots;

  std::string m_outputFilename;
  std::string m_terminal;

  Plots       m_plots;
};

} // namespace ns3

#endif /* GNUPLOT_H */
