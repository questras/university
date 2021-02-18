

if [ $# -eq 1 ] || [ $# -eq 2 ]
then	
	name=$1
	g++ ${name}/${name}.cpp -o ${name}/${name}

	if [ $# -eq 2 ]
	then
		data=$2
		if [ ${data} == "all" ]
		then
			counter=1
			for file in ${name}/*.in
			do
				echo -e "\033[0;32m${counter}. ${file}:\033[0m"
				time ./${name}/${name} < ${file}
				counter=$((counter+1))
			done
		else
			time ./${name}/${name} < ${name}/${data}.in
		fi
	else
		./${name}/${name}
	fi

else
	echo "Wrong number of arguments"
fi
