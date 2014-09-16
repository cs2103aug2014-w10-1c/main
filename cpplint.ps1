# Grab the CppLint script
Invoke-WebRequest https://gist.githubusercontent.com/lowjoel/8e75c651494fe7975dda/raw/cpplint.py -OutFile cpplint.py

# Get all files
$cpp = Get-ChildItem -recurse -filter *.cpp
$h = Get-ChildItem -recurse -filter *.h
$files = ($cpp + $h) |
	Foreach-Object {$_.FullName} |			# Full name
	Resolve-Path -relative |				# Relative path
	Foreach-Object {$_.substring(2)} |		# Remove dots
	?{ $_ -notmatch "packages\\*" }			# Exclude NuGet

# Execute CppLint
$arguments = @('cpplint.py', '--output=vs7', '--filter=-legal/copyright,-build/include,-whitespace/tab') + $files
$env:PATH += ";C:\python27"
&python $arguments 2>&1 | %{
	if ($_.gettype().Name -eq "ErrorRecord") {
		$Host.UI.WriteErrorLine($_)
	} else {
		echo $_
	}
}
if ($LastExitCode -ne 0) {
	Throw "Linting completed with errors."
}
