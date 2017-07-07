# Abra

Send a magic packet.

Abra is a multi-platform command-line tool that sends a magic packet to a computer on the network, waking it from a sleep/powered-off state.

Specify the MAC address of the target computer when you run Abra, and a magic packet will be constructed and sent, waking up that (ethernet connected, wake-on-lan supporting) computer:

```
abra E9:2B:99:3C:00:15
```

Additional command-line options can be used to specify a network broadcast address and port number to route the packet:

```
abra -i 192.168.1.255 -p 9 E9:2B:99:3C:00:15
```

Example run:

```
abra -i 192.168.1.255 -V E9:2B:99:3C:00:15
abra: magic packet sent!
Summary:
  Target: E9:2B:99:3C:00:15
  Broadcast Route: 192.168.1.255
  Port Number: 7
```

Note: Currently, only broadcast via ipv4 addresses is supported, ipv6 support coming soon.

All options and their behaviors are described in the 'Usage' section below.

## Build

### Linux, MacOS, BSD

Simple: `clone` Abra, run `make` (GNU make) in the project root directory.

The resulting binary **'abra'** will be located in the project root directory.

### Windows

Building on Windows requires the *MinGW* compiler, and `make` from *MSYS* **(not mingw32-make)**.

The same procedure applies as on *NIX: `clone` Abra, run `make` in the project root directory.

The resulting binary **'abra.exe'** will be located in the project root directory.

## Usage

```
Abra - Send a magic packet.

Usage:
  abra [-hvI] [-i ip-address] [-p port] mac-address

Mandatory Arguments:
  mac-address                 48-bit target MAC address.

Optional Arguments:
  -i, --ip-address=ADDRESS    Target broadcast address (ipv4),
                              defaults to 255.255.255.255.
  -p, --port=PORT             Target port,
                              defaults to 7.

Options:
  -h, --help                  Display help information and exit.
  -v, --version               Display build information and exit.
  -I, --info                  Same as -V, --version.
  -V, --verbose               Display a detailed summary on success.

Example:
  abra -i 192.168.1.255 -p 9 E9:2B:99:3C:00:15
```
