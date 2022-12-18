# Custom printers snooper
This is a collection of tools meant to log and post-process Custom Kube printers Ethernet captures.
The main goal is to collect 'real' data sent to the printers so to figure out the actual goods that have been ordered (and hopefully) paid.
The first step is to collect all data sent to the Custom Kube printers. Usually printers IP addresses are static and set once and forever.

To find all IPs use the well known nmap tool as follows:

    nmap -sn 192.168.1.0/24

This for example is the mapping of my printers:

* 192.168.1.50       Stampante Bar
* 192.168.1.100      PC cassa
* 192.168.1.210      Stampante cassa

The first thing to do in order to capture all printer's data is to hook to your network a switch capable of port mirroring. Netgear GS108Ev3 is perfect for this task. Run `tcpdump` to collect all traffic sent to the printers:

    tcpdump -i eth0 -n -w birramedia.pcap port 9100

You can easily script above command to aotimatically collect dumps on a daily basis using for example a Raspberry board.
Custom printers listen TCP port 9100 and accept 'raw' protocol. Installed Windows driver reports 'Custom Kube 80mm 200dpi'. Such protocol just carries raw binary data as it would be transmitted over parallel port LPTx.
To extract such binary data you can use Wireshark decoding facilities or just use command line `chaosreader` tool as follows:

    chaosreader -ve -p 9100 birramedia.pcap  

This will generate a bunch of raw files, each one for every printer session. This is the actual binary data sent to the printers. Such raw binary data is NOT Postscript nor ASCII or other simple stuff. The data is a quite modified variand of HP's PCL (Printer Control Language) which makes use of escape sequencies starting with ESC (0x1b) to control the printer. PCL reference can be found here: https://www.pclviewer.com/resources/reference/
As the modified PCL is not documented anywhere, some reverse engineering effort has been put into decondig this (see appendix for details). Such effort resulted in `kubepcl` tool which converts PCL files into PBM images as follows:

    kubepcl birramedia.pcl birramedia.pbm

Now that you have a viewable PBM image, just OCR it as follows

    tesseract -l ita birramedia.pbm birramedia

To extract machine readable text files.

# Appendix
My printers are 200DPI and is 80mm (or 80.5) wide. There are 8 dots per millimeter so the width of the page should be 640.
Install Custom Kube II drivers and print to localhost. Use pbmgenerator tool to create PBM files with known patterns.
Print such images and grab resulting PCL data from localhost:
    
    nc -l 127.0.0.1 9100 > printerdump_localhost.pcl

Use different patterns to sort out which PCL commands mean what.
