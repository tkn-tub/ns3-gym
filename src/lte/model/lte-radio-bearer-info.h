/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 */


#include <ns3/object.h>
#include <ns3/pointer.h>

namespace ns3 {

class LteRlc;
class LtePdcp;

/**
 * store information on active radio bearer instance
 * 
 */
class LteRadioBearerInfo : public Object
{

public:
  LteRadioBearerInfo (void);
  virtual ~LteRadioBearerInfo (void);
  static TypeId GetTypeId (void);

  Ptr<LteRlc> m_rlc;
  Ptr<LtePdcp> m_pdcp;  

};


} // namespace ns3
