/*test machine: CSELAB_KH-1250-17
* date: 10/23/2019
* name: Ansh Sikka
* x500: sikka008
*/

# Multithreaded MapReduce
Implementation of MapReduce, a tool/algorithm that uses multiple processes large datasets and synchronize output of key-value pairs. Uses producer-consumer multithreading model.
## Purpose
The purpose of this project is to keep large datasets in sync by using mapper and reducer processes. This project also show how to manipulate threads in terms of producers and consumers. The producer puts packets of data into the queue (lines of test files) and multiple consumers process each word in the line and synchronize a histogram. 

## How to compile

Make sure you have a file you can pass in. Know the number of consumers you have and if needed, the size of the bounded buffer.

Before executing all recipes, do this: 
`make clean`
`make`

Then,
`./wcs <Number of Consumers> <filename>`

If you want to log data to the screen,
`./wcs <Number of Consumers> <filename> -p`

If you want to use a restricted buffer,
`./wcs <Number of Consumers> <filename> -b <Queue Size>`
If you want to both log data and use a restricted buffer
`./wcs <Number of Consumers> <filename> -bp <Queue Size>`



## Assumptions
* Can use atoi function from standard string library
* Can utilize some global variables
* Can use `fileno` (gives warnings, but program works correctly)
* Can use `size_t` to cast to a `long int` (supresses warnings about downcasting from pointer to int when passing in consumer IDs)

## Team Names and x500 + Contributions
Ansh Sikka -sikka008

I did have a partner: Jacob Isler: isle0011
We did work on the project individually. However we did use each other to
bounce off each others ideas on how to move forward with the product. There 
was little to no overlap of code. The general strategies were similar between the projects.
