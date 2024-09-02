void handle_request(int client_fd);
void parse_request(char *request, char *method, char *path, char *version);
void send_response(int cfd, char *version, char *filename);
void parse_path(char *path, char *fname);
void client_error(int cfd, char *version, char *error_cause, int error_code, char *error_msg, char *error_description);
void get_filetype(char *fname, char *ftype);
void create_http_response_headers(char *resp, char *version, int status_code, char* status_msg, int content_length, char *content_type);