set -e

DB_DIR=/usr/local/.devpkg
EXE=devpkg
URL=https://github.com/DaveGamble/cJSON.git

# init first
rm -rf $DB_DIR
if [ ! $? -eq 0 ]; then exit; fi

$EXE -S
if [ ! $? -eq 0 ]; then exit; fi

$EXE -L
if [ ! $? -eq 0 ]; then exit; fi

$EXE -F $URL
$EXE -B $URL

$EXE -L
if [ ! $? -eq 0 ]; then exit; fi

rm -rf $DB_DIR

$EXE -S
if [ ! $? -eq 0 ]; then exit; fi

$EXE -L
if [ ! $? -eq 0 ]; then exit; fi

$EXE -I $URL

$EXE -L
if [ ! $? -eq 0 ]; then exit; fi
