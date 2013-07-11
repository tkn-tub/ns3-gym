/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include <algorithm>  // find
#include <climits>    // CHAR_BIT
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>

#include "ns3/core-module.h"
#include "ns3/hash.h"

NS_LOG_COMPONENT_DEFINE ("Hasher");

using namespace ns3;


/** Namespace for hasher-example */
namespace HashExample {

/**
 * Keep track of collisions
 */
class Collider {

public:
  std::string m_name;                   /**< Name of this hash */
  Hasher      m_hash;                   /**< The hash */

  enum Bits {
    Bits32,                             /**< Use 32-bit hash function */
    Bits64                              /**< Use 64-bit hash function */
  };

  /**
   * Constructor
   *
   * \param [in] name Hash function name
   * \param [in] hash Hash function
   * \param [in] bits Which hash length to use
   */
  Collider (const std::string name, Hasher hash, const enum Bits bits)
    : m_name (name),
      m_hash (hash),
      m_bits (bits)
  {  }

  /**
   * Add a string to the Collider
   *
   * \param [in] phrase the string to add
   * \return true if this was a new string
   */
  bool Add (const std::string phrase)
  {
    uint64_t h = GetHash (phrase);
    
    // Check for collisions
    if (m_dict.count (h) > 0)
      {
        // we've seen this hash before, why?
        if (phrase == m_dict[h])
          {
            // duplicate phrase
            return false;
          }

        // new phrase generating a real collision
        // alphabetize
        if ( m_dict[h] < phrase)
          {       
            m_coll.push_back (std::make_pair (h, phrase));
          }
        else
          {
            m_coll.push_back (std::make_pair (h, m_dict[h]));
            m_dict[h] = phrase;
          }            
      }
    else
      {
        // Insert new hash
        m_dict.insert (std::make_pair (h, phrase));
      }
    return true;
  }  // Add ()

  /**
   * \return The hash name, including the length
   */
  std::string GetName () const
  {
    std::string name = m_name;

    switch (m_bits)
      {
      case Bits32:   name += " (32-bit version)";  break;
      case Bits64:   name += " (64-bit version)";  break;
      default:       name += " (unknown!?!)";
      }
    return name;
  }

  /** Print the collisions found */
  void Report () const
  {
    std::cout << std::endl;
    
    std::cout << GetName () << ": " << m_coll.size () << " collisions:"
              << std::endl;
    for (collision_t::const_iterator it = m_coll.begin ();
         it != m_coll.end ();
         ++it)
      {
        uint64_t h = it->first;
        
        std::cout << std::setfill ('0') << std::hex << std::setw(8) << h
                  << std::dec << std::setfill(' ')  << "  "
                  << std::setw(20) << std::left
                  << m_dict.find (h)->second
                  << it->second
                  << std::right
                  << std::endl;
      }
  }  // Report ()

  
private:

  /**
   * Get the appropriate hash value
   *
   * \param [in] phrase the string to hash
   * \return the hash value, using the number of bits set in the constructor
   */
  uint64_t GetHash (const std::string phrase)
    {
      m_hash.clear ();
      uint64_t h = 0;

      if (m_bits == Bits32)
        {
          h = m_hash.GetHash32 (phrase);
        }
      else
        {
          h = m_hash.GetHash64 (phrase);
        }
      return h;
    }

  /** Hash function */
  enum Bits m_bits;
  
  /** Hashed dictionary of first instance of each hash */
  typedef std::map <uint64_t, std::string> hashdict_t;

  /** The dictionary map, indexed by hash */
  hashdict_t  m_dict;

  /** Collision map of subsequent instances */
  typedef std::vector < std::pair<uint64_t, std::string> > collision_t;

  /** The list of collisions */
  collision_t m_coll;            

};  // class Collider

  
/**
 * Word list and hashers to test
 */
class Dictionary {
public:
  /** Constructor */
  Dictionary ()
    : m_nphrases (0)
  {
    m_words.reserve (320000);
  }

  /** Add a Collider containing a hash function */
  void Add (Collider c)
  {
    m_hashes.push_back (c);
  }

  /**
   * Add a string to the dictionary
   *
   * \param [in] phrase the string to add
   */
  void Add (const std::string phrase)
  {
    if (phrase.size () == 0)
      {
        return ;
      }

    int newPhrases = 0;
    for (std::vector <Collider>::iterator it = m_hashes.begin ();
           it != m_hashes.end ();
           ++it)
        {
          newPhrases += it->Add (phrase);
        }

      if (newPhrases)
        {
          ++m_nphrases;
          m_words.push_back (phrase);
        }
  }  // Add ()

  /**
   * Report the expected number of collisions
   *
   * See, e.g.,
   * http://www.math.dartmouth.edu/archive/m19w03/public_html/Section6-5.pdf
   *
   *  \f[
   *     E(collisions)  =  n - k + k (1 - 1/k)^n
   *  \f]
   *               
   *  where <i>n</i> is the number of entries in the table, and
   *  <i>k</i> is the number of buckets.
   * 
   *  This form is numerically unstable for low collision rates.
   *  Expanding for large \f$ k \f$ we get
   *
   * \f{eqnarray*}{
   *   E(c) &=& \frac{1}{k} {n \choose 2}
   *            - \frac{1}{{{k^2}}} {n \choose 3}
   *            + \frac{1}{{{k^3}}} {n \choose 4}
   *            -  \ldots \\
   *        &=& \frac{1}{k} {n \choose 2}
   *            \left[ {1 - \frac{{n - 2}}{{3k}}
   *                      + \frac{{\left( {n - 2} \right)
   *                               \left( {n - 3} \right)}}{{12{k^2}}}
   *                      -  \ldots } \right] \\
   *        &=& \frac{1}{k} {n \choose 2}
   *            \left\{ {1 - \frac{{n - 2}}{{3k}}
   *                         \left[ {1 + \frac{{n - 3}}{{4k}}
   *                                   -  \ldots }
   *                         \right]}
   *            \right\}
   * \f}
   * 
   *   For simplicity, we'll use the first two terms
   *   of the second form.
   */
  void ReportExpectedCollisions () const
  {
    // Expected number of collisions
    //
    // Number of buckets = k = 2^bits
    long double k32 = 0xFFFFFFFF;
    long double k64 = 0xFFFFFFFFFFFFFFFF;

    long double n = m_nphrases;
    long double Ec32 = n * (n - 1) / ( 2 * k32) * (1 - (n - 2)/(3 * k32));
    long double Ec64 = n * (n - 1) / ( 2 * k64) * (1 - (n - 2)/(3 * k64));
      
    // Output collisions
    std::cout << "" << std::endl;
    std::cout << "Number of words or phrases: " << n << std::endl;
    std::cout << "Expected number of collisions: (32-bit table) " << Ec32
              << std::endl;
    std::cout << "Expected number of collisions: (64-bit table) " << Ec64
              << std::endl;
  }  // ReportExpectedCollisions
  
    
  /** Print the collisions for each Collider */
  void Report () const
  {
    ReportExpectedCollisions ();
    
    for (std::vector <Collider>::const_iterator it = m_hashes.begin ();
         it != m_hashes.end ();
         ++it)
      {
        it->Report ();
      }
  }  // Report ()

  /**
   * Time and report the execution of one hash across the entire Dictionary
   *
   * \param [in] hindex index of the hash Collider to use
   */
  void TimeOne (const int hindex)
  {
    // Hashing speed
    uint32_t reps = 100;
    Hasher h = m_hashes[hindex].m_hash;
    int start = clock ();
    for (std::vector<std::string>::const_iterator w = m_words.begin ();
         w != m_words.end();
         ++w)
      {
        for (uint32_t i = 0; i < reps; ++i)
          {
            h.clear ().GetHash32 (*w);
          }
      }
    int stop = clock ();
    double delta = stop - start;
    double per = 1e9 * delta / (m_nphrases * reps * CLOCKS_PER_SEC);
    
    std::cout << std::left
              << std::setw (32) << m_hashes[hindex].GetName ()
              << std::right
              << std::setw (10) << m_nphrases
              << std::setw (10) << reps
              << std::setw (10) << stop - start
              << std::setw (12) << per
              << std::endl;
    
  }  // TimeOne ()

  /** Report the execution time of each hash across the entire Dictionary */
  void Time ()
  {
    std::cout << "" << std::endl;
    std::cout << std::left
              << std::setw (32) << "Hash timing"
              << std::right
              << std::setw (10) << "Phrases"
              << std::setw (10) << "Reps"
              << std::setw (10) << "Ticks"
              << std::setw (12) << "ns/hash"
              << std::endl;
    
    for (unsigned int i = 0; i < m_hashes.size (); ++i)
      {
        TimeOne (i);
      }
  }  // Time ()

private:
  unsigned long m_nphrases;             /**< Number of strings hashed */
  std::vector <Collider> m_hashes;      /**< List of hash Colliders */
  std::vector <std::string> m_words;    /**< List of unique words */

};  // class Dictionary


/**
 * Source word list files
 */
class DictFiles
{

public:
  
  /**
   * CommandLine callback function to add a file argument to the list
   *
   * \param [in] file the word file to add
   * \return true if the file is new to the list
   */
  bool Add (const std::string file)
  {
    if (std::find (m_files.begin (), m_files.end (), file) == m_files.end ())
      {
        m_files.push_back (file);
      }
    
    return true;
  }

  /**
   * Add phrases from the files into the dict
   *
   * \param [in,out] dict the Dictionary to add words to
   */
  void ReadInto (Dictionary & dict)
  {
    if (m_files.size () == 0)
      {
        Add ("/usr/share/dict/web2");
      }

    std::cout << "Hashing the dictionar"
              << (m_files.size () == 1 ? "y" : "ies")
              << std::endl;

    for (std::vector <std::string>::const_iterator it = m_files.begin ();
         it != m_files.end ();
         ++it)
      {
        std::string dictFile = *it;
        std::cout << "Dictionary file: " << dictFile << std::endl;
  
        // Find collisions
  
        // Open the file
        std::ifstream dictStream;
        dictStream.open (dictFile.c_str () );
        if (! dictStream.is_open () )
          {
            std::cerr << "Failed to open dictionary file."
                      << "'" << dictFile << "'"
                      << std::endl;
            continue;
          }
    
        while (dictStream.good () )
          {
            std::string phrase;
            getline (dictStream, phrase);
            dict.Add (phrase);
          }  // while
        
        dictStream.close ();

      }  // for m_files

  }  // ReadInto

private:
  std::vector <std::string> m_files;    /**< List of word files to use */

};  // class DictFiles


}  // namespace HashExample

using namespace HashExample;

int 
main (int argc, char *argv[])
{
  std::cout << std::endl;
  std::cout << "Hasher" << std::endl;

  bool timing = false;
  DictFiles files;

  CommandLine cmd;
  cmd.Usage ("Find hash collisions in the dictionary.");
  cmd.AddValue ("dict", "Dictionary file to hash",
                MakeCallback(&DictFiles::Add,
                             &files));
  cmd.AddValue ("time", "Run timing test", timing);
  cmd.Parse (argc, argv);

  Dictionary dict;
  dict.Add ( Collider ("FNV1a",
                       Hasher ( Create<Hash::Function::Fnv1a> () ),
                       Collider::Bits32));
  dict.Add ( Collider ("FNV1a",
                       Hasher ( Create<Hash::Function::Fnv1a> () ),
                       Collider::Bits64));

  dict.Add ( Collider ("Murmur3",
                       Hasher ( Create<Hash::Function::Murmur3> () ),
                       Collider::Bits32));
  dict.Add ( Collider ("Murmur3",
                       Hasher ( Create<Hash::Function::Murmur3> () ),
                       Collider::Bits64));
  
  files.ReadInto (dict);
  
  dict.Report ();
  
  if (timing)
    {
      dict.Time ();
    }  // if (timing)


}  // main


/*  Example Output:

./waf --run="hasher-example --time \
  --dict=/usr/share/dict/web2 \
  --dict=/usr/share/dict/web2a \
  --dict=/usr/share/dict/propernames \
  --dict=/usr/share/dict/connectives"

Waf: Entering directory `build'
Waf: Leaving directory `build'
'build' finished successfully (3.028s)

Hasher
Hashing the dictionaries
Dictionary file: /usr/share/dict/web2
Dictionary file: /usr/share/dict/web2a
Dictionary file: /usr/share/dict/propernames
Dictionary file: /usr/share/dict/connectives

Number of words or phrases: 312094
Expected number of collisions: (32-bit table) 11.3389
Expected number of collisions: (64-bit table) 2.6401e-09

FNV1a (32-bit version): 13 collisions:
a75b0ae7  elephantlike        interventralia
091c4808  diversionary        propenseness
172be6ba  bairnishness        sora
e6cb5099  purifier            spongoblastic
4a841078  ameliorable         unsmotherable
6ed21de2  brand-newness       peripherial
22acb19b  Petrarchism         dewy-pinioned
5723634a  grain gold          hyphenation
f58026c1  seven-channeled     turritella
946fc6ec  multiradiate        sister block
88625851  brachtmema          ule tree
dc28b5ea  Un-lutheran         gutturotetany
9255bf44  re-sorter           working stress

FNV1a (64-bit version): 0 collisions:

Murmur3 (32-bit version): 11 collisions:
5ea83eee  impalace            metahewettite
e06fbdde  constancy           oligosynthetic
2a713795  hypermonosyllable   presatisfaction
c8bf0ef9  Hadromerina         starky
d9c04b3d  Accipiter           syllable
c0da8f81  seriation           trigonon
17612b26  daemon              unerring
c2349ad7  air spring          iron
1d91386f  nine-pounder        semicrescentic
fe17b1a5  cone speaker        oblong-wedgeshaped
faa12798  saw bearing         wilting point

Murmur3 (64-bit version): 0 collisions:

Hash timing                        Phrases      Reps     Ticks     ns/hash
FNV1a (32-bit version)              312094       100   3140531     100.628
FNV1a (64-bit version)              312094       100   3145240     100.779
Murmur3 (32-bit version)            312094       100   4152139     133.041
Murmur3 (64-bit version)            312094       100   4191464     134.301

*/
