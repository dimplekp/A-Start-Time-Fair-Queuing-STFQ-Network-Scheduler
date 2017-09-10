# A Start Time Fair Queuing STFQ Network Scheduler

A proportional-share STFQ network scheduler in C++ that

- schedules network bandwidth across multi-threaded streams
- limits the maximum aggregate rate at which a set of streams can send or receive
- divides specified fractions of send and receive bandwidth among different network streams
- synchronizes shared objects using lock and condition variables

Evaluate the performance of scheduler using a program that sends an array of bytes across the network by putting data into the underlying socket and returns only when it is its turn to do so. Strong emphasis on following standards for multithreading programming.
