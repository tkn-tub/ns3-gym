/* This code snippet was ripped out of the gcc 
 * documentation and slightly modified to work
 * with gcc 4.x
 */
#ifndef SGI_HASHMAP_H
#define SGI_HASHMAP_H

/* To use gcc extensions.
 */
#ifdef __GNUC__
  #if __GNUC__ < 3
     #include <hash_map.h>
namespace Sgi { using ::hash_map; }; // inherit globals
  #else 
     #if __GNUC__ < 4
       #include <ext/hash_map>
       #if __GNUC_MINOR__ == 0
namespace Sgi = std;               // GCC 3.0
       #else
namespace Sgi = ::__gnu_cxx;       // GCC 3.1 and later
       #endif
     #else  // gcc 4.x and later
       #include <ext/hash_map>
       namespace Sgi = ::__gnu_cxx;
     #endif
  #endif
#else      // ...  there are other compilers, right?
namespace Sgi = std;
#endif


#endif /* SGI_HASHMAP_H */
