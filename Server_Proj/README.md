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

# Message Error Checking #
- get, del, set, oks, knf
- err bad, err len, err svr