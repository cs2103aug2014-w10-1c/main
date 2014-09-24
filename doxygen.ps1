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

.\doxygen\doxygen.exe .\Doxyfile