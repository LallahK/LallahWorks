#!/usr/bin/env bash
./F_to_C
EXITCODE=$?
if [ "${EXITCODE}" -eq "0" ]; then
	echo "Command returned succesfully"
else 
	echo "Command terminated with failure code: ${EXITCODE}"
fi
