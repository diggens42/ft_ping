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





### TO DO

-signals
-errorhandling like og ping command
-setup vm 
- one more flag
- r flag is weird
- ft_atoll for -c flag
- get_ms helper function