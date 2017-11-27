# Chat Application for mc833

This project is compiled using cmake tool

commands to compile:
~~~~
cmake .
make
~~~~

The client usage is:
`./client <hostname> <port> <nickname>`

inside the client application you have several commands
/exit -> Closes the chat, sending a disconnect message to the server
/pm -> Asks for a nickname to start a private chat (After this, the chat will change to private mode)
/global -> Sets the chat to global mode

- On global mode the sent messages are broadcast to all clients
- On private model all messages are sent only to a specific user with the given nickname

The server usage is:
`./server <port>`