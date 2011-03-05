/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro <g.piro@poliba.it>
 */

/*
 * Test for LTE AMC module
 */

#include <iostream>
#include <vector>
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/log.h>
#include <string>
#include <ns3/mobility-module.h>
#include <ns3/spectrum-helper.h>
#include <ns3/lte-helper.h>
#include <ns3/enb-phy.h>
#include <ns3/ue-phy.h>
#include <ns3/packet-burst.h>
#include <ns3/constant-position-mobility-model.h>
#include <ns3/constant-velocity-mobility-model.h>
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/lte-spectrum-phy.h"
#include "ns3/enb-lte-spectrum-phy.h"
#include "ns3/ue-lte-spectrum-phy.h"
#include "ns3/ue-net-device.h"
#include "ns3/enb-net-device.h"
#include "ns3/ue-manager.h"
#include "ns3/spectrum-propagation-loss-model.h"
#include "ns3/lte-propagation-loss-model.h"
#include "ns3/lte-spectrum-value-helper.h"
#include "ns3/amc-module.h"



NS_LOG_COMPONENT_DEFINE ("TestAmcModule");

using namespace ns3;


int main (int argc, char** argv)
{

  /*
   * This example shows how the AMC module works.
   *
   * The value of the CQI feedback and the selected MCS is
   * reported varying the SINR value.
   *
   * We suppose that
   *   - the eNB uses 50 sub channels for downlink transmission
   *   - the total power transmission is equal to 43 dbM and it is distributed uniformly among dl sub channels
   *
   * We analyze how the SINR value, estimated by the UE varying its distance form the eNB,
   * impacts on the CQI value and the MCS selected by the AMC
   */


  LogComponentEnable ("LtePropagationLossModel", LOG_LEVEL_ALL);


  std::cout << "AMC MODULE" << std::endl;

  LteSpectrumValueHelper h;

  double powerTx = 43; // in dBm
  // Define a list of sub channels for the downlink
  std::vector<int> channels;
  for (int i = 0; i < 50; i++)
    {
      channels.push_back (i);
    }


  Ptr<SpectrumValue> psdTx = h.CreateDownlinkTxPowerSpectralDensity (powerTx, channels);
  Ptr<SpectrumValue> psdNoise = h.CreateDownlinkNoisePowerSpectralDensity ();


  // DEFINE THE LTE PROPAGATION LOSS MODEL
  LtePropagationLossModel lossModel;

  // CREATE UE PHYSICAL LAYER
  Ptr<UeLtePhy> uePhy = CreateObject<UeLtePhy> ();
  Ptr<UeLteSpectrumPhy> dl = CreateObject<UeLteSpectrumPhy> ();
  uePhy->SetDownlinkSubChannels (channels);
  uePhy->SetDownlinkSpectrumPhy (dl);



  // DEFINE THE AMC MODULE
  AmcModule amc;



  // CONFIGURE MOBILITY
  Ptr<ConstantPositionMobilityModel> enbMobility = CreateObject<ConstantPositionMobilityModel> ();
  enbMobility->SetPosition (Vector (0.0, 0.0, 0.0));

  for (int distance = 30; distance < 1000; distance = distance + 30)
    {

      std::cout << "Test AMC module for distance ue-enb = " << distance << " m" << std::endl;

      Ptr<ConstantVelocityMobilityModel> ueMobility = CreateObject<ConstantVelocityMobilityModel> ();
      ueMobility->SetPosition (Vector (distance, 0.0, 0.0));
      ueMobility->SetVelocity (Vector (30.0, 0.0, 0.0));

      uePhy->GetDownlinkSpectrumPhy ()->SetMobility (ueMobility);

      lossModel.CreateChannelRealization (enbMobility, ueMobility);

      Ptr<JakesFadingLossModel> m = lossModel.GetChannelRealization (enbMobility, ueMobility)->GetJakesFadingLossModel ();
      m->SetPhy (uePhy);


      // ADD PROPAGATION LOSS
      Ptr<SpectrumValue> psdRx = lossModel.CalcRxPowerSpectralDensity (psdTx, enbMobility, ueMobility);


      // Compute SINR
      std::vector<double> sinr, rx, n;
      std::cout << "POWER TX: " << std::endl;
      for (Values::const_iterator it = psdRx->ConstValuesBegin ();
           it != psdRx->ConstValuesEnd (); it++ )
        {
          double power; // power transmission for the current sub channel [dB]
          if ((*it) != 0.)
            {
              power = (*it);
              power = 10 * log10 (180000. * power);
            }
          else
            {
              power = 0.;
            }
          rx.push_back (power);
          std::cout << " " << power;
        }
      std::cout << std::endl;
      std::cout << "NOISE: " << std::endl;
      for (Values::const_iterator it = psdNoise->ConstValuesBegin ();
           it != psdNoise->ConstValuesEnd (); it++ )
        {

          double noise = (*it);
          noise = 10 * log10 (180000. * noise);
          n.push_back (noise);
          std::cout << " " << noise;
        }
      std::cout << std::endl;
      int subChannels = rx.size ();
      std::cout << "SINR: ";
      for (int i = 0; i < subChannels; i++)
        {
          if (rx.at (i) != 0)
            {
              double sinr_ = rx.at (i) - n.at (i);
              sinr.push_back (sinr_);
              std::cout << " " << sinr_;
            }
        }
      std::cout << std::endl;


      // COMPUTE THE CQI value
      std::vector<int> cqi = amc.CreateCqiFeedbacks (sinr);
      int feedbacks = cqi.size ();
      std::cout << "CQI: ";
      for (int i = 0; i < feedbacks; i++)
        {
          std::cout << " " << cqi.at (i);
        }
      std::cout << std::endl;


      // COMPUTE THE MCS scheme
      std::cout << "MCS: ";
      for (int i = 0; i < feedbacks; i++)
        {
          std::cout << " " << amc.GetMcsFromCqi (cqi.at (i));
        }
      std::cout << std::endl;


      // COMPUTE THE TRANSPORT BLOCK SIZE
      std::cout << "TB: ";
      for (int i = 0; i < feedbacks; i++)
        {
          std::cout << " " << amc.GetTbSizeFromMcs (amc.GetMcsFromCqi (cqi.at (i)));
        }
      std::cout << std::endl;

    }


  Simulator::Destroy ();

  return 0;
}
