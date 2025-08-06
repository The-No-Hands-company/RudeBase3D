# PowerShell script to sort build errors by file
$inputFile = "build_errors.txt"
$outputFile = "sorted_build_errors.txt"

$errors = Get-Content -Path $inputFile
$errorGroups = @{}
$filePathRegex = '^(.*?)\(\d+,\d+\): error'

foreach ($line in $errors) {
    if ($line -match $filePathRegex) {
        $filePath = $matches[1].Trim()
        if (-not $errorGroups.ContainsKey($filePath)) {
            $errorGroups[$filePath] = @()
        }
        $errorGroups[$filePath] += $line
    }
}

$sortedFilePaths = $errorGroups.Keys | Sort-Object
$outputContent = @()

foreach ($filePath in $sortedFilePaths) {
    $outputContent += "`n$filePath"
    $outputContent += $errorGroups[$filePath]
}

$outputContent | Out-File -FilePath $outputFile -Encoding UTF8
Write-Host "Errors have been sorted by file and written to $outputFile"
