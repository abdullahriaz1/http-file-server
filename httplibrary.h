#define BUFFER_SIZE 4096

typedef struct httpMessage {
    char method[4];
    char uri[BUFFER_SIZE];
    char version[9];
    //Status will be adjusted depending on errors
    int status_code;
    int content_length;
    char buf[BUFFER_SIZE + 1];
} httpMessage;

//validate the uri
bool check_uri(char[]);
//check the buf to find request line, store vals in httpMessage, check for carriage return and newline, set status code
void read_request(int, httpMessage *);

//For PUT request, use content length to determine how many bytes we need to read from httpMessage and loop through the message
void handle_input();
//Check if get or put
void parse_request(int, httpMessage *);

//For GET request, loop through file and send to client
void send_file(int, int, httpMessage *);
//Respond with status. For GET with status 200 or 201, invoke send_file.
void send_response(int, httpMessage *);
