/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/un.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/route.h>
#include <netinet/in.h>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define noENABLE_LOG

#define EXIT_ERROR(x, err)						\
  std::cout << __FILE__ << ":" << __LINE__ << ": Unrecoverable Error: " << x; \
  std::cout << " errno=" << strerror (errno) << std::endl;		\
  exit (err)

#ifdef ENABLE_LOG
#define LOG(x) \
  std::cout << x << std::endl;
#else
#define LOG(x)
#endif


#define CHECK_ARG(el,var) \
  {							\
    char start[] = "--"  el  "=";		     	\
    if (strncmp (*argv, start, strlen (start)) == 0) 	\
      {						     	\
	var = *argv + strlen (start);		     	\
	LOG ("--" << el << "=" << var);		     	\
      }						     	\
  }

#define ASCII_DOT (0x2e)
#define ASCII_ZERO (0x30)
#define ASCII_a (0x41)
#define ASCII_z (0x5a)
#define ASCII_A (0x61)
#define ASCII_Z (0x7a)
#define ASCII_COLON (0x3a)
#define ASCII_ZERO (0x30)

static char
AsciiToLowCase (char c)
{
  if (c >= ASCII_a && c <= ASCII_z) {
    return c;
  } else if (c >= ASCII_A && c <= ASCII_Z) {
    return c + (ASCII_a - ASCII_A);
  } else {
    return c;
  }
}

static uint32_t 
AsciiToIpv4 (const char *address)
{
  uint32_t host = 0;
  while (true) {
    uint8_t byte = 0;
    while (*address != ASCII_DOT &&
           *address != 0) {
      byte *= 10;
      byte += *address - ASCII_ZERO;
      address++;
    }
    host <<= 8;
    host |= byte;
    if (*address == 0) {
      break;
    }
    address++;
  }
  return host;
}

static void 
AsciiToMac48 (const char *str, uint8_t addr[6])
{
  int i = 0;
  while (*str != 0 && i < 6) 
    {
      uint8_t byte = 0;
      while (*str != ASCII_COLON && *str != 0) 
	{
	  byte <<= 4;
	  char low = AsciiToLowCase (*str);
	  if (low >= ASCII_a) 
	    {
	      byte |= low - ASCII_a + 10;
	    } 
	  else 
	    {
	      byte |= low - ASCII_ZERO;
	    }
	  str++;
	}
      addr[i] = byte;
      i++;
      if (*str == 0) 
	{
	  break;
	}
      str++;
    }
}

static void
SetInetAddress (sockaddr *ad, uint32_t networkOrder)
{
  struct sockaddr_in *sin = (struct sockaddr_in*)ad;
  sin->sin_family = AF_INET;
  sin->sin_port = 0; // unused
  sin->sin_addr.s_addr = htonl (networkOrder);
}


static int
CreateTap (const char *mac_addr, const char *ip, 
	   const char *gw, const char *netmask)
{
  // opening the tun device usually requires root privs
  int tap = open ("/dev/net/tun", O_RDWR);
  if (tap == -1)
    {
      EXIT_ERROR ("Could not open /dev/net/tun", 1);
    }
  // now, crate a tap device.
  struct ifreq ifr;
  // make sure that the tap device will not send us the tun_pi header.
  ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
  ifr.ifr_name[0] = 0; // allow the kernel to pick a device name.
  int status = ioctl (tap, TUNSETIFF, (void *) &ifr);
  if (status == -1)
    {
      EXIT_ERROR ("Could not allocate a tap device", 2);
    }
  std::string tapDeviceName = (char *)ifr.ifr_name;
  LOG ("Allocated TAP device=" << tapDeviceName);

  // set its hardware address to something we know will be unique within the simulation
  ifr.ifr_hwaddr.sa_family = 1; // this is ARPHRD_ETHER from if_arp.h
  AsciiToMac48 (mac_addr, (uint8_t*)ifr.ifr_hwaddr.sa_data);
  status = ioctl (tap, SIOCSIFHWADDR, &ifr);
  if (status == -1)
    {
      EXIT_ERROR ("Could not set hardware address=" << mac_addr << " for=" << (char *)ifr.ifr_name, 3);
    }
  LOG ("device=" << (char *)ifr.ifr_name << " addr=" << mac_addr);


  // The ip address must be set using an AF_INET socket.
  int fd = socket (AF_INET, SOCK_DGRAM, 0);

  // set interface up.
  status = ioctl (fd, SIOCGIFFLAGS, &ifr);
  if (status == -1)
    {
      EXIT_ERROR ("Could not get flags for interface=" << (char *)ifr.ifr_name, 4);
    }
  ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
  status = ioctl (fd, SIOCSIFFLAGS, &ifr);
  if (status == -1)
    {
      EXIT_ERROR ("Could not bring interface " << (char *)ifr.ifr_name << " up", 5);
    }
  LOG ("device=" << (char *)ifr.ifr_name << " is up");


  // set its ip address.
  SetInetAddress (&ifr.ifr_addr, AsciiToIpv4 (ip));
  status = ioctl (fd, SIOCSIFADDR, &ifr);
  if (status == -1)
    {
      EXIT_ERROR ("Could not set ip address=" << ip << " for=" <<  (char *)ifr.ifr_name, 6);
    }
  LOG ("device=" << (char *)ifr.ifr_name << " addr=" << ip);

  // set its ip mask to be /32
  SetInetAddress (&ifr.ifr_netmask, 0xffffffff);
  status = ioctl (fd, SIOCSIFNETMASK, &ifr);
  if (status == -1)
    {
      EXIT_ERROR ("Could not set ip mask=" << netmask << " for=" <<  (char *)ifr.ifr_name, 7);
    }
  LOG ("device=" << (char *)ifr.ifr_name << " mask=" << netmask); 

  // add routing entry for gateway.
  struct rtentry rt;
  SetInetAddress (&rt.rt_dst, AsciiToIpv4 (gw));
  SetInetAddress (&rt.rt_genmask, 0xffffffff);
  rt.rt_flags = RTF_UP;
  rt.rt_metric = 2;
  rt.rt_dev = (char*)tapDeviceName.c_str ();
  status = ioctl (fd, SIOCADDRT, &rt);
  if (status == -1)
    {
      EXIT_ERROR ("Could not add routing table entry", 8);
    }
  LOG ("added routing table entry for gw.");

  // add routing entry for subnet through gateway.
  uint32_t network = AsciiToIpv4 (ip) & AsciiToIpv4 (netmask);
  SetInetAddress (&rt.rt_dst, network);
  SetInetAddress (&rt.rt_gateway, AsciiToIpv4 (gw));
  SetInetAddress (&rt.rt_genmask, AsciiToIpv4 (netmask));
  rt.rt_flags = RTF_UP | RTF_GATEWAY;
  rt.rt_metric = 2;
  rt.rt_dev = (char*)tapDeviceName.c_str ();
  status = ioctl (fd, SIOCADDRT, &rt);
  if (status == -1)
    {
      EXIT_ERROR ("Could not add routing table entry", 9);
    }
  LOG ("added routing table entry for subnet.");

  return tap;
}

static struct sockaddr_un
DecodeFromString (const char *path, socklen_t *len)
{
  sockaddr_un un;
  uint8_t *buffer = (uint8_t *)&un;
  std::istringstream iss;
  iss.str (path);
  uint8_t n = 0;
  while (!iss.bad () && !iss.eof () && !iss.fail ())
    {
      char c;
      iss.read (&c, 1);
      uint32_t tmp;
      iss >> std::hex >> tmp;
      //LOG (std::hex << tmp);
      buffer[n] = tmp;
      n++;
    }
  *len = n;
  return un;
}

static void
SendFd (const char *path, int fd)
{
  // send back configuration to caller.
  int sock = socket (PF_UNIX, SOCK_DGRAM, 0);
  if (sock == -1)
    {
      EXIT_ERROR ("Socket creation", 10);
    }
  LOG ("Socket Created");
  
  socklen_t local_len;
  struct sockaddr_un local = DecodeFromString (path, &local_len);
  LOG ("len=" << local_len);
  int status = connect (sock, (struct sockaddr*)&local, local_len);
  if (status == -1)
    {
      EXIT_ERROR ("Could not connect to caller", 11);
    }
  LOG ("Socket Connected");

  // we send a single byte whose content is meaningless.
  // we also return as ancillary data the tap file descriptor
  struct cmsghdr *cmsg;
  size_t msg_size = sizeof(int);
  char control[CMSG_SPACE(msg_size)];
  struct iovec iov;
  struct msghdr msg;
  char buffer = 0;
  iov.iov_base = &buffer;
  iov.iov_len = 1;
  msg.msg_name = 0;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = control;
  msg.msg_controllen = sizeof (control);
  msg.msg_flags = 0;

  cmsg = CMSG_FIRSTHDR(&msg);
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  cmsg->cmsg_len = CMSG_LEN(msg_size);
  msg.msg_controllen = cmsg->cmsg_len;

  int *fdptr = (int*) (CMSG_DATA(cmsg));
  *fdptr = fd;

  ssize_t len = sendmsg(sock, &msg, 0);
  if (len == -1)
    {
      EXIT_ERROR ("Could not send SCM_RIGHTS", 12);
    }
  LOG ("Sent SCM_RIGHTS");
}




int main (int argc, char *argv[])
{
  char *path = 0;
  char *mac_addr = 0;
  char *ip_addr = 0;
  char *ip_gw = 0;
  char *ip_netmask = 0;
  char *stop = 0;
  argv++;
  argc--;
  while (argc > 0)
    {
      CHECK_ARG("path", path);
      CHECK_ARG("mac-addr", mac_addr);
      CHECK_ARG("ip-addr", ip_addr);
      CHECK_ARG("ip-gw", ip_gw);
      CHECK_ARG("ip-netmask", ip_netmask);
      CHECK_ARG("stop", stop);
      argv++;
      argc--;
    }
  
  int tap = CreateTap (mac_addr, ip_addr, ip_gw, ip_netmask);

  if (stop)
    {
      while (1) {}
    }

  SendFd (path, tap);

  return 0;
}
