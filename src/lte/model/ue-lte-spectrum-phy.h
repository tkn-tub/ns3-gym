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

#ifndef UE_LTE_SPECTRUM_PHY_H
#define UE_LTE_SPECTRUM_PHY_H


#include "lte-spectrum-phy.h"

namespace ns3 {

class LteNetDevice;
class EnbNetDevice;

/**
 * \ingroup lte
 *
 * The UeLteSpectrumPhy models the UL/DL physical layer for the UE
 */
class UeLteSpectrumPhy : public LteSpectrumPhy
{

public:
  UeLteSpectrumPhy ();
  virtual ~UeLteSpectrumPhy ();

  static TypeId GetTypeId (void);

  void CalcSinrValues (Ptr <const SpectrumValue> rxPsd, Ptr <const SpectrumValue> noise);

private:
};






}

#endif /* UE_LTE_SPECTRUM_PHY_H */
