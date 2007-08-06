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
CompositeTraceResolver::Add (std::string name, 
                             Callback<TraceResolver *,TraceContext const &> createResolver)
{
  TraceContext traceContext = GetContext ();
  DoAdd (name, createResolver, traceContext);
}

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
#include "trace-context-element.h"

namespace ns3 {

class TraceSourceTest : public TraceContextElement
{
public:
  enum Sources {
    DOUBLEA,
    DOUBLEB,
    SUBRESOLVER,
  };
  static uint16_t GetUid (void) 
  {static uint16_t uid = Register<TraceSourceTest> ("TraceSourceTest"); return uid;}
  void Print (std::ostream &os)
  {os << "tracesource=";
    if (m_sources == DOUBLEA) {os << "doubleA";}
    else if (m_sources == DOUBLEB) {os << "doubleB";}
    else if (m_sources == SUBRESOLVER) {os << "subresolver";}
  }
  TraceSourceTest () : m_sources (TraceSourceTest::DOUBLEA) {}
  TraceSourceTest (enum Sources sources) :m_sources (sources) {}
  bool IsDoubleA (void) {return m_sources == TraceSourceTest::DOUBLEA;}
  bool IsDoubleB (void) {return m_sources == TraceSourceTest::DOUBLEB;}
private:
  enum TraceSourceTest::Sources m_sources;
};

class SubTraceSourceTest : public TraceContextElement
{
public:
  enum Sources {
    INT,
  };
  static uint16_t GetUid (void) 
  {static uint16_t uid = Register<SubTraceSourceTest> ("SubTraceSourceTest"); return uid;}
  void Print (std::ostream &os)
  {os << "subtracesource=int";}
  SubTraceSourceTest () : m_sources (SubTraceSourceTest::INT) {}
  SubTraceSourceTest (enum Sources sources) : m_sources (sources) {}
private:
  enum Sources m_sources;
};

class CompositeTraceResolverTest : public Test
{
public:
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
  TraceSourceTest source;
  context.Get (source);
  if (source.IsDoubleA ())
    {
      m_gotDoubleA = true;
    }
  else if (source.IsDoubleB ())
    {
      m_gotDoubleB = true;
    }
  else
    {
      NS_FATAL_ERROR ("should not get any other trace source in this sink");
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
  subresolver->Add ("trace-int", m_traceInt, 
                    SubTraceSourceTest (SubTraceSourceTest::INT));
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

  resolver.Add ("trace-double-a", traceDoubleA, 
                TraceSourceTest (TraceSourceTest::DOUBLEA));
  resolver.Add ("trace-double-b", traceDoubleB, 
                TraceSourceTest (TraceSourceTest::DOUBLEB));

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
		TraceSourceTest (TraceSourceTest::SUBRESOLVER));

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
