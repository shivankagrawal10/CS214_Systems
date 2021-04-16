# Threads #

## Names and NetIDs ##
Shivank Agrawal (sa1461), Ameel Jani (aj580)

## Testing Strategy ##
We divided the program into 4 parts: User interface, directory and file queue functionality (sequential), word frequency and analysis functionality (sequential), and threading all the componenets.

### Testing User Interface ###

### Testing Directory and File Queue functionality ###
In this section we built out the methods responsible for searching through subdirectories and adding path names to the file and directory queues. To ensure this was working properly, we tested by feeding the program multi-layered directories. This was to ensure that our method was properly going into directories and adding path names of proper suffix to the queues. To test the queue functionality, we tried adding and removing items in edge cases such as when the queue was empty. 

### Testing WFD and Analysis ###

We wanted to know that our frequency counter was accurate. To ensure this we compared our generated frequency of text files to an online word frequency counter. We got the same results. To check edge cases, we fed in empty files, files with only space characters, files with only numbers, and files with only punctuation.

### Testing Multithreading ###