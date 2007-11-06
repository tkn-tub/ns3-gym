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
#include <ostream>

namespace ns3 {

GnuplotDataset::GnuplotDataset (std::string title)
  : m_title (title),
    m_style (LINES)
{}
void 
GnuplotDataset::SetStyle (enum Style style)
{
  m_style = style;
}
void 
GnuplotDataset::Add (double x, double y)
{
  m_dataset.push_back (std::make_pair (x,y));
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
Gnuplot::AddDataset (const GnuplotDataset &dataset)
{
  m_datasets.push_back (new GnuplotDataset (dataset));
}

void 
Gnuplot::GenerateOutput (std::ostream &os)
{
  os << "set terminal png" << std::endl;
  os << "set output '" << m_pngFilename << "'" << std::endl;
  os << "plot ";
  for (Datasets::const_iterator i = m_datasets.begin (); i != m_datasets.end ();)
    {
      os << "'-' title '" << (*i)->m_title << "'";
      switch ((*i)->m_style) {
      case GnuplotDataset::LINES:
	os << " with lines";
	break;
      case GnuplotDataset::POINTS:
	os << " with points";
	break;
      case GnuplotDataset::LINES_POINTS:
	os << " with linespoints";
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
	  os << j->first << " " << j->second << std::endl;
	}
      os << "e" << std::endl;
    }
}

} // namespace ns3
