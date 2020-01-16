#!/usr/bin/perl
# Runs tcpdump on a daily basis

#use strict;
use warnings;

sub midnight_seconds {
   my @time = localtime();
   my $secs = ($time[2] * 3600) + ($time[1] * 60) + $time[0];

   return $secs;
}

# Evaluate how many seconds we have to run tcpdump
my $tomidnight = (24 * 3600 - midnight_seconds());

# Prepare file name (TODO)
my $filename = "temp.pcap";
# See https://www.tutorialspoint.com/perl/perl_date_time.htm
my @months = qw( Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec );
my @days = qw(Sun Mon Tue Wed Thu Fri Sat Sun);

($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime();
print "$mday $months[$mon] $days[$wday]\n";

# Prepare command
my $command = "timeout $tomidnight tcpdump -i eth0 -r $filename";

# Ececute it
print "Executing $command" . "\n";

# Wait before looping back
sleep(100);
