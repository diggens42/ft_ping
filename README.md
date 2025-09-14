Here's a comprehensive README.md for your ft_ping project:

```markdown
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
```

### Makefile Targets
- `make` - Build the project
- `make clean` - Remove object files
- `make fclean` - Remove object files and executable
- `make re` - Rebuild the project from scratch

## Usage

```bash
sudo ./ft_ping [OPTIONS] HOST
```

### Implemented Options

| Option | Long Form | Description |
|--------|-----------|-------------|
| `-c COUNT` | `--count=COUNT` | Stop after sending COUNT packets |
| `-d` | `--debug` | Enable SO_DEBUG socket option |
| `-i INTERVAL` | `--interval=INTERVAL` | Wait INTERVAL seconds between packets |
| `-n` | `--numeric` | Numeric output only (no DNS lookups) |
| `-p PATTERN` | `--pattern=PATTERN` | Specify up to 16 pad bytes (hex) |
| `-q` | `--quiet` | Quiet output (summary only) |
| `-r` | `--ignore-routing` | Bypass routing table (SO_DONTROUTE) |
| `-s SIZE` | `--size=SIZE` | Specify packet payload size (8-65507 bytes) |
| `-t TYPE` | `--type=TYPE` | ICMP packet type to send |
| `-T TOS` | `--tos=TOS` | Set Type of Service field |
| `-v` | `--verbose` | Verbose output (show all ICMP responses) |
| `-w TIMEOUT` | `--timeout=TIMEOUT` | Stop after TIMEOUT seconds |
| `-W LINGER` | `--linger=LINGER` | Per-packet receive timeout |
| `--ttl=N` | | Set IP Time To Live |
| `-?` | `--help` | Display help message |

### Examples

```bash
# Basic ping
sudo ./ft_ping google.com

# Send exactly 5 packets
sudo ./ft_ping -c 5 192.168.1.1

# Verbose mode with 2-second interval
sudo ./ft_ping -v -i 2 example.com

# Quiet mode with custom packet size
sudo ./ft_ping -q -s 128 localhost

# Numeric output only (skip DNS)
sudo ./ft_ping -n 8.8.8.8
```

## Technical Implementation

### Key Components

1. **Raw Socket Creation**: Uses `SOCK_RAW` with `IPPROTO_ICMP` for direct ICMP packet handling
2. **Packet Structure**: Custom ICMP header implementation with checksum calculation
3. **Time Measurement**: Microsecond precision timing using `gettimeofday()`
4. **Flexible Parser**: Table-driven option parser supporting both short and long options
5. **Modular Design**: Clean separation of concerns with dedicated handlers for each feature

### ICMP Packet Format

```
+----------------+----------------+
|     Type (8)   |     Code (8)   |
+----------------+----------------+
|         Checksum (16)           |
+----------------+----------------+
|     Identifier |    Sequence     |
+----------------+----------------+
|          Timestamp              |
+----------------+----------------+
|            Data...              |
+----------------+----------------+
```

## Requirements & Limitations

- **Root Privileges**: Required for raw socket creation (`CAP_NET_RAW` capability)
- **Platform**: Linux/Unix systems only (uses Linux-specific headers)
- **Maximum Packet Size**: 65507 bytes (IPv4 limitation)
- **Minimum Packet Size**: 8 bytes (ICMP timestamp size)

## Error Handling

The program handles various error conditions:
- Network unreachable
- Host unreachable  
- TTL exceeded
- Permission denied (when run without root)
- DNS resolution failures
- Socket creation failures

## Author

**fwahl** - 42 Heilbronn Student
