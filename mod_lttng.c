/* 
* License:
* Copyright 2004 The Apache Software Foundation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Module Name: mod_lttng
*
* Purpose: Apache implements a set of hooks and filters to allow 
*          modules to view and modify requests sent to the server.
*          This module takes advantage of Apache hooks and filters to 
*          implement several LTTng ust hooks.
*
* Last Modified: 16-08-2016
*
* Author: Naser Ezzati (n.ezzati@polymtl.ca)
*
* Version: 0.9
*
*/

#define TRACEPOINT_CREATE_PROBES
#define TRACEPOINT_DEFINE

#include <apr_strings.h>
#include <apr_base64.h>
#include <apr_lib.h>

#include "httpd.h"
#include "http_config.h"
#include "http_connection.h"
#include "http_protocol.h"
#include "http_request.h"

#include "ust_apache_trace.h"

#include <lttng/tracepoint.h>
#include <lttng/tracelog.h>


module AP_MODULE_DECLARE_DATA lttng_module;

static int apache_request_entry(request_rec *r);
static int apache_request_exit(request_rec *r);
static int apache_log_request(request_rec *r);
static int apache_accept_connection(conn_rec *c, void *csd);
static int apache_close_connection(conn_rec *c);
static int apache_suspend_connection(conn_rec *c);
static int apache_resume_connection(conn_rec *c);



static int apache_request_entry(request_rec *r)
{

    if (r == NULL)
        return 0;

    long id = -1;
    if (r->connection != NULL)
    {   
        id = r->connection->id;
    }
    const char *host = NULL;
    if (r->headers_in != NULL)
    {   
        host = r->useragent_ip;
        //host = apr_table_get(r->headers_in, "Host");
    }

    const char *content_length = NULL;
    if (r->headers_in != NULL)
    {   
        content_length = apr_table_get(r->headers_in, "Content-Length");
    }

    const char *userAgent = NULL;
    if (r->headers_in != NULL)
    {   
        userAgent = apr_table_get(r->headers_in, "User-Agent");
    }

    const char *redirectLocation = NULL;
    if (r->headers_out != NULL) 
    {
        redirectLocation = apr_table_get(r->headers_out, "Location");
    }

    const char *handler = r->handler;
    const char *theRequest = r->the_request;
    const char *protocol = r->protocol;
    const char *host2 = r->hostname;
    const char *method = r->method;
    const char *contentType = r->content_type;
    const char *path = r->path_info;
    const char *uri = r->uri;
    const char *filename = r->filename;
    const char *canonical = r->canonical_filename;
    const char *args = r->args;
    const char *user = r->user;
    const char *ap_auth_type = r->ap_auth_type;

    //char *req_info = apr_psprintf(r->pool, " ");
    char *req_info = "";

    if (theRequest != NULL && apr_strnatcmp(theRequest, "") != 0) {
        req_info = apr_psprintf(r->pool, "%s request:'%s' ", req_info, theRequest);
    }

    if (user != NULL && apr_strnatcmp(user, "") != 0) {
        req_info = apr_psprintf(r->pool, "%s user:'%s'", req_info, user);
    }

    if (path != NULL && apr_strnatcmp(path, "") != 0) {
        req_info = apr_psprintf(r->pool, "%s path: '%s'", req_info, path);
    }
    if (contentType != NULL && apr_strnatcmp(contentType, "") != 0) {
        req_info = apr_psprintf(r->pool, "%s content_type: '%s'", req_info, contentType);
    }
    if (content_length != NULL && apr_strnatcmp(content_length, "") != 0) {
        req_info = apr_psprintf(r->pool, "%s content_length: '%s'", req_info, content_length);
    }
    if (filename != NULL && apr_strnatcmp(filename, "") != 0) {
        req_info = apr_psprintf(r->pool, "%s filename: '%s'", req_info, filename);
    }
    if (canonical != NULL && apr_strnatcmp(canonical, "") != 0) {
        req_info = apr_psprintf(r->pool, "%s canonical_filename: '%s'", req_info, canonical);
    }
    if (userAgent != NULL && apr_strnatcmp(userAgent, "") != 0) {
        req_info = apr_psprintf(r->pool, "%s user_agent: '%s'", req_info, userAgent);
    }
    if (redirectLocation != NULL && apr_strnatcmp(redirectLocation, "") != 0) {
        req_info = apr_psprintf(r->pool, "%s redirect_location: '%s'", req_info, redirectLocation);
    }
    if (args != NULL && apr_strnatcmp(args, "") != 0) {
        req_info = apr_psprintf(r->pool, "%s args: '%s'", req_info, args);
    }
    if (ap_auth_type != NULL && apr_strnatcmp(ap_auth_type, "") != 0) {
        req_info = apr_psprintf(r->pool, "%s ap_auth_type': '%s'", req_info, ap_auth_type);
    }

    tracepoint(ust_apache, request_entry, id, host, method, uri, protocol, req_info);
    return DECLINED;
}

static int apache_request_exit(request_rec *r)
{
    long id = -1;
    if (r->connection != NULL)
    {   
        id = r->connection->id;
    }
    int status = r->status;

    tracepoint(ust_apache, request_exit, id, status);
    return DECLINED;
}

static int apache_log_request(request_rec *r)
{
    long id = -1;
    if (r->connection != NULL)
         {
             id = r->connection->id;
         }
    int status = r->status;
    
    tracepoint(ust_apache, request_exit, id, status);
    return DECLINED;
}

static int apache_accept_connection(conn_rec *c, void *csd)
{
    const char *client_host = c->remote_host;
    const char *client_ip = c->client_ip;
    const char *local_ip = c->local_ip ;
    const char *local_host = c->local_host ;
    const long id = c->id;

    tracepoint(ust_apache, accept_connection, id, client_ip, client_host, local_ip, local_host);

    return DECLINED;
}

static int apache_close_connection(conn_rec *c)
{
    const long id = c->id;
    tracepoint(ust_apache, close_connection, id);

    return DECLINED;
}

static int apache_suspend_connection(conn_rec *c)
{
    const long id = c->id;
    tracepoint(ust_apache, suspend_connection, id);

    return DECLINED;
}

static int apache_resume_connection(conn_rec *c)
{
    const long id = c->id;
    tracepoint(ust_apache, resume_connection, id);

    return DECLINED;
}

void lttng_register_hooks(apr_pool_t *p)
{
    ap_hook_post_read_request(apache_request_entry, NULL,NULL, APR_HOOK_FIRST);
    ap_hook_pre_connection(apache_accept_connection, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_pre_close_connection(apache_close_connection, NULL, NULL, APR_HOOK_MIDDLE);
    //ap_hook_resume_connection(apache_resume_connection, NULL, NULL, APR_HOOK_MIDDLE);
    //ap_hook_suspend_connection(apache_suspend_connection, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_log_transaction(apache_request_exit,NULL,NULL,APR_HOOK_FIRST);
}

module AP_MODULE_DECLARE_DATA lttng_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    lttng_register_hooks
};
