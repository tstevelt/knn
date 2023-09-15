#!/bin/sh

case "$1" in
	'-n' | '-norm' )
		knn -normalize -i 3 -f ../Data/Recommend_Label.csv -o Recommend_Label_NC.csv   -m Recommend.model  
		head ../Data/Recommend_Label.csv Recommend_Label_NC.csv
		echo ""
		echo "=== Recommend.model ==="
		cat Recommend.model
		;;

	'-t' | '-test' )
		knn -test -i 3 -f Recommend_Label_NC.csv -o Test.txt
		cat Test.txt
		;;

	'-p' | '-predict' )
		knn -predict -i 3  -p ../Data/Recommend_Predict.csv -f Recommend_Label_NC.csv -m Recommend.model -o Predict.csv
		head -20 Predict.csv
		;;

	'-c' | '-clean' )
		rm -f Predict.csv Recommend_Label_NC.csv Recommend.model Test.txt
		;;

	* )
		echo "USAGE: Recommend.sh {-norm|-test|-predict|-clean}"
		exit 1
		;;
esac


