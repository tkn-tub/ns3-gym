/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2006 INRIA
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
#ifndef TAGS_H
#define TAGS_H

#include <stdint.h>
#include <ostream>
#include <vector>

namespace ns3 {

template <typename T>
class TagPrettyPrinter;

/**
 * \ingroup constants
 * \brief Tag maximum size
 * The maximum size (in bytes) of a Tag is stored
 * in this constant.
 */
#define TAGS_MAX_SIZE 16

class Tags {
public:
    inline Tags ();
    inline Tags (Tags const &o);
    inline Tags &operator = (Tags const &o);
    inline ~Tags ();

    template <typename T>
    void add (T const&tag);

    template <typename T>
    bool remove (T &tag);

    template <typename T>
    bool peek (T &tag) const;

    void prettyPrint (std::ostream &os);

    inline void removeAll (void);

    enum {
        SIZE = TAGS_MAX_SIZE
    };
private:
    struct TagData {
        struct TagData *m_next;
        uint32_t m_id;
        uint32_t m_count;
        uint8_t m_data[Tags::SIZE];
    };

    bool remove (uint32_t id);
    struct Tags::TagData *allocData (void);
    void freeData (struct TagData *data);

    static struct Tags::TagData *gFree;
    static uint32_t gN_free;

    struct TagData *m_next;
};

/**
 * \brief pretty print packet tags
 * 
 * This class is used to register a pretty-printer
 * callback function to print in a nice user-friendly
 * way the content of the target type. To register
 * such a type, all you need to do is instantiate
 * an instance of this type as a static variable.
 */
template <typename T>
class TagRegistration {
public:
    /**
     * \param uuid a uuid generated with uuidgen
     * \param fn a function which can pretty-print an instance
     *        of type T in the output stream.
     */
    TagRegistration<T> (std::string uuid, void(*fn) (T *, std::ostream &));
private:
    static void prettyPrinterCb (uint8_t *buf, std::ostream &os);
    static void(*m_prettyPrinter) (T *, std::ostream &);
};

}; // namespace ns3



/**************************************************************
     An implementation of the templates defined above
 *************************************************************/
#include <cassert>
#include <string>

namespace ns3 {

class TagRegistry {
public:
    typedef void (*PrettyPrinter) (uint8_t [Tags::SIZE], std::ostream &);
    static void record (std::string uuid, PrettyPrinter prettyPrinter);
    static uint32_t lookupUid (std::string uuid);
    static void prettyPrint (uint32_t uid, uint8_t buf[Tags::SIZE], std::ostream &os);
private:
    typedef std::vector<std::pair<std::string,PrettyPrinter> > TagsData;
    typedef std::vector<std::pair<std::string,PrettyPrinter> >::const_iterator TagsDataCI;
    static bool m_sorted;
    static TagsData m_registry;
};
/**
 * The TypeUid class is used to create a mapping Type --> uid
 * Note that we use a static getUuid function which contains a
 * static std::string variable rather than a simpler static
 * member std::string variable to ensure the proper order
 * of initialization when these methods are invoked
 * from the constructor of another static variable.
 */
template <typename T>
class TypeUid {
public:
    static void record (std::string uuid);
    static const uint32_t getUid (void);
private:
    static std::string *getUuid (void);
    T m_realType;
};

template <typename T>
void TypeUid<T>::record (std::string uuid)
{
    *(getUuid ()) = uuid;
}

template <typename T>
const uint32_t TypeUid<T>::getUid (void)
{
    static const uint32_t uid = TagRegistry::lookupUid (*(getUuid ()));
    return uid;
}

template <typename T>
std::string *TypeUid<T>::getUuid (void)
{
    static std::string uuid;
    return &uuid;
}



/**
 * Implementation of the TagRegistration registration class.
 * It records a callback with the TagRegistry
 * This callback performs type conversion before forwarding
 * the call to the user-provided function.
 */
template <typename T>
TagRegistration<T>::TagRegistration (std::string uuid, void (*prettyPrinter) (T *, std::ostream &))
{
    assert (sizeof (T) <= Tags::SIZE);
    m_prettyPrinter  = prettyPrinter;
    TagRegistry::record (uuid, &TagRegistration<T>::prettyPrinterCb);
    TypeUid<T>::record (uuid);
}
template <typename T>
void 
TagRegistration<T>::prettyPrinterCb (uint8_t *buf, std::ostream &os)
{
    assert (sizeof (T) <= Tags::SIZE);
    T *tag = reinterpret_cast<T *> (buf);
    (*m_prettyPrinter) (tag, os);
}

template <typename T>
void (*TagRegistration<T>::m_prettyPrinter) (T *, std::ostream &) = 0;




template <typename T>
void 
Tags::add (T const&tag)
{
    assert (sizeof (T) <= Tags::SIZE);
    uint8_t const*buf = reinterpret_cast<uint8_t const*> (&tag);
    // ensure this id was not yet added
    for (struct TagData *cur = m_next; cur != 0; cur = cur->m_next) {
        assert (cur->m_id != TypeUid<T>::getUid ());
    }
    struct TagData *newStart = allocData ();
    newStart->m_count = 1;
    newStart->m_next = 0;
    newStart->m_id = TypeUid<T>::getUid ();
    memcpy (newStart->m_data, buf, sizeof (T));
    newStart->m_next = m_next;
    m_next = newStart;
}

template <typename T>
bool
Tags::remove (T &tag)
{
    assert (sizeof (T) <= Tags::SIZE);
    return remove (TypeUid<T>::getUid ());
}

template <typename T>
bool
Tags::peek (T &tag) const
{
    assert (sizeof (T) <= Tags::SIZE);
    uint8_t *buf = reinterpret_cast<uint8_t *> (&tag);
    for (struct TagData *cur = m_next; cur != 0; cur = cur->m_next) {
        if (cur->m_id == TypeUid<T>::getUid ()) {
            /* found tag */
            memcpy (buf, cur->m_data, sizeof (T));
            return true;
        }
    }
    /* no tag found */
    return false;
}

Tags::Tags ()
    : m_next ()
{}

Tags::Tags (Tags const &o)
    : m_next (o.m_next)
{
    if (m_next != 0) {
        m_next->m_count++;
    }
}

Tags &
Tags::operator = (Tags const &o)
{
    // self assignment
    if (m_next == o.m_next) {
        return *this;
    }
    removeAll ();
    m_next = o.m_next;
    if (m_next != 0) {
        m_next->m_count++;
    }
    return *this;
}

Tags::~Tags ()
{
    removeAll ();
}

void
Tags::removeAll (void)
{
    struct TagData *prev = 0;
    for (struct TagData *cur = m_next; cur != 0; cur = cur->m_next) {
        cur->m_count--;
        if (cur->m_count > 0) {
            break;
        }
        if (prev != 0) {
            freeData (prev);
        }
        prev = cur;
    }
    if (prev != 0) {
        freeData (prev);
    }
    m_next = 0;
}


}; // namespace ns3

#endif /* TAGS_H */
