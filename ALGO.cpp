_____________________
Algorithm 1 : Some Constants And Synchronization Primitives
---------------------
BARBERS = 0;//NUMBER OF BARBERS
CUSTMERS = 0;//NUMBER OF CUSTMERS IN WAITING SEATS
ACCESS_SEATS_MUTEX = 1;//MUTUAL EXCLUSION FOR THE BARBER
NUMBER_OF_FREE_SEATS = N;//NUMBER OF AVALIBAKE SEATS IN WAITIG ROOM
---------------------
_____________________
Algorithm 2 : Braber Routine
---------------------
while True do
|   wait(CUSTMERS);//Wait for custmers ZERO CUSTMERS ->(ASLEEP)
|   wait(MUTEX);// When ever wait(1) executes it decreaments value to 0
|   NUMBER_OF_FREE_SEATS++;//SEAT GETS FREE
|   sem_post(BARBERS);//bring custmer to haircut
|   sem_post(ACCESS_SEATS_MUTEX);//release the mutex on the chair
|   cutting_hair();
end

_____________________
Algorithm 3 : Custmer Routine
---------------------
while True do
sem_wait(ACCESS_SEATS_MUTEX);
if ACCESS_SEATS_MUTEX > 0  then
|   NUMBER_OF_FREE_SEATS--;
|   sem_post(CUSTMERS);//notify the barber that thers a custmer
|   sem_post(ACCES SEATS);//realse the lock
|   sem_wait(BARBERS);//wait in the waiting room if barber is busy custmer is having hair cut
|   get_haircut(); 
else{
|    sem_post(ACCES SEATS);//release the lock custmers leaves
}


#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

int FreeSeats = N;
mutex Seats;
condition_variable Barber, Customers;

void barber()
{
    while(true) {
        // waits for a customer (sleeps)
        unique_lock<mutex> lock(Seats);
        Customers.wait(lock, []{
            return FreeSeats < N; });
        
        // a chair gets free
        FreeSeats++;
        
        // bring customer for haircut
        Barber.notify_one();
        
        // release the mutex on the chair
        lock.unlock();
        
        // barber is cutting hair
    }
}

void customer(int id)
{
    while(true) {
        // protect seats so only 1 customer tries to sit in a chair if that's the case
        unique_lock<mutex> lock(Seats);
        if(FreeSeats > 0) {
            // sitting down
            FreeSeats--;
            
            // notify the barber
            Customers.notify_one();
            
            // release the lock
            lock.unlock();
            
            // wait in the waiting room if barber is busy
            unique_lock<mutex> barber_lock(Seats);
            Barber.wait(barber_lock);
            
            // customer is having hair cut
        } else {
            // release the lock
            lock.unlock();
            
            // customer leaves
        }
    }
}

int main()
{
    // start barber thread
    thread barber_thread(barber);
    
    // start customer threads
    thread customer_threads[Customers];
    for(int i = 0; i < Customers; i++) {
        customer_threads[i] = thread(customer, i);
    }
    
    // join customer threads
    for(int i = 0; i < Customers; i++) {
        customer_threads[i].join();
    }
    
    // join barber thread
    barber_thread.join();
    
    return 0;
}


Semaphore Customers = 0;
Semaphore Barber = 0;
Mutex Seats = 1;
int FreeSeats = N;
  
Barber {
    while(true) {
            /* waits for a customer (sleeps). */
            down(Customers);
  
            /* mutex to protect the number of available seats.*/
            down(Seats);
  
            /* a chair gets free.*/
            FreeSeats++;
              
            /* bring customer for haircut.*/
            up(Barber);
              
            /* release the mutex on the chair.*/
            up(Seats);
            /* barber is cutting hair.*/
    }
}
  
Customer {
    while(true) {
            /* protects seats so only 1 customer tries to sit
            in a chair if that's the case.*/
            down(Seats); //This line should not be here.
            if(FreeSeats > 0) {
                  
                /* sitting down.*/
                FreeSeats--;
                  
                /* notify the barber. */
                up(Customers);
                  
                /* release the lock */
                up(Seats);
                  
                /* wait in the waiting room if barber is busy. */
                down(Barber);
                // customer is having hair cut
            } else {
                /* release the lock */
                up(Seats);
                // customer leaves
            }
    }
}

