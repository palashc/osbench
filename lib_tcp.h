/*
 * tcp_lib.c - routines for managing TCP connections.
 *
 * Positive port/program numbers are RPC ports, negative ones are TCP ports.
 *
 * Copyright (c) 1994-1996 Larry McVoy.
 */
#include	<assert.h>
#include    <ctype.h>
#include    <stdio.h>
#include    <errno.h>
#include	<rpc/rpc.h>
#include	<rpc/types.h>
#include    <sys/types.h>
#include    <stdlib.h>
#include    <fcntl.h>
#include    <signal.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include	<arpa/inet.h>
#include    <sys/stat.h>
#include    <sys/wait.h>
#include	<time.h>
#include    <sys/time.h>
#include    <sys/un.h>
#include    <sys/resource.h>
#include 	<stdarg.h>
#define	ulong	unsigned long

#define	TCP_SELECT	(u_long)404038	/* XXX - unregistered */
#define	TCP_XACT	(u_long)404039	/* XXX - unregistered */
#define	TCP_CONTROL	(u_long)404040	/* XXX - unregistered */
#define	TCP_DATA	(u_long)404041	/* XXX - unregistered */
#define	TCP_CONNECT	(u_long)404042	/* XXX - unregistered */
#define	UDP_XACT 	(u_long)404032	/* XXX - unregistered */
#define	UDP_DATA 	(u_long)404033	/* XXX - unregistered */
#define	VERS		(u_long)1

#define	SOCKOPT_READ	0x0001
#define	SOCKOPT_WRITE	0x0002
#define	SOCKOPT_RDWR	0x0003
#define	SOCKOPT_PID	0x0004
#define	SOCKOPT_REUSE	0x0008
#define	SOCKOPT_NONE	0
#define	SOCKBUF		(1024*1024)


int	tcp_server(int prog, int rdwr);
int	tcp_done(int prog);
int	tcp_accept(int sock, int rdwr);
int	tcp_connect(char *host, int prog, int rdwr);
void sock_optimize(int sock, int rdwr);
int	sockport(int s);
/*
 * Get a TCP socket, bind it, figure out the port,
 * and advertise the port as program "prog".
 *
 * XXX - it would be nice if you could advertise ascii strings.
 */
int
tcp_server(int prog, int rdwr)
{
	int	sock;
	struct	sockaddr_in s;

	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket");
		exit(1);
	}
	sock_optimize(sock, rdwr);
	bzero((void*)&s, sizeof(s));
	s.sin_family = AF_INET;
	if (prog < 0) {
		s.sin_port = htons(-prog);
	}
	if (bind(sock, (struct sockaddr*)&s, sizeof(s)) < 0) {
		perror("bind");
		exit(2);
	}
	if (listen(sock, 100) < 0) {
		perror("listen");
		exit(4);
	}
	if (prog > 0) {
		pmap_unset((u_long)prog, (u_long)1);
		if (!pmap_set((u_long)prog, (u_long)1, (u_long)IPPROTO_TCP,
		    (unsigned short)sockport(sock))) {
			perror("pmap_set");
			exit(5);
		}
	}
	return (sock);
}

/*
 * Unadvertise the socket
 */
int
tcp_done(int prog)
{
	if (prog > 0) {
		pmap_unset((u_long)prog, (u_long)1);
	}
	return (0);
}

/*
 * Accept a connection and return it
 */
int
tcp_accept(int sock, int rdwr)
{
	struct	sockaddr_in s;
	int	newsock, namelen;

	namelen = sizeof(s);
	bzero((void*)&s, namelen);

retry:
	if ((newsock = accept(sock, (struct sockaddr*)&s, &namelen)) < 0) {
		if (errno == EINTR)
			goto retry;
		perror("accept");
		exit(6);
	}
	sock_optimize(newsock, rdwr);
	return (newsock);
}

/*
 * Connect to the TCP socket advertised as "prog" on "host" and
 * return the connected socket.
 *
 * Hacked Thu Oct 27 1994 to cache pmap_getport calls.  This saves
 * about 4000 usecs in loopback lat_connect calls.  I suppose we
 * should time gethostbyname() & pmap_getprot(), huh?
 */
int
tcp_connect(char *host, int prog, int rdwr)
{
	static	struct hostent *h;
	static	struct sockaddr_in s;
	static	u_short	save_port;
	static	u_long save_prog;
	static	char *save_host;
	int	sock;
	static	int tries = 0;

	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket");
		exit(1);
	}
	if (rdwr & SOCKOPT_PID) {
		static	unsigned short port;
		struct sockaddr_in sin;

		if (!port) {
			port = (unsigned short)(getpid() << 4);
			if (port < 1024) {
				port += 1024;
			}
		}
		do {
			port++;
			bzero((void*)&sin, sizeof(sin));
			sin.sin_family = AF_INET;
			sin.sin_port = htons(port);
		} while (bind(sock, (struct sockaddr*)&sin, sizeof(sin)) == -1);
	}
	sock_optimize(sock, rdwr);
	if (!h || host != save_host || prog != save_prog) {
		save_host = host;	/* XXX - counting on them not
					 * changing it - benchmark only.
					 */
		save_prog = prog;
		if (!(h = gethostbyname(host))) {
			perror(host);
			exit(2);
		}
		bzero((void *) &s, sizeof(s));
		s.sin_family = AF_INET;
		bcopy((void*)h->h_addr, (void *)&s.sin_addr, h->h_length);
		if (prog > 0) {
			save_port = pmap_getport(&s, prog,
			    (u_long)1, IPPROTO_TCP);
			if (!save_port) {
				perror("lib TCP: No port found");
				exit(3);
			}
			s.sin_port = htons(save_port);
		} else {
			s.sin_port = htons(-prog);
		}
	}
	if (connect(sock, (struct sockaddr*)&s, sizeof(s)) < 0) {
		if (errno == ECONNRESET 
		    || errno == ECONNREFUSED
		    || errno == EAGAIN) {
			close(sock);
			if (++tries > 10) return(-1);
			return (tcp_connect(host, prog, rdwr));
		}
		perror("connect");
		exit(4);
	}
	tries = 0;
	return (sock);
}

void
sock_optimize(int sock, int flags)
{
	if (flags & SOCKOPT_READ) {
		int	sockbuf = SOCKBUF;

		while (setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &sockbuf,
		    sizeof(int))) {
			sockbuf >>= 1;
		}

	}
	if (flags & SOCKOPT_WRITE) {
		int	sockbuf = SOCKBUF;

		while (setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sockbuf,
		    sizeof(int))) {
			sockbuf >>= 1;
		}

	}
	if (flags & SOCKOPT_REUSE) {
		int	val = 1;
		if (setsockopt(sock, SOL_SOCKET,
		    SO_REUSEADDR, &val, sizeof(val)) == -1) {
			perror("SO_REUSEADDR");
		}
	}
}

int
sockport(int s)
{
	int	namelen;
	struct sockaddr_in sin;

	namelen = sizeof(sin);
	if (getsockname(s, (struct sockaddr *)&sin, &namelen) < 0) {
		perror("getsockname");
		return(-1);
	}
	return ((int)ntohs(sin.sin_port));
}
