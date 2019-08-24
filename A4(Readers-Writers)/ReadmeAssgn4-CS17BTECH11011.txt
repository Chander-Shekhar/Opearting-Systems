To compile the RW :
g++ rw-CS17BTECH11011.cpp -o rw -pthread

To compile the Fair RW :
g++ frw-CS17BTECH11011.cpp -o frw -pthread

To run the RW :
./rw < inp-params.txt

It outputs to a file named "RW-log.txt".

To run the FRW:
./frw < inp-params.txt

It outputs to a file named "FairRW-log.txt".

Note: keep the inp-params.txt file in the same folder as these. You need to pass inp-params.txt as a argument.
