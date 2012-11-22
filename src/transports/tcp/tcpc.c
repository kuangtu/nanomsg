/*
    Copyright (c) 2012 250bpm s.r.o.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom
    the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include "tcpc.h"

#include "../../utils/err.h"
#include "../../utils/cont.h"
#include "../../utils/addr.h"

#include <string.h>

/*  Implementation of sp_epbase interface. */
static int sp_tcpc_close (struct sp_epbase *self, int linger);
static const struct sp_epbase_vfptr sp_tcpc_epbase_vfptr =
    {sp_tcpc_close};

/*  cp_io_hndl callbacks. */
static void sp_tcpc_received (struct sp_usock *self, size_t len);
static void sp_tcpc_sent (struct sp_usock *self, size_t len);
static void sp_tcpc_connected (struct sp_usock *self);
static void sp_tcpc_accepted (struct sp_usock *self, int s);
static void sp_tcpc_err (struct sp_usock *self, int errnum);
static const struct sp_usock_vfptr sp_tcpc_usock_vfptr = {
    sp_tcpc_received,
    sp_tcpc_sent,
    sp_tcpc_connected,
    sp_tcpc_accepted,
    sp_tcpc_err
};

int sp_tcpc_init (struct sp_tcpc *self, const char *addr, void *hint)
{
    int rc;
    int port;
    const char *colon;
    struct sockaddr_storage ss;
    socklen_t sslen;

    /*  Make sure we're working from a clean slate. Required on Mac OS X. */
    memset (&ss, 0, sizeof (ss));

    /*  Parse the port. */
    rc = sp_addr_parse_port (addr, &colon);
    if (rc < 0)
        return rc;
    port = rc;

    /*  TODO: Parse the local address, if any. */

    /*  Parse the address. */
    /*  TODO:  Get the actual value of the IPV4ONLY socket option. */
    rc = sp_addr_parse_remote (addr, colon - addr, SP_ADDR_IPV4ONLY,
        &ss, &sslen);
    if (rc < 0)
        return rc;

    /*  Combine the port and the address. */
    if (ss.ss_family == AF_INET)
        ((struct sockaddr_in*) &ss)->sin_port = htons (port);
    else if (ss.ss_family == AF_INET6)
        ((struct sockaddr_in6*) &ss)->sin6_port = htons (port);
    else
        sp_assert (0);

    /*  Initialise the base class. */
    sp_epbase_init (&self->epbase, &sp_tcpc_epbase_vfptr, hint);

    /*  Open the socket and start connecting. */
    rc = sp_usock_init (&self->usock, &sp_tcpc_usock_vfptr,
        AF_INET, SOCK_STREAM, IPPROTO_TCP, sp_epbase_getcp (&self->epbase));
    errnum_assert (rc == 0, -rc);
    rc = sp_usock_connect (&self->usock, (struct sockaddr*) &ss, sslen);
    errnum_assert (rc == 0, -rc);

    return 0;
}

static int sp_tcpc_close (struct sp_epbase *self, int linger)
{
    struct sp_tcpc *tcpc;

    tcpc = sp_cont (self, struct sp_tcpc, epbase);
    sp_assert (0);
}

static void sp_tcpc_received (struct sp_usock *self, size_t len)
{
    /*  The socket is never used for receiving. */
    sp_assert (0);
}

static void sp_tcpc_sent (struct sp_usock *self, size_t len)
{
    /*  The socket is never used for sending. */
    sp_assert (0);
}

static void sp_tcpc_connected (struct sp_usock *self)
{
    /*  TODO */
    sp_assert (0);
}

static void sp_tcpc_accepted (struct sp_usock *self, int s)
{
    /*  The socket is never used for accepting new connections. */
    sp_assert (0);
}

static void sp_tcpc_err (struct sp_usock *self, int errnum)
{
    /*  TODO */
    sp_assert (0);
}
