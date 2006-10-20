/* -*- Mode:NS3; -*- */
#include "ns3/packet.h"
#include "ns3/header.h"
#include <iostream>

using namespace ns3;

/* A sample Header implementation
 */
class MyHeader : public Header {
public:
    MyHeader ();
    virtual ~MyHeader ();

    void SetData (uint16_t data);
    uint16_t GetData (void) const;
private:
    virtual void PrintTo (std::ostream &os) const;
    virtual void SerializeTo (Buffer::Iterator start) const;
    virtual void DeserializeFrom (Buffer::Iterator start);
    virtual uint32_t GetSerializedSize (void) const;

    uint16_t m_data;
};

MyHeader::MyHeader ()
{}
MyHeader::~MyHeader ()
{}
void 
MyHeader::PrintTo (std::ostream &os) const
{
    os << "MyHeader data=" << m_data << std::endl;
}
uint32_t
MyHeader::GetSerializedSize (void) const
{
    return 2;
}
void 
MyHeader::SerializeTo (Buffer::Iterator start) const
{
    // serialize in head of buffer
    start.WriteHtonU16 (m_data);
}
void 
MyHeader::DeserializeFrom (Buffer::Iterator start)
{
    // deserialize from head of buffer
    m_data = start.ReadNtohU16 ();
}

void 
MyHeader::SetData (uint16_t data)
{
    m_data = data;
}
uint16_t 
MyHeader::GetData (void) const
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
Receive (Packet p)
{
    MyHeader my;
    p.Peek (my);
    p.Remove (my);
    std::cout << "received data=" << my.GetData () << std::endl;
    struct MyTag myTag;
    p.PeekTag (myTag);
}


int main (int argc, char *argv[])
{
    Packet p;
    MyHeader my;
    my.SetData (2);
    std::cout << "send data=2" << std::endl;
    p.Add (my);
    struct MyTag myTag;
    myTag.m_streamId = 5;
    p.AddTag (myTag);
    Receive (p);
    return 0;
}
