/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
  Network Simulation Cradle
  Copyright (C) 2003-2005 Sam Jansen

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc., 59
  Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#ifndef NSC_SIM_INTERFACE_H
#define NSC_SIM_INTERFACE_H

#define NSC_VERSION 0x000500

/**
 * \ingroup nsctcp
 * \brief Struct interface to NSC stack
 */
struct INetStack
{
  virtual ~INetStack() {}

  /**
   * \brief Initialize the stack
   * \param hz timer_interrupt frequency
   */
  virtual void init (int hz) = 0;

  /**
   * \brief Deliver complete packet to the NSC network stack
   * \param if_id interface ID
   * \param data data
   * \param datalen data length
   */
  virtual void if_receive_packet (int if_id, const void *data, int datalen) = 0;
  /**
   * \brief Send complete packet to the NSC network stack
   * \param data data
   * \param datalen data length
   */
  virtual void if_send_packet (const void *data, int datalen) = 0;
  /**
   * \brief Signal the completion of send procedure to the NSC network stack
   * \param if_id interface ID
   */
  virtual void if_send_finish (int if_id) = 0;

  /**
   * \brief Attach an interface to the stack
   * \param addr address
   * \param mask network mask
   * \param mtu MTU
   */
  virtual void if_attach (const char *addr, const char *mask, int mtu) = 0;
  /**
   * \brief Add a default gateway to the interface
   * \param addr gateway address
   */
  virtual void add_default_gateway (const char *addr) = 0;

  /**
   * \brief Returns the internal id of the stack instance.
   *
   * Purely for debugging/diagnostic purposes.
   * \return internal stack id
   */
  virtual int get_id () = 0;

  /**
   * \brief Return a short one-word name of the stack
   *
   * Should return a short one-word name of the stack. Eg. Linux 2.4.x ->
   * linux24, FreeBSD 5.x -> freebsd5. This can be used to identify output
   * from a stack, for example a packet trace file.
   * \return short one-word name of the stack
   */
  virtual const char *get_name () = 0;

  /**
   * \brief Get the timer_interrupt frequency
   *
   * This is used so the simulator can call the stack timer_interrupt function
   * the correct amount of times per second. For example, lwip has a hz of 10,
   * which it returns here to say that it's timer_interrupt should be called
   * 10 times a second. FreeBSD uses 100, as does Linux 2.4, while Linux 2.6
   * uses 1000. (This is often configurable in the kernel in question, also.)
   *
   * \return frequency
   */
  virtual int get_hz () = 0;

  /**
   * \brief The stack timer_interrupt function
   */
  virtual void timer_interrupt () = 0;

  /**
   * \brief Increment the time ticks
   */
  virtual void increment_ticks () = 0;

  /**
   * \brief Set the buffer size
   */
  virtual void buffer_size (int size) = 0;

  /**
   * \brief Create a new UDP socket
   */
  virtual struct INetDatagramSocket *new_udp_socket () { return NULL; }
  /**
   * \brief Create a new TCP socket
   */
  virtual struct INetStreamSocket *new_tcp_socket () { return NULL; }
  /**
   * \brief Create a new SCTP socket
   */
  virtual struct INetStreamSocket *new_sctp_socket () { return NULL; }

  // The following I've made optional to implement for now. Eases
  // integration of new features.
  /**
   * \brief use sysctl to modify system parameters
   * \param sysctl_name name of the parameter to modify
   * \param oldval old value
   * \param oldlenp old value length
   * \param newval new value
   * \param newlen new value length
   * \returns
   */
  virtual int sysctl (const char *sysctl_name, void *oldval, size_t *oldlenp,
                      void *newval, size_t newlen)
  {
    return -1;
  }

  // alternate, simpler interface. the stack cradle code is expected
  // to convert the string-value to something that the stack can handle.
  // The idea here is that this is a front-end to the sysctl(2) call,
  // much like the sysctl(8) program.
  /**
   * \brief Set system parameters using sysctl
   * \param name name of the parameter to modify
   * \param value new value
   * \returns
   */
  virtual int sysctl_set (const char *name, const char *value)
  {
    return -1;
  }

  // same as above, cradle code is expected to convert the sysctl value
  // into a string.
  // returns length of the string in value, i.e. retval > len: 'output truncated'.
  /**
   * \brief Get system parameters using sysctl
   * \param name name of the parameter to modify
   * \param value value
   * \param len value length
   * \returns length of the string in value, i.e. retval > len: 'output truncated'.
   */
  virtual int sysctl_get (const char *name, char *value, size_t len)
  {
    return -1;
  }

  /**
   * \brief Tell the cradle code to put the name of sysctl number 'idx' into name[].
   *
   * The idea is that this can be used to get a list of all available sysctls:
   * \verbatim
     char buf[256]
     for (i=0; sysctl_getnum(i, buf, sizeof(buf)) > 0 ;i++)
        puts(buf);
     \endverbatim
   *
   * \param idx index
   * \param name sysctl name
   * \param len sysctl length
   * \returns -1 if idx is out of range and the length of the sysctl name otherwise.
   */
  virtual int sysctl_getnum (size_t idx, char *name, size_t len)
  {
    return -1;
  }

  /**
   * \brief Show the NSC configuration
   */
  virtual void show_config ()
  {
    ;
  }

  /**
   * \brief Optional function to get variables for this stack
   * \param var the variable
   * \param result the result
   * \param result_len result length
   * \returns true on success
   */
  virtual bool get_var (const char *var, char *result, int result_len)
  {
    return false;
  }

  /**
   * \brief Optional function to set variables for this stack
   * \param var the variable
   * \param val the new value
   * \returns true on success
   */
  virtual bool set_var (const char *var, const char *val)
  {
    return false;
  }

  /**
   * \brief Set the level of debugging or diagnostic information to print out.
   *
   * This normally means kernel messages printed out during initialisation but
   * may also include extra debugging messages that are part of NSC.
   *
   * \param level debugging/diagnostic level
   */
  virtual void set_diagnostic (int level) {}

  /**
   * \brief Simple interface to support sending any textual command to a stack
   *
   * @returns 0 on success
   */
  virtual int cmd (const char *)
  {
    return 1;
  }
};

/**
 * \ingroup nsctcp
 * \brief Struct interface to NSC Stream (i.e., TCP) Sockets
 */
struct INetStreamSocket
{
  virtual ~INetStreamSocket() {}

  /**
   * \brief Connect to a remote peer
   */
  virtual void connect (const char *, int) = 0;
  /**
   * \brief Disconnect from a remote peer
   */
  virtual void disconnect () = 0;
  /**
   * \brief Put the socket in Listening state on a port
   */
  virtual void listen (int) = 0;
  /**
   * \brief Accept an incoming connection
   */
  virtual int accept (INetStreamSocket **) = 0;
  /**
   * \brief Send some data
   * \param data the data
   * \param datalen data length
   * \return the number of data sent or -1 on error
   */
  virtual int send_data (const void *data, int datalen) = 0;
  /**
   * \brief Read some data
   * \param buf the buffer to store the data
   * \param buflen buffer length
   * \return the number of data read or -1 on error
   */
  virtual int read_data (void *buf, int *buflen) = 0;
  /**
   * \brief Set the socket options
   *
   * We need to pass the option name in as a string here. The reason for
   * this is that different operating systems you compile on will have
   * different numbers defined for the constants SO_SNDBUF and so on.
   *
   * \param optname name of the option
   * \param val option value
   * \param valsize size of the option value
   * \returns
   */
  virtual int setsockopt (char *optname, void *val, size_t valsize) = 0;
  /**
   * \brief Print the socket state
   */
  virtual void print_state (FILE *) = 0;
  /**
   * \brief Check the connection state
   * \returns true if socket is in connected state
   */
  virtual bool is_connected () = 0;
  /**
   * \brief Check the listening state
   * \returns true if socket is in listening state
   */
  virtual bool is_listening () = 0;
  /**
   * \brief Get the peer name
   *
   * \note not implemented
   *
   * \param sa sockaddr structure to fill
   * \param salen sockaddr structure length
   * \returns -1 on error (always returns -1)
   */
  virtual int getpeername (struct sockaddr *sa, size_t *salen) {
    return -1;
  }
  /**
   * \brief Get the socket local name
   *
   * \note not implemented
   *
   * \param sa sockaddr structure to fill
   * \param salen sockaddr structure length
   * \returns -1 on error (always returns -1)
   */
  virtual int getsockname (struct sockaddr *sa, size_t *salen) {
    return -1;
  }
  /**
   * \brief Optional function used to get variables for this TCP connection.
   *
   * \note not implemented
   *
   * \param var variable requested
   * \param result result result
   * \param result_len result length
   * \return always false
   */
  virtual bool get_var (const char *var, char *result, int result_len)
  {
    return false;
  }
  /**
   * \brief Optional function used to set variables for this TCP connection.
   *
   * \note not implemented
   *
   * \param var variable to set
   * \param val value to set
   * \return always false
   */
  virtual bool set_var (const char *var, const char *val)
  {
    return false;
  }
};

/**
 * \ingroup nsctcp
 * \brief Struct interface to NSC Datagram (i.e., UDP) Sockets
 */
struct INetDatagramSocket
{
  virtual ~INetDatagramSocket() {}

  /**
   * \brief Set the destination address and port
   */
  virtual void set_destination (const char *, int) = 0;
  /**
   * \brief Send a datagram
   * \param data the data
   * \param datalen data length
   */
  virtual void send_data (const void *data, int datalen) = 0;
};

/**
 * \ingroup nsctcp
 * \brief Struct interface to NSC send capabilities
 */
struct ISendCallback
{
  virtual ~ISendCallback() {}

  /**
   * \brief Invoked by NSCs 'ethernet driver' to re-inject a packet into ns-3.
   */
  virtual void send_callback (const void *data, int datalen) = 0;
};

/**
 * \ingroup nsctcp
 * \brief Struct interface to NSC soft interrupt capabilities
 */
struct IInterruptCallback
{
  virtual ~IInterruptCallback() {}

  /**
   * \brief Called by the NSC stack whenever something of interest has happened
   */
  virtual void wakeup () = 0;
  /**
   * \brief Get the actual time
   */
  virtual void gettime (unsigned int *, unsigned int *) = 0;
};

typedef int (*FRandom)();
typedef INetStack *(*FCreateStack)(ISendCallback *, IInterruptCallback *, 
                                   FRandom);

#define CREATE_STACK_FUNC(a,b,c) extern "C" INetStack *nsc_create_stack ( \
    ISendCallback *a, IInterruptCallback *b, FRandom c)

#endif /* NSC_SIM_INTERFACE_H */
