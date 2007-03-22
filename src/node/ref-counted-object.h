// -*- Mode:NS3 -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
// Based on original class declaration by Mathieu Lacage
//

#ifndef __REF_COUNTED_OBJECT__
#define __REF_COUNTED_OBJECT__

class RefCountedObject
{
public:
  RefCountedObject() : m_count(1) {}
  virtual ~RefCountedObject() {}
  
  void Ref()
  {
    m_count++;
  }

  void Unref()
  {
    if (!--m_count) delete this;  // Refcount to zero, delete
  }
private:
  uint32_t m_count;
};
#endif

