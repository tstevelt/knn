#!/bin/sh


if [ "$1" = '' ]
then
	echo "USAGE: Price.sh { test | predict | clean }"
	exit 1
fi

MODE=$1

case "$MODE" in
	'test' )
		for FILE in strong medium random
		do
			echo "Testing ../Data/$FILE.csv"
			sort -R ../Data/$FILE.csv > $FILE.csv
			knn -test -i 4 -k 10 -f $FILE.csv -o $FILE.txt
			cat $FILE.txt
		done
		;;

	'predict' )
		sort -R ../Data/predict.csv | head -5 > predict.csv
		echo ""
		echo "predict 5 rows against strong.csv"
		knn -predict -i 4 -k 5 -f ../Data/strong.csv -p predict.csv -o strong_predict.txt
		cat strong_predict.txt
		echo "press [Enter] to continue ..."
		read x
		echo ""
		echo "predict 5 rows against random.csv"
		knn -predict -i 4 -k 5 -f ../Data/random.csv -p predict.csv -o random_predict.txt
		cat random_predict.txt
		;;

	
	'clean' )
		for FILE in strong medium random
		do
			rm -f $FILE.csv $FILE.txt
		done
		rm -f predict.csv strong_predict.txt random_predict.txt
		;;

	* ) 
		echo "Unknown option, specify test, predict or clean"
		exit 1 
		;;
esac



