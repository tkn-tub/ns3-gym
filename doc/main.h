/**
 * \mainpage ns-3 Documentation
 *
 * \section intro-sec Introduction
 * <a href="http://www.nsnam.org/">ns-3</a> documentation is maintained using 
 * <a href="http://www.doxygen.org">Doxygen</a>.
 * Doxygen is typically used for 
 * API documentation, and organizes such documentation across different
 * modules.   This project uses Doxygen for building the definitive
 * maintained API documentation,  Separate GNU texinfo documents are used for 
 * a tutorial, reference manual, and testing and validation manual.
 *
 * The ns-3 project documentation is organized as follows:
 *     - <b><a href="modules.html">modules</a></b>:  The "Modules" tab (above) 
 *       organizes  all of the public API and supporting manual text 
 *       along the  source code directory structure.   
 *     - <a href="http://www.nsnam.org/tutorials.html">tutorial</a>:  The ns-3 tutorial is a separate document maintained in <a href="http://www.gnu.org/software/texinfo/"> GNU Texinfo</a>. 
 *     - <a href="http://www.nsnam.org/tutorials.html">Reference manual</a>:  The ns-3 reference manual is a separate document maintained in <a href="http://www.gnu.org/software/texinfo/"> GNU Texinfo</a>. 
 *     - <a href="http://www.nsnam.org/tutorials.html">Testing and validation manual</a>:  The ns-3 testing and validation manual is a separate document maintained in <a href="http://www.gnu.org/software/texinfo/"> GNU Texinfo</a>. 
 *     - The <b><a href="http://www.nsnam.org/wiki/index.php/Main_Page">ns-3 wiki</a></b> 
 *       contains additional user-contributed material.  Some wiki-contributed
 *       material may migrate to and overlap with the Doxygen and manual information.
 *
 * \section install-sec Building the Documentation
 * 
 * ns-3 requires Doxygen version 1.5.4 or greater to fully build all items,
 * although earlier versions of Doxygen will mostly work.
 * 
 * Type "./waf --doxygen" or "./waf --doxygen-no-build" to build the 
 *  documentation.  The doc/ directory contains
 * configuration for Doxygen (doxygen.conf) and main.h.  The Doxygen 
 * build process puts html files into the doc/html/ directory, and latex 
 * filex into the doc/latex/ directory.
 * 
 * \section module-sec Module overview
 *
 * The ns-3 library is split across many modules:
 *     - aodv
 *     - applications
 *     - bridge
 *     - click
 *     - config-store
 *     - core
 *     - csma
 *     - csma-layout
 *     - dsdv
 *     - emu
 *     - energy
 *     - flow-monitor
 *     - internet
 *     - lte
 *     - mesh
 *     - mobility
 *     - mpi
 *     - netanim
 *     - network
 *     - nix-vector-routing
 *     - ns3tcp
 *     - ns3wifi
 *     - olsr
 *     - openflow
 *     - point-to-point
 *     - point-to-point-layout
 *     - propagation
 *     - spectrum
 *     - stats
 *     - tap-bridge
 *     - template
 *     - test
 *     - tools
 *     - topology-read
 *     - uan
 *     - virtual-net-device
 *     - visualizer
 *     - wifi
 *     - wimax
 *
 * More detail can be found in the <b><a href="modules.html">Modules</a></b>
 * tab.
 *
 */
/**
 * \namespace ns3
 * \brief Every class exported by the ns3 library is enclosed in the
 * ns3 namespace.
 */

