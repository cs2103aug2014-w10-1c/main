Set-StrictMode -version 2

# Modified from
# http://www.howtogeek.com/tips/how-to-extract-zip-files-using-powershell/
function UnzipFile($file, $destination) {
	mkdir $destination >$null
	$file = (Resolve-Path $file)[0].Path
	$destination = (Resolve-Path $destination)[0].Path

	echo ('Extracting ' + $file + ' => ' + $destination)

	if (Get-Command "7z.exe" -ErrorAction SilentlyContinue) {
		7z x -y "-o$destination" $file
	} else {
		$shell = new-object -com shell.application
		$zip = $shell.NameSpace($file)
		foreach ($item in $zip.items()) {
			$shell.Namespace($destination).copyhere($item)
		}
	}
}

$doxygen_dir = '.\doxygen'
$doxygen_zip = $doxygen_dir + '.zip'
$doxygen_exists = Test-Path $doxygen_dir
if ($doxygen_exists -ne $true) {
	echo 'Downloading doxygen...'
	Invoke-WebRequest ftp://ftp.stack.nl/pub/users/dimitri/doxygen-1.8.8.windows.bin.zip -OutFile $doxygen_zip
	UnzipFile $doxygen_zip $doxygen_dir
}

&$doxygen_dir\doxygen.exe .\Doxyfile 2>&1 | %{
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
