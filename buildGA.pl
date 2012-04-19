# $ perl buildGA.pl

use Term::ANSIColor;

$compilier = "g++";

$outputExe = "gaClique";

@comp_link_OPTIONS = ("-o", $outputExe);

$mainEntry = "gaClique.cpp";

@include = ("setOperations.cpp", "dimacsClq.cpp");

##############################################
############# build
##############################################

@build = ($compilier, @comp_link_OPTIONS, $mainEntry, @include);

print color 'bold red';
print(@build);
print("\n\n");
system(@build);
print("\n\n-----build complete\n\n");

print color 'reset';


##############################################
############# execute
##############################################

@mutations = ("0", "1", "2");
@iterations = ("1", "20");
@args = <DIMACS_cliques/*.clq>;

foreach $mutationType (@mutations){
	foreach $iterationCount (@iterations){
		foreach $argInstance (@args) {
		
			$execute = "./";
			$execute .= $outputExe;
			$execute .= " ";
			$execute .= $argInstance;
			$execute .= " ";
			$execute .= $mutationType;
			$execute .= " ";
			$execute .= $iterationCount;
			print($argInstance."\t");
			system($execute);
		} 
	}
}
