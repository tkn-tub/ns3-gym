.. include:: replace.txt

Resources
---------

The Web
*******

There are several important resources of which any |ns3| user must be
aware.  The main web site is located at http://www.nsnam.org and 
provides access to basic information about the |ns3| system.  Detailed 
documentation is available through the main web site at
http://www.nsnam.org/documents.html.  You can also find documents 
relating to the system architecture from this page.

There is a Wiki that complements the main |ns3| web site which you will
find at http://www.nsnam.org/wiki/.  You will find user and developer 
FAQs there, as well as troubleshooting guides, third-party contributed code, 
papers, etc. 

The source code may be found and browsed at http://code.nsnam.org/. 
There you will find the current development tree in the repository named
``ns-3-dev``. Past releases and experimental repositories of the core
developers may also be found there.

Mercurial
*********

Complex software systems need some way to manage the organization and 
changes to the underlying code and documentation.  There are many ways to
perform this feat, and you may have heard of some of the systems that are
currently used to do this.  The Concurrent Version System (CVS) is probably
the most well known.

The |ns3| project uses Mercurial as its source code management system.
Although you do not need to know much about Mercurial in order to complete
this tutorial, we recommend becoming familiar with Mercurial and using it 
to access the source code.  Mercurial has a web site at 
http://www.selenic.com/mercurial/,
from which you can get binary or source releases of this Software
Configuration Management (SCM) system.  Selenic (the developer of Mercurial)
also provides a tutorial at 
http://www.selenic.com/mercurial/wiki/index.cgi/Tutorial/,
and a QuickStart guide at
http://www.selenic.com/mercurial/wiki/index.cgi/QuickStart/.

You can also find vital information about using Mercurial and |ns3|
on the main |ns3| web site.

Waf
***

Once you have source code downloaded to your local system, you will need 
to compile that source to produce usable programs.  Just as in the case of
source code management, there are many tools available to perform this 
function.  Probably the most well known of these tools is ``make``.  Along
with being the most well known, ``make`` is probably the most difficult to
use in a very large and highly configurable system.  Because of this, many
alternatives have been developed.  Recently these systems have been developed
using the Python language.

The build system Waf is used on the |ns3| project.  It is one 
of the new generation of Python-based build systems.  You will not need to 
understand any Python to build the existing |ns3| system, and will 
only have to understand a tiny and intuitively obvious subset of Python in 
order to extend the system in most cases.

For those interested in the gory details of Waf, the main web site can be 
found at http://code.google.com/p/waf/.

Development Environment
***********************

As mentioned above, scripting in |ns3| is done in C++ or Python.
As of ns-3.2, most of the |ns3| API is available in Python, but the 
models are written in C++ in either case.  A working 
knowledge of C++ and object-oriented concepts is assumed in this document.
We will take some time to review some of the more advanced concepts or 
possibly unfamiliar language features, idioms and design patterns as they 
appear.  We don't want this tutorial to devolve into a C++ tutorial, though,
so we do expect a basic command of the language.  There are an almost 
unimaginable number of sources of information on C++ available on the web or
in print.

If you are new to C++, you may want to find a tutorial- or cookbook-based
book or web site and work through at least the basic features of the language
before proceeding.  For instance, `this tutorial
<http://www.cplusplus.com/doc/tutorial/>`_.

The |ns3| system uses several components of the GNU "toolchain" 
for development.  A 
software toolchain is the set of programming tools available in the given 
environment. For a quick review of what is included in the GNU toolchain see,
http://en.wikipedia.org/wiki/GNU_toolchain.  |ns3| uses gcc, 
GNU binutils, and gdb.  However, we do not use the GNU build system tools, 
neither make nor autotools.  We use Waf for these functions.

Typically an |ns3| author will work in Linux or a Linux-like
environment.  For those running under Windows, there do exist environments 
which simulate the Linux environment to various degrees.  The |ns3| 
project supports development in the Cygwin environment for 
these users.  See http://www.cygwin.com/ 
for details on downloading (MinGW is presently not officially supported,
although some of the project maintainers to work with it). Cygwin provides 
many of the popular Linux system commands.  It can, however, sometimes be 
problematic due to the way it actually does its emulation, and sometimes
interactions with other Windows software can cause problems.

If you do use Cygwin or MinGW; and use Logitech products, we will save you
quite a bit of heartburn right off the bat and encourage you to take a look
at the `MinGW FAQ
<http://oldwiki.mingw.org/index.php/FAQ>`_.

Search for "Logitech" and read the FAQ entry, "why does make often 
crash creating a sh.exe.stackdump file when I try to compile my source code."
Believe it or not, the ``Logitech Process Monitor`` insinuates itself into
every DLL in the system when it is running.  It can cause your Cygwin or
MinGW DLLs to die in mysterious ways and often prevents debuggers from 
running.  Beware of Logitech software when using Cygwin.

Another alternative to Cygwin is to install a virtual machine environment
such as VMware server and install a Linux virtual machine.

Socket Programming
******************

We will assume a basic facility with the Berkeley Sockets API in the examples
used in this tutorial.  If you are new to sockets, we recommend reviewing the
API and some common usage cases.  For a good overview of programming TCP/IP
sockets we recommend `TCP/IP Sockets in C, Donahoo and Calvert
<http://www.elsevier.com/wps/find/bookdescription.cws_home/717656/description#description>`_.

There is an associated web site that includes source for the examples in the
book, which you can find at:
http://cs.baylor.edu/~donahoo/practical/CSockets/.

If you understand the first four chapters of the book (or for those who do
not have access to a copy of the book, the echo clients and servers shown in 
the website above) you will be in good shape to understand the tutorial.
There is a similar book on Multicast Sockets,
`Multicast Sockets, Makofske and Almeroth
<http://www.elsevier.com/wps/find/bookdescription.cws_home/700736/description#description>`_.
that covers material you may need to understand if you look at the multicast 
examples in the distribution.
