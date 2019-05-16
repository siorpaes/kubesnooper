# rsnooper
All IP addresses are static
To find all IPs
nmap -sn 192.168.1.0/24

192.168.1.50       Stampante Bar
192.168.1.100      PC cassa
192.168.1.210      Stampante cassa

Printers listen TCP port 9100 and accept 'raw' (aka Jetdirect) protocol.
Windows drivers report 'Custom Kube 80mm 200dpi'.
Such protocol just carries raw data as it would be transmitted over parallel port LPTx.
Data conveyed can be PostScript or, more likey, PCL which stands for Printer Control Language
invented by HP. Such protocol makes use of escape sequencies starting with ESC (0x1b) to
control the printer.


PCL reference
https://www.pclviewer.com/resources/reference/

Printer is 200DPI and is 80mm (or 80.5) wide
There are 8 dots per millimeter so the width of the page should be 640

Install Custom Kube II drivers and print to localhost.
Use pbmgenerator to create PBM files with known patterns.
Grab print data:
nc -l 127.0.0.1 9100 > printerdump_localhost.pcl

Print an image and log the PCL file.

