/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 Timo Bingmann
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
 * Author: Timo Bingmann <timo.bingmann@student.kit.edu>
 */
#include "ns3/random-variable.h"
#include "ns3/gnuplot.h"
#include <map>
#include <cmath>

using namespace ns3;

/// Round a double number to the given precision. e.g. dround(0.234, 0.1) = 0.2
/// and dround(0.257, 0.1) = 0.3
double dround(double number, double precision)
{
  number /= precision;
  if (number >= 0)
    number = std::floor(number + 0.5);
  else
    number = std::ceil(number - 0.5);
  number *= precision;
  return number;
}

static GnuplotDataset
Histogramm (RandomVariable rndvar, unsigned int probes, double precision, const std::string& title, bool notcontinous = false)
{
  typedef std::map<double, unsigned int> histogramm_maptype;
  histogramm_maptype histogramm;

  for(unsigned int i = 0; i < probes; ++i)
    {
      double val = dround( rndvar.GetValue(), precision );

      ++histogramm[val];
    }

  Gnuplot2dDataset data;
  data.SetTitle(title);

  if (notcontinous)
    {
      data.SetStyle(Gnuplot2dDataset::IMPULSES);
    }

  for(histogramm_maptype::const_iterator hi = histogramm.begin();
      hi != histogramm.end(); ++hi)
    {
      data.Add(hi->first, (double)hi->second / (double)probes / precision);
    }

  return data;
}

int main (int argc, char *argv[])
{
  unsigned int probes = 1000000;
  double precision = 0.01;

  GnuplotCollection gnuplots("main-random-variables.pdf");
  gnuplots.SetTerminal("pdf enhanced");

  {
    Gnuplot plot;
    plot.SetTitle("UniformVariable");
    plot.AppendExtra("set yrange [0:]");

    plot.AddDataset( Histogramm(UniformVariable(0.0, 1.0), probes, precision,
                                "UniformVariable [0.0 .. 1.0)") );

    plot.AddDataset( Gnuplot2dFunction("0.1",
                                       "0 <= x && x <= 1 ? 1.0 : 0") );

    gnuplots.AddPlot(plot);
  }

  {
    Gnuplot plot;
    plot.SetTitle("ExponentialVariable");
    plot.AppendExtra("set xrange [0:8]");
    plot.AppendExtra("ExpDist(x,l) = 1/l * exp(-1/l * x)");

    plot.AddDataset( Histogramm(ExponentialVariable(0.5), probes, precision,
                                "ExponentialVariable m=0.5") );

    plot.AddDataset( Gnuplot2dFunction("ExponentialDistribution mean 0.5",
                                       "ExpDist(x, 0.5)") );

    plot.AddDataset( Histogramm(ExponentialVariable(1.0), probes, precision,
                                "ExponentialVariable m=1") );

    plot.AddDataset( Gnuplot2dFunction("ExponentialDistribution mean 1.0",
                                       "ExpDist(x, 1.0)") );

    plot.AddDataset( Histogramm(ExponentialVariable(1.5), probes, precision,
                                "ExponentialVariable m=1.5") );

    plot.AddDataset( Gnuplot2dFunction("ExponentialDistribution mean 1.5",
                                       "ExpDist(x, 1.5)") );

    gnuplots.AddPlot(plot);
  }

  {
    Gnuplot plot;
    plot.SetTitle("ParetoVariable");
    plot.AppendExtra("set xrange [0:2]");

    plot.AddDataset( Histogramm(ParetoVariable(1.0, 1.5), probes, precision,
                                "ParetoVariable m=1.0 s=1.5") );

    plot.AddDataset( Histogramm(ParetoVariable(1.0, 2.0), probes, precision,
                                "ParetoVariable m=1.0 s=2.0") );

    plot.AddDataset( Histogramm(ParetoVariable(1.0, 2.5), probes, precision,
                                "ParetoVariable m=1.0 s=2.5") );

    gnuplots.AddPlot(plot);
  }

  {
    Gnuplot plot;
    plot.SetTitle("WeibullVariable");
    plot.AppendExtra("set xrange [0:3]");

    plot.AddDataset( Histogramm(WeibullVariable(1.0, 1.0), probes, precision,
                                "WeibullVariable m=1.0 s=1.0") );

    plot.AddDataset( Histogramm(WeibullVariable(1.0, 2.0), probes, precision,
                                "WeibullVariable m=1.0 s=2.0") );

    plot.AddDataset( Histogramm(WeibullVariable(1.0, 3.0), probes, precision,
                                "WeibullVariable m=1.0 s=3.0") );

    gnuplots.AddPlot(plot);
  }

  {
    Gnuplot plot;
    plot.SetTitle("NormalVariable");
    plot.AppendExtra("set xrange [-3:3]");
    plot.AppendExtra("NormalDist(x,m,s) = 1 / (s * sqrt(2*pi)) * exp(-1.0 / 2.0 * ((x-m) / s)**2)");

    plot.AddDataset( Histogramm(NormalVariable(0.0, 1.0), probes, precision,
                                "NormalVariable m=0.0 v=1.0") );

    plot.AddDataset( Gnuplot2dFunction("NormalDist {/Symbol m}=0.0 {/Symbol s}=1.0",
                                       "NormalDist(x,0.0,1.0)") );

    plot.AddDataset( Histogramm(NormalVariable(0.0, 2.0), probes, precision,
                                "NormalVariable m=0.0 v=2.0") );

    plot.AddDataset( Gnuplot2dFunction("NormalDist {/Symbol m}=0.0 {/Symbol s}=sqrt(2.0)",
                                       "NormalDist(x,0.0,sqrt(2.0))") );

    plot.AddDataset( Histogramm(NormalVariable(0.0, 3.0), probes, precision,
                                "NormalVariable m=0.0 v=3.0") );

    plot.AddDataset( Gnuplot2dFunction("NormalDist {/Symbol m}=0.0 {/Symbol s}=sqrt(3.0)",
                                       "NormalDist(x,0.0,sqrt(3.0))") );

    gnuplots.AddPlot(plot);
  }

  {
    Gnuplot plot;
    plot.SetTitle("EmpiricalVariable");
    plot.AppendExtra("set xrange [*:*]");

    EmpiricalVariable emp1;
    emp1.CDF(0.0,  0.0 / 15.0);
    emp1.CDF(0.2,  1.0 / 15.0);
    emp1.CDF(0.4,  3.0 / 15.0);
    emp1.CDF(0.6,  6.0 / 15.0);
    emp1.CDF(0.8, 10.0 / 15.0);
    emp1.CDF(1.0, 15.0 / 15.0);

    plot.AddDataset( Histogramm(emp1, probes, precision,
                                "EmpiricalVariable (Stairs)") );

    gnuplots.AddPlot(plot);
  }

  {
    Gnuplot plot;
    plot.SetTitle("DeterministicVariable");
    plot.AppendExtra("set xrange [*:*]");

    double values[] = { 0.0, 0.2, 0.2, 0.4, 0.2, 0.6, 0.8, 0.8, 1.0 };
    DeterministicVariable det1 (values, sizeof(values) / sizeof(values[0]));

    plot.AddDataset( Histogramm(det1, probes, precision,
                                "DeterministicVariable", true) );

    gnuplots.AddPlot(plot);
  }

  {
    Gnuplot plot;
    plot.SetTitle("LogNormalVariable");
    plot.AppendExtra("set xrange [0:3]");

    plot.AppendExtra("LogNormalDist(x,m,s) = 1.0/x * NormalDist(log(x), m, s)");

    plot.AddDataset( Histogramm(LogNormalVariable(0.0, 1.0), probes, precision,
                                "LogNormalVariable m=0.0 s=1.0") );

    plot.AddDataset( Gnuplot2dFunction("LogNormalDist(x, 0.0, 1.0)",
                                       "LogNormalDist(x, 0.0, 1.0)") );

    plot.AddDataset( Histogramm(LogNormalVariable(0.0, 0.5), probes, precision,
                                "LogNormalVariable m=0.0 s=0.5") );

    plot.AddDataset( Histogramm(LogNormalVariable(0.0, 0.25), probes, precision,
                                "LogNormalVariable m=0.0 s=0.25") );

    plot.AddDataset( Gnuplot2dFunction("LogNormalDist(x, 0.0, 0.25)",
                                       "LogNormalDist(x, 0.0, 0.25)") );

    plot.AddDataset( Histogramm(LogNormalVariable(0.0, 0.125), probes, precision,
                                "LogNormalVariable m=0.0 s=0.125") );

    plot.AddDataset( Histogramm(LogNormalVariable(0.0, 2.0), probes, precision,
                                "LogNormalVariable m=0.0 s=2.0") );

    plot.AddDataset( Gnuplot2dFunction("LogNormalDist(x, 0.0, 2.0)",
                                       "LogNormalDist(x, 0.0, 2.0)") );

    plot.AddDataset( Histogramm(LogNormalVariable(0.0, 2.5), probes, precision,
                                "LogNormalVariable m=0.0 s=2.5") );

    gnuplots.AddPlot(plot);
  }

  {
    Gnuplot plot;
    plot.SetTitle("TriangularVariable");
    plot.AppendExtra("set xrange [*:*]");

    plot.AddDataset( Histogramm(TriangularVariable(0.0, 1.0, 0.5), probes, precision,
                                "TriangularVariable [0.0 .. 1.0) m=0.5") );

    plot.AddDataset( Histogramm(TriangularVariable(0.0, 1.0, 0.4), probes, precision,
                                "TriangularVariable [0.0 .. 1.0) m=0.4") );

    plot.AddDataset( Histogramm(TriangularVariable(0.0, 1.0, 0.65), probes, precision,
                                "TriangularVariable [0.0 .. 1.0) m=0.65") );

    gnuplots.AddPlot(plot);
  }

  {
    Gnuplot plot;
    plot.SetTitle("GammaVariable");
    plot.AppendExtra("set xrange [0:10]");
    plot.AppendExtra("set yrange [0:1]");
    plot.AppendExtra("GammaDist(x,a,b) = x**(a-1) * 1/b**a * exp(-x/b) / gamma(a)");

    plot.AppendExtra("set label 1 '{/Symbol g}(x,{/Symbol a},{/Symbol b}) = x^{/Symbol a-1} e^{-x {/Symbol b}^{-1}} ( {/Symbol b}^{/Symbol a} {/Symbol G}({/Symbol a}) )^{-1}' at 0.7, 0.9");

    plot.AddDataset( Histogramm(GammaVariable(1.0, 1.0), probes, precision,
                                "GammaVariable a=1.0 b=1.0") );

    plot.AddDataset( Gnuplot2dFunction("{/Symbol g}(x, 1.0, 1.0)",
                                       "GammaDist(x, 1.0, 1.0)") );

    plot.AddDataset( Histogramm(GammaVariable(1.5, 1.0), probes, precision,
                                "GammaVariable a=1.5 b=1.0") );

    plot.AddDataset( Gnuplot2dFunction("{/Symbol g}(x, 1.5, 1.0)",
                                       "GammaDist(x, 1.5, 1.0)") );

    plot.AddDataset( Histogramm(GammaVariable(2.0, 1.0), probes, precision,
                                "GammaVariable a=2.0 b=1.0") );

    plot.AddDataset( Gnuplot2dFunction("{/Symbol g}(x, 2.0, 1.0)",
                                       "GammaDist(x, 2.0, 1.0)") );

    plot.AddDataset( Histogramm(GammaVariable(4.0, 1.0), probes, precision,
                                "GammaVariable a=4.0 b=1.0") );

    plot.AddDataset( Gnuplot2dFunction("{/Symbol g}(x, 4.0, 1.0)",
                                       "GammaDist(x, 4.0, 1.0)") );

    plot.AddDataset( Histogramm(GammaVariable(2.0, 2.0), probes, precision,
                                "GammaVariable a=2.0 b=2.0") );

    plot.AddDataset( Gnuplot2dFunction("{/Symbol g}(x, 2.0, 2.0)",
                                       "GammaDist(x, 2.0, 2.0)") );

    plot.AddDataset( Histogramm(GammaVariable(2.5, 3.0), probes, precision,
                                "GammaVariable a=2.5 b=3.0") );

    plot.AddDataset( Gnuplot2dFunction("{/Symbol g}(x, 2.5, 3.0)",
                                       "GammaDist(x, 2.5, 3.0)") );

    plot.AddDataset( Histogramm(GammaVariable(2.5, 4.5), probes, precision,
                                "GammaVariable a=2.5 b=4.5") );

    plot.AddDataset( Gnuplot2dFunction("{/Symbol g}(x, 2.5, 4.5)",
                                       "GammaDist(x, 2.5, 4.5)") );

    gnuplots.AddPlot(plot);
  }

  {
    Gnuplot plot;
    plot.SetTitle("ErlangVariable");
    plot.AppendExtra("set xrange [0:10]");
    plot.AppendExtra("ErlangDist(x,k,l) = x**(k-1) * 1/l**k * exp(-x/l) / (k-1)!");

    plot.AppendExtra("set label 1 'Erlang(x,k,{/Symbol l}) = x^{k-1} e^{-x {/Symbol l}^{-1}} ( {/Symbol l}^k (k-1)! )^{-1}' at 0.7, 0.9");

    plot.AddDataset( Histogramm(ErlangVariable(1, 1.0), probes, precision,
                                "ErlangVariable k=1 {/Symbol l}=1.0") );

    plot.AddDataset( Gnuplot2dFunction("Erlang(x, 1, 1.0)",
                                       "ErlangDist(x, 1, 1.0)") );

    plot.AddDataset( Histogramm(ErlangVariable(2, 1.0), probes, precision,
                                "ErlangVariable k=2 {/Symbol l}=1.0") );

    plot.AddDataset( Gnuplot2dFunction("Erlang(x, 2, 1.0)",
                                       "ErlangDist(x, 2, 1.0)") );

    plot.AddDataset( Histogramm(ErlangVariable(3, 1.0), probes, precision,
                                "ErlangVariable k=3 {/Symbol l}=1.0") );

    plot.AddDataset( Gnuplot2dFunction("Erlang(x, 3, 1.0)",
                                       "ErlangDist(x, 3, 1.0)") );

    plot.AddDataset( Histogramm(ErlangVariable(5, 1.0), probes, precision,
                                "ErlangVariable k=5 {/Symbol l}=1.0") );

    plot.AddDataset( Gnuplot2dFunction("Erlang(x, 5, 1.0)",
                                       "ErlangDist(x, 5, 1.0)") );

    plot.AddDataset( Histogramm(ErlangVariable(2, 2.0), probes, precision,
                                "ErlangVariable k=2 {/Symbol l}=2.0") );

    plot.AddDataset( Gnuplot2dFunction("Erlang(x, 2, 2.0)",
                                       "ErlangDist(x, 2, 2.0)") );

    plot.AddDataset( Histogramm(ErlangVariable(2, 3.0), probes, precision,
                                "ErlangVariable k=2 {/Symbol l}=3.0") );

    plot.AddDataset( Gnuplot2dFunction("Erlang(x, 2, 3.0)",
                                       "ErlangDist(x, 2, 3.0)") );

    plot.AddDataset( Histogramm(ErlangVariable(2, 5.0), probes, precision,
                                "ErlangVariable k=2 {/Symbol l}=5.0") );

    plot.AddDataset( Gnuplot2dFunction("Erlang(x, 2, 5.0)",
                                       "ErlangDist(x, 2, 5.0)") );

    gnuplots.AddPlot(plot);
  }

  gnuplots.GenerateOutput(std::cout);

  return 0;
}
