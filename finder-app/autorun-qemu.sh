#!/bin/sh
cd $(dirname $0)
echo "Running test script"
echo "Listing contents of /home:"
ls -l /home
./finder-test.sh
rc=$?
if [ ${rc} -eq 0 ]; then
    echo "Completed with success!!"
else
    echo "Completed with failure, failed with rc=${rc}"
fi
echo "finder-app execution complete, dropping to terminal"
exit $rc

