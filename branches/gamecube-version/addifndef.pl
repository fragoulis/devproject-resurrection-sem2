
open($fh, "header.txt");
@headerLines = <$fh>;
close($fh);

FixDir($ARGV[0] or ".");

sub FixDir
{
	my $dir = $_[0];
	opendir(DIR, $dir);
	my @files = readdir(DIR);
	closedir(DIR);

	foreach (@files)
	{
		if (!/^\./) {
			my $path = $dir . "/" . $_;
			if (-d $path) {
				FixDir($path);
			}
			else {
				if (/h$/) {
					open($fh, $path);
					@lines = <$fh>;
					close($fh);
					
					open(FH, ">$path");
					$filename = $_;
					$name = substr($filename, 0, index($filname, '.')-1);
					#print uc($name);
					#exit;
					$def = "_RES_" . uc($name) . "_H_";
					print FH "#ifndef $def\n";
					print FH "#define $def\n";
					#foreach (@headerLines) {
					#	$line = $_;
					#	$line =~ s/FILENAME/$filename/;
					#	print FH $line;
					#}
					foreach (@lines) {
					#	next if /\/\/\*\*/;
						next if /#pragma once/;
						print FH;
					}
					
					print FH "\n\n#endif\n";
				}
			}
		}
	}
}