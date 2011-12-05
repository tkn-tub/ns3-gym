#!/usr/bin/perl
use strict;
use IO::CaptureOutput qw(capture qxx qxy);
use Statistics::Descriptive;
use Cwd;

my $nIterations = 1;

open( FILE, '>times.csv' );
print FILE "#sTime\tnFloors\tnEnb\tnUe\trTime\trTDev\n";

my @nUe = ( 1, 5, 10, 15, 20, 25, 30 );
my @nEnb = ( 1, 2, 4, 6, 8, 12, 14, 18, 22 );
my @nFloors = ( 0, 1 );
my @simTime = ( 1, 5);

my $traceDirectory = ".";
my $traceDirectory = getcwd() . "/"; 

foreach my $time (@simTime)
{
   foreach my $floor (@nFloors)
   {
      foreach my $enb (@nEnb)
      {
         foreach my $ue (@nUe)
         {
            my $timeStats = Statistics::Descriptive::Full->new();
            for ( my $iteration = 0 ; $iteration < $nIterations ; $iteration++ )
            {
               my $launch = "time ./waf --run \'lena-profiling --simTime=$time --nUe=$ue --nEnb=$enb --nFloors=$floor --traceDirectory=$traceDirectory\'";
               my $out, my $err;
               print "$launch\n";
               capture { system($launch ) } \$out, \$err;
               $err =~ /real(.+)m(.+)s/;
               my $minutes = $1;
               my $seconds = $minutes * 60 + $2;
               $timeStats->add_data($seconds);
            }
            print FILE "$time\t$floor\t$enb\t$ue\t";
            print FILE $timeStats->mean() . "\t"
              . $timeStats->standard_deviation() . "\n";
         }
      }
   }
}
