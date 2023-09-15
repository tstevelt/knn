#!/bin/sh

if [ "$1" = '' ]
then
	echo "USAGE: Price.sh { run | clean }"
	exit 1
fi

MODE=$1

case "$MODE" in
	'run' )
		K=12
		INPUT=3
		HILO=-hilo

		echo ""
		echo "Normalizing ..."
		knn -normalize -i $INPUT -r             -f ../Data/Price_Label.csv -o Price_Label_NC.csv -m Price.model
		head ../Data/Price_Label.csv Price_Label_NC.csv
		echo ""
		echo "==== Price.model ===="
		cat Price.model

		echo ""
		echo "Testing ..."
		knn -test -v   -i $INPUT -r -k $K $HILO -f Price_Label_NC.csv -o Test.txt -m Price.model
		head Test.txt
		grep Accuracy Test.txt
		echo ""
		echo "Predicting ..."
		knn -predict   -i $INPUT -r -k $K $HILO -f Price_Label_NC.csv -p ../Data/Price_Predict.csv -o Predict.csv -m Price.model
		if [ -f Price_assert.txt ]
		then
			head Price_assert.txt
		fi
		echo ""
		head Predict.csv
		;;

	'clean' )
		rm -f Predict.csv Price_assert.txt Price_Label_NC.csv Price.model Test.txt
		;;

	* )
		echo "Unknown option, specify either run or clean"
		;;
	
esac


