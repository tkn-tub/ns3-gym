/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
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
#include "composite-trace-resolver.h"

namespace ns3 {

CompositeTraceResolver::CompositeTraceResolver (TraceContext const &context)
  : TraceResolver (context)
{}

CompositeTraceResolver::~CompositeTraceResolver ()
{}

void 
CompositeTraceResolver::DoAdd (std::string name, 
			       Callback<TraceResolver *,TraceContext const &> createResolver,
			       TraceContext const &context)
{
  struct CallbackTraceSourceItem item;
  item.name = name;
  item.createResolver = createResolver;
  item.context = context;
  m_items.push_back (item);
}

TraceResolver::TraceResolverList 
CompositeTraceResolver::DoLookup (std::string id) const
{
  if (id == "*")
    {
      TraceResolver::TraceResolverList list;
      for (TraceItems::const_iterator i = m_items.begin (); i != m_items.end (); i++)
	{
	  list.push_back (i->createResolver (i->context));
	}
      return list;
    }
  std::string::size_type start, end;
  start = id.find_first_of ("(", 0);
  end = id.find_first_of (")", 0);
  if (start != 0 || end != (id.size ()-1))
    {
      for (TraceItems::const_iterator i = m_items.begin (); i != m_items.end (); i++)
	{
	  if (i->name == id)
	    {
	      TraceResolver::TraceResolverList list;
	      list.push_back (i->createResolver (i->context));
	      return list;
	    }
	}
    }
  std::list<std::string> names;
  std::string alternatives = std::string (id, start+1, end-1);
  std::string::size_type next, cur;
  next = 0;
  cur = 0;
  while (true)
    {
      std::string element;
      next = alternatives.find ("|", cur);
      if (next == std::string::npos)
	{
	  element = std::string (alternatives, cur, alternatives.size ());
	  names.push_back (element);
	  break;
	}
      element = std::string (alternatives, cur, next);
      names.push_back (element);
      cur = next + 1;
    }
  TraceResolver::TraceResolverList list;
  for (std::list<std::string>::const_iterator i = names.begin (); i != names.end (); i++)
    {
      for (TraceItems::const_iterator j = m_items.begin (); j != m_items.end (); j++)
	{
	  if (j->name == *i)
	    {
	      list.push_back (j->createResolver (j->context));
	      break;
	    }
	}
    }
  return list;
}

}//namespace ns3

#ifdef RUN_SELF_TESTS

#include "ns3/test.h"

namespace ns3 {

class CompositeTraceResolverTest : public Test
{
public:
  enum TraceSources {
    TEST_TRACE_DOUBLEA,
    TEST_TRACE_DOUBLEB,
    TEST_TRACE_SUBRESOLVER,
  };
  enum SubTraceSources {
    TEST_SUBTRACE_INT,
  };
  CompositeTraceResolverTest ();
  virtual ~CompositeTraceResolverTest ();
  virtual bool RunTests (void);
private:
  void TraceDouble (TraceContext const &context, double v);
  void TraceInt (TraceContext const &context, int v);
  TraceResolver *CreateSubResolver (TraceContext const &context);


  bool m_gotDoubleA;
  bool m_gotDoubleB;
  CallbackTraceSource<int> m_traceInt;
  bool m_gotInt;
};

CompositeTraceResolverTest::CompositeTraceResolverTest ()
  : Test ("CompositeTraceResolver")
{}
CompositeTraceResolverTest::~CompositeTraceResolverTest ()
{}
void 
CompositeTraceResolverTest::TraceDouble (TraceContext const &context, double v)
{
  enum CompositeTraceResolverTest::TraceSources source;
  context.Get (source);
  switch (source)
    {
    case TEST_TRACE_DOUBLEA:
      m_gotDoubleA = true;
      break;
    case TEST_TRACE_DOUBLEB:
      m_gotDoubleB = true;
      break;
    default:
      NS_FATAL_ERROR ("should not get any other trace source in this sink");
      break;
    }
  
}

void 
CompositeTraceResolverTest::TraceInt (TraceContext const &context, int v)
{
  m_gotInt = true;
}

TraceResolver *
CompositeTraceResolverTest::CreateSubResolver (TraceContext const &context)
{
  CompositeTraceResolver *subresolver = new CompositeTraceResolver (context);
  subresolver->Add ("trace-int", m_traceInt, TEST_SUBTRACE_INT);
  return subresolver;
}
bool 
CompositeTraceResolverTest::RunTests (void)
{
  bool ok = true;

  CallbackTraceSource<double> traceDoubleA;
  CallbackTraceSource<double> traceDoubleB;
  TraceContext context;

  CompositeTraceResolver resolver (context) ;

  resolver.Add ("trace-double-a", traceDoubleA, TEST_TRACE_DOUBLEA);
  resolver.Add ("trace-double-b", traceDoubleB, TEST_TRACE_DOUBLEB);

  resolver.Connect ("/*", MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));

  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  if (!m_gotDoubleA || m_gotDoubleB)
    {
      ok = false;
    }
  m_gotDoubleA = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (!m_gotDoubleA || !m_gotDoubleB)
    {
      ok = false;
    }
  m_gotDoubleA = false;
  m_gotDoubleB = false;

  resolver.Disconnect ("/*", MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));

  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (m_gotDoubleA || m_gotDoubleB)
    {
      ok = false;
    }

  resolver.Connect ("/trace-double-a", 
		    MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));
  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (!m_gotDoubleA || m_gotDoubleB)
    {
      ok = false;
    }
  resolver.Disconnect ("/trace-double-a", 
		       MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));

  resolver.Connect ("/(trace-double-a)", 
		    MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));
  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (!m_gotDoubleA || m_gotDoubleB)
    {
      ok = false;
    }
  resolver.Disconnect ("/trace-double-a", 
		       MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));

  resolver.Connect ("/(trace-double-a|trace-double-b)", 
		    MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));
  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (!m_gotDoubleA || !m_gotDoubleB)
    {
      ok = false;
    }
  resolver.Disconnect ("/trace-double-a", 
		       MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));
  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (m_gotDoubleA || !m_gotDoubleB)
    {
      ok = false;
    }


  resolver.Disconnect ("/(trace-double-a|trace-double-b)", 
		       MakeCallback (&CompositeTraceResolverTest::TraceDouble, this));
  m_gotDoubleA = false;
  m_gotDoubleB = false;
  traceDoubleA (0);
  traceDoubleB (0);
  if (m_gotDoubleA || m_gotDoubleB)
    {
      ok = false;
    }

  resolver.Add ("subresolver", 
		MakeCallback (&CompositeTraceResolverTest::CreateSubResolver, this),
		TEST_TRACE_SUBRESOLVER);

  resolver.Connect ("/subresolver/trace-int", 
		    MakeCallback (&CompositeTraceResolverTest::TraceInt, this));
  m_gotInt = false;
  m_traceInt (1);
  if (!m_gotInt)
    {
      ok = false;
    }

  resolver.Disconnect ("/subresolver/trace-int", 
		       MakeCallback (&CompositeTraceResolverTest::TraceInt, this));
  m_gotInt = false;
  m_traceInt (1);
  if (m_gotInt)
    {
      ok = false;
    }

  resolver.Connect ("/*/trace-int", 
		    MakeCallback (&CompositeTraceResolverTest::TraceInt, this));
  m_gotInt = false;
  m_traceInt (1);
  if (!m_gotInt)
    {
      ok = false;
    }

  resolver.Disconnect ("/subresolver/trace-int", 
		       MakeCallback (&CompositeTraceResolverTest::TraceInt, this));
  m_gotInt = false;
  m_traceInt (1);
  if (m_gotInt)
    {
      ok = false;
    }


  

  return ok;
}

static CompositeTraceResolverTest g_compositeTraceResolverTest;

}//namespace ns3


#endif /* RUN_SELF_TESTS */
