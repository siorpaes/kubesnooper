#!/usr/bin/perl
# Runs tcpdump on a daily basis

use strict;
use warnings;

sub midnight_seconds {
   my @time = localtime();
   my $secs = ($time[2] * 3600) + ($time[1] * 60) + $time[0];

   return $secs;
}

while(1){
   # Evaluate how many seconds we have to run tcpdump
   my $tomidnight = (24 * 3600 - midnight_seconds());

   # Prepare file name
   # See https://www.tutorialspoint.com/perl/perl_date_time.htm
   my @months = qw( Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec );
   my @days = qw(Sun Mon Tue Wed Thu Fri Sat Sun); 
   my ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst) = localtime();
   $year += 1900;
   my $time = sprintf ("%02d:%02d:%02d", $hour, $min, $sec);

   my $filename = "$days[$wday]-$mday-$months[$mon]-$year--$time.pcap";
   print "Dumping to: " . $filename . "\n";

   # Prepare tcpdump command
   #my $tcpdumpcommand = "timeout $tomidnight tcpdump -n -i eth0 port 9100 -w $filename";
   my $tcpdumpcommand = "timeout 60 tcpdump -n -i eth0 tcp -w $filename";

   # Ececute it
   print "Executing: $tcpdumpcommand" . "\n";
   system $tcpdumpcommand;

   # Transfer dump file to server (TODO)
   my $curlcommand = "curl -F rdata=@" . $filename . " www.nuvolau.com/rsnooper/rsnooper.php";
   print "Executing: $curlcommand" . "\n";
   system $curlcommand;

   # Wait before looping back
   print "Waiting...\n";
   sleep(20);
}
