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
#include "ns3/wall-clock-ms.h"
#include "ns3/packet.h"
#include "ns3/chunk-constant-data.h"
#include "ns3/chunk-udp.h"
#include "ns3/chunk-ipv4.h"
#include <iostream>

using namespace ns3;

static void 
benchPtrA (uint32_t n)
{
    ChunkConstantData data = ChunkConstantData (2000, 1);
    ChunkUdp udp;
    ChunkIpv4 ipv4;

    for (uint32_t i = 0; i < n; i++) {
        Packet p;
        p.add (&data);
        p.add (&udp);
        p.add (&ipv4);
        Packet o = p;
        o.peek (&ipv4);
        o.remove (&ipv4);
        o.peek (&udp);
        o.remove (&udp);
        o.peek (&data);
        o.remove (&data);
    }
}

static void 
benchPtrB (uint32_t n)
{
    ChunkConstantData data = ChunkConstantData (2000, 1);
    ChunkUdp udp;
    ChunkIpv4 ipv4;

    for (uint32_t i = 0; i < n; i++) {
        Packet p;
        p.add (&data);
        p.add (&udp);
        p.add (&ipv4);
    }
}

static void
ptrC2 (Packet p)
{
    ChunkConstantData data = ChunkConstantData (2000, 1);
    ChunkUdp udp;

    p.peek (&udp);
    p.remove (&udp);
    p.peek (&data);
    p.remove (&data);
}

static void 
ptrC1 (Packet p)
{
    ChunkIpv4 ipv4;
    p.peek (&ipv4);
    p.remove (&ipv4);
    ptrC2 (p);
}

static void
benchPtrC (uint32_t n)
{
    ChunkConstantData data = ChunkConstantData (2000, 1);
    ChunkUdp udp;
    ChunkIpv4 ipv4;

    for (uint32_t i = 0; i < n; i++) {
        Packet p;
        p.add (&data);
        p.add (&udp);
        p.add (&ipv4);
        ptrC1 (p);
    }
}


static void
runBench (void (*bench) (uint32_t), uint32_t n, char const *name)
{
    WallClockMs time;
    time.start ();
    (*bench) (n);
    unsigned long long deltaMs = time.end ();
    double ps = n;
    ps *= 1000;
    ps /= deltaMs;
    std::cout << name<<"=" << ps << " packets/s" << std::endl;
}

int main (int argc, char *argv[])
{
    uint32_t n = 0;
    while (argc > 0) {
        if (strncmp ("--n=", argv[0],strlen ("--n=")) == 0) {
            char const *nAscii = argv[0] + strlen ("--n=");
            n = atoi (nAscii);
        }
        argc--;
        argv++;
    }

    runBench (&benchPtrA, n, "a");
    runBench (&benchPtrB, n, "b");
    runBench (&benchPtrC, n, "c");

    return 0;
}
