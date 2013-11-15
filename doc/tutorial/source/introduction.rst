.. include:: replace.txt

Introduction
------------

The |ns3| simulator is a discrete-event network simulator targeted 
primarily for research and educational use.  The 
`ns-3 project
<http://www.nsnam.org>`_, 
started in 2006, is an open-source project developing |ns3|.

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

* |ns3| is open-source, and the project strives to maintain an 
  open environment for researchers to contribute and share their software.  
* |ns3| is not a backwards-compatible extension of `ns-2
  <http://www.isi.edu/nsnam/ns>`_; 
  it is a new simulator.  The two simulators are both written in C++ but 
  |ns3| is a new simulator that does not support the |ns2| APIs.  Some 
  models from |ns2| have already been ported from |ns2| to |ns3|. The 
  project will continue to maintain |ns2| while |ns3| is being built,
  and will study transition and integration mechanisms.

About |ns3|
***********

|ns3| has been developed to provide an open, extensible network simulation
platform, for networking research and education.  In brief, |ns3| provides
models of how packet data networks work and perform, and provides a
simulation engine for users to conduct simulation experiments.  Some of the
reasons to use |ns3| include to perform studies that are more difficult
or not possible to perform with real systems, to study system behavior in
a highly controllled, reproducible environment, and to learn about how
networks work.  Users will note that the available model set in |ns3| 
focuses on modeling how Internet protocols and networks work, but
|ns3| is not limited to Internet systems; several users are using
|ns3| to model non-Internet-based systems.

Many simulation tools exist for network simulation studies.  Below are
a few distinguishing features of |ns3| in contrast to other tools.

* |ns3| is designed as a set of libraries that can be combined together
  and also with other external software libraries.  While some simulation
  platforms provide users with a single, integrated graphical user 
  interface environment in which all tasks are carried out, |ns3| is 
  more modular in this regard.  Several external animators and
  data analysis and visualization tools can be used with |ns3|.  However,
  users should expect to work at the command line and with C++ and/or
  Python software development tools. 
* |ns3| is primarily used on Linux systems, although support exists
  for FreeBSD, Cygwin (for Windows), and native Windows Visual Studio
  support is in the process of being developed. 
* |ns3| is not an officially supported software product of any company.
  Support for |ns3| is done on a best-effort basis on the 
  ns-3-users mailing list.
 
 
For |ns2| Users
***************

For those familiar with |ns2| (a popular tool that preceded |ns3|), 
the most visible outward change when moving to 
|ns3| is the choice of scripting language.  Programs in |ns2| are 
scripted in OTcl and results of simulations can be visualized using the 
Network Animator nam.  It is not possible to run a simulation
in |ns2| purely from C++ (i.e., as a main() program without any OTcl).
Moreover, some components of |ns2| are written in C++ and others in OTcl.
In |ns3|, the simulator is written entirely in C++, with optional
Python bindings.  Simulation scripts can therefore be written in C++
or in Python.  New animators and visualizers are available and under
current development.  Since |ns3|
generates pcap packet trace files, other utilities can be used to
analyze traces as well.
In this tutorial, we will first concentrate on scripting 
directly in C++ and interpreting results via trace files.  

But there are similarities as well (both, for example, are based on C++ 
objects, and some code from |ns2| has already been ported to |ns3|).
We will try to highlight differences between |ns2| and |ns3|
as we proceed in this tutorial.

A question that we often hear is "Should I still use |ns2| or move to
|ns3|?"  In this author's opinion, unless the user is somehow vested
in |ns2| (either based on existing personal comfort with and knowledge
of |ns2|, or based on a specific simulation model that is only available
in |ns2|), a user will be more productive with |ns3| for the following
reasons:

* |ns3| is actively maintained with an active, responsive users mailing 
  list, while |ns2| is only lightly maintained and has not seen
  significant development in its main code tree for over a decade.
* |ns3| provides features not available in |ns2|, such as a implementation
  code execution environment (allowing users to run real implementation
  code in the simulator)
* |ns3| provides a lower base level of abstraction compared with |ns2|,
  allowing it to align better with how real systems are put together.
  Some limitations found in |ns2| (such as supporting multiple types of
  interfaces on nodes correctly) have been remedied in |ns3|.

|ns2| has a more diverse set of contributed modules than does |ns3|, owing to
its long history.  However, |ns3| has more detailed models in several
popular areas of research (including sophisticated LTE and WiFi models),
and its support of implementation code admits a very wide spectrum
of high-fidelity models.  Users may be surprised to learn that the
whole Linux networking stack can be encapsulated in an |ns3| node,
using the Direct Code Execution (DCE) framework.  |ns2|
models can sometimes be ported to |ns3|, particularly if they have been
implemented in C++.  

If in doubt, a good guideline would be to look at both simulators (as
well as other simulators), and in particular the models available
for your research, but keep in mind that your experience may be better
in using the tool that is being actively developed and 
maintained (|ns3|).

Contributing
************

|ns3| is a research and educational simulator, by and for the 
research community.  It will rely on the ongoing contributions of the 
community to develop new models, debug or maintain existing ones, and share 
results.  There are a few policies that we hope will encourage people to 
contribute to |ns3| like they have for |ns2|:

* Open source licensing based on GNU GPLv2 compatibility
* `wiki
  <http://www.nsnam.org/wiki>`_
* `Contributed Code
  <http://www.nsnam.org/wiki/Contributed_Code>`_ page, similar to |ns2|'s popular Contributed Code
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

