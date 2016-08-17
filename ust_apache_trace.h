#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER ust_apache
 
#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "ust_apache_trace.h"
 
#if !defined(_UST_APACHE_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _UST_APACHE_H
#include <stdint.h>
#include <unistd.h>
#include <lttng/ust-events.h>
#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(ust_apache,
    request_entry,   
    TP_ARGS(int ,connection_id, const char*, client_ip,  const char*, method, const char*, uri,  const char*, protocol, char *, request_info),
    TP_FIELDS(
        ctf_integer(int, connection_id, connection_id)
        ctf_string(client_ip, client_ip)
        ctf_string(method, method)
        ctf_string(uri, uri)
        ctf_string(protocol, protocol)
        ctf_string(request_info, request_info)
        )
)

TRACEPOINT_EVENT(ust_apache,
    request_exit,   
    TP_ARGS(int, connection_id, int, status),
    TP_FIELDS(
        ctf_integer(int, connection_id, connection_id)
        ctf_integer(int, status, status)
    )
)

TRACEPOINT_EVENT(ust_apache,
    accept_connection,   
    TP_ARGS(int, connection_id, const char *, client_ip, const char *, client_hostname, const char *, local_ip,const char *, local_hostname),
    TP_FIELDS(
        ctf_integer(int, connection_id, connection_id)
        ctf_string(client_ip, client_ip)
        ctf_string(client_hostname, client_hostname)
        ctf_string(local_ip, local_ip)
        ctf_string(local_hostname, local_hostname)
    )
)

TRACEPOINT_EVENT(ust_apache,
    close_connection,   
    TP_ARGS(int, connection_id),
    TP_FIELDS(
        ctf_integer(int, connection_id, connection_id)
    )
)

TRACEPOINT_EVENT(ust_apache,
    suspend_connection,   
    TP_ARGS(int, connection_id),
    TP_FIELDS(
        ctf_integer(int, connection_id, connection_id)
    )
)

TRACEPOINT_EVENT(ust_apache,
    resume_connection,   
    TP_ARGS(int, connection_id),
    TP_FIELDS(
        ctf_integer(int, connection_id, connection_id)
    )
)

TRACEPOINT_EVENT(ust_apache,
    apache_module_entry,
    TP_ARGS(const char *, name),
    TP_FIELDS(
        ctf_string(name, name)
    )
 )

TRACEPOINT_EVENT(ust_apache,
    apache_module_invoke,
    TP_ARGS(const char *, name, const char*, src),
    TP_FIELDS(
        ctf_string(name, name)
        ctf_string(src, src)
    )
)

TRACEPOINT_EVENT(ust_apache,
    apache_module_complete,
    TP_ARGS(const char *, name, const char*, src, int, result),
    TP_FIELDS(
        ctf_string(name, name)
        ctf_string(src, src)
        ctf_integer(int, result, result)
    )
)

TRACEPOINT_EVENT(ust_apache,
    apache_module_exit,
    TP_ARGS(const char *, name, int, result),
    TP_FIELDS(
        ctf_string(name, name)
        ctf_integer(int, result, result)
    )
 )

#endif /* _UST_APACHE_H */
#include <lttng/tracepoint-event.h>

