COUNTER=0
while [ $COUNTER -lt 10 ]; do
	echo execucao $COUNTER
	let COUNTER=$[ $COUNTER + 1 ]
done
