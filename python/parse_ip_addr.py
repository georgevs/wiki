#!/usr/bin/env python3

'''
Parses the output of the `ip addr` command and yields structured information
about network interfaces, including their links and addresses.

parse_ip_addr(lines) -> yields NetworkInterface objects

Each NetworkInterface contains:
- interface: Interface object with index, name, flags, and rest of the line
- link: Link object with kind, mac address, and rest of the line
- addrs: list of Inet and Inet6 objects representing IP addresses

Install:
    chmod +x path/to/parse_ip_addr.py
    ln -s path/to/parse_ip_addr.py ~/.local/bin/parse_ip_addr

Usage:
    ip addr | parse_ip_addr --format csv --sort name ip
'''


import collections
import csv
import operator
import os
import re
import sys


NetworkInterface = collections.namedtuple('NetworkInterface', 'interface link addrs')
Interface = collections.namedtuple('Interface', 'index name flags rest')
Link = collections.namedtuple('Link', 'kind mac rest')
Inet = collections.namedtuple('Inet', 'address rest')
Inet6 = collections.namedtuple('Inet6', 'address rest')


Row = collections.namedtuple('Row', 'index ip name kind mac flags rest')


def main(config):
    fp = open(config.file_path, 'rt') if config.file_path else sys.stdin
    fpo = open(config.out_file_path, 'wt') if config.out_file_path else sys.stdout

    lines = iter(fp.readline, '')
    nics = parse_ip_addr(lines)

    if config.format == 'csv':
        write_csv(nics, fpo, config)
    else:
        print(*nics, sep=os.linesep, file=fpo)


def write_csv(nics, fpo, config):
    header = None if config.no_header else Row._fields
    rows = (
        Row(nic.interface.index, addr.address, nic.interface.name, 
            nic.link.kind if nic.link else '', nic.link.mac if nic.link else '', 
            '|'.join(nic.interface.flags), addr.rest)
        for nic in nics
        for addr in nic.addrs
    )

    if config.sort_order:
        order = [*config.sort_order, *(field for field in Row._fields if field not in config.sort_order)]
        header = order if header else None
        rows = sorted(rows, key=operator.attrgetter(*config.sort_order))
        rows = (tuple(getattr(row, field) for field in order) for row in rows)

    writer = csv.writer(fpo)
    if header: writer.writerow(map(str.upper, header))
    for row in rows: writer.writerow(row)


def parse_ip_addr(lines):
    make_interface = lambda index, name, flags, rest: Interface(int(index), name, flags.split(','), rest)
    parse_interface = make_parse_pattern(r'^(\d+): (.+?): <(.+?)> (.+)', make_interface)
    parse_link = make_parse_pattern(r'^\s+link/(.+?) ([0-9a-fA-F:]+) (.+)', Link)
    parse_inet = make_parse_pattern(r'^\s+inet ([0-9\.a-fA-F:/]+) (.+)', Inet)
    parse_inet6 = make_parse_pattern(r'^\s+inet6 ([0-9a-fA-F:/]+) (.+)', Inet6)

    nic = None

    for line in lines:
        if (match := parse_interface(line)):
            if nic: yield nic
            nic = NetworkInterface(match, None, [])
        elif nic:
            if (match := parse_link(line)): 
                nic = nic._replace(link=match)
            elif (match := parse_inet(line)):
                nic = nic._replace(addrs=nic.addrs + [match])
            elif (match := parse_inet6(line)):
                nic = nic._replace(addrs=nic.addrs + [match])

    if nic:
        yield nic    


def make_parse_pattern(pattern, ctor=lambda *xs: True):
    search_pattern = re.compile(pattern).search
    return lambda line: (match := search_pattern(line)) and ctor(*match.groups())


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('file_path', type=str, nargs='?')
    parser.add_argument('--format', type=str, choices=['csv'])
    parser.add_argument('--no-header', action='store_true')
    parser.add_argument('--out', dest='out_file_path', type=str)
    parser.add_argument('--sort', dest='sort_order', nargs='*', choices=Row._fields)
    config = parser.parse_args()
    sys.exit(main(config))