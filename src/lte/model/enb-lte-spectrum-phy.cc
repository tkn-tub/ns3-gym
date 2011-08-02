/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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
#include "enb-lte-spectrum-phy.h"
#include "lte-net-device.h"
#include "lte-spectrum-value-helper.h"

NS_LOG_COMPONENT_DEFINE ("EnbLteSpectrumPhy");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (EnbLteSpectrumPhy);

EnbLteSpectrumPhy::EnbLteSpectrumPhy ()
{
  SetMobility (0);
  SetDevice (0);
  SetChannel (0);
  SetState (LteSpectrumPhy::IDLE);
  //GetSpectrumInterference ()->SetErrorModel (CreateObject<LteSpectrumErrorModel> ());

  LteSpectrumValueHelper psdHelper;
  Ptr<SpectrumValue> noisePsd = psdHelper.CreateUplinkNoisePowerSpectralDensity ();
  SetNoisePowerSpectralDensity (noisePsd);
}


EnbLteSpectrumPhy::~EnbLteSpectrumPhy ()
{
}


TypeId
EnbLteSpectrumPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EnbLteSpectrumPhy")
    .SetParent<LteSpectrumPhy> ()
    .AddConstructor<EnbLteSpectrumPhy> ()
  ;
  return tid;
}




void
EnbLteSpectrumPhy::CalcSinrValues (Ptr <const SpectrumValue> rxPsd,Ptr <const SpectrumValue> noise)
{
  NS_LOG_FUNCTION (this << rxPsd << noise);
  /*
   * TO DO:
   * Compute the SINR of the incoming signal, using the rxPsd and the Noise Psd.
   * Transfer this value to the device that will compute the CQI valuei or the MCS.
   *
   * Downlink:
   * the UE receives the signal from the eNB. It computes the SINR and tranfers
   * it to the UeNetDevice. The UeNetDevice, receiving SINR values, uses the AMC module to convert
   * SINR to CQI. Then, it will send CQI feedback to the eNB.
   *
   * Uplink:
   * when the eNB receives the signal from a UE, it must computes the quality of channel
   * for this particular uplink connection. So, the eNB computes the SINR and transfers it to the
   * net device.
   */
}


} // namespace ns3
