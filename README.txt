KNN – a K-Nearest-Neighbor program written in C

The knn program can be used for both classification and regression problems.

USAGE: knn -normalize -f input -o output -m model  [options]
USAGE: knn -test      -f input [options]
USAGE: knn -predict   -f input -o output -p predict [-m model] [options]
{-c|-r} classification (string) vs regression (numeric) (default is c)
-i #    input count (default 4)
-k #    neighbor count (default 5)
-hilo   during regression prediction, eliminate highest and lowest neighbor
-v      verbose
-d      debug

Data Format

Unlike most other ML programs, the training/testing data rows start with an ID column.  The ID shows up in verbose classification output.  It also begins to address one of the common criticisms against ML programs, in that we can determine during debugging what is causing network behavior.


Sample Data and Examples
The Data directory contains some sample CSV data files.  The Examples directory contains some shell scripts which demonstrate various features of knn.  The Python directory contains a python script which operates against the same basic data file (does not contain leading ID column).


DemoData.sh
CLASSIFICATION. Works with three data files which display strong, medium and random separation of values against 4 different labels. 

Iris.sh
The standard Iris CLASSIFICATION demonstration.  Data file modified to include a randomly generated leading ID column.
  
Price.sh
Demonstrates REGRESSION example to predict the price of a stock, based on three fundamentals.

Recommend.sh
Demonstrates CLASSIFICATION to STRONG BUY, BUY, HOLD or SELL a stock.  Data is skewed, there are only a few SELL stocks and no STRONG SELL stocks.  Data is based on the same three fundamentals.

ComparePythonToC.sh
This uses the same data file as the Recommend.sh example and tests it with the Python script and the C program. Execution times are drastically different.

References

Python script from Jason Brownlee at Machine Learning Mastery.
https://machinelearningmastery.com/tutorial-to-implement-k-nearest-neighbors-in-python-from-scratch/



