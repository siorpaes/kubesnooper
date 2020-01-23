# Introduction
This Perl script runs tcpdump on a daily basis.
Capture files are saved locally and uploaded to a server using Curl.
The server side PHP script is rsnooper.php

# Misc Raspberry tips
Add to `/etc/rc.local` dumping script execution

Install RTC as on online guides. Make sure the red LED is on. Hook up network to get proper synced date. Write it to RTC with:

    sudo hwclock -w
    sudo hwclock -r

Enable UART console appending `enable_uart=1` to `/boot/config.txt`. Interface it with FTDI cable as follows:

*  Pin 6    GND
*  Pin 8    TX (FTDI Yellow)
*  Pin 10   RX (FTDI Orange)
*  Pin 17   3V3 (if needed by VIP cable)

