/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2006 INRIA
 * All rights reserved.
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

#include "trace-container.h"
#include "stream-tracer.h"
#include <utility>
#include <cassert>

namespace ns3 {

TraceContainer::TraceContainer ()
{}
TraceContainer::~TraceContainer ()
{
	m_uiList.erase (m_uiList.begin (), m_uiList.end ());
	m_siList.erase (m_siList.begin (), m_siList.end ());
	m_fList.erase (m_fList.begin (), m_fList.end ());
}

void 
TraceContainer::setUiVariableCallback (char const *name, Callback<void,uint64_t, uint64_t> callback)
{
	for (UiListI i = m_uiList.begin (); i != m_uiList.end (); i++) {
		if ((*i).second == name) {
			(*i).first->setCallback (callback);
			return;
		}
	}
	assert (false);
}
void 
TraceContainer::setSiVariableCallback (char const *name, Callback<void,int64_t, int64_t> callback)
{
	for (SiListI i = m_siList.begin (); i != m_siList.end (); i++) {
		if ((*i).second == name) {
			(*i).first->setCallback (callback);
			return;
		}
	}
	assert (false);
}
void 
TraceContainer::setFVariableCallback (char const *name, Callback<void,double, double> callback)
{
	assert (false);
}
void 
TraceContainer::setStream (char const *name, std::ostream *os)
{
	for (StreamTracerListI i = m_traceStreamList.begin (); i != m_traceStreamList.end (); i++) {
		if ((*i).second == name) {
			(*i).first->setStream (os);
			return;
		}
	}
	assert (false);
}

void 
TraceContainer::registerUiVariable (char const *name, UiVariableTracerBase *var)
{
	// ensure unicity
	for (UiListI i = m_uiList.begin (); i != m_uiList.end (); i++) {
		if (i->second == name) {
			m_uiList.erase (i);
			break;
		}
	}
	m_uiList.push_back (std::make_pair (var, name));
}
void 
TraceContainer::registerSiVariable (char const *name, SiVariableTracerBase *var)
{
	// ensure unicity
	for (SiListI i = m_siList.begin (); i != m_siList.end (); i++) {
		if (i->second == name) {
			m_siList.erase (i);
			break;
		}
	}
	m_siList.push_back (std::make_pair (var, name));
}
void 
TraceContainer::registerFVariable (char const *name, FVariableTracerBase *var)
{
	assert (false);
}

void 
TraceContainer::registerStream (char const *name, StreamTracer *stream)
{
	// ensure unicity
	for (StreamTracerListI i = m_traceStreamList.begin (); i != m_traceStreamList.end (); i++) {
		if (i->second == name) {
			m_traceStreamList.erase (i);
			break;
		}
	}
	m_traceStreamList.push_back (std::make_pair (stream,name));

}

void 
TraceContainer::registerCallback (char const *name, CallbackTracerBase *tracer)
{
	for (CallbackListI i = m_callbackList.begin (); i != m_callbackList.end (); i++) {
		if (i->second == name) {
			m_callbackList.erase (i);
			break;
		}
	}
	m_callbackList.push_back (std::make_pair (tracer, name));
}




}; // namespace ns3

#include <iostream>
void 
ns3::TraceContainer::printDebug (void)
{
	if (!m_uiList.empty ()) {
		std::cout << "ui var: " << std::endl;
		for (UiListI i = m_uiList.begin (); i != m_uiList.end (); i++) {
			std::cout << "    \"" << (*i).second << "\""<<std::endl;
		}
	}
	if (!m_siList.empty ()) {
		std::cout << "si var: " << std::endl;
		for (SiListI i = m_siList.begin (); i != m_siList.end (); i++) {
			std::cout << "    \"" << (*i).second << "\""<<std::endl;
		}
	}
	if (!m_fList.empty ()) {
		std::cout << "f var: " << std::endl;
		for (FListI i = m_fList.begin (); i != m_fList.end (); i++) {
			std::cout << "    \"" << (*i).second << "\""<<std::endl;
		}
	}
	if (!m_callbackList.empty ()) {
		std::cout << "callback list: "<<std::endl;
		for (CallbackListI i = m_callbackList.begin (); i != m_callbackList.end (); i++) {
			std::cout << "    \"" << i->second << "\""<<std::endl;
		}
	}
}
