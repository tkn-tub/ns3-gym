/**
 * \mainpage ns-3 Documentation
 *
 * \section intro-sec Introduction
 * <a href="http://www.nsnam.org/">ns-3</a> documentation is maintained using 
 * <a href="http://www.doxygen.org">Doxygen</a>.
 * Doxygen is typically used for 
 * API documentation, and organizes such documentation across different
 * modules.   This project uses Doxygen both for building the manual around 
 * the API documentation,  and a separate GNU texinfo document is used for 
 * the manual.
 *
 * The ns-3 project documentation is organized as follows:
 *     - <b><a href="modules.html">modules</a></b>:  The "Modules" tab (above) 
 *       organizes  all of the public API and supporting manual text 
 *       along the  source code directory structure.   This forms the 
 *       "ns-3 manual", and it is available in HTML and PDF forms.
 *     - \ref howtos-anchor "HOWTOs": A set of HOWTOs and FAQs is
 *       maintained on another Doxygen "Related Page" 
 *     - <a href="http://www.nsnam.org/docs/tutorial/tutorial.html">tutorial</a>:  The ns-3 tutorial is a separate document maintained in <a href="http://www.gnu.org/software/texinfo/"> GNU Texinfo</a>. 
 *     - The <b><a href="http://www.nsnam.org/wiki/index.php/Main_Page">ns-3 wiki</a></b> 
 *       contains additional user-contributed material.  Some wiki-contributed
 *       material may migrate to and overlap with the Doxygen information.
 *
 * \section install-sec Building the Documentation
 * 
 * ns-3 requires Doxygen version 1.5.4 or greater to fully build all items,
 * although earlier versions of Doxygen will mostly work.
 * 
 * Type "./waf check" followed by "./waf --doxygen" to build the documentation.
 * There is a program that runs during "./waf check" that builds pieces of
 * the documentation through introspection.  The doc/ directory contains
 * configuration for Doxygen (doxygen.conf and main.txt).  The Doxygen 
 * build process puts html files into the doc/html/ directory, and latex 
 * filex into the doc/latex/ directory.
 * 
 * \section module-sec Module overview
 *
 * The ns-3 library is split across multiple modules:
 *     - core: located in src/core and contains a number of facilities which
 *       do not depend on any other module. Some of these facilities are
 *       OS-dependent.
 *     - simulator: located in src/simulator and contains event scheduling
 *       facilities.
 *     - common: located in src/common and contains facilities specific
 *       to network simulations but shared by pretty much every model
 *       of a network component.
 *     - node: located in src/node. Defines the abstract interfaces which 
 *       must be implemented by every node and more specifically, by ipv4 nodes.       
 *     - devices: located in src/devices. Contains a set of MAC-level models
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

