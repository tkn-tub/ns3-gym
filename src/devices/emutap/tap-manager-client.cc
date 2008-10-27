#include "tap-manager-client.h"
#include "ns3/log.h"
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <linux/un.h>
#include <errno.h>
#include <string.h>
#include <iomanip>
#include <iostream>
#include <list>


NS_LOG_COMPONENT_DEFINE("TapManagerClient");

#define TAP_MANAGER "ns3-tap-manager"

namespace ns3 {

static std::string
EncodeAsString (struct sockaddr_un un, int len)
{
  uint8_t *buffer = (uint8_t *)&un;
  std::ostringstream oss;
  oss.setf (std::ios::hex, std::ios::basefield);
  oss.fill('0');
  for (uint8_t i = 0; i < len; i++)
    {
      oss << ":" << std::setw (2) << (uint32_t)buffer[i];
    }
  return oss.str ();
}

bool
TapManagerClient::Exists (std::string filename) const
{
  struct stat st;
  int retval = ::stat (filename.c_str (), &st);
  return retval == 0;
}

std::string
TapManagerClient::FindManager (void) const
{
  std::list<std::string> locations;
  locations.push_back ("./src/devices/tap");
  locations.push_back ("./build/debug/src/devices/tap");
  locations.push_back ("./build/optimized/src/devices/tap");
  for (std::list<std::string>::const_iterator i = locations.begin (); i != locations.end (); ++i)
    {
      if (Exists (*i + "/" + TAP_MANAGER))
	{
	  return *i + "/" + TAP_MANAGER;
	}
    }
  NS_FATAL_ERROR ("Could not find manager");
  return ""; // quiet compiler
}

int
TapManagerClient::AllocateTap (Mac48Address host, Ipv4Address ad, Ipv4Mask mask, Ipv4Address gateway)
{
  NS_LOG_FUNCTION (host << ad << mask << gateway);
  // create a socket to get information back from the tap manager.
  int sock = socket (PF_UNIX, SOCK_DGRAM, 0);
  if (sock == -1)
    {
      NS_FATAL_ERROR ("Socket creation, errno=" << strerror (errno));
    }

  struct sockaddr_un un;
  memset (&un, 0, sizeof (un));
  un.sun_family = AF_UNIX;
  int status = bind (sock, (struct sockaddr*)&un, sizeof (sa_family_t)); // let the kernel allocate an endpoint for us.
  if (status == -1)
    {
      NS_FATAL_ERROR ("Could not bind: errno=" << strerror (errno));
    }
  socklen_t len = sizeof (un);
  status = getsockname (sock, (struct sockaddr*)&un, &len);
  if (status == -1)
    {
      NS_FATAL_ERROR ("Could not get socket address: errno=" << strerror (errno));
    }
  NS_LOG_DEBUG ("Allocated enpoint=" << EncodeAsString (un, len) << ", len=" << len);

  pid_t pid = ::fork ();
  if (pid == 0)
    {
      // child.
      NS_LOG_DEBUG ("Child");

      std::ostringstream oss;
      oss << "--path=" << EncodeAsString (un, len);
      std::string pathArg = oss.str ();
      oss.str ("");
      oss << "--mac-addr=" << host;
      std::string hostArg = oss.str ();
      oss.str ("");
      oss << "--ip-addr=" << ad;
      std::string ipArg = oss.str ();
      oss.str ("");
      oss << "--ip-gw=" << gateway;
      std::string ipGw = oss.str ();
      oss.str ("");
      oss << "--ip-netmask=" << mask;
      std::string ipMask = oss.str ();
      oss.str ("");
      status = ::execl (FindManager ().c_str (), 
			TAP_MANAGER, 
			pathArg.c_str (),
			hostArg.c_str (),
			ipArg.c_str (),
			ipGw.c_str (),
			ipMask.c_str (),
			(char *)NULL);
      if (status == -1)
	{
	  NS_LOG_ERROR ("Cannot execl tap-manager, errno=" << ::strerror (errno));
	}
      ::_exit (-1);
    }
  else
    {
      // parent
      NS_LOG_DEBUG ("Parent");
      int st;
      pid_t waited = waitpid (pid, &st, 0);
      if (waited == -1)
	{
	  NS_FATAL_ERROR ("Cannot wait for tap-manager, errno=" << strerror (errno));
	}
      NS_ASSERT (pid == waited);
      if (!WIFEXITED (st))
	{
	  // tap manager did not complete successfully
	  NS_FATAL_ERROR ("tap-manager did not exit correctly");
	}
      else if (WEXITSTATUS (st) != 0)
	{
	  NS_FATAL_ERROR ("tap-manager did not complete successfully, err=" << WEXITSTATUS (st));
	}
      // the tap fd should be available on our unix socket now.
      size_t msg_size = sizeof(int);
      char control[CMSG_SPACE(msg_size)];
      struct cmsghdr *cmsg;
      uint8_t buffer;
      struct iovec iov;
      iov.iov_base = &buffer;
      iov.iov_len = 1;
      struct msghdr msg;
      msg.msg_name = 0;
      msg.msg_namelen = 0;
      msg.msg_iov = &iov;
      msg.msg_iovlen = 1;
      msg.msg_control = control;
      msg.msg_controllen = sizeof (control);
      msg.msg_flags = 0;
      ssize_t bytesRead = recvmsg (sock, &msg, 0);
      if (bytesRead != 1)
	{
	  NS_FATAL_ERROR ("Did not get byte from tap-manager");
	}
      NS_LOG_ERROR ("read bytes=" << bytesRead);
      for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL; cmsg = CMSG_NXTHDR(&msg, cmsg)) 
	{
	  if (cmsg->cmsg_level == SOL_SOCKET &&
	      cmsg->cmsg_type == SCM_RIGHTS)
	    {
	      int *fd = (int*)CMSG_DATA (cmsg);
	      NS_LOG_ERROR ("got tap fd=" << *fd);
	      return *fd;
	    }
	}
      NS_FATAL_ERROR ("Did not get SCM_RIGHTS from tap-manager");
    }
  return -1;
}

} // namespace ns3
