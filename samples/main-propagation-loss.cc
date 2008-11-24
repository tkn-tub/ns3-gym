/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
#include "ns3/propagation-loss-model.h"
#include "ns3/static-mobility-model.h"
#include "ns3/config.h"
#include "ns3/string.h"

using namespace ns3;

static void
PrintOne (double minTxpower, double maxTxpower, double stepTxpower, double min, double max, double step)
{
  Ptr<StaticMobilityModel> a = CreateObject<StaticMobilityModel> ();
  Ptr<StaticMobilityModel> b = CreateObject<StaticMobilityModel> ();
  Ptr<LogDistancePropagationLossModel> log = CreateObject<LogDistancePropagationLossModel> ();

  Ptr<PropagationLossModel> model = log;

  a->SetPosition (Vector (0.0, 0.0, 0.0));
  for (double x = min; x < max; x+= step)
    {
      b->SetPosition (Vector (x, 0.0, 0.0));
      std::cout << x << " ";
      for (double txpower = minTxpower; txpower < maxTxpower; txpower += stepTxpower)
        {
          double rxPowerDbm = txpower + model->GetLoss (a, b);
          std::cout << rxPowerDbm << " ";
        }
      std::cout << std::endl;
    }
}

int main (int argc, char *argv[])
{

  Config::SetDefault ("ns3::LogDistancePropagationLossModel::ReferenceDistance", StringValue ("1.0"));
  Config::SetDefault ("ns3::LogDistancePropagationLossModel::Exponent", StringValue ("4"));

  PrintOne (-10, 20, 5, 0, 10000, 2);

  return 0;
}
