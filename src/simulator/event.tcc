/* -*-	Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
 * Copyright (c) 2005 INRIA
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

#ifndef EVENT_TCC
#define EVENT_TCC

#include "event.h"
#include "event-impl.h"

/**
 * yans namespace
 */
namespace yans {

/**
 * \defgroup make_event make_event
 *
 * Every make_event template function returns a newly-created Event
 * which holds a pointer to a special subclass of the EventImpl 
 * base class. Each of these subclasses holds information about which
 * function or method to call and which parameters must be forwarded
 * to this function or method.
 *
 * Sample code is shown below:
 * \include samples/main-event.cc
 */

template<typename T>
class EventMemberImpl0 : public EventImpl {
public:
	typedef void (T::*F)(void);

	EventMemberImpl0 (T *obj, F function) 
		: m_obj (obj), 
		  m_function (function)
	{}
	virtual ~EventMemberImpl0 () {}
private:
	virtual void notify (void) { 
		(m_obj->*m_function) (); 
	}
	T* m_obj;
	F m_function;
};

template<typename T, typename T1>
class EventMemberImpl1 : public EventImpl {
public:
	typedef void (T::*F)(T1);

	EventMemberImpl1 (T *obj, F function, T1 a1) 
		: m_obj (obj), 
		  m_function (function),
		  m_a1 (a1)
	{ }
	virtual ~EventMemberImpl1 () {}
private:
	virtual void notify (void) { 
		(m_obj->*m_function) (m_a1);
	}
	T* m_obj;
	F m_function;
	T1 m_a1;
};

template<typename T, typename T1, typename T2>
class EventMemberImpl2 : public EventImpl {
public:
	typedef void (T::*F)(T1, T2);

	EventMemberImpl2 (T *obj, F function, T1 a1, T2 a2) 
		: m_obj (obj), 
		  m_function (function),
		  m_a1 (a1),
		  m_a2 (a2)
	{ }
	virtual ~EventMemberImpl2 () {}
private:
	virtual void notify (void) { 
		(m_obj->*m_function) (m_a1, m_a2);
	}
	T* m_obj;
	F m_function;
	T1 m_a1;
	T2 m_a2;
};

template<typename T, typename T1, typename T2, typename T3>
class EventMemberImpl3 : public EventImpl {
public:
	typedef void (T::*F)(T1, T2, T3);

	EventMemberImpl3 (T *obj, F function, T1 a1, T2 a2, T3 a3) 
		: m_obj (obj), 
		  m_function (function),
		  m_a1 (a1),
		  m_a2 (a2),
		  m_a3 (a3)
	{ }
	virtual ~EventMemberImpl3 () {}
private:
	virtual void notify (void) { 
		(m_obj->*m_function) (m_a1, m_a2, m_a3);
	}
	T* m_obj;
	F m_function;
	T1 m_a1;
	T2 m_a2;
	T3 m_a3;
};

template<typename T, typename T1, typename T2, typename T3, typename T4>
class EventMemberImpl4 : public EventImpl {
public:
	typedef void (T::*F)(T1, T2, T3, T4);

	EventMemberImpl4 (T *obj, F function, T1 a1, T2 a2, T3 a3, T4 a4) 
		: m_obj (obj), 
		  m_function (function),
		  m_a1 (a1),
		  m_a2 (a2),
		  m_a3 (a3),
		  m_a4 (a4)
	{ }
	virtual ~EventMemberImpl4 () {}
private:
	virtual void notify (void) { 
		(m_obj->*m_function) (m_a1, m_a2, m_a3, m_a4);
	}
	T* m_obj;
	F m_function;
	T1 m_a1;
	T2 m_a2;
	T3 m_a3;
	T4 m_a4;
};

template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
class EventMemberImpl5 : public EventImpl {
public:
	typedef void (T::*F)(T1, T2, T3, T4, T5);

	EventMemberImpl5 (T *obj, F function, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
		: m_obj (obj), 
		  m_function (function),
		  m_a1 (a1),
		  m_a2 (a2),
		  m_a3 (a3),
		  m_a4 (a4),
		  m_a5 (a5)
	{ }
	virtual ~EventMemberImpl5 () {}
private:
	virtual void notify (void) { 
		(m_obj->*m_function) (m_a1, m_a2, m_a3, m_a4, m_a5);
	}
	T* m_obj;
	F m_function;
	T1 m_a1;
	T2 m_a2;
	T3 m_a3;
	T4 m_a4;
	T5 m_a5;
};

/**
 * \ingroup make_event
 * \param f class method member pointer
 * \param t class instance
 * \return a wrapper Event
 * Build Events for class method members which take no arguments.
 */
template<typename T>
Event make_event(void (T::*f) (void), T* t) {
	return Event (new EventMemberImpl0<T>(t, f));
}
/**
 * \ingroup make_event
 * \param f class method member pointer
 * \param t class instance
 * \param a1 first argument to pass to the target method when the event expires
 * \return a wrapper Event
 * Build Events for class method members which take only one argument
 */
template<typename T, typename T1>
Event make_event(void (T::*f) (T1), T* t, T1 a1) {
	return Event (new EventMemberImpl1<T, T1>(t, f, a1));
}
/**
 * \ingroup make_event
 * \param f class method member pointer
 * \param t class instance
 * \param a1 first argument to pass to the target method when the event expires
 * \param a2 second argument to pass to the target method when the event expires
 * \return a wrapper Event
 * Build Events for class method members which take two arguments
 */
template<typename T, typename T1, typename T2>
Event make_event(void (T::*f) (T1, T2), T* t, T1 a1, T2 a2) {
	return Event (new EventMemberImpl2<T, T1, T2>(t, f, a1, a2));
}
/**
 * \ingroup make_event
 * \param f class method member pointer
 * \param t class instance
 * \param a1 first argument to pass to the target method when the event expires
 * \param a2 second argument to pass to the target method when the event expires
 * \param a3 third argument to pass to the target method when the event expires
 * \return a wrapper Event
 * Build Events for class method members which take three arguments
 */
template<typename T, typename T1, typename T2, typename T3>
Event make_event(void (T::*f) (T1, T2, T3), T* t, T1 a1, T2 a2, T3 a3) {
	return Event (new EventMemberImpl3<T, T1, T2, T3>(t, f, a1, a2, a3));
}
/**
 * \ingroup make_event
 * \param f class method member pointer
 * \param t class instance
 * \param a1 first argument to pass to the target method when the event expires
 * \param a2 second argument to pass to the target method when the event expires
 * \param a3 third argument to pass to the target method when the event expires
 * \param a4 fourth argument to pass to the target method when the event expires
 * \return a wrapper Event
 * Build Events for class method members which take four arguments
 */
template<typename T, typename T1, typename T2, typename T3, typename T4>
Event make_event(void (T::*f) (T1, T2, T3, T4), T* t, T1 a1, T2 a2, T3 a3, T4 a4) {
	return Event (new EventMemberImpl4<T, T1, T2, T3, T4>(t, f, a1, a2, a3, a4));
}
/**
 * \ingroup make_event
 * \param f class method member pointer
 * \param t class instance
 * \param a1 first argument to pass to the target method when the event expires
 * \param a2 second argument to pass to the target method when the event expires
 * \param a3 third argument to pass to the target method when the event expires
 * \param a4 fourth argument to pass to the target method when the event expires
 * \param a5 fifth argument to pass to the target method when the event expires
 * \return a wrapper Event
 * Build Events for class method members which take five arguments.
 */
template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
Event make_event(void (T::*f) (T1, T2, T3, T4, T5), T* t, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
	return Event (new EventMemberImpl5<T, T1, T2, T3, T4, T5>(t, f, a1, a2, a3, a4, a5));
}

template<typename T1>
class EventFunctionImpl1 : public EventImpl {
public:
	typedef void (*F)(T1);

	EventFunctionImpl1 (F function, T1 a1) 
		: m_function (function),
		  m_a1 (a1)
	{ }
	virtual ~EventFunctionImpl1 () {}
private:
	virtual void notify (void) { 
		(*m_function) (m_a1);
	}
	F m_function;
	T1 m_a1;
};

template<typename T1, typename T2>
class EventFunctionImpl2 : public EventImpl {
public:
	typedef void (*F)(T1, T2);

	EventFunctionImpl2 (F function, T1 a1, T2 a2) 
		: m_function (function),
		  m_a1 (a1),
		  m_a2 (a2)
	{ }
	virtual ~EventFunctionImpl2 () {}
private:
	virtual void notify (void) { 
		(*m_function) (m_a1, m_a2);
	}
	F m_function;
	T1 m_a1;
	T2 m_a2;
};

template<typename T1, typename T2, typename T3>
class EventFunctionImpl3 : public EventImpl {
public:
	typedef void (*F)(T1, T2, T3);

	EventFunctionImpl3 (F function, T1 a1, T2 a2, T3 a3) 
		: m_function (function),
		  m_a1 (a1),
		  m_a2 (a2),
		  m_a3 (a3)
	{ }
	virtual ~EventFunctionImpl3 () {}
private:
	virtual void notify (void) { 
		(*m_function) (m_a1, m_a2, m_a3);
	}
	F m_function;
	T1 m_a1;
	T2 m_a2;
	T3 m_a3;
};

template<typename T1, typename T2, typename T3, typename T4>
class EventFunctionImpl4 : public EventImpl {
public:
	typedef void (*F)(T1, T2, T3, T4);

	EventFunctionImpl4 (F function, T1 a1, T2 a2, T3 a3, T4 a4) 
		: m_function (function),
		  m_a1 (a1),
		  m_a2 (a2),
		  m_a3 (a3),
		  m_a4 (a4)
	{ }
	virtual ~EventFunctionImpl4 () {}
private:
	virtual void notify (void) { 
		(*m_function) (m_a1, m_a2, m_a3, m_a4);
	}
	F m_function;
	T1 m_a1;
	T2 m_a2;
	T3 m_a3;
	T4 m_a4;
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
class EventFunctionImpl5 : public EventImpl {
public:
	typedef void (*F)(T1, T2, T3, T4, T5);

	EventFunctionImpl5 (F function, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) 
		: m_function (function),
		  m_a1 (a1),
		  m_a2 (a2),
		  m_a3 (a3),
		  m_a4 (a4),
		  m_a5 (a5)
	{ }
	virtual ~EventFunctionImpl5 () {}
private:
	virtual void notify (void) { 
		(*m_function) (m_a1, m_a2, m_a3, m_a4, m_a5);
	}
	F m_function;
	T1 m_a1;
	T2 m_a2;
	T3 m_a3;
	T4 m_a4;
	T5 m_a5;
};


/**
 * \ingroup make_event
 * \param f function pointer
 * \return a wrapper Event
 * Build Events for function pointers which take no arguments
 */
Event make_event(void (*f) (void));

/**
 * \ingroup make_event
 * \param f function pointer
 * \param a1 first argument to pass to the target function when the event expires
 * \return a wrapper Event
 * Build Events for function pointers which take one argument
 */
template<typename T1>
Event make_event(void (*f) (T1), T1 a1) {
	return Event (new EventFunctionImpl1<T1>(f, a1));
}
/**
 * \ingroup make_event
 * \param f function pointer
 * \param a1 first argument to pass to the target function when the event expires
 * \param a2 second argument to pass to the target function when the event expires
 * \return a wrapper Event
 * Build Events for function pointers which take two argument
 */
template<typename T1, typename T2>
Event make_event(void (*f) (T1, T2), T1 a1, T2 a2) {
	return Event (new EventFunctionImpl2<T1, T2>(f, a1, a2));
}
/**
 * \ingroup make_event
 * \param f function pointer
 * \param a1 first argument to pass to the target function when the event expires
 * \param a2 second argument to pass to the target function when the event expires
 * \param a3 third argument to pass to the target function when the event expires
 * \return a wrapper Event
 * Build Events for function pointers which take three argument
 */
template<typename T1, typename T2, typename T3>
Event make_event(void (*f) (T1, T2, T3), T1 a1, T2 a2, T3 a3) {
	return Event (new EventFunctionImpl3<T1, T2, T3>(f, a1, a2, a3));
}
/**
 * \ingroup make_event
 * \param f function pointer
 * \param a1 first argument to pass to the target function when the event expires
 * \param a2 second argument to pass to the target function when the event expires
 * \param a3 third argument to pass to the target function when the event expires
 * \param a4 fourth argument to pass to the target function when the event expires
 * \return a wrapper Event
 * Build Events for function pointers which take four argument
 */
template<typename T1, typename T2, typename T3, typename T4>
Event make_event(void (*f) (T1, T2, T3, T4), T1 a1, T2 a2, T3 a3, T4 a4) {
	return Event (new EventFunctionImpl4<T1, T2, T3, T4>(f, a1, a2, a3, a4));
}
/**
 * \ingroup make_event
 * \param f function pointer
 * \param a1 first argument to pass to the target function when the event expires
 * \param a2 second argument to pass to the target function when the event expires
 * \param a3 third argument to pass to the target function when the event expires
 * \param a4 fourth argument to pass to the target function when the event expires
 * \param a5 fifth argument to pass to the target function when the event expires
 * \return a wrapper Event
 * Build Events for function pointers which take five argument
 */
template<typename T1, typename T2, typename T3, typename T4, typename T5>
Event make_event(void (*f) (T1, T2, T3, T4, T5), T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
	return Event (new EventFunctionImpl5<T1, T2, T3, T4, T5>(f, a1, a2, a3, a4, a5));
}


}; // namespace yans

#endif /* EVENT_TCC */
