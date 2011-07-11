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
 * Author: Jaume Nin <jnin@cttc.es>
 */

#ifndef EPC_HELPER_H_
#define EPC_HELPER_H_

#include "ns3/object.h"

namespace ns3 {

/**
 * Helper class to handle the creation of the EPC entities and protocols
 */
class EpcHelper : public Object
{
public:
  /**
   * Constructor
   */
  EpcHelper ();

  /**
   * Destructor
   */
  virtual ~EpcHelper ();

  /**
   * Inherited from ns3::Object
   */
  static TypeId GetTypeId (void);



private:

};

} // namespace ns3

#endif /* EPC_HELPER_H_ */
