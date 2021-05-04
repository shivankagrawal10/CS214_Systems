Names: 
Shivank Agrawal (sa1461) Ameel Jani (aj580)

# Compile and Run Instructions #
1. make {to compile the server}
2. ./server (portnumber) {to run the server}
3. open new terminal windows
4. netcat ilab#.cs.rutgers.edu (portnumber) {to connect}
5. send/receive/delete key/pair items

# Server #

Checking the server involved verifying if the server was working, checking that clients could connect, and checking that the outputs were proper. To test the server was working and that a client could connect we ran the server program and used netcat to connect to the server, if this connection went through successfully we put a print message on the server end. We also wanted to check that multiple clients could connect to the same server and tested this by opening multiple terminals and connecting to the server. To check for proper server functionality, we tested proper inputs as given in the problem statement. We attempted to put wrong inputs or wrong input sizes to check that our server could detect errors.

# Linked List #
Primary testing involved verifying that key pair values were properly allocated to memory and that the the nodes were linked proplerly. We tested these in a separate file just to ensure functionality. Testing this involved inserting, deleting, and sorting multiple key pair values. We varied the lengths of the key pair string values to ensure that we were able to accommodate for long string inputs.

# Threading #
In order to test the functionality of the multithreading and locks present in our code, we first started the server up on one machine and then connected to that server using netcat from several other machines. We then entered in client messages concurrently from multiple clients. We then checked to make sure the different threads (representing different client connections) modified the Linked List structure in a deterministic way. We also checked to make sure our program didn't experience a deadlock at any point. We ran the server code executable using gdb to more closely verify this. 

# Message Error Checking #
We entered several messages as the client to the server in order to test our program's recognition of improperly formatted messages. We made sure that for GET and DEL messages, the server would send "KNF" to the client if the specified key was not present in the Linked List Data Structure. 

Additionally, we tested if our program would recognize and report ERR BAD if the first line of the message was something other than SET, GET, or DEL followed by a \n. We also tested if our program would recognize and report ERR BAD if the 2nd line of the client message was anything other than a number followed by \n. We also tested if the program would recognize and report ERR LEN if the payload message (including the \n statements) length was not exactly equal to the specified payload length in the 2nd line. We also made sure to report ERR BAD if the payload for a GET or DEL message only consisted of "" or \n and if any of the 2 fields for a SET message only equaled "" or \n. Furthermore, we made sure that the program reported ERR SRV to the client if any of the pthread, pthread_mutex, or malloc functions in the thread function for that client failed. 

Finally, we made sure to test using gdb and printf statements whether the server terminated the connection to the specific client whose inputs forced the program to report the errors to the client. 
