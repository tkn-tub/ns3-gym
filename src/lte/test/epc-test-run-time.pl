#!/usr/bin/perl
use strict;
use IO::CaptureOutput qw(capture qxx qxy);
use Statistics::Descriptive;
use Cwd;

my $nIterations = 1;

open( FILE, '>epcTimes.csv' );
print FILE "#sTime\tnodes\trTime\trTDev\n";

my @nodes = ( 1,2,3,4,5,6,7, 8, 12, 14);
my @simTime = ( 1, 2, 5, 7, 10);

foreach my $time (@simTime)
{
         foreach my $node (@nodes)
         {
            my $timeStats = Statistics::Descriptive::Full->new();
            for ( my $iteration = 0 ; $iteration < $nIterations ; $iteration++ )
            {
               my $launch = "time ./waf --run \'lena-simple-epc --simTime=$time --numberOfNodes=$node'";
               my $out, my $err;
               print "$launch\n";
               capture { system($launch ) } \$out, \$err;
               $err =~ /real(.+)m(.+)s/;
               my $minutes = $1;
               my $seconds = $minutes * 60 + $2;
               $timeStats->add_data($seconds);
            }
            print FILE "$time\t$node\t";
            print FILE $timeStats->mean() . "\t"
              . $timeStats->standard_deviation() . "\n";
         }
}
