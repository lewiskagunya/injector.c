# Network Packet Injector (C / Raw Sockets)

### ⚖️ Ethical Disclaimer
**For Educational Purposes Only.** This tool was developed to understand the low-level mechanics of the TCP/IP stack and header construction. It demonstrates techniques like IP Spoofing which are used in network attacks. This project is part of a SOC Analyst training path to better identify and mitigate malformed or spoofed traffic. Only use this in controlled lab environments.

---

## 🛡️ SOC Analyst Perspective
As a security professional, understanding how an attacker crafts a packet from scratch is vital. This project explores:
* **IP Spoofing:** Manually defining the `saddr` (Source Address) to bypass basic filters.
* **Header Integrity:** Calculating the Internet Checksum to ensure the packet is accepted by routers.
* **Detection Evasion:** Understanding how raw sockets can bypass the OS networking stack.

## 🚀 Project Overview
This is a C-based packet injector inspired by Jon Erickson's *Hacking: The Art of Exploitation*. Unlike standard networking code, this program uses **Raw Sockets** (`SOCK_RAW`) and the `IP_HDRINCL` flag to manually build both the IP and UDP headers.

### Key Features
- **Manual Header Construction:** Defines `struct iphdr` and `struct udphdr` manually in memory.
- **Custom Payload:** Injects a specific string into the data segment of the packet.
- **Checksum Calculation:** Implements the RFC 1071 algorithm to ensure packet validity.

## 🛠️ Technical Implementation
The core logic involves mapping a memory buffer to networking structures:
1. **Layer 3 (IP):** Sets version, IHL, TTL, and the spoofed source IP.
2. **Layer 4 (UDP):** Sets source/destination ports and length.
3. **Payload:** Appends the data after the headers.

## 🖥️ Usage
### Compilation# Start a listener in one terminal
sudo tcpdump -i lo udp port 12345 -Xvv

# Run the injector in another terminal
sudo ./injector
To compile the injector:
```bash
gcc injector.c -o injector
