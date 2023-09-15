Client -> Socket -> Listener -> Init -> Accept -> Parse -> Response -> Socket -> Client
main -> read request -> handle input -> check valid uri
     -> parse request -> send file + response
     -> send response -> send response of put
httpMessage struct will carry all values that we need for httpMessage

So far, programmed functionality of GET. 
I parsed the request with sscanf to get the tokens separated by spaces, then stored those in httpMessage struct.
Checked if the URI was valid according to requirements, then checked if version was 1.1, then checked if method was put or get.
If get, then execute sending file by writing to socket. 
If put, then execute receiving file by reading from socket and writing to file.