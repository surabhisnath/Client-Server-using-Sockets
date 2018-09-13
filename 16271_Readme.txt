Computer Networks: Assignment 1

Client-Server program using sockets

------------------------------------------------------------------------------

This zip file contains 4 files:

1) 16271_Server.c
2) 16271_Client.c
3) 16271_Makefile
4) 16271_Readme.txt - (this file)

---------------------------------------------------------------------------------------------------------------

Steps to run the program:


16271_Makefile needs to be run as follows to compile the Server and Client code:

make -f 16271_Makefile

Run the server using:

./sv

Open a new terminal for every client and run using:

./cl

This will ask for a promt for:

1) IP Address
2) Port Number

If both are provided correctly, the client will get accepted by the server:

Accepted client (client id) 

Now messages can be sent among clients simply by typing the message. Each client, including the sender receives:

Received from client (client id): (message)

----------------------------------------------------------------------------------------------------------------

Leaving by Informing and Abruptly:


Both client and server can leave either by informing, or abruptly:

1) Leaving by informing: Both server and client can type either "exit" or "quit" or "bye" to leave and disconnect
2) Leaving abruptly: Both server and client can leave by hitting Ctrl+C

When a client leaves, the server displays:

Client (client id) exited

If the server leaves, the active clients are forced to quit, they display:

Client exited

-----------------------------------------------------------------------------------------------------------------

Assumptions made:


1) Max number of clients that can be handled = 100
2) The port on which server runs is port number 5000
3) All messages are less than 3000 characters
4) Each client is given a unique client id in ascending order as 1, 2, 3...

-----------------------------------------------------------------------------------------------------------------