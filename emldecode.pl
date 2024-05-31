# emldecode.pl file.eml - Decode "=XX" encoded strings in .eml files.
# Helpful when dealing with e-mail sent by non-techies.
my $file = shift or die "Usage: $0 file.eml\n";
open my $fh, '<', $file or die "Can't open $file: $!\n";
while (<$fh>) { s/=([0-9a-f]{2})/chr(hex($1))/egi; print; }

