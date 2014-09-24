Set-StrictMode -version 2

# Modified from
# http://www.howtogeek.com/tips/how-to-extract-zip-files-using-powershell/
function UnzipFile($file, $destination) {
	mkdir $destination >$null
	$file = (Resolve-Path $file)[0].Path
	$destination = (Resolve-Path $destination)[0].Path

	Write-Progress ('Extracting ' + $file + ' => ' + $destination)

	$shell = new-object -com shell.application
	$zip = $shell.NameSpace($file)
	foreach ($item in $zip.items()) {
		$shell.Namespace($destination).copyhere($item)
	}
}

$doxygen_dir = 'doxygen'
$doxygen_zip = $doxygen_dir + '.zip'
$doxygen_exists = Test-Path $doxygen_dir
if ($doxygen_exists -ne $true) {
	Invoke-WebRequest ftp://ftp.stack.nl/pub/users/dimitri/doxygen-1.8.8.windows.bin.zip -OutFile $doxygen_zip
	UnzipFile $doxygen_zip $doxygen_dir
}

.\doxygen\doxygen.exe .\Doxyfile 2>&1 | %{
	if ($_.gettype().Name -eq "ErrorRecord") {
		if ($_ -match 'warning:') {
			$Host.UI.WriteWarningLine($_)
		} else {
			$Host.UI.WriteErrorLine($_)
		}
	} else {
		echo $_
	}
}
if ($LastExitCode -ne 0) {
	Throw "doxygen completed with errors."
}

$documentation_dir = 'docs'
$github_pages_dir = 'you_documentation'
if ($env:YOU_DOCUMENTATION_KEY) {
	$ssh_config_dir = (Resolve-Path '~/.ssh')[0].Path
	mkdir $ssh_config_dir

	$key = [System.Convert]::FromBase64String($env:YOU_DOCUMENTATION_KEY)
	[System.IO.File]::WriteAllBytes($ssh_config_dir + '\\id_rsa', $key)

	echo 'Host github.com
	HostName github.com
	User git
	IdentityFile ' + $ssh_config_dir + '\\id_rsa' |
	Add-Content ($ssh_config_dir + '/config')

	&git clone git@github.com:cs2103aug2014-w10-1c/you-documentation.git $github_pages_dir --quiet 2>&1 | %{
		if ($_.gettype().Name -eq "ErrorRecord") {
			$Host.UI.WriteErrorLine($_)
		} else {
			echo $_
		}
	}

	Remove-Item $github_pages_dir -Exclude *.git -Recurse
	pushd $github_pages_dir
	move ('..\' + $documentation_dir + '\html\*') '.'
	&git add --all
	&git commit -m 'Latest documentation'
	&git push

	popd
}
