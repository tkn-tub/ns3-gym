/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 CTTC
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 * Modified by Marco Miozzo <mmiozzo@cttc.es> (add data and ctrl diversity)
 */

#include <ns3/log.h>
#include <ns3/packet-burst.h>
#include <ns3/ptr.h>
#include <ns3/lte-spectrum-signal-parameters.h>
#include <ns3/lte-control-messages.h>


NS_LOG_COMPONENT_DEFINE ("LteSpectrumSignalParameters");

namespace ns3 {

LteSpectrumSignalParameters::LteSpectrumSignalParameters ()
{
  NS_LOG_FUNCTION (this);
}

LteSpectrumSignalParameters::LteSpectrumSignalParameters (const LteSpectrumSignalParameters& p)
  : SpectrumSignalParameters (p)
{
  NS_LOG_FUNCTION (this << &p);
  packetBurst = p.packetBurst->Copy ();
}

Ptr<SpectrumSignalParameters>
LteSpectrumSignalParameters::Copy ()
{
  NS_LOG_FUNCTION (this);
  // Ideally we would use:
  //   return Copy<LteSpectrumSignalParameters> (*this);
  // but for some reason it doesn't work. Another alternative is 
  //   return Copy<LteSpectrumSignalParameters> (this);
  // but it causes a double creation of the object, hence it is less efficient.
  // The solution below is copied from the implementation of Copy<> (Ptr<>) in ptr.h
  Ptr<LteSpectrumSignalParameters> lssp (new LteSpectrumSignalParameters (*this), false);  
  return lssp;
}



LteSpectrumSignalParametersDataFrame::LteSpectrumSignalParametersDataFrame ()
{
  NS_LOG_FUNCTION (this);
}

LteSpectrumSignalParametersDataFrame::LteSpectrumSignalParametersDataFrame (const LteSpectrumSignalParametersDataFrame& p)
: SpectrumSignalParameters (p)
{
  NS_LOG_FUNCTION (this << &p);
  cellId = p.cellId;
  if (p.packetBurst)
    {
      packetBurst = p.packetBurst->Copy ();
    }
  ctrlMsgList = p.ctrlMsgList;
}

Ptr<SpectrumSignalParameters>
LteSpectrumSignalParametersDataFrame::Copy ()
{
  NS_LOG_FUNCTION (this);
  // Ideally we would use:
  //   return Copy<LteSpectrumSignalParametersDataFrame> (*this);
  // but for some reason it doesn't work. Another alternative is 
  //   return Copy<LteSpectrumSignalParametersDataFrame> (this);
  // but it causes a double creation of the object, hence it is less efficient.
  // The solution below is copied from the implementation of Copy<> (Ptr<>) in ptr.h
  Ptr<LteSpectrumSignalParametersDataFrame> lssp (new LteSpectrumSignalParametersDataFrame (*this), false);  
  return lssp;
}



LteSpectrumSignalParametersDlCtrlFrame::LteSpectrumSignalParametersDlCtrlFrame ()
{
  NS_LOG_FUNCTION (this);
}

LteSpectrumSignalParametersDlCtrlFrame::LteSpectrumSignalParametersDlCtrlFrame (const LteSpectrumSignalParametersDlCtrlFrame& p)
: SpectrumSignalParameters (p)
{
  NS_LOG_FUNCTION (this << &p);
  cellId = p.cellId;
  ctrlMsgList = p.ctrlMsgList;
}

Ptr<SpectrumSignalParameters>
LteSpectrumSignalParametersDlCtrlFrame::Copy ()
{
  NS_LOG_FUNCTION (this);
  // Ideally we would use:
  //   return Copy<LteSpectrumSignalParametersDlCtrlFrame> (*this);
  // but for some reason it doesn't work. Another alternative is 
  //   return Copy<LteSpectrumSignalParametersDlCtrlFrame> (this);
  // but it causes a double creation of the object, hence it is less efficient.
  // The solution below is copied from the implementation of Copy<> (Ptr<>) in ptr.h
  Ptr<LteSpectrumSignalParametersDlCtrlFrame> lssp (new LteSpectrumSignalParametersDlCtrlFrame (*this), false);  
  return lssp;
}


LteSpectrumSignalParametersUlSrsFrame::LteSpectrumSignalParametersUlSrsFrame ()
{
  NS_LOG_FUNCTION (this);
}

LteSpectrumSignalParametersUlSrsFrame::LteSpectrumSignalParametersUlSrsFrame (const LteSpectrumSignalParametersUlSrsFrame& p)
: SpectrumSignalParameters (p)
{
  NS_LOG_FUNCTION (this << &p);
  cellId = p.cellId;
}

Ptr<SpectrumSignalParameters>
LteSpectrumSignalParametersUlSrsFrame::Copy ()
{
  NS_LOG_FUNCTION (this);
  // Ideally we would use:
  //   return Copy<LteSpectrumSignalParametersUlSrsFrame> (*this);
  // but for some reason it doesn't work. Another alternative is 
  //   return Copy<LteSpectrumSignalParametersUlSrsFrame> (this);
  // but it causes a double creation of the object, hence it is less efficient.
  // The solution below is copied from the implementation of Copy<> (Ptr<>) in ptr.h
  Ptr<LteSpectrumSignalParametersUlSrsFrame> lssp (new LteSpectrumSignalParametersUlSrsFrame (*this), false);  
  return lssp;
}







} // namespace ns3
