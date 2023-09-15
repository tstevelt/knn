#!/bin/sh


if [ "$1" = '' ]
then
	echo "USAGE: Iris.sh { run | clean }"
	exit 1
fi

MODE=$1

case "$MODE" in
	'run' )
		K=5
		INPUT=4
		HILO=

		echo ""
		echo "Normalizing ..."
		knn -normalize -i $INPUT -f ../Data/iris.csv -o iris_NC.csv -m iris.model
		head ../Data/iris.csv iris_NC.csv
		echo ""
		echo "==== iris.model ===="
		cat iris.model

		echo ""
		echo "Testing ..."
		knn -test -v   -i $INPUT -k $K $HILO -f iris_NC.csv -o Test.txt -m iris.model
		head Test.txt
		grep Accuracy Test.txt
		echo ""
#		echo "Predicting ..."
#		knn -predict   -i $INPUT -r -k $K $HILO -f iris_NC.csv -p ../Data/iris_Predict.csv -o Predict.csv -m iris.model | grep assert > ./iris_assert.txt
#		head iris_assert.txt
#		echo ""
#		head Predict.csv
		;;

	'clean' )
		rm -f Predict.csv iris_assert.txt iris_NC.csv iris.model Test.txt
		;;

	* )
		echo "Unknown option, specify either run or clean"
		;;
	
esac


