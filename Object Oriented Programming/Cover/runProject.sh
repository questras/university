#!/bin/bash

DIR=$1
TEMP_OUT="$(mktemp)"

# Variables to echo colorful text.
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

total=0
correct=0

javac src/cover/*.java -d classes

for f in ${DIR}/*.in; do
	echo -e "Test: $f"
	((total++))
	time java -cp classes cover.Main <$f >${f%.in}moj.out
	if cmp -s "${f%in}out" "${f%.in}moj.out"
	then
		echo -e "${f%.in} ${GREEN}passed${NC}"
		((correct++))
	else
		echo -e "${f%.in} ${RED}failed${NC}"
	fi

	rm -f ${TEMP_OUT}
	#rm -f ${f%.in}moj.out
done

echo -e "passed ${correct}/${total}"
