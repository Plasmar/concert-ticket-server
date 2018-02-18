# concert-ticket-server
A C++ implementation of a server that issues tickets for a concert with a few extra features

This is an implemenation of a server that provides tickets to various concerts.

I have included two seperate versions of the server:

  1. First as cts.cpp
  2. The other is comprised of two files (hall.cpp, hall.h).
      ** The latter was an excercise in incorporating header files in the design of the server code. 
      
Extra features:
--The business requirements called for various prices applied to tickets depending on seat location (hi, med, low range) 
--The code is designed to provide a discount to persons buying tickets in a group
--The code can print out the seating chart at any point with the PR command
--The code can calculate revenue, incorporating discounted ticket prices and normal priced tickets.
