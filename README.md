mod_lttng
==============

mod_lttng is an Apache2 module that enables tracing Apache requests using LTTng (www.lttng.org) on Linux systems. 


Instructions
------------

1- First build the mod_lttng Apache module. To do so run this command:

sudo apxs -i -a -c src/mod_lttng.c -ldl -llttng-ust

2- Then make sure that mod_lttng is installed:

httpd -M 
or 
apache2ctl -M

-M command of httpd shows all installed and active Apache modules. So you should be able to see the lttng_module in the list.

3- Restart Apache

to run Apache use this command:

sudo LD_PRELOAD=liblttng-ust-fork.so httpd
or
sudo LD_PRELOAD=liblttng-ust-fork.so httpd systemctl restart apache2

4- Start lttng tracing:

lttng create

lttng enable-event -u -a

lttng start

wget http://localhost

lttng stop

lttng view

That is it.!

