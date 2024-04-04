use strict;
use warnings;
use List::Util qw(reduce sum zip);

my %freq = (A => 0.7, B => 0.2, C => 0.1);
my $n = 3;

my %table = %freq;
for my $i (0..$n-2) {
    my $word = reduce { $freq{$a} > $freq{$b} ? $a : $b } keys %freq;
    my $prob = delete $freq{$word};
    $freq{$word . $_} = $prob * $table{$_} for keys %table;
}

print "Codebook for Tunstall coding:\n";
my @key = sort keys %freq;
for my $i (0..$#key) {
    my $word = $key[$i];
    printf "%s (%0" . $n . "b): %.3f\n", $word, $i, $freq{$word};
}

printf "Entropy: %.3f\n", -sum(map { $_ * log($_) } values %table);
printf "Average code word length: %.3f\n", sum(map { $n / length($_) * $freq{$_} } @key);
