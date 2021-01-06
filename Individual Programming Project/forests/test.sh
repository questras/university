#!/bin/bash

if (($# != 2))
then
	echo "Wrong number of arguments."
	exit 1
fi

# Programme to be executed.
PROGRAMME=$1
# Directory with tests.
DIR=$2

# Temporary files to store stdout and stderr of [PROGRAMME].
TEMP_OUT="$(mktemp)"
TEMP_ERR="$(mktemp)"

# Flags to use with Valgrind
VALGRIND_FLAGS="--error-exitcode=15 --leak-check=full --show-leak-kinds=all \
				--errors-for-leak-kinds=all -q"

# Variables to echo colorful text.
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

for f in ${DIR}/*.in
do
	valgrind ${VALGRIND_FLAGS} ${PROGRAMME} <$f >${TEMP_OUT} 2>${TEMP_ERR}

	if cmp -s "${f%in}out" "${TEMP_OUT}"
	then
		if cmp -s "${f%in}err" "${TEMP_ERR}"
		then
			echo -e "${f%.in} ${GREEN}passed${NC}"
		else
			echo -e "${f%.in} ${RED}failed${NC}"
		fi
	else
		echo -e "${f%.in} ${RED}failed${NC}"
	fi

	# Remove temporary files.
	rm -f ${TEMP_OUT}
	rm -f ${TEMP_ERR}
done
