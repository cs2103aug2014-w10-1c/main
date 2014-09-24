Set-StrictMode -version 2

# Grab the CppLint script
$cpplint = 'cpplint.py'
$cpplint_exists = Test-Path $cpplint
if ($cpplint_exists -ne $true) {
	Invoke-WebRequest https://gist.githubusercontent.com/lowjoel/8e75c651494fe7975dda/raw/cpplint.py -OutFile $cpplint
}

# Get all files
$cpp = Get-ChildItem -recurse -filter *.cpp
$h = Get-ChildItem -recurse -filter *.h
$files = ($cpp + $h) |
	Foreach-Object {$_.FullName} |			# Full name
	Resolve-Path -relative |				# Relative path
	Foreach-Object {$_.substring(2)} |		# Remove dots
	?{ $_ -notmatch "packages\\*" }	|		# Exclude NuGet
	?{ $_ -notmatch "pugixml\\*" }			# Exclude pugixml

# Execute CppLint
$arguments = @(
		'cpplint.py',
		'--output=vs7',
		'--filter=-legal/copyright,-build/include,-whitespace/tab,-whitespace/indent, -build/c++11'
	) + $files
$env:PATH += ";C:\python27"
&python $arguments 2>&1 | %{
	if ($_.gettype().Name -eq "ErrorRecord") {
		if (Get-Command 'Add-AppveyorCompilationMessage' -errorAction SilentlyContinue) {
			$matches = $_.ToString().split("`r`n") |
				Select-String -Pattern '^([^(]+)\(([\d]+)\): +([^[]*)\[([^\]]+)\]' -AllMatches

			foreach ($match in $matches) {
				$match = $match.Matches
				$file = $match.Groups[1].Value
				$line = [System.Convert]::ToInt32($match.Groups[2].Value)
				$message = $match.Groups[3].Value.Trim()
				$category = $match.Groups[4].Value

				Add-AppveyorCompilationMessage -Message $message -Category Warning -FileName $file -Line $line -ProjectName ('Linting: ' + $category)
			}
		}

		$Host.UI.WriteErrorLine($_)
	} else {
		echo $_
	}
}
if ($LastExitCode -ne 0) {
	Throw "Linting completed with errors."
}
