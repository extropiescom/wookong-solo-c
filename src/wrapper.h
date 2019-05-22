

char* get_count(int port);
char* get_info(int port);
char* get_modifypin(int port);
char* get_generate(int port);
char* get_format(int port);
char* get_import(int port);
char* get_reboot(int port);

char* post_address(int port, char* data);
char* post_sign(int port, char* data);
char* post_xrppub(int port, char* data);
char* post_update(int port, char* data);
char* post_verify(int port, char* data);

