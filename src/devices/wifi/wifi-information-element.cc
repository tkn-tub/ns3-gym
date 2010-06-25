/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Dean Armstrong
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
 * Authors: Dean Armstrong <deanarm@gmail.com>
 */

#include "ns3/wifi-information-element.h"

namespace ns3 {

WifiInformationElement::~WifiInformationElement ()
{}

bool
WifiInformationElement::operator< (WifiInformationElement const & a) const
{
    return (ElementId () < a.ElementId ());
}

bool
WifiInformationElement::operator== (WifiInformationElement const & a) const
{
    if (ElementId () != a.ElementId ())
        return false;

    if (GetInformationSize () != a.GetInformationSize ())
        return false;

    uint32_t ieSize = GetInformationSize ();

    Buffer myIe, hisIe;
    myIe.AddAtEnd (ieSize);
    hisIe.AddAtEnd (ieSize);

    SerializeInformation (myIe.Begin ());
    a.SerializeInformation (hisIe.Begin ());

    return (memcmp(myIe.PeekData (), hisIe.PeekData (), ieSize) == 0);
}

}
