/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
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

#include "ui-variable-tracer.h"
#include "si-variable-tracer.h"
#include "f-variable-tracer.h"
#include "callback-tracer.h"
#include "ns3/callback.h"
#include <list>
#include <string>

namespace ns3 {

class StreamTracer;

/**
 * \brief register every source of trace events
 *
 * Model authors use the TraceContainer class to register
 * their trace event sources. Model users use the TraceContainer
 * class to connect their trace event listeners to the
 * model trace event sources.
 *
 * TraceContainer can be used to register the following event sources:
 *   - ns3::StreamTracer : can be connected to any std::ostream
 *   - ns3::CallbackTracer: can be connected to any ns3::Callback
 *   - ns3::UiVariableTracer
 *   - ns3::SiVariableTracer
 *   - ns3::FVariableTracer
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
    void setUiVariableCallback (char const *name, 
                       Callback<void,uint64_t, uint64_t> callback);
    /**
     * \param name the name of the target event source
     * \param callback the callback being connected to the target event source
     *
     * This method targets only event sources which are variables of any signed
     * integer type.
     */
    void setSiVariableCallback (char const *name, Callback<void,int64_t, int64_t> callback);
    /**
     * \param name the name of the target event source
     * \param callback the callback being connected to the target event source
     *
     * This method targets only event sources which are variables of any double type.
     */
    void setFVariableCallback (char const *name, Callback<void,double, double> callback);
    /**
     * \param name the name of the target event source
     * \param os the output stream being connected to the source trace stream
     *
     * This method targets only event sources which are of type StreamTracer.
     */
    void setStream (char const *name, std::ostream *os);

    /**
     * \param name the name of the target event source
     * \param callback the callback being connected to the target event source.
     *
     * This method targets only event sources which are of type CallbackTracer<T1>
     */
    template <typename T1>
    void setCallback (char const *name, Callback<void,T1> callback);
    /**
     * \param name the name of the target event source
     * \param callback the callback being connected to the target event source.
     *
     * This method targets only event sources which are of type CallbackTracer<T1,T2>
     */
    template <typename T1, typename T2>
    void setCallback (char const *name, Callback<void,T1,T2> callback);
    /**
     * \param name the name of the target event source
     * \param callback the callback being connected to the target event source.
     *
     * This method targets only event sources which are of type CallbackTracer<T1,T2,T3>
     */
    template <typename T1, typename T2, typename T3>
    void setCallback (char const *name, Callback<void,T1,T2,T3> callback);
    /**
     * \param name the name of the target event source
     * \param callback the callback being connected to the target event source.
     *
     * This method targets only event sources which are of type CallbackTracer<T1,T2,T3,T4>
     */
    template <typename T1, typename T2, typename T3, typename T4>
    void setCallback (char const *name, Callback<void,T1,T2,T3,T4> callback);
    /**
     * \param name the name of the target event source
     * \param callback the callback being connected to the target event source.
     *
     * This method targets only event sources which are of type CallbackTracer<T1,T2,T3,T4,T5>
     */
    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    void setCallback (char const *name, Callback<void,T1,T2,T3,T4,T5> callback);

    /**
     * \param name the name of the registered event source
     * \param var the event source being registered
     *
     * This method registers only event sources of type "unsigned integer".
     */
    void registerUiVariable (char const *name, UiVariableTracerBase *var);
    /**
     * \param name the name of the registered event source
     * \param var the event source being registered
     *
     * This method registers only event sources of type "signed integer".
     */
    void registerSiVariable (char const *name, SiVariableTracerBase *var);
    /**
     * \param name the name of the registered event source
     * \param var the event source being registered
     *
     * This method registers only event sources of type "double".
     */
    void registerFVariable (char const *name, FVariableTracerBase *var);
    /**
     * \param name the name of the registered event source
     * \param stream the event source being registered
     *
     * This method registers only event sources of type StreamTracer.
     */
    void registerStream (char const *name, StreamTracer *stream);

    /**
     * \param name the name of the registeref event source
     * \param tracer the callback tracer being registered.
     *
     * This method registers only event sources of type CallbackTracer
     */
    void registerCallback (char const *name, CallbackTracerBase*tracer);

    /**
     * Print the list of registered event sources in this container only.
     */
    void printDebug (void);
private:
    typedef std::list<std::pair<UiVariableTracerBase *, std::string> > UiList;
    typedef std::list<std::pair<UiVariableTracerBase *, std::string> >::iterator UiListI;
    typedef std::list<std::pair<SiVariableTracerBase *, std::string> > SiList;
    typedef std::list<std::pair<SiVariableTracerBase *, std::string> >::iterator SiListI;
    typedef std::list<std::pair<FVariableTracerBase *, std::string> > FList;
    typedef std::list<std::pair<FVariableTracerBase *, std::string> >::iterator FListI;
    typedef std::list<std::pair<StreamTracer *, std::string> > StreamTracerList;
    typedef std::list<std::pair<StreamTracer *, std::string> >::iterator StreamTracerListI;
    typedef std::list<std::pair<CallbackTracerBase *, std::string> > CallbackList;
    typedef std::list<std::pair<CallbackTracerBase *, std::string> >::iterator CallbackListI;

    UiList m_uiList;
    SiList m_siList;
    FList m_fList;
    StreamTracerList m_traceStreamList;
    CallbackList m_callbackList;
};

}; // namespace ns3

#ifndef NDEBUG
#include <cassert>
#endif

namespace ns3 {

template <typename T1>
void 
TraceContainer::setCallback (char const *name, Callback<void,T1> callback)
{
    for (CallbackListI i = m_callbackList.begin (); i != m_callbackList.end (); i++) {
        if (i->second == name) {
            static_cast<CallbackTracer<T1> *> (i->first)->setCallback (callback);
            return;
        }
    }
#ifndef NDEBUG
    assert (false);
#endif
}
template <typename T1, typename T2>
void 
TraceContainer::setCallback (char const *name, Callback<void,T1,T2> callback)
{
    for (CallbackListI i = m_callbackList.begin (); i != m_callbackList.end (); i++) {
        if (i->second == name) {
            static_cast<CallbackTracer<T1,T2> *> (i->first)->setCallback (callback);
            return;
        }
    }
#ifndef NDEBUG
    assert (false);
#endif
}
template <typename T1, typename T2, typename T3>
void 
TraceContainer::setCallback (char const *name, Callback<void,T1,T2,T3> callback)
{
    for (CallbackListI i = m_callbackList.begin (); i != m_callbackList.end (); i++) {
        if (i->second == name) {
            static_cast<CallbackTracer<T1,T2,T3> *> (i->first)->setCallback (callback);
            return;
        }
    }
#ifndef NDEBUG
    assert (false);
#endif
}
template <typename T1, typename T2, typename T3, typename T4>
void 
TraceContainer::setCallback (char const *name, Callback<void,T1,T2,T3,T4> callback)
{
    for (CallbackListI i = m_callbackList.begin (); i != m_callbackList.end (); i++) {
        if (i->second == name) {
            static_cast<CallbackTracer<T1,T2,T3,T4> *> (i->first)->setCallback (callback);
            return;
        }
    }
#ifndef NDEBUG
    assert (false);
#endif
}
template <typename T1, typename T2, typename T3, typename T4, typename T5>
void 
TraceContainer::setCallback (char const *name, Callback<void,T1,T2,T3,T4,T5> callback)
{
    for (CallbackListI i = m_callbackList.begin (); i != m_callbackList.end (); i++) {
        if (i->second == name) {
            static_cast<CallbackTracer<T1,T2,T3,T4,T5> *> (i->first)->setCallback (callback);
            return;
        }
    }
#ifndef NDEBUG
    assert (false);
#endif
}


}; // namespace ns3

#endif /* TRACED_VARIABLE_CONTAINER_H */
