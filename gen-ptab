#!/usr/bin/perl
# Generate a joint probability table for a given distribution of marginal probabilities.
my $n = 3;
my %tab = (A => 1/2, B => 1/3, C => 1/6);
# ...
sub generate {
    my ($n, $prefix, $p) = @_;
    if ($n == 0) {
        printf "%s %.3f\n", $prefix, $p;
    } else {
        generate($n - 1, $prefix . $_, $p * $tab{$_}) for sort keys %tab;
    }
}
generate($n, "", 1);