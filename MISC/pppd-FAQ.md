pppd 的常见问题

This is a list of Frequently Asked Questions about using ppp-2.x and their answers.



Q: Can you give me an example of how I might set up my machine to dial out to an ISP?



A: Here's an example for dialling out to an ISP via a modem on /dev/tty02.  The modem uses hardware (CTS/RTS) flow control, and the
serial port is run at 38400 baud.  The ISP assigns our IP address.

To configure pppd for this connection, create a file under /etc/ppp/peers called (say) my-isp containing the following:

	tty02 crtscts 38400
	connect 'chat -v -f /etc/ppp/chat/my-isp'
	defaultroute

The ppp connection is then initiated using the following command:

	pppd call my-isp

When you run this, pppd will use the chat program to dial the ISP and invoke its ppp service.  Chat will read the file specified with -f, namely /etc/ppp/chat/my-isp, to find a list of strings to expect to receive, and strings to send.  This file would contain something like this:

	ABORT "NO CARRIER"
	ABORT "NO DIALTONE"
	ABORT "ERROR"
	ABORT "NO ANSWER"
	ABORT "BUSY"
	ABORT "Username/Password Incorrect"
	"" "at"
	OK "at&d2&c1"
	OK "atdt2479381"
	"name:" "^Uusername"
	"word:" "\qpassword"
	"annex" "\q^Uppp"
	"Switching to PPP-ppp-Switching to PPP"

You will need to change the details here.  The first string on each line is a string to expect to receive; the second is the string to send.  You can add or delete lines according to the dialog required to access your ISP's system.  This example is for a modem with a standard AT command set, dialling out to an Annex terminal server.  The \q toggles "quiet" mode; when quiet mode is on, the strings to be sent are replaced by ?????? in the log.  You may need to go through the dialog manually using kermit or tip first to determine what should go in the script.

To terminate the link, run the following script, called (say) kill-ppp:

	#!/bin/sh
	unit=ppp${1-0}
	piddir=/var/run
	if [ -f $piddir/$unit.pid ]; then
	  kill -1 `cat $piddir/$unit.pid`
	fi

On some systems (SunOS, Solaris, Ultrix), you will need to change /var/run to /etc/ppp.


------------------------------------------------------------------------

Q: Can you give me an example of how I could set up my office machine
so I can dial in to it from home?

A: Let's assume that the office machine is called "office" and is on a
local ethernet subnet.  Call the home machine "home" and give it an IP
address on the same subnet as "office".  We'll require both machines
to authenticate themselves to each other.

Set up the files on "office" as follows:

/etc/ppp/options contains:

auth		# require the peer to authenticate itself
lock
# other options can go here if desired

/etc/ppp/chap-secrets contains:

home	office	"beware the frub-jub"	home
office	home	"bird, my son!%&*"	-

Set up a modem on a serial port so that users can dial in to the
modem and get a login prompt.

On "home", set up the files as follows:

/etc/ppp/options contains the same as on "office".

/etc/ppp/chap-secrets contains:

home	office	"beware the frub-jub"	-
office	home	"bird, my son!%&*"	office

Create a file called /etc/ppp/peers/office containing the following:

	tty02 crtscts 38400
	connect 'chat -v -f /etc/ppp/chat/office'
	defaultroute

(You may need to change some of the details here.)

Create the /etc/ppp/chat/office file containing the following:

	ABORT "NO CARRIER"
	ABORT "NO DIALTONE"
	ABORT "ERROR"
	ABORT "NO ANSWER"
	ABORT "BUSY"
	ABORT "ogin incorrect"
	"" "at"
	OK "at&d2&c1"
	OK "atdt2479381"
	"name:" "^Uusername"
	"word:" "\qpassword"
	"$" "\q^U/usr/sbin/pppd proxyarp"
	"~"

You will need to change the details.  Note that the "$" in the
second-last line is expecting the shell prompt after a successful
login - you may need to change it to "%" or something else.

You then initiate the connection (from home) with the command:

	pppd call office

------------------------------------------------------------------------

Q: When I try to establish a connection, the modem successfully dials
the remote system, but then hangs up a few seconds later.  How do I
find out what's going wrong?

A: There are a number of possible problems here.  The first thing to
do is to ensure that pppd's messages are visible.  Pppd uses the
syslog facility to log messages which help to identify specific
problems.  Messages from pppd have facility "daemon" and levels
ranging from "debug" to "error".

Usually it is useful to see messages of level "notice" or higher on
the console.  To see these, find the line in /etc/syslog.conf which
has /dev/console on the right-hand side, and add "daemon.notice" in
the list on the left.  The line will end up looking something like
this:

*.err;kern.debug;auth.notice;mail.crit;daemon.notice	/dev/console

Note that the whitespace is tabs, *not* spaces.

If you are having problems, it may be useful to see messages of level
"info" as well, in which case you would change "daemon.notice" to
"daemon.info".

In addition, it is useful to collect pppd's debugging output in a
file - the debug option to pppd causes it to log the contents of all
control packets sent and received in human-readable form.  To do this,
add a line like this to /etc/syslog.conf:

daemon,local2.debug		/etc/ppp/log

and create an empty /etc/ppp/log file.

When you change syslog.conf, you will need to send a HUP signal to
syslogd to causes it to re-read syslog.conf.  You can do this with a
command like this (as root):

	kill -HUP `cat /etc/syslogd.pid`

(On some systems, you need to use /var/run/syslog.pid instead of
/etc/syslogd.pid.)

After setting up syslog like this, you can use the -v flag to chat and
the `debug' option to pppd to get more information.  Try initiating
the connection again; when it fails, inspect /etc/ppp/log to see what
happened and where the connection failed.


------------------------------------------------------------------------

Q: When I try to establish a connection, I get an error message saying
"Serial link is not 8-bit clean".  Why?

A: The most common cause is that your connection script hasn't
successfully dialled out to the remote system and invoked ppp service
there.  Instead, pppd is talking to something (a shell or login
process on the remote machine, or maybe just the modem) which is only
outputting 7-bit characters.

This can also arise with a modem which uses an AT command set if the
dial command is issued before pppd is invoked, rather than within a
connect script started by pppd.  If the serial port is set to 7
bits/character plus parity when the last AT command is issued, the
modem serial port will be set to the same setting.

Note that pppd *always* sets the local serial port to 8 bits per
character, with no parity and 1 stop bit.  So you shouldn't need to
issue an stty command before invoking pppd.


------------------------------------------------------------------------

Q: When I try to establish a connection, I get an error message saying
"Serial line is looped back".  Why?

A: Probably your connection script hasn't successfully dialled out to
the remote system and invoked ppp service there.  Instead, pppd is
talking to something which is just echoing back the characters it
receives.  The -v option to chat can help you find out what's going
on.  It can be useful to include "~" as the last expect string to
chat, so chat won't return until it's seen the start of the first PPP
frame from the remote system.

Another possibility is that your phone connection has dropped for some
obscure reason and the modem is echoing the characters it receives
from your system.


------------------------------------------------------------------------

Q: I installed pppd successfully, but when I try to run it, I get a
message saying something like "peer authentication required but no
authentication files accessible".

A: When pppd is used on a machine which already has a connection to
the Internet (or to be more precise, one which has a default route in
its routing table), it will require all peers to authenticate
themselves.  The reason for this is that if you don't require
authentication, you have a security hole, because the peer can
basically choose any IP address it wants, even the IP address of some
trusted host (for example, a host mentioned in some .rhosts file).

On machines which don't have a default route, pppd does not require
the peer to authenticate itself.  The reason is that such machines
would mostly be using pppd to dial out to an ISP which will refuse to
authenticate itself.  In that case the peer can use any IP address as
long as the system does not already have a route to that address.
For example, if you have a local ethernet network, the peer can't use
an address on that network.  (In fact it could if it authenticated
itself and it was permitted to use that address by the pap-secrets or
chap-secrets file.)

There are 3 ways around the problem:

1. If possible, arrange for the peer to authenticate itself, and
create the necessary secrets files (/etc/ppp/pap-secrets and/or
/etc/ppp/chap-secrets).

2. If the peer refuses to authenticate itself, and will always be
using the same IP address, or one of a small set of IP addresses, you
can create an entry in the /etc/ppp/pap-secrets file like this:

  ""	  *	  ""	  his-ip.his-domain his-other-ip.other-domain

(that is, using the empty string for the client name and password
fields).  Of couse, you replace the 4th and following fields in the
example above with the IP address(es) that the peer may use.  You can
use either hostnames or numeric IP addresses.

3. You can add the `noauth' option to the /etc/ppp/options file.
Pppd will then not ask the peer to authenticate itself.  If you do
this, I *strongly* recommend that you remove the set-uid bit from the
permissions on the pppd executable, with a command like this:

	chmod u-s /usr/sbin/pppd

Then, an intruder could only use pppd maliciously if they had already
become root, in which case they couldn't do any more damage using pppd
than they could anyway.


------------------------------------------------------------------------

Q: What do I need to put in the secrets files?

A: Three things:
   - secrets (i.e. passwords) to use for authenticating this host to
     other hosts (i.e., for proving our identity to others);
   - secrets which other hosts can use for authenticating themselves
     to us (i.e., so that they can prove their identity to us); and
   - information about which IP addresses other hosts may use, once
     they have authenticated themselves.

There are two authentication files: /etc/ppp/pap-secrets, which
contains secrets for use with PAP (the Password Authentication
Protocol), and /etc/ppp/chap-secrets, which contains secrets for use
with CHAP (the Challenge Handshake Authentication Protocol).  Both
files have the same simple format, which is as follows:

- The file contains a series of entries, each of which contains a
secret for authenticating one machine to another.

- Each entry is contained on a single logical line.  A logical line
may be continued across several lines by placing a backslash (\) at
the end of each line except the last.

- Each entry has 3 or more fields, separated by whitespace (spaces
and/or tabs).  These fields are, in order:
	* The name of the machine that is authenticating itself
	  (the "client").
	* The name of the machine that is authenticating the client
	  (the "server").
	* The secret to be used for authenticating that client to that
	  server.  If this field begins with the at-sign `@', the rest
	  of the field is taken as the name of a file containing the
	  actual secret.
	* The 4th and any following fields list the IP address(es)
	  that the client may use.

- The file may contain comments, which begin with a `#' and continue
to the end of the line.

- Double quotes `"' should be used around a field if it contains
characters with special significance, such as space, tab, `#', etc.

- The backslash `\' may be used before characters with special
significance (space, tab, `#', `\', etc.) to remove that significance.

Some important points to note:

* A machine can be *both* a "client" and a "server" for the purposes
of authentication - this happens when both peers require the other to
authenticate itself.  So A would authenticate itself to B, and B would
also authenticate itself to A (possibly using a different
authentication protocol).

* If both the "client" and the "server" are running ppp-2.x, they need
to have a similar entry in the appropriate secrets file; the first two
fields are *not* swapped on the client, compared to the server.  So
the client might have an entry like this:

	ay	bee	"our little secret"	-

and the corresponding entry on the server could look like this:

	ay	bee	"our little secret"	123.45.67.89


------------------------------------------------------------------------

Q: Explain about PAP and CHAP?

PAP stands for the Password Authentication Protocol.  With this
protocol, the "client" (the machine that needs to authenticate itself)
sends its name and a password, in clear text, to the "server".  The
server returns a message indicating whether the name and password are
valid.

CHAP stands for the Challenge Handshake Authentication Protocol.  It
is designed to address some of the deficiencies and vulnerabilities of
PAP.  Like PAP, it is based on the client and server having a shared
secret, but the secret is never passed in clear text over the link.
Instead, the server sends a "challenge" - an arbitrary string of
bytes, and the client must prove it knows the shared secret by
generating a hash value from the challenge combined with the shared
secret, and sending the hash value back to the server.  The server
also generates the hash value and compares it with the value received
from the client.

At a practical level, CHAP can be slightly easier to configure than
PAP because the server sends its name with the challenge.  Thus, when
finding the appropriate secret in the secrets file, the client knows
the server's name.  In contrast, with PAP, the client has to find its
password (i.e. the shared secret) before it has received anything from
the server.  Thus, it may be necessary to use the `remotename' option
to pppd when using PAP authentication so that it can select the
appropriate secret from /etc/ppp/pap-secrets.

Microsoft also has a variant of CHAP which uses a different hashing
arrangement from normal CHAP.  There is a client-side (authenticatee)
implementation of Microsoft's CHAP in ppp-2.3; see README.MSCHAP80.
In ppp-2.4.2, server-side (authenticator) support was added as well as
support for Microsoft CHAP v2; see README.MSCHAP81.


------------------------------------------------------------------------

Q: When the modem hangs up, without the remote system having
terminated the connection properly, pppd does not notice the hangup,
but just keeps running.  How do I get pppd to notice the hangup and
exit?

A: Pppd detects modem hangup by looking for an end-of-file indication
from the serial driver, which should be generated when the CD (carrier
detect) signal on the serial port is deasserted.  For this to work:

- The modem has to be set to assert CD when the connection is made and
deassert it when the phone line hangs up.  Usually the AT&C1 modem
command sets this mode.

- The cable from the modem to the serial port must connect the CD
signal (on pin 8).

- Some serial drivers have a "software carrier detect" mode, which
must be *disabled*.  The method of doing this varies between systems.
Under SunOS, use the ttysoftcar command.  Under NetBSD, edit /etc/ttys
to remove the "softcar" flag from the line for the serial port, and
run ttyflags.


------------------------------------------------------------------------

Q: Why should I use PPP compression (BSD-Compress or Deflate) when my
modem already does V.42 compression?  Won't it slow the CPU down a
lot?

A: Using PPP compression is preferable, especially when using modems
over phone lines, for the following reasons:

- The V.42 compression in the modem isn't very strong - it's an LZW
technique (same as BSD-Compress) with a 10, 11 or 12 bit code size.
With BSD-Compress you can use a code size of up to 15 bits and get
much better compression, or you can use Deflate and get even better
compression ratios.

- I have found that enabling V.42 compression in my 14.4k modem
increases the round-trip time for a character to be sent, echoed and
returned by around 40ms, from 160ms to 200ms (with error correction
enabled).  This is enough to make it feel less responsive on rlogin or
telnet sessions.  Using PPP compression adds less than 5ms (small
enough that I couldn't measure it reliably).  I admit my modem is a
cheapie and other modems may well perform better.

- While compression and decompression do require some CPU time, they
reduce the amount of time spent in the serial driver to transmit a
given amount of data.  Many machines require an interrupt for each
character sent or received, and the interrupt handler can take a
significant amount of CPU time.  So the increase in CPU load isn't as
great as you might think.  My measurements indicate that a system with
a 33MHz 486 CPU should be able to do Deflate compression for serial
link speeds of up to 100kb/s or more.  It depends somewhat on the type
of data, of course; for example, when compressing a string of nulls
with Deflate, it's hard to get a high output data rate from the
compressor, simply because it compresses strings of nulls so well that
it has to eat a very large amount of input data to get each byte of
output.


------------------------------------------------------------------------

Q: I get messages saying "Unsupported protocol (...) received".  What do
these mean?

A: If you only get one or two when pppd starts negotiating with the
peer, they mean that the peer wanted to negotiate some PPP protocol
that pppd doesn't understand.  This doesn't represent a problem, it
simply means that there is some functionality that the peer supports
that pppd doesn't, so that functionality can't be used.

If you get them sporadically while the link is operating, or if the
protocol numbers (in parentheses) don't correspond to any valid PPP
protocol that the peer might be using, then the problem is probably
that characters are getting corrupted on the receive side, or that
extra characters are being inserted into the receive stream somehow.
If this is happening, most packets that get corrupted should get
discarded by the FCS (Frame Check Sequence, a 16-bit CRC) check, but a
small number may get through.

One possibility may be that you are receiving broadcast messages on
the remote system which are being sent over your serial link.  Another
possibility is that your modem is set for XON/XOFF (software) flow
control and is inserting ^Q and ^S characters into the receive data
stream.


------------------------------------------------------------------------

Q: I get messages saying "Protocol-Reject for unsupported protocol ...".
What do these mean?

A: This is the other side of the previous question.  If characters are
getting corrupted on the way to the peer, or if your system is
inserting extra bogus characters into the transmit data stream, the
peer may send protocol-reject messages to you, resulting in the above
message (since your pppd doesn't recognize the protocol number
either.)


------------------------------------------------------------------------

Q: I get a message saying something like "ioctl(TIOCSETD): Operation
not permitted".  How do I fix this?

A: This is because pppd is not running as root.  If you have not
installed pppd setuid-root, you will have to be root to run it.  If
you have installed pppd setuid-root and you still get this message, it
is probably because your shell is using some other copy of pppd than
the installed one - for example, if you are in the pppd directory
where you've just built pppd and your $PATH has . before /usr/sbin (or
wherever pppd gets installed).


------------------------------------------------------------------------

Q: Has your package been ported to HP/UX or IRIX or AIX?

A: No.  I don't have access to systems running HP/UX or AIX.  No-one
has volunteered to port it to HP/UX.  I had someone who did a port for
AIX 4.x, but who is no longer able to maintain it.  And apparently AIX
3.x is quite different, so it would need a separate port.

IRIX includes a good PPP implementation in the standard distribution,
as far as I know.


------------------------------------------------------------------------

Q: Under SunOS 4, when I try to modload the ppp modules, I get the
message "can't open /dev/vd: No such device".

A: First check in /dev that there is an entry like this:

crw-r--r--  1  root         57,   0 Oct 2  1991 vd

If not, make one (mknod /dev/vd c 57 0).  If the problem still exists,
probably your kernel has been configured without the vd driver
included.  The vd driver is needed for loadable module support.

First, identify the config file that was used.  When you boot your
machine, or if you run /etc/dmesg, you'll see a line that looks
something like this:

SunOS Release 4.1.3_U1 (CAP_XBOX) #7: Thu Mar 21 15:31:56 EST 1996
			^^^^^^^^
			this is the config file name

The config file will be in the /sys/`arch -k`/conf directory (arch -k
should return sun4m for a SparcStation 10, sun3x for a Sun 3/80,
etc.).  Look in there for a line saying "options VDDRV".  If that line
isn't present (or is commented out), add it (or uncomment it).

You then need to rebuild the kernel as described in the SunOS
manuals.  Basically you need to run config and make like this:

	/usr/etc/config CAP_XBOX
	cd ../CAP_XBOX
	make

(replacing the string CAP_XBOX by the name of the config file for your
kernel, of course).

Then copy the new kernel to /:

	mv /vmunix /vmunix.working
	cp vmunix /

and reboot.  Modload should then work.


------------------------------------------------------------------------

Q: I'm running Linux (or NetBSD or FreeBSD), and my system comes with
PPP already.  Should I consider installing this package?  Why?

A: The PPP that is already installed in your system is (or is derived
from) some version of this PPP package.  You can find out what version
of this package is already installed with the command "pppd --help".
If this is older than the latest version, you may wish to install the
latest version so that you can take advantage of the new features or
bug fixes.


------------------------------------------------------------------------

Q: I'm running pppd in demand mode, and I find that pppd often dials
out unnecessarily when I try to make a connection within my local
machine or with a machine on my local LAN.  What can I do about this?

A: Very often the cause of this is that a program is trying to contact
a nameserver to resolve a hostname, and the nameserver (specified in
/etc/resolv.conf, usually) is on the far side of the ppp link.  You
can try executing a command such as `ping myhost' (where myhost is the
name of the local machine, or some other machine on a local LAN), to
see whether that starts the ppp link.  If it does, check the setup of
your /etc/hosts file to make sure you have the local machine and any
hosts on your local LAN listed, and /etc/resolv.conf and/or
/etc/nsswitch.conf files to make sure you resolve hostnames from
/etc/hosts if possible before trying to contact a nameserver.


------------------------------------------------------------------------

Q: Since I installed ppp-2.3.6, dialin users to my server have been
getting this message when they run pppd:

peer authentication required but no suitable secret(s) found for 
authenticating any peer to us (ispserver)

A: In 2.3.6, the default is to let an unauthenticated peer only use IP
addresses to which the machine doesn't already have a route.  So on a
machine with a default route, everyone has to authenticate.  If you
really don't want that, you can put `noauth' in the /etc/ppp/options
file.  Note that there is then no check on who is using which IP
address.  IMHO, this is undesirably insecure, but I guess it may be
tolerable as long as you don't use any .rhosts files or anything like
that.  I recommend that you require dialin users to authenticate, even
if just with PAP using their login password (using the `login' option
to pppd).  If you do use `noauth', you should at least have a pppusers
group and set the permissions on pppd to allow only user and group to
execute it.

------------------------------------------------------------------------

Q: When running pppd as a dial-in server, I often get the message
"LCP: timeout sending Config-Requests" from pppd.  It seems to be
random, but dial-out always works fine.  What is wrong?

A: Most modern modems auto-detects the speed of the serial line
between the modem and the computer.  This auto-detection occurs when
the computer sends characters to the modem, when the modem is in
command mode.  It does not occur when the modem is in data mode.
Thus, if you send commands to the modem at 2400 bps, and then change
the serial port speed to 115200 bps, the modem will not detect this
change until something is transmitted from the computer to the modem.
When running pppd in dial-in mode (i.e. without a connect script),
pppd sets the speed of the serial port, but does not transmit
anything.  If the modem was already running at the specified speed,
everything is fine, but if not, you will just receive garbage from the
modem.  To cure this, use an init script such as the following:

	pppd ttyS0 115200 modem crtscts init "chat '' AT OK"

To reset the modem and enable auto-answer, use:

	pppd ttyS0 115200 modem crtscts init "chat '' ATZ OK ATS0=1 OK"
