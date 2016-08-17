mod_lttng
==============

mod_lttng is an Apache2 module that enables tracing Apache requests using LTTng (www.lttng.org) on Linux systems. 


Instructions
------------

1- First build the mod_lttng Apache module. To do so run this command:

sudo apxs -i -a -c src/mod_lttng.c -ldl -llttng-ust

2- Then make sure that mod_lttng is installed:

httpd -M 

-M command of httpd shows all installed and active Apache modules. So you should be able to see the mod_lttng in the list.

3- Restart apache

to run appache use this command:

sudo LD_PRELOAD=liblttng-ust-fork.so httpd


4- Start lttng tracing:

lttng create

lttng enable-events -u -a

lttng start

wget http://localhost

lttng stop

lttng view

That is it.!

