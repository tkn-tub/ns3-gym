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

#ifndef TRACE_CONTAINER_H
#define TRACE_CONTAINER_H

#include "ui-traced-variable.tcc"
#include "si-traced-variable.tcc"
#include "f-traced-variable.tcc"
#include "callback-logger.h"
#include "ns3/callback.h"
#include "packet.h"
#include <list>
#include <string>

namespace ns3 {

class PacketLogger;
class TraceStream;

/**
 * \brief register every source of trace events
 *
 * Model authors use the TraceContainer class to register
 * their trace event sources. Model users use the TraceContainer
 * class to connect their trace event listeners to the
 * model trace event sources.
 *
 * TraceContainer can be used to register the following event sources:
 *   - ns3::PacketLogger : can be connected to ns3::PcapWriter
 *   - ns3::TraceStream : can be connected to any std::ostream
 *   - ns3::CallbackLogger: can be connected to ns3::Callback
 *   - ns3::UiTracedVariable
 *   - ns3::SiTracedVariable
 *   - ns3::FTracedVariable
 *
 * The following sample code shows how you can:
 *   - create trace event sources
 *   - register the trace event sources in a trace container
 *   - set event sinks to each event source
 *  
 * \include samples/main-trace.cc
 */
class TraceContainer {
public:
	TraceContainer ();
	~TraceContainer ();

	/**
	 * \param name the name of the target event source
	 * \param callback the callback being connected to the target event source
	 *
	 * This method targets only event sources which are variables of any unsigned
	 * integer type.
	 */
	void set_ui_variable_callback (char const *name, 
				       Callback<void,uint64_t, uint64_t> callback);
	/**
	 * \param name the name of the target event source
	 * \param callback the callback being connected to the target event source
	 *
	 * This method targets only event sources which are variables of any signed
	 * integer type.
	 */
	void set_si_variable_callback (char const *name, Callback<void,int64_t, int64_t> callback);
	/**
	 * \param name the name of the target event source
	 * \param callback the callback being connected to the target event source
	 *
	 * This method targets only event sources which are variables of any double type.
	 */
	void set_f_variable_callback (char const *name, Callback<void,double, double> callback);
	/**
	 * \param name the name of the target event source
	 * \param callback the callback being connected to the target event source
	 *
	 * This method targets only event sources which are of type PacketLogger.
	 */
	void set_packet_logger_callback (char const *name, Callback<void,Packet const> callback);
	/**
	 * \param name the name of the target event source
	 * \param os the output stream being connected to the source trace stream
	 *
	 * This method targets only event sources which are of type TraceStream.
	 */
	void set_stream (char const *name, std::ostream *os);

	/**
	 * \param name the name of the target event source
	 * \param callback the callback being connected to the target event source.
	 *
	 * This method targets only event sources which are of type CallbackLogger<T1>
	 */
	template <typename T1>
	void set_callback (char const *name, Callback<void,T1> callback);
	/**
	 * \param name the name of the target event source
	 * \param callback the callback being connected to the target event source.
	 *
	 * This method targets only event sources which are of type CallbackLogger<T1,T2>
	 */
	template <typename T1, typename T2>
	void set_callback (char const *name, Callback<void,T1,T2> callback);
	/**
	 * \param name the name of the target event source
	 * \param callback the callback being connected to the target event source.
	 *
	 * This method targets only event sources which are of type CallbackLogger<T1,T2,T3>
	 */
	template <typename T1, typename T2, typename T3>
	void set_callback (char const *name, Callback<void,T1,T2,T3> callback);
	/**
	 * \param name the name of the target event source
	 * \param callback the callback being connected to the target event source.
	 *
	 * This method targets only event sources which are of type CallbackLogger<T1,T2,T3,T4>
	 */
	template <typename T1, typename T2, typename T3, typename T4>
	void set_callback (char const *name, Callback<void,T1,T2,T3,T4> callback);
	/**
	 * \param name the name of the target event source
	 * \param callback the callback being connected to the target event source.
	 *
	 * This method targets only event sources which are of type CallbackLogger<T1,T2,T3,T4,T5>
	 */
	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void set_callback (char const *name, Callback<void,T1,T2,T3,T4,T5> callback);

	/**
	 * \param name the name of the registered event source
	 * \param var the event source being registered
	 *
	 * This method registers only event sources of type "unsigned integer".
	 */
	void register_ui_variable (char const *name, UiTracedVariableBase *var);
	/**
	 * \param name the name of the registered event source
	 * \param var the event source being registered
	 *
	 * This method registers only event sources of type "signed integer".
	 */
	void register_si_variable (char const *name, SiTracedVariableBase *var);
	/**
	 * \param name the name of the registered event source
	 * \param var the event source being registered
	 *
	 * This method registers only event sources of type "double".
	 */
	void register_f_variable (char const *name, FTracedVariableBase *var);
	/**
	 * \param name the name of the registered event source
	 * \param logger the event source being registered
	 *
	 * This method registers only event sources of type PacketLogger.
	 */
	void register_packet_logger (char const *name, PacketLogger *logger);
	/**
	 * \param name the name of the registered event source
	 * \param stream the event source being registered
	 *
	 * This method registers only event sources of type TraceStream.
	 */
	void register_stream (char const *name, TraceStream *stream);

	/**
	 * \param name the name of the registeref event source
	 * \param logger the callback logger being registered.
	 *
	 * This method registers only event sources of type CallbackLogger
	 */
	void register_callback (char const *name, CallbackLoggerBase*logger);

	/**
	 * Print the list of registered event sources in this container only.
	 */
	void print_debug (void);
private:
	typedef std::list<std::pair<UiTracedVariableBase *, std::string> > UiList;
	typedef std::list<std::pair<UiTracedVariableBase *, std::string> >::iterator UiListI;
	typedef std::list<std::pair<SiTracedVariableBase *, std::string> > SiList;
	typedef std::list<std::pair<SiTracedVariableBase *, std::string> >::iterator SiListI;
	typedef std::list<std::pair<FTracedVariableBase *, std::string> > FList;
	typedef std::list<std::pair<FTracedVariableBase *, std::string> >::iterator FListI;
	typedef std::list<std::pair<PacketLogger *, std::string> > PacketLoggerList;
	typedef std::list<std::pair<PacketLogger *, std::string> >::iterator PacketLoggerListI;
	typedef std::list<std::pair<TraceStream *, std::string> > TraceStreamList;
	typedef std::list<std::pair<TraceStream *, std::string> >::iterator TraceStreamListI;
	typedef std::list<std::pair<CallbackLoggerBase *, std::string> > CallbackList;
	typedef std::list<std::pair<CallbackLoggerBase *, std::string> >::iterator CallbackListI;

	UiList m_ui_list;
	SiList m_si_list;
	FList m_f_list;
	PacketLoggerList m_packet_logger_list;
	TraceStreamList m_trace_stream_list;
	CallbackList m_callback_list;
};

}; // namespace ns3

#ifndef NDEBUG
#include <cassert>
#endif

namespace ns3 {

template <typename T1>
void 
TraceContainer::set_callback (char const *name, Callback<void,T1> callback)
{
	for (CallbackListI i = m_callback_list.begin (); i != m_callback_list.end (); i++) {
		if (i->second == name) {
			static_cast<CallbackLogger<T1> *> (i->first)->set_callback (callback);
			return;
		}
	}
#ifndef NDEBUG
	assert (false);
#endif
}
template <typename T1, typename T2>
void 
TraceContainer::set_callback (char const *name, Callback<void,T1,T2> callback)
{
	for (CallbackListI i = m_callback_list.begin (); i != m_callback_list.end (); i++) {
		if (i->second == name) {
			static_cast<CallbackLogger<T1,T2> *> (i->first)->set_callback (callback);
			return;
		}
	}
#ifndef NDEBUG
	assert (false);
#endif
}
template <typename T1, typename T2, typename T3>
void 
TraceContainer::set_callback (char const *name, Callback<void,T1,T2,T3> callback)
{
	for (CallbackListI i = m_callback_list.begin (); i != m_callback_list.end (); i++) {
		if (i->second == name) {
			static_cast<CallbackLogger<T1,T2,T3> *> (i->first)->set_callback (callback);
			return;
		}
	}
#ifndef NDEBUG
	assert (false);
#endif
}
template <typename T1, typename T2, typename T3, typename T4>
void 
TraceContainer::set_callback (char const *name, Callback<void,T1,T2,T3,T4> callback)
{
	for (CallbackListI i = m_callback_list.begin (); i != m_callback_list.end (); i++) {
		if (i->second == name) {
			static_cast<CallbackLogger<T1,T2,T3,T4> *> (i->first)->set_callback (callback);
			return;
		}
	}
#ifndef NDEBUG
	assert (false);
#endif
}
template <typename T1, typename T2, typename T3, typename T4, typename T5>
void 
TraceContainer::set_callback (char const *name, Callback<void,T1,T2,T3,T4,T5> callback)
{
	for (CallbackListI i = m_callback_list.begin (); i != m_callback_list.end (); i++) {
		if (i->second == name) {
			static_cast<CallbackLogger<T1,T2,T3,T4,T5> *> (i->first)->set_callback (callback);
			return;
		}
	}
#ifndef NDEBUG
	assert (false);
#endif
}


}; // namespace ns3

#endif /* TRACED_VARIABLE_CONTAINER_H */
