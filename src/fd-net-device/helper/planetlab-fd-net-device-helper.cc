/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 INRIA
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
 * Author: Alina Quereilhac <alina.quereilhac@inria.fr>
 *         Claudio Freire <klaussfreire@sourceforge.net>
 *
 */

#include "planetlab-fd-net-device-helper.h"
#include "encode-decode.h"

#include "ns3/abort.h"
#include "ns3/config.h"
#include "ns3/fd-net-device.h"
#include "ns3/log.h"
#include "ns3/names.h"
#include "ns3/object-factory.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/trace-helper.h"
#include "ns3/internet-module.h"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <iomanip>
#include <limits>
#include <linux/if_tun.h>
#include <memory>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netpacket/packet.h>

#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include <string>

NS_LOG_COMPONENT_DEFINE ("PlanetLabFdNetDeviceHelper");

namespace ns3 {

#define PLANETLAB_MAGIC 75867

PlanetLabFdNetDeviceHelper::PlanetLabFdNetDeviceHelper ()
{
  m_tapIp = Ipv4Address ("255.255.255.255");
  m_tapMask = Ipv4Mask ("255.255.255.255");
}

void
PlanetLabFdNetDeviceHelper::SetTapIpAddress (Ipv4Address address)
{
  m_tapIp = address;
}

void
PlanetLabFdNetDeviceHelper::SetTapMask (Ipv4Mask mask)
{
  m_tapMask = mask;
}

Ptr<NetDevice>
PlanetLabFdNetDeviceHelper::InstallPriv (Ptr<Node> node) const
{
  Ptr<NetDevice> d = FdNetDeviceHelper::InstallPriv (node);
  Ptr<FdNetDevice> device = d->GetObject<FdNetDevice> ();

  //
  // The PlanetLab mechanism to create a TAP device doesn't allow
  // for the moment to set the IFF_NOPI flag. In consequence, a PI
  // header will be present in the traffic.
  // We need to explicitly set the encapsulation mode to DIXPI,
  // so the FdNetDevice is able to treat correctly the traffic
  // traversing TAP device.
  //
  Ptr<FdNetDevice> fdnd = device->GetObject<FdNetDevice> ();
  fdnd->SetEncapsulationMode (FdNetDevice::DIXPI);

  SetFileDescriptor (device);
  return device;
}

void
PlanetLabFdNetDeviceHelper::SetFileDescriptor (Ptr<FdNetDevice> device) const
{
  NS_LOG_LOGIC ("Creating TAP device");

  //
  // Call out to a separate process running as suid root in order to create a
  // TAP device.  We do this to avoid having the entire simulation running as root.
  //
  int fd = CreateFileDescriptor ();
  device->SetFileDescriptor (fd);
}

int
PlanetLabFdNetDeviceHelper::CreateFileDescriptor (void) const
{
  NS_LOG_FUNCTION (this);

  //
  // We're going to fork and exec that program soon, but first we need to have
  // a socket to talk to it with.  So we create a local interprocess (Unix)
  // socket for that purpose.
  //
  int sock = socket (PF_UNIX, SOCK_DGRAM, 0);
  NS_ABORT_MSG_IF (sock == -1, "PlanetLabFdNetDeviceHelper::CreateFileDescriptor(): Unix socket creation error, errno = " << strerror (errno));

  //
  // Bind to that socket and let the kernel allocate an endpoint
  //
  struct sockaddr_un un;
  memset (&un, 0, sizeof (un));
  un.sun_family = AF_UNIX;
  int status = bind (sock, (struct sockaddr*)&un, sizeof (sa_family_t));
  NS_ABORT_MSG_IF (status == -1, "PlanetLabFdNetDeviceHelper::CreateFileDescriptor(): Could not bind(): errno = " << strerror (errno));
  NS_LOG_INFO ("Created Unix socket");
  NS_LOG_INFO ("sun_family = " << un.sun_family);
  NS_LOG_INFO ("sun_path = " << un.sun_path);

  //
  // We have a socket here, but we want to get it there -- to the program we're
  // going to exec.  What we'll do is to do a getsockname and then encode the
  // resulting address information as a string, and then send the string to the
  // program as an argument.  So we need to get the sock name.
  //
  socklen_t len = sizeof (un);
  status = getsockname (sock, (struct sockaddr*)&un, &len);
  NS_ABORT_MSG_IF (status == -1, "PlanetLabFdNetDeviceHelper::CreateFileDescriptor(): Could not getsockname(): errno = " << strerror (errno));

  //
  // Now encode that socket name (family and path) as a string of hex digits
  //
  std::string path = BufferToString ((uint8_t *)&un, len);
  NS_LOG_INFO ("Encoded Unix socket as \"" << path << "\"");

  //
  // Fork and exec the process to create our socket.  If we're us (the parent)
  // we wait for the child (the creator) to complete and read the socket it
  // created and passed back using the ancillary data mechanism.
  //
  pid_t pid = ::fork ();
  if (pid == 0)
    {
      NS_LOG_DEBUG ("Child process");

      //
      // build a command line argument from the encoded endpoint string that
      // the socket creation process will use to figure out how to respond to
      // the (now) parent process.  We're going to have to give this program
      // quite a bit of information.
      //
      // -i<IP-address> The IP address to assign to the new tap device;
      // -n<network-prefix> The network prefix to assign to the new tap device;
      // -t Set teh IFF_TAP flag
      // -p<path> the path to the unix socket described above.
      //
      // Example tap-creator -i1.2.3.1 -n24 -t -pblah
      //

      std::ostringstream ossIp;
      ossIp << "-i" << m_tapIp;

      std::ostringstream ossPrefix;
      ossPrefix << "-n" << m_tapMask.GetPrefixLength ();

      std::ostringstream ossMode;
      ossMode << "-t";

      std::ostringstream ossPath;
      ossPath << "-p" << path;

      //
      // Execute the socket creation process image.
      //
      status = ::execlp (PLANETLAB_TAP_CREATOR,
                         PLANETLAB_TAP_CREATOR,               // argv[0] (filename)
                         ossIp.str ().c_str (),               // argv[1] (-i<IP address>)
                         ossPrefix.str ().c_str (),           // argv[2] (-n<prefix>)
                         ossMode.str ().c_str (),             // argv[3] (-t <tap>)
                         ossPath.str ().c_str (),             // argv[4] (-p<path>)
                         (char *)NULL);

      //
      // If the execlp successfully completes, it never returns.  If it returns it failed or the OS is
      // broken.  In either case, we bail.
      //
      NS_FATAL_ERROR ("PlanetLabFdNetDeviceHelper::CreateFileDescriptor(): Back from execlp(), errno = " << ::strerror (errno));
    }
  else
    {
      NS_LOG_DEBUG ("Parent process");
      //
      // We're the process running the emu net device.  We need to wait for the
      // socket creator process to finish its job.
      //
      int st;
      pid_t waited = waitpid (pid, &st, 0);
      NS_ABORT_MSG_IF (waited == -1, "PlanetLabFdNetDeviceHelper::CreateFileDescriptor(): waitpid() fails, errno = " << strerror (errno));
      NS_ASSERT_MSG (pid == waited, "PlanetLabFdNetDeviceHelper::CreateFileDescriptor(): pid mismatch");

      //
      // Check to see if the socket creator exited normally and then take a
      // look at the exit code.  If it bailed, so should we.  If it didn't
      // even exit normally, we bail too.
      //
      if (WIFEXITED (st))
        {
          int exitStatus = WEXITSTATUS (st);
          NS_ABORT_MSG_IF (exitStatus != 0,
                           "PlanetLabFdNetDeviceHelper::CreateFileDescriptor(): socket creator exited normally with status " << exitStatus);
        }
      else
        {
          NS_FATAL_ERROR ("PlanetLabFdNetDeviceHelper::CreateFileDescriptor(): socket creator exited abnormally");
        }

      //
      // At this point, the socket creator has run successfully and should
      // have created our tap device, initialized it with the information we
      // passed and sent it back to the socket address we provided.  A socket
      // (fd) we can use to talk to this tap device should be waiting on the
      // Unix socket we set up to receive information back from the creator
      // program.  We've got to do a bunch of grunt work to get at it, though.
      //
      // The struct iovec below is part of a scatter-gather list.  It describes a
      // buffer.  In this case, it describes a buffer (an integer) that will
      // get the data that comes back from the socket creator process.  It will
      // be a magic number that we use as a consistency/sanity check.
      //
      struct iovec iov;
      uint32_t magic;
      iov.iov_base = &magic;
      iov.iov_len = sizeof(magic);

      //
      // The CMSG macros you'll see below are used to create and access control
      // messages (which is another name for ancillary data).  The ancillary
      // data is made up of pairs of struct cmsghdr structures and associated
      // data arrays.
      //
      // First, we're going to allocate a buffer on the stack to receive our
      // data array (that contains the socket).  Sometimes you'll see this called
      // an "ancillary element" but the msghdr uses the control message termimology
      // so we call it "control."
      //
      size_t msg_size = sizeof(int);
      char control[CMSG_SPACE (msg_size)];

      //
      // There is a msghdr that is used to minimize the number of parameters
      // passed to recvmsg (which we will use to receive our ancillary data).
      // This structure uses terminology corresponding to control messages, so
      // you'll see msg_control, which is the pointer to the ancillary data and
      // controllen which is the size of the ancillary data array.
      //
      // So, initialize the message header that describes the ancillary/control
      // data we expect to receive and point it to buffer.
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
      // Now we can actually receive the interesting bits from the tap
      // creator process.  Lots of pain to get four bytes.
      //
      ssize_t bytesRead = recvmsg (sock, &msg, 0);
      NS_ABORT_MSG_IF (bytesRead != sizeof(int), "PlanetLabFdNetDeviceHelper::CreateFileDescriptor(): Wrong byte count from socket creator");

      //
      // There may be a number of message headers/ancillary data arrays coming in.
      // Let's look for the one with a type SCM_RIGHTS which indicates it's the
      // one we're interested in.
      //
      struct cmsghdr *cmsg;
      for (cmsg = CMSG_FIRSTHDR (&msg); cmsg != NULL; cmsg = CMSG_NXTHDR (&msg, cmsg))
        {
          if (cmsg->cmsg_level == SOL_SOCKET
              && cmsg->cmsg_type == SCM_RIGHTS)
            {
              //
              // This is the type of message we want.  Check to see if the magic
              // number is correct and then pull out the socket we care about if
              // it matches
              //
              if (magic == PLANETLAB_MAGIC)
                {
                  NS_LOG_INFO ("Got SCM_RIGHTS with correct magic " << magic);
                  int *rawSocket = (int*)CMSG_DATA (cmsg);
                  NS_LOG_INFO ("Got the socket from the socket creator = " << *rawSocket);
                  return *rawSocket;
                }
              else
                {
                  NS_LOG_INFO ("Got SCM_RIGHTS, but with bad magic " << magic);
                }
            }
        }
      NS_FATAL_ERROR ("Did not get the raw socket from the socket creator");
    }

}

} // namespace ns3


