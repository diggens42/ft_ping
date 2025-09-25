### NOTES

ICMP = Internet Control Message Protocol
It’s a network layer protocol (part of the IP suite, defined in RFC 792).
Its job is to send control messages and error reporting between devices.
Unlike TCP or UDP, ICMP is not used to carry "user data," but rather to send information about the health of the network.


📦 What’s in a ping packet?

A complete ICMP packet looks like this:
[ IP header ] + [ ICMP header ] + [ ICMP payload ]
IP header → 20 bytes (not counted in ping -s size)
ICMP header → 8 bytes (type, code, checksum, id, seq)
ICMP payload → variable size (this is what packet_size controls)


The ping command implements the ICMP (Internet Control Message Protocol) echo request/reply mechanism. Here's how it works under the hood:

## Core Mechanism

Ping sends ICMP Echo Request packets to a target host and waits for ICMP Echo Reply packets back. It measures the round-trip time (RTT) and reports packet loss statistics.

## Implementation Details

**Socket Creation**
You'll need a raw socket with `SOCK_RAW` and `IPPROTO_ICMP`. This requires root privileges since you're crafting packets at the IP level:
```c
int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
```

**ICMP Packet Structure**
The ICMP header has this structure:
- Type (8 bits): 8 for Echo Request, 0 for Echo Reply
- Code (8 bits): 0 for echo messages
- Checksum (16 bits): Covers ICMP header and data
- Identifier (16 bits): Usually process ID to match requests/replies
- Sequence Number (16 bits): Incremented for each packet sent

**Checksum Calculation**
You must calculate the Internet checksum over the entire ICMP packet. Set checksum field to 0 first, then compute the 16-bit one's complement sum of all 16-bit words in the header and data.

**Timing Mechanism**
Before sending each packet, record a timestamp (usually in the data payload). When you receive a reply, compare with the current time to calculate RTT. Use `gettimeofday()` or similar for microsecond precision.

**Send/Receive Loop**
The basic flow is:
1. Construct ICMP Echo Request with incremented sequence number
2. Send packet using `sendto()`
3. Use `select()` or `poll()` with timeout to wait for reply
4. Receive with `recvfrom()`
5. Parse received packet to extract ICMP reply and validate it matches your request
6. Calculate and display RTT
7. Wait 1 second (default interval) and repeat

**IP Header Handling**
When you receive packets on a raw ICMP socket, you get the full IP packet. You'll need to parse the IP header to find the ICMP portion. The IP header length varies (typically 20 bytes but can be longer with options).

**Error Handling**
Handle various ICMP error types that might come back instead of echo replies, like Destination Unreachable (type 3) or Time Exceeded (type 11).

The key challenges in your C implementation will be proper checksum calculation, handling the raw socket mechanics, and parsing the received packets correctly to extract timing information.



### types

Common ICMP Types You Might Use with --type (and their implications):
While ping is primarily for echo requests, the --type option allows you to send other ICMP message types. Here are a few examples, though not all of them will necessarily elicit a "reply" in the way an echo request does:
8 (Echo Request): This is the default and most common type used by ping. It's used to determine if a host is reachable and to measure round-trip time.
0 (Echo Reply): This is the response to an Echo Request. You wouldn't typically send this as an initial message from ping.
3 (Destination Unreachable): Indicates that a datagram could not be delivered to its destination. Various codes within this type specify why (e.g., network unreachable, host unreachable, port unreachable). Sending this with ping might not produce a direct "reply" but could be used for specific network testing.
4 (Source Quench): Historically used to tell a sender to reduce its transmission rate. Less common in modern networks.
5 (Redirect): Informs the sender that a better route exists for the destination.
11 (Time Exceeded): Sent when a packet's Time To Live (TTL) reaches zero (e.g., by traceroute), or when a packet reassembly timeout occurs.
13 (Timestamp Request): Used to request a timestamp from a host, allowing for time synchronization or more precise round-trip time measurements, considering the actual time it took for the packet to be processed.
14 (Timestamp Reply): The response to a Timestamp Request.

**inetutils2.0 ping list of options**

Usage: ping [OPTION...] HOST ...
Send ICMP ECHO_REQUEST packets to network hosts.

Options controlling ICMP request types:
  --address           send ICMP_ADDRESS packets (root only)
  --echo              send ICMP_ECHO packets (default)
  --mask              same as --address
  --timestamp         send ICMP_TIMESTAMP packets
  -t, --type=TYPE     send TYPE packets ✅

Options valid for all request types:
  -c, --count=COUNT   stop after sending (and receiving) COUNT ECHO_RESPONSE packets ✅
  -d, --debug         set the SO_DEBUG option on the socket being used
  <!-- -f, --flood         flood ping; outputs packets as fast as they come back or 100 times per second, whichever is more -->
  -i, --interval=WAIT wait WAIT seconds between sending each packet ✅
  <!-- -l, --preload=PRELOAD if preload is specified, ping sends that many packets as fast as possible before falling into normal mode of behavior -->
  -n, --numeric       numeric output only; no attempt will be made to lookup symbolic names for host addresses
  -p, --pattern=PATTERN
                       specify up to 16 "pad" bytes to fill out the packet you send
  -q, --quiet         quiet output; only summary is printed at start and finish ✅
  -R, --route         record route; includes RECORD_ROUTE option; prints the route
  -r, --ignore-routing
                       bypass normal routing and send directly to a host on an attached network
  -s, --size=PACKETSIZE
                       specify the number of data bytes to send
  -v, --verbose       verbose output; lists non-ECHO_RESPONSE ICMP packets received
  --ttl=N             set packet time-to-live to N
  -T, --tos=NUM       set Type-of-Service (TOS) to NUM
  -w, --timeout=N     stop after N seconds of sending packets
  -W, --linger=N      number of seconds to wait for response before quitting
  --ip-timestamp=FLAG
                       include timestamp option; FLAG is one of "tsonly" or "tsaddr"

  -?, --help          display this help list ✅
  --usage             display a short usage message
  -V, --version       print the program version





sudo setcap cap_net_raw+ep ./ft_ping for socket creation error
   sudo tcpdump -i any -nn -vv icmp  --> tcpdump to check received package data

### TO DO

-signals
Supported Types in GNU Inetutils Ping
The GNU Inetutils ping --type or -t flag accepts these values:

fix print for timestamp replies (no rtt)

types to add :
address - Send ICMP_ADDRESS packets (requires root privileges)
mask - Same as address (alias for address requests)


-->not case sensitive


### test cases:

./ping -v --ttl=1 8.8.8.8
-->should print ttl exceeded error msg and ip header dump
