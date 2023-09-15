#!/bin/sh 

RunC ()
{
	knn -normalize -i 3 -f ../Data/Recommend_Label.csv -o Recommend_Label_NC.csv -m Recommend.model 
	knn -test -i 3 -f Recommend_Label_NC.csv -o Test.txt
	grep Accuracy Test.txt
	rm -f Recommend_Label_NC.csv Recommend.model Test.txt
}

echo ""
echo "Data file ./Data/Recommend_Label.csv contains 2644 lines."
echo "Each line contains 3 input fields and a label"

echo ""
echo ""
echo "Timing python script knn.py loading, normalizing and testing"
time ../Python/knn.py

echo ""
echo ""
echo "Timing C program 'knn' running once to normalize and again to test"
time RunC
echo ""

