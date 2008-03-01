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
#include "gnuplot.h"
#include "ns3/assert.h"
#include <ostream>

namespace ns3 {

GnuplotDataset::GnuplotDataset ()
  : m_title ("untitled"),
    m_style (LINES),
    m_errorBars (NONE)
{}

GnuplotDataset::GnuplotDataset (std::string title)
  : m_title (title),
    m_style (LINES),
    m_errorBars (NONE)
{}
void 
GnuplotDataset::SetStyle (enum Style style)
{
  m_style = style;
}
void 
GnuplotDataset::SetErrorBars (enum ErrorBars errorBars)
{
  m_errorBars = errorBars;
}
void 
GnuplotDataset::Add (double x, double y)
{
  NS_ASSERT (m_errorBars == NONE);
  struct Data data;
  data.x = x;
  data.y = y;
  data.dx = 0.0;
  data.dy = 0.0;
  m_dataset.push_back (data);
}
void 
GnuplotDataset::Add (double x, double y, double errorDelta)
{
  NS_ASSERT (m_errorBars == X || m_errorBars == Y);
  struct Data data;
  data.x = x;
  data.y = y;
  data.dx = errorDelta;
  data.dy = errorDelta;
  m_dataset.push_back (data);
}

Gnuplot::Gnuplot (std::string pngFilename)
  : m_pngFilename (pngFilename)
{}

Gnuplot::~Gnuplot ()
{
  for (Datasets::const_iterator i = m_datasets.begin (); i != m_datasets.end (); i++)
    {
      delete *i;
    }
  m_datasets.clear ();
}

void 
Gnuplot::SetLegend (std::string xLegend, std::string yLegend)
{
  m_xLegend = xLegend;
  m_yLegend = yLegend;
}

void 
Gnuplot::AddDataset (const GnuplotDataset &dataset)
{
  m_datasets.push_back (new GnuplotDataset (dataset));
}

void 
Gnuplot::GenerateOutput (std::ostream &os)
{
  os << "set terminal png" << std::endl;
  os << "set output '" << m_pngFilename << "'" << std::endl;
  os << "set xlabel '" << m_xLegend << "'" << std::endl;
  os << "set ylabel '" << m_yLegend << "'" << std::endl;
  os << "plot ";
  for (Datasets::const_iterator i = m_datasets.begin (); i != m_datasets.end ();)
    {
      os << "'-' title '" << (*i)->m_title << "'";
      switch ((*i)->m_style) {
      case GnuplotDataset::LINES:
        os << " with lines";
	break;
      case GnuplotDataset::POINTS:
        switch ((*i)->m_errorBars)
          {
          case GnuplotDataset::NONE:
            os << " with points";
            break;
          case GnuplotDataset::X:
            os << " with xerrorbars";
            break;
          case GnuplotDataset::Y:
            os << " with yerrorbars";
            break;
          case GnuplotDataset::XY:
            os << " with xyerrorbars";
            break;
          }
	break;
      case GnuplotDataset::LINES_POINTS:
        switch ((*i)->m_errorBars)
          {
          case GnuplotDataset::NONE:
            os << " with linespoints";
            break;
          case GnuplotDataset::X:
            os << " with xerrorlines";
            break;
          case GnuplotDataset::Y:
            os << " with yerrorlines";
            break;
          case GnuplotDataset::XY:
            os << " with xyerrorlines";
            break;
          }
	break;
      case GnuplotDataset::DOTS:
	os << " with dots";
	break;
      case GnuplotDataset::IMPULSES:
	os << " with impulses";
	break;
      case GnuplotDataset::STEPS:
	os << " with steps";
	break;
      case GnuplotDataset::FSTEPS:
	os << " with fsteps";
	break;
      case GnuplotDataset::HISTEPS:
	os << " with histeps";
	break;
      }
      i++;
      if (i != m_datasets.end ())
	{
	  os << ", ";
	}
    }
  os << std::endl;
  for (Datasets::const_iterator i = m_datasets.begin (); i != m_datasets.end (); i++)
    {
      for (GnuplotDataset::Dataset::const_iterator j = (*i)->m_dataset.begin ();
	   j != (*i)->m_dataset.end (); j++)
	{
          switch ((*i)->m_errorBars) {
          case GnuplotDataset::NONE:
            os << j->x << " " << j->y << std::endl;
            break;
          case GnuplotDataset::X:
            os << j->x << " " << j->y << " " << j->dx << std::endl;
            break;
          case GnuplotDataset::Y:
            os << j->x << " " << j->y << " " << j->dy << std::endl;
            break;
          case GnuplotDataset::XY:
            os << j->x << " " << j->y << " " << j->dx << " " << j->dy << std::endl;
            break;
          }
	}
      os << "e" << std::endl;
    }
}

} // namespace ns3
