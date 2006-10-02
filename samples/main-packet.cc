/* -*-    Mode:C++; c-basic-offset:4; tab-width:4; indent-tabs-mode:nil -*- */
#include "ns3/packet.h"
#include "ns3/header.h"
#include <iostream>

using namespace ns3;

/* A sample Chunk implementation
 */
class MyHeader : public Header {
public:
    MyHeader ();
    virtual ~MyHeader ();

    void setData (uint16_t data);
    uint16_t getData (void) const;
private:
    virtual void printTo (std::ostream &os) const;
    virtual void serializeTo (Buffer::Iterator start) const;
    virtual void deserializeFrom (Buffer::Iterator start);
    virtual uint32_t getSerializedSize (void) const;

    uint16_t m_data;
};

MyHeader::MyHeader ()
{}
MyHeader::~MyHeader ()
{}
void 
MyHeader::printTo (std::ostream &os) const
{
    os << "MyHeader data=" << m_data << std::endl;
}
uint32_t
MyHeader::getSerializedSize (void) const
{
    return 2;
}
void 
MyHeader::serializeTo (Buffer::Iterator start) const
{
    // serialize in head of buffer
    start.writeHtonU16 (m_data);
}
void 
MyHeader::deserializeFrom (Buffer::Iterator start)
{
    // deserialize from head of buffer
    m_data = start.readNtohU16 ();
}

void 
MyHeader::setData (uint16_t data)
{
    m_data = data;
}
uint16_t 
MyHeader::getData (void) const
{
    return m_data;
}

/* A sample Tag implementation
 */
struct MyTag {
    uint16_t m_streamId;
};

static TagRegistration<struct MyTag> g_MyTagRegistration ("ns3::MyTag", 0);


static void
receive (Packet p)
{
    MyHeader my;
    p.peek (my);
    p.remove (my);
    std::cout << "received data=" << my.getData () << std::endl;
    struct MyTag myTag;
    p.peekTag (myTag);
}


int main (int argc, char *argv[])
{
    Packet p;
    MyHeader my;
    my.setData (2);
    std::cout << "send data=2" << std::endl;
    p.add (my);
    struct MyTag myTag;
    myTag.m_streamId = 5;
    p.addTag (myTag);
    receive (p);
    return 0;
}
