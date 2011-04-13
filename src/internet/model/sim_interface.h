#ifndef __SIM_INTERFACE_H__
#define __SIM_INTERFACE_H__
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

#define NSC_VERSION 0x000500

struct INetStack
{
    virtual ~INetStack() {}

    virtual void init(int hz) = 0;
    
    virtual void if_receive_packet(int if_id, const void *data, int datalen) = 0;
    
    virtual void if_send_packet(const void *data, int datalen) = 0;	
    virtual void if_send_finish(int if_id) = 0;

    virtual void if_attach(const char *addr, const char *mask, int mtu) = 0;
    virtual void add_default_gateway(const char *addr) = 0;

    /** Purely for debugging/diagnostic purposes. This returns the internal id
     * of the stack instance.
     */
    virtual int get_id() = 0;

    /** Should return a short one-word name of the stack. Eg. Linux 2.4.x ->
     * linux24, FreeBSD 5.x -> freebsd5. This can be used to identify output
     * from a stack, for example a packet trace file. */
    virtual const char *get_name() = 0;

    /** This is used so the simulator can call the stack timer_interrupt function
     * the correct amount of times per second. For example, lwip has a hz of 10,
     * which it returns here to say that it's timer_interrupt should be called
     * 10 times a second. FreeBSD uses 100, as does Linux 2.4, while Linux 2.6
     * uses 1000. (This is often configurable in the kernel in question, also.)
     */
    virtual int get_hz() = 0;

    virtual void timer_interrupt() = 0;
    virtual void increment_ticks() = 0;

    virtual void buffer_size(int size) = 0;
    
    virtual struct INetDatagramSocket *new_udp_socket() { return NULL; }
    virtual struct INetStreamSocket *new_tcp_socket() { return NULL; }
    virtual struct INetStreamSocket *new_sctp_socket() { return NULL; }

    // The following I've made optional to implement for now. Eases
    // integration of new features.
    virtual int sysctl(const char *sysctl_name, void *oldval, size_t *oldlenp,
        void *newval, size_t newlen)
    {
        return -1;
    }

    // alternate, simpler interface. the stack cradle code is expected
    // to convert the string-value to something that the stack can handle.
    // The idea here is that this is a front-end to the sysctl(2) call,
    // much like the sysctl(8) program.
    virtual int sysctl_set(const char *name, const char *value)
    {
        return -1;
    }

    // same as above, cradle code is expected to convert the sysctl value
    // into a string.
    // returns length of the string in value, i.e. retval > len: 'output truncated'.
    virtual int sysctl_get(const char *name, char *value, size_t len)
    {
        return -1;
    }

    // this tells the cradle code to put the name of sysctl number 'idx'
    // into name[].
    // The idea is that this can be used to get a list of all available sysctls:
    // char buf[256]
    // for (i=0; sysctl_getnum(i, buf, sizeof(buf)) > 0 ;i++)
    //    puts(buf);
    // returns -1 if idx is out of range and the length of the sysctl name otherwise.
    virtual int sysctl_getnum(size_t idx, char *name, size_t len)
    {
        return -1;
    }

    virtual void show_config()
    {
        ;
    }

    /* Optional functions used to get and set variables for this stack */
    virtual bool get_var(const char *var, char *result, int result_len)
    {
        return false;
    }
    
    virtual bool set_var(const char *var, const char *val)
    {
        return false;
    }

    /** The level of debugging or diagnostic information to print out. This 
     * normally means kernel messages printed out during initialisation but
     * may also include extra debugging messages that are part of NSC. */
    virtual void set_diagnostic(int level) {}

    /** Simple interface to support sending any textual command to a stack 
     *
     * @returns 0 on success
     */
    virtual int cmd(const char *) 
    {
        return 1;
    }
};

struct INetStreamSocket
{
    virtual ~INetStreamSocket() {}

    virtual void connect(const char *, int) = 0;
    virtual void disconnect() = 0;
    virtual void listen(int) = 0;
    virtual int accept(INetStreamSocket **) = 0;
    virtual int send_data(const void *data, int datalen) = 0;
    virtual int read_data(void *buf, int *buflen) = 0;
    /* We need to pass the option name in as a string here. The reason for
     * this is that different operating systems you compile on will have
     * different numbers defined for the constants SO_SNDBUF and so on. */
    virtual int setsockopt(char *optname, void *val, size_t valsize) = 0;
    virtual void print_state(FILE *) = 0;
    virtual bool is_connected() = 0;
    virtual bool is_listening() = 0;

    virtual int getpeername(struct sockaddr *sa, size_t *salen) {
	    return -1;
    }
    virtual int getsockname(struct sockaddr *sa, size_t *salen) {
	    return -1;
    }
    /* Optional functions used to get and set variables for this TCP
     * connection. */
    virtual bool get_var(const char *var, char *result, int result_len)
    {
        return false;
    }
    
    virtual bool set_var(const char *var, const char *val)
    {
        return false;
    }
};

struct INetDatagramSocket
{
    virtual ~INetDatagramSocket() {}

    virtual void set_destination(const char *, int) = 0;
    virtual void send_data(const void *data, int datalen) = 0;
};
struct ISendCallback
{
    virtual ~ISendCallback() {}

    virtual void send_callback(const void *data, int datalen) = 0;
};

struct IInterruptCallback
{
    virtual ~IInterruptCallback() {}

    virtual void wakeup() = 0;
    virtual void gettime(unsigned int *, unsigned int *) = 0;
};

typedef int (*FRandom)();
typedef INetStack *(*FCreateStack)(ISendCallback *, IInterruptCallback *, 
        FRandom);

#define CREATE_STACK_FUNC(a,b,c) extern "C" INetStack *nsc_create_stack(\
        ISendCallback *a, IInterruptCallback *b, FRandom c)

#endif
