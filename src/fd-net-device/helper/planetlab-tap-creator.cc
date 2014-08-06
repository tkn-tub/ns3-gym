/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 INRIA
 *
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

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h> // for strerror
#include <iostream>
#include <iomanip>
#include <sstream>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <net/route.h>
#include <netinet/in.h>

#include "creator-utils.h"

#define PLANETLAB_MAGIC 75867

#define VSYS_TUNTAP "/vsys/fd_tuntap.control"
#define VSYS_VIFUP_IN "/vsys/vif_up.in"
#define VSYS_VIFUP_OUT "/vsys/vif_up.out"

using namespace ns3;

/**
 *
 * Reads vif FD from "fd", writes interface name to vif_name, and returns vif FD.
 * vif_name should be IFNAMSIZ chars long.
 *
 */
int
ReceiveVifFd (int fd, char *vif_name)
{
  struct msghdr msg;
  struct iovec iov;
  int rv;
  size_t ccmsg[CMSG_SPACE (sizeof(int)) / sizeof(size_t)];
  struct cmsghdr *cmsg;

  /* Use IOV to read interface name */
  iov.iov_base = vif_name;
  iov.iov_len = IFNAMSIZ;

  msg.msg_name = 0;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  /* old BSD implementations should use msg_accrights instead of
   * msg_control; the interface is different. */
  msg.msg_control = ccmsg;
  msg.msg_controllen = sizeof(ccmsg);

  while (((rv = recvmsg (fd, &msg, 0)) == -1) && errno == EINTR)
    {
    }
  ABORT_IF (rv == -1, "Could not receive fd from Vsys", 0);
  ABORT_IF (!rv, "Could not receive fd from Vsys (EOF)", 0);

  cmsg = CMSG_FIRSTHDR (&msg);
  ABORT_IF (!cmsg->cmsg_type == SCM_RIGHTS, "got control message of unknown type" << cmsg->cmsg_type, 0);

  int* retfd  = (int*)CMSG_DATA (cmsg);
  return *retfd;
}

/**
 *
 * Creates a TUN/TAP device on a PlanetLab virtual machine (sliver).
 * PlanetLab provides the Vsys API to enable privileged operating system calls
 * in an environment with restricted privileges.
 * To create a TUN/TAP device the it is necessary to connect to the
 * /vsys/fd_tuntap.control socket, and send the device initialization parameters.
 *
 */
int
TunAlloc (int iftype, char *if_name)
{
  int control_fd;
  struct sockaddr_un addr;
  int ret;

  control_fd = socket (AF_UNIX, SOCK_STREAM, 0);
  ABORT_IF (control_fd == -1, "Could not create UNIX socket", 0);

  memset (&addr, 0, sizeof(struct sockaddr_un));

  /* Clear structure */
  addr.sun_family = AF_UNIX;
  strncpy (addr.sun_path, VSYS_TUNTAP, sizeof(addr.sun_path) - 1);

  ret = connect (control_fd, (struct sockaddr *) &addr,
                 sizeof(struct sockaddr_un));
  ABORT_IF (ret == -1, "Could not connect to Vsys control socket", 0);

  /* passing type param */
  ret = send (control_fd, &iftype, sizeof(iftype), 0);
  ABORT_IF (ret != sizeof(iftype), "Could not send paramater to Vsys control socket", 0);

  return ReceiveVifFd (control_fd, if_name);

}

/**
 *
 * Sets the TAP/TUN interface up.
 * When a TAP/TUN device is created on PlanetLab it must be set up
 * writing some configuration information to the vsys fifo at /vsys/vif_up.in
 *
 */
void
SetTunUp (const char *ip, const char *prefix, const char *if_name)
{
  FILE *in;
  FILE *out;
  char errbuff[4096];

  memset(errbuff, 0, 4096);

  in = fopen (VSYS_VIFUP_IN, "a");

  if (in == NULL)
    {
      ABORT_IF (in == NULL, "Failed to open " << VSYS_VIFUP_IN, 0);
    }

  out = fopen (VSYS_VIFUP_OUT, "r");

  if (out == NULL)
    {
      ABORT_IF (out == NULL, "Failed to open " << VSYS_VIFUP_OUT, 0);
    }

  // send input to process
  fprintf (in, "%s\n%s\n%s\nsnat=1\n", if_name, ip, prefix);

  // close pipe to indicate end parameter passing and flush the fifo
  fclose (in);

  fread((void*)errbuff, 4096, 1, out);
 
  // the error buffer will not be empty if we read an error
  ABORT_IF (strcmp(errbuff, "") != 0, errbuff, 0);

  fclose (out);
}

int
main (int argc, char *argv[])
{
  int c;
  char *ip = NULL;
  char *prefix = NULL;
  char *path = NULL;
  int iftype = IFF_TUN;
  char if_name[4096];

  memset(if_name, 0, 4096);
  opterr = 0;

  while ((c = getopt (argc, argv, "vi:n:tp:")) != -1)
    {
      switch (c)
        {
        case 'i':
          ip = optarg;            // ip address of the new device
          break;
        case 'n':
          prefix = optarg;        // prefix for the new device
          break;
        case 't':
          iftype = IFF_TAP;       // mode for the device (TAP or TUN)
          break;
        case 'p':
          path = optarg;          // path back to the tap bridge
          break;
        case 'v':
          gVerbose = true;
          break;
        }
    }

  ABORT_IF (ip == NULL, "IP Address is a required argument", 0);
  LOG ("Provided IP Address is \"" << ip << "\"");

  ABORT_IF (prefix == NULL, "Prefix is a required argument", 0);
  LOG ("Provided prefix \"" << prefix << "\"");

  ABORT_IF (path == NULL, "path is a required argument", 0);
  LOG ("Provided path is \"" << path << "\"");

  LOG ("Creating Tap");
  // allocate a TAP device in planetLab
  int fd = TunAlloc (iftype, if_name);
  ABORT_IF (fd == -1, "main(): Unable to create tap device", 1);

  // set the TAP interface up
  SetTunUp (ip, prefix, (const char*)if_name);

  //
  // Send the socket back to the tap net device so it can go about its business
  //
  SendSocket (path, fd, PLANETLAB_MAGIC);

  return 0;
}
