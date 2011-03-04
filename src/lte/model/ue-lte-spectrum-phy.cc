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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#include <ns3/waveform-generator.h>
#include <ns3/object-factory.h>
#include <ns3/log.h>
#include <math.h>
#include <ns3/simulator.h>
#include <ns3/trace-source-accessor.h>
#include "ns3/spectrum-error-model.h"
#include "lte-spectrum-phy.h"
#include "lte-net-device.h"
#include "ue-lte-spectrum-phy.h"
#include "lte-spectrum-value-helper.h"
#include "ue-net-device.h"
#include "ue-phy.h"

NS_LOG_COMPONENT_DEFINE ("UeLteSpectrumPhy");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (UeLteSpectrumPhy);

UeLteSpectrumPhy::UeLteSpectrumPhy ()
{
  SetMobility (0);
  SetDevice (0);
  SetChannel (0);
  SetState (LteSpectrumPhy::IDLE);
  //GetSpectrumInterference ()->SetErrorModel (CreateObject<LteSpectrumErrorModel> ());

  LteSpectrumValueHelper psdHelper;
  Ptr<SpectrumValue> noisePsd = psdHelper.CreateDownlinkNoisePowerSpectralDensity ();
  SetNoisePowerSpectralDensity (noisePsd);
}


UeLteSpectrumPhy::~UeLteSpectrumPhy ()
{
}


TypeId
UeLteSpectrumPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UeLteSpectrumPhy")
    .SetParent<LteSpectrumPhy> ()
    .AddConstructor<UeLteSpectrumPhy> ()
  ;
  return tid;
}



void
UeLteSpectrumPhy::CalcSinrValues (Ptr <const SpectrumValue> rxPsd, Ptr <const SpectrumValue> noise)
{
  /*
   * TO DO:
   * Compute the SINR of the incoming signal, using the rxPsd and the Noise Psd.
   * Transfer this value to the device that will compute the CQI value.
   *
   * the UE receives the signal from the eNB. It computes the SINR and tranfers
   * it to the UeNetDevice. The UeNetDevice, receiving SINR values, uses the AMC module to convert
   * SINR to CQI. Then, it will send CQI feedback to the eNB.
   *
   */
  NS_LOG_FUNCTION (this << *rxPsd << *noise);

  std::vector<double> sinr, rx, n;

  for (Values::const_iterator it = rxPsd->ConstValuesBegin ();
       it != rxPsd->ConstValuesEnd (); it++ )
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
    }

  for (Values::const_iterator it = noise->ConstValuesBegin ();
       it != noise->ConstValuesEnd (); it++ )
    {
      double noise = (*it);
      noise = 10 * log10 (180000. * noise);
      n.push_back (noise);
    }


  // compute sinr

  int subChannels = rx.size ();
  NS_LOG_INFO (this << "sinr: ");

  for (int i = 0; i < subChannels; i++)
    {
      if (rx.at (i) != 0)
        {
          double sinr_ = rx.at (i) - n.at (i);

          sinr.push_back (sinr_);

          NS_LOG_INFO (this << rx.at (i) << n.at (i) << sinr_);
        }
    }

  // forward computed SINRs to the device that will use the AMC Module to compute CQI feedbacks
  Ptr<UeLtePhy> phy = GetDevice ()->GetObject<UeNetDevice> ()->GetPhy ()->GetObject<UeLtePhy> ();
  phy->CreateCqiFeedbacks (sinr);
}


} // namespace ns3
