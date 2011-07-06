/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) University of Washington
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
 */

#include <unistd.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>

#include "emu-encode-decode.h"

#define EMU_MAGIC 65867

static int gVerbose = 0;

#define LOG(msg) \
  if (gVerbose) \
    { \
      std::cout << __FUNCTION__ << "(): " << msg << std::endl;   \
    }

#define ABORT(msg, printErrno) \
  std::cout << __FILE__ << ": fatal error at line " << __LINE__ << ": " << __FUNCTION__ << "(): " << msg << std::endl; \
  if (printErrno) \
    { \
      std::cout << "    errno = " << errno << " (" << strerror (errno) << ")" << std::endl; \
    } \
  exit (-1); 

#define ABORT_IF(cond, msg, printErrno) \
  if (cond) \
    { \
      ABORT (msg, printErrno); \
    }

/**
 * \brief Send the socket file descriptor we created back to the emu device, 
 * which will read it as soon as we're done.
 *
 * \param path The socket address information from the Unix socket we use
 * to send the created socket back to.
 * \param fd The socket we're going to send. 
 */
static void
SendSocket (const char *path, int fd)
{
  //
  // Open a Unix (local interprocess) socket to call back to the emu net
  // device.
  //
  LOG ("Create Unix socket");
  int sock = socket (PF_UNIX, SOCK_DGRAM, 0);
  ABORT_IF (sock == -1, "Unable to open socket", 1);

  //
  // We have this string called path, which is really a hex representation
  // of the endpoint that the net device created.  It used a forward encoding
  // method (EmuBufferToString) to take the sockaddr_un it made and passed 
  // the resulting string to us.  So we need to take the inverse method
  // (EmuStringToBuffer) and build the same sockaddr_un over here.
  //
  socklen_t clientAddrLen;
  struct sockaddr_un clientAddr;

  LOG ("Decode address " << path);
  bool rc = ns3::EmuStringToBuffer (path, (uint8_t *)&clientAddr, &clientAddrLen);
  ABORT_IF (rc == false, "Unable to decode path", 0);

  LOG ("Connect");
  int status = connect (sock, (struct sockaddr*)&clientAddr, clientAddrLen);
  ABORT_IF (status == -1, "Unable to connect to emu device", 1);

  LOG ("Connected");

  //
  // This is arcane enough that a few words are worthwhile to explain what's 
  // going on here.
  //
  // The interesting information (the socket FD) is going to go back to the
  // emu net device as an integer of ancillary data.  Ancillary data is bits 
  // that are not a part a socket payload (out-of-band data).  We're also 
  // going to send one integer back.  It's just initialized to a magic number
  // we use to make sure that the emu device is talking to the emu socket 
  // creator and not some other creator process.
  //
  // The struct iovec below is part of a scatter-gather list.  It describes a
  // buffer.  In this case, it describes a buffer (an integer) containing the
  // data that we're going to send back to the emu net device (that magic 
  // number).
  // 
  struct iovec iov;
  uint32_t magic = EMU_MAGIC;
  iov.iov_base = &magic;
  iov.iov_len = sizeof(magic);

  //
  // The CMSG macros you'll see below are used to create and access control 
  // messages (which is another name for ancillary data).  The ancillary 
  // data is made up of pairs of struct cmsghdr structures and associated
  // data arrays.
  // 
  // First, we're going to allocate a buffer on the stack to contain our 
  // data array (that contains the socket).  Sometimes you'll see this called
  // an "ancillary element" but the msghdr uses the control message termimology
  // so we call it "control."
  //
  size_t msg_size = sizeof(int);
  char control[CMSG_SPACE (msg_size)];

  //
  // There is a msghdr that is used to minimize the number of parameters
  // passed to sendmsg (which we will use to send our ancillary data).  This
  // structure uses terminology corresponding to control messages, so you'll
  // see msg_control, which is the pointer to the ancillary data and controllen
  // which is the size of the ancillary data array.
  //
  // So, initialize the message header that describes our ancillary/control data
  // and point it to the control message/ancillary data we just allocated space
  // for.
  //
  struct msghdr msg;
  msg.msg_name = 0;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = control;
  msg.msg_controllen = sizeof (control);
  msg.msg_flags = 0;

  //
  // A cmsghdr contains a length field that is the length of the header and
  // the data.  It has a cmsg_level field corresponding to the originating 
  // protocol.  This takes values which are legal levels for getsockopt and
  // setsockopt (here SOL_SOCKET).  We're going to use the SCM_RIGHTS type of 
  // cmsg, that indicates that the ancillary data array contains access rights 
  // that we are sending back to the emu net device.
  //
  // We have to put together the first (and only) cmsghdr that will describe
  // the whole package we're sending.
  //
  struct cmsghdr *cmsg;
  cmsg = CMSG_FIRSTHDR (&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN (msg_size);
  //
  // We also have to update the controllen in case other stuff is actually
  // in there we may not be aware of (due to macros).
  //
  msg.msg_controllen = cmsg->cmsg_len;

  //
  // Finally, we get a pointer to the start of the ancillary data array and
  // put our file descriptor in.
  //
  int *fdptr = (int*)(CMSG_DATA (cmsg));
  *fdptr = fd; // 

  //
  // Actually send the file descriptor back to the emulated net device.
  //
  ssize_t len = sendmsg (sock, &msg, 0);
  ABORT_IF (len == -1, "Could not send socket back to emu net device", 1);

  LOG ("sendmsg complete");
}

int
main (int argc, char *argv[])
{
  int c;
  char *path = NULL;

  opterr = 0;

  while ((c = getopt (argc, argv, "vp:")) != -1)
    {
      switch (c)
        {
        case 'v':
          gVerbose = true;
          break;
        case 'p':
          path = optarg;
          break;
        }
    }

  //
  // This program is spawned by an emu net device running in a simulation.  It
  // wants to create a raw socket as described below.  We are going to do the
  // work here since we're running suid root.  Once we create the raw socket,
  // we have to send it back to the emu net device.  We do that over a Unix
  // (local interprocess) socket.  The emu net device created a socket to 
  // listen for our response on, and it is expected to have encoded the address
  // information as a string and to have passed that string as an argument to
  // us.  We see it here as the "path" string.  We can't do anything useful 
  // unless we have that string.
  //
  ABORT_IF (path == NULL, "path is a required argument", 0);
  LOG ("Provided path is \"" << path << "\"");
  //
  // The whole reason for all of the hoops we went through to call out to this
  // program will pay off here.  We created this program to run as suid root
  // in order to keep the main simulation program from having to be run with
  // root privileges.  We need root privileges to be able to open a raw socket
  // though.  So all of these hoops are to allow us to exeucte the following
  // single line of code:
  //
  LOG ("Creating raw socket");
  int sock = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL));
  ABORT_IF (sock == -1, "CreateSocket(): Unable to open raw socket", 1);

  //
  // Send the socket back to the emu net device so it can go about its business
  //
  SendSocket (path, sock);

  return 0;
}
