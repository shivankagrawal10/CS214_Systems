# Threads #

## Names and NetIDs ##
Shivank Agrawal (sa1461), Ameel Jani (aj580)

## Testing Strategy ##

We divided the program into 4 parts: User interface, directory and file queue functionality (sequential), word frequency and analysis functionality (sequential), and threading all the componenets.

## Testing User Interface ##
To start with, we determined that users should only be able to enter the optional (-) arguments at the beginning of the arguments list (before the file/directory arguments). We then tested the program with different amounts of arguments starting with dashes. We also tried adding optional arguments to the program to see if it would recognize that and return EXIT_FAILURE. Some examples of these were -d-1, -dr, and -f0.05. In the first 2, the program recognized the invalid arguments and returned an error, while in the last one (the -f0.05 one), the program truncated the double argument into an int argument. We also made sure that in the end, if the number of file pairs with the provided suffix obtained from the directory and file arguments was 0, the program returned EXIT_FAILURE and printed a corresponding message to stderror.     

## Directory and File Queue ##
Testing Directory and File Queue functionality In this section we built out the methods responsible for searching through subdirectories and adding path names to the file and directory queues. To ensure this was working properly, we tested by feeding the program multi-layered directories. This was to ensure that our method was properly going into directories and adding path names of proper suffix to the queues. To test the queue functionality, we tried adding and removing items in edge cases such as when the queue was empty.

## Testing WFD and Analysis ##
We wanted to know that our frequency counter was accurate. To ensure this we compared our generated frequency of text files to an online word frequency counter. We got the same results. To check edge cases, we fed in empty files, files with only space characters, files with only numbers, and files with only punctuation.

## Testing Multithreading ##
We tested the multithreading by adding print statements in all of our thread functions. We added them right before and after the thread functions obtained the lock (to indicate when each thread obtained the lock) as well as when they were waiting for another thread to place items in the queues. This helped us ensure that our threads were able to avoid deadlocking and were able to ultimately tokenize the appropriate files correctly. We also tested the program on different amounts of file and directory threads. We tested it on very large numbers of threads (which ensured that some threads wouldn't actually) do work and very small amounts of threads (1 or 0).     
