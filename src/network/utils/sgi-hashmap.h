/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@cutebugs.net>
 */

/* This code snippet was ripped out of the gcc 
 * documentation and slightly modified to work
 * with gcc 4.x
 */
#ifndef SGI_HASHMAP_H
#define SGI_HASHMAP_H

/* To use gcc extensions.
 */
#ifdef __GNUC__
  #if __GNUC__ < 3 // GCC 2.x
    #include <hash_map.h>
    namespace sgi { using ::hash_map; }; // inherit globals
  #else
    #if __GNUC__ < 4 // GCC 3.x
      #include <ext/hash_map>
      #if __GNUC_MINOR__ == 0
        namespace sgi = std; // GCC 3.0
      #else
        namespace sgi = ::__gnu_cxx; // GCC 3.1 and later
      #endif
    #else
       #if __GNUC__ < 5 // GCC 4.x
         #if __GNUC_MINOR__ < 3 // GCC 4.0, 4.1 and 4.2
           #ifdef __clang__ // Clang identifies as GCC 4.2
             #undef __DEPRECATED
           #endif
           #include <ext/hash_map>
           namespace sgi = ::__gnu_cxx;
         #else
           #undef __DEPRECATED
           #include <backward/hash_map>
           namespace sgi = ::__gnu_cxx;
         #endif
       #else // GCC 5.x
         #undef __DEPRECATED
         #include <backward/hash_map>
         namespace sgi = ::__gnu_cxx;
       #endif
     #endif
  #endif
#else      // ...  there are other compilers, right?
namespace sgi = std;
#endif


#endif /* SGI_HASHMAP_H */
