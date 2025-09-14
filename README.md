# ft_ping

A custom implementation of the Unix `ping` command, built from scratch in C. This project recreates the functionality of the ICMP echo request/reply mechanism used to test network connectivity.

## Overview

`ft_ping` sends ICMP ECHO_REQUEST packets to network hosts and listens for ECHO_REPLY packets. The program provides detailed statistics about packet loss, round-trip times, and network reliability.

### Core Features

- **ICMP Protocol Implementation**: Direct interaction with raw sockets to send and receive ICMP packets
- **DNS Resolution**: Automatic hostname to IP address resolution using `getaddrinfo()`
- **Real-time Statistics**: Continuous tracking of packet loss, RTT (Round Trip Time), and network performance
- **Signal Handling**: Graceful termination with statistics display on SIGINT/SIGQUIT
- **Flexible Configuration**: Multiple command-line options for customizing ping behavior

## Compilation

### Requirements
- GCC or Clang compiler
- Linux/Unix operating system
- Root privileges (required for raw socket creation)

### Build Instructions
```bash
# Clone the repository
git clone [your-repo-url]
cd ft_ping

# Compile the project
make

# Run with root privileges
sudo ./ft_ping google.com