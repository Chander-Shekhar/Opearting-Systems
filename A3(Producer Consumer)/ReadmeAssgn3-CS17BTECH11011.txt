To compile the Semaphore Implementation :
g++ prod_cons-sems-CS17BTECH11011.cpp -o semaphore -pthread

To compile the Lock Implementation :
g++ prod_cons-locks-CS17BTECH11011.cpp -o lock -pthread

To run the Semaphore Implementation :
./semaphore < inp-params.txt

It outputs to a file named lock_log.txt .

To run the Lock Implementation :
./lock < inp-params.txt

It outputs to a file named semaphore_log.txt .

Note: keep the inp-params.txt file in the same folder as these. You need to pass inp-params.txt as a argument.