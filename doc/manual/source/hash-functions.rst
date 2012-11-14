.. include:: replace.txt

Hash Functions
----------------

|ns3| provides a generic interface to general purpose hash functions.
In the simplest usage, the hash function returns the 32-bit or 64-bit
hash of a data buffer or string.  The default underlying hash function
is murmur3_, chosen because it has good hash function properties and
offers a 64-bit version.  The venerable FNV1a hash is also available.

There is a straight-forward mechanism to
add (or provide at run time) alternative hash function implementations.

.. _murmur3: http://code.google.com/p/smhasher/wiki/MurmurHash3

Basic Usage
***********

The simplest way to get a hash value of a data buffer or string is just::

  #include "ns3/hash.h"

  char * buffer = ...
  size_t buffer_size = ...
  
  Hash::Hash32_t  buffer_hash = Hash32 ( (const char *) buffer, buffer_size);

  std::string s;

  Hash::Hash32_t  s_hash = Hash32 (s);

The ``Hash::Hash32_t`` type is just an ``uint32_t``, as a reminder that hash
values are opaque.

Equivalent functions and ``Hash::Hash64_t`` type are defined for 64-bit hash
values.

Adding New Hash Function Implementations
****************************************

To add the hash function ``foo``, follow the ``hash-murmur3.h``/``.cc`` pattern:

 * Create a class declaration (``.h``) and definition (``.cc``) inheriting
   from ``HashImplementation``.
 * ``include`` the declaration in ``hash.h`` (at the point where
   ``hash-murmur3.h`` is included.
 * In your own code, instantiate a ``Hash`` object via the constructor
   ``Hash (Ptr<HashImplementation> (Foo))``
   

If your hash function is a single function, e.g. ``hashf``, you don't
even need to create a new class derived from HashImplementation::

  Hash (Ptr<HashImplementation> (Hash32Implementation<&hashf>))

For this to compile, your ``hashf`` has to match one of the function pointer
signatures::

  typedef Hash::Hash32_t (*Hash32Function_ptr) (const char *, const size_t);
  typedef Hash::Hash64_t (*Hash64Function_ptr) (const char *, const size_t);


Sources for Hash Functions
**************************

Sources for other hash function implementations include:

 * Peter Kankowski: http://www.strchr.com
 * Arash Partow:  http://www.partow.net/programming/hashfunctions/index.html
 * SMHasher:
 * Sanmayce:  http://www.sanmayce.com/Fastest_Hash/index.html 
