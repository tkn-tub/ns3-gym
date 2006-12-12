#!/usr/bin/env perl

use Math::Random qw(:all);

@values = random_uniform (10000, 0, 10000000);

foreach $value (@values) {
    print $value . "\n";
}
