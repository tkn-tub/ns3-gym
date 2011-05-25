.. include:: replace.txt

Introduction
------------

The |ns3| simulator is a discrete-event network simulator targeted 
primarily for research and educational use.  The 
`ns-3 project
<http://www.nsnam.org>`_, 
started in 2006, is an open-source project developing |ns3|.

Primary documentation for the |ns3| project is available in four
forms:

* `ns-3 Doxygen/Manual
  <http://www.nsnam.org/doxygen/index.html>`_:  
  Documentation of the public APIs of the simulator
* Tutorial (this document)
* `Reference Manual
  <http://www.nsnam.org/docs/manual.html>`_:  Reference Manual
* `ns-3 wiki
  <http://www.nsnam.org/wiki/index.php>`_

The purpose of this tutorial is to introduce new |ns3| users to the 
system in a structured way.  It is sometimes difficult for new users to
glean essential information from detailed manuals and to convert this
information into working simulations.  In this tutorial, we will build 
several example simulations, introducing and explaining key concepts and
features as we go.

As the tutorial unfolds, we will introduce the full |ns3| documentation 
and provide pointers to source code for those interested in delving deeper
into the workings of the system.

A few key points are worth noting at the onset:

* Ns-3 is not an extension of `ns-2
  <http://www.isi.edu/nsnam/ns>`_; 
  it is a new simulator.  The two simulators are both written in C++ but 
  |ns3| is a new simulator that does not support the ns-2 APIs.  Some 
  models from ns-2 have already been ported from ns-2 to |ns3|. The 
  project will continue to maintain ns-2 while |ns3| is being built,
  and will study transition and integration mechanisms.
* |ns3| is open-source, and the project strives to maintain an 
  open  environment for researchers to contribute and share their software.  

 
For ns-2 Users
**************

For those familiar with ns-2, the most visible outward change when moving to 
|ns3| is the choice of scripting language.  Ns-2 is 
scripted in OTcl and results of simulations can be visualized using the 
Network Animator nam.  It is not possible to run a simulation
in ns-2 purely from C++ (i.e., as a main() program without any OTcl).
Moreover, some components of ns-2 are written in C++ and others in OTcl.
In |ns3|, the simulator is written entirely in C++, with optional
Python bindings.  Simulation scripts can therefore be written in C++
or in Python.  The results of some simulations can be visualized by
nam, but new animators are under development.  Since |ns3|
generates pcap packet trace files, other utilities can be used to
analyze traces as well.
In this tutorial, we will first concentrate on scripting 
directly in C++ and interpreting results via trace files.  

But there are similarities as well (both, for example, are based on C++ 
objects, and some code from ns-2 has already been ported to |ns3|).
We will try to highlight differences between ns-2 and |ns3|
as we proceed in this tutorial.

A question that we often hear is "Should I still use ns-2 or move to
|ns3|?"  The answer is that it depends.  |ns3| does not have
all of the models that ns-2 currently has, but on the other hand, |ns3|
does have new capabilities (such as handling multiple interfaces on nodes 
correctly, use of IP addressing and more alignment with Internet
protocols and designs, more detailed 802.11 models, etc.).  ns-2
models can usually be ported to |ns3| (a porting guide is under
development).  There is active development on multiple fronts for 
|ns3|.  The |ns3| developers believe (and certain early users
have proven) that |ns3| is ready for active use, and should be an 
attractive alternative for users looking to start new simulation projects.  

Contributing
************

|ns3| is a research and educational simulator, by and for the 
research community.  It will rely on the ongoing contributions of the 
community to develop new models, debug or maintain existing ones, and share 
results.  There are a few policies that we hope will encourage people to 
contribute to |ns3| like they have for ns-2:

* Open source licensing based on GNU GPLv2 compatibility
* `wiki
  <http://www.nsnam.org/wiki/index.php>`_
* `Contributed Code
  <http://www.nsnam.org/wiki/index.php/Contributed_Code>`_ page, similar to ns-2's popular Contributed Code
  `page
  <http://nsnam.isi.edu/nsnam/index.php/Contributed_Code>`_ 
* Open `bug tracker
  <http://www.nsnam.org/bugzilla>`_

We realize that if you are reading this document, contributing back to 
the project is probably not your foremost concern at this point, but
we want you to be aware that contributing is in the spirit of the project and
that even the act of dropping us a note about your early experience 
with |ns3| (e.g. "this tutorial section was not clear..."), 
reports of stale documentation, etc. are much appreciated. 

Tutorial Organization
*********************

The tutorial assumes that new users might initially follow a path such as the
following:

* Try to download and build a copy;
* Try to run a few sample programs;
* Look at simulation output, and try to adjust it.

As a result, we have tried to organize the tutorial along the above
broad sequences of events.

