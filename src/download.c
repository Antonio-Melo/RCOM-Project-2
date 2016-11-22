#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getip.c"
#include "clientTCP.c"

static const char DEFAULT_USER[] = "anonymous";
static const char DEFAULT_PASS[] = "eu";
static const char DEFAULT_PORT[] = "21";

typedef struct  ftpConnection{
  char * user;      /*user name*/
  char * pass;      /*password*/
  char * host;      /*server host*/
  char * port;      /*server port*/
  char * path;      /*file path*/
  char * filename;  /*name of file*/
  int user_len, pass_len, host_len, port_len, path_len, filename_len; /*length of char arrays*/
} ftpConnection;

int parseURL(char * url, ftpConnection *connection);
int terminate(ftpConnection *connection);

int main(int argc, char *argv[])
{

  if (argc != 2) {
    fprintf(stderr,"usage: download URL\n");
    exit(1);
  }

  ftpConnection client;
  if (parseURL(argv[1], &client) != 0){
    exit(2);
  }

  printf("User: %.*s\n", client.user_len, client.user);
  printf("Pass: %.*s\n", client.pass_len, client.pass);
  printf("Host: %.*s\n", client.host_len, client.host);
  printf("Port: %.*s\n", client.port_len, client.port);
  printf("Path: %.*s\n", client.path_len, client.path);
  printf("File: %.*s\n", client.filename_len, client.filename);

  getIPbyname(client.host);

  conetion();

  terminate(&client);

  return 0;
}

int parseURL(char * url, ftpConnection *connection){
  char buf1[100], buf2[100];
  int len1 = 0, len2 = 0;
  char * ptr = url;

  if (strncmp(url, "ftp://", 6) != 0){
    printf("Protocol is missing in the url inserted\n");
    return -1;
  }

  ptr+= 6;
  connection->user = (char  *) malloc(strlen(DEFAULT_USER));
  strncpy(connection->user,DEFAULT_USER,strlen(DEFAULT_USER));
  connection->user_len = strlen(DEFAULT_USER);

  connection->pass = (char  *) malloc(strlen(DEFAULT_PASS));
  strncpy(connection->pass,DEFAULT_PASS,strlen(DEFAULT_PASS));
  connection->pass_len = strlen(DEFAULT_PASS);

  connection->host_len = 0;

  connection->port = (char  *) malloc(strlen(DEFAULT_PORT));
  strncpy(connection->port,DEFAULT_PORT,strlen(DEFAULT_PORT));
  connection->port_len = strlen(DEFAULT_PORT);


  do{
    buf1[len1] = *ptr;
    len1++;
    ptr++;

    if((*ptr == ':') && (len1 > 1)){  //ao aparecer ':', buf1 poderá ser user ou server host
    ptr++;

    do{
      buf2[len2] = *ptr;
      len2++;
      ptr++;

      if(*ptr == '@'){ //ao parecer '@', buf1 e' user, buf2 e' password
      free(connection->user);
      free(connection->pass);
      connection->user = malloc(len1);
      connection->pass = malloc(len2);
      memcpy(connection->user,&buf1, len1);
      memcpy(connection->pass,&buf2, len2);
      connection->user_len = len1;
      connection->pass_len = len2;
      len1 = 0;
      len2 = 0;
    }else if (*ptr == '/'){ //ao parecer '/', buf1 e' host, buf2 e' port
    connection->host = malloc(len1);
    connection->port = malloc(len2);
    memcpy(connection->host,&buf1, len1);
    memcpy(connection->port,&buf2, len2);
    connection->host_len = len1;
    connection->port_len = len2;
    len1 = 0;
    len2 = 0;
  }

}while(*ptr != '\0' && *ptr != '@' && *ptr != '/');
if(*ptr == '/') break;
else ptr++;

} else if (*ptr == '/'){ //ao aparecer '/', buf1 é server host
connection->host = malloc(len1);
memcpy(connection->host,&buf1, len1);
connection->host_len = len1;
len1 = 0;

}
}while(*ptr != '\0' && *ptr != '/');

if (connection->host_len == 0){
  printf("Host server is missing in the url inserted\n");
  return -2;
}

connection->path_len = 0;
connection->filename_len = 0;

do{
  buf1[len1] = *ptr;
  len1++;
  ptr++;
  if(*ptr == '/'){
    memcpy(&buf2[len2],&buf1, len1);
    len2 += len1;
    len1 = 0;
  }
}while(*ptr != '\0');

if ((len1 - 1) == 0){
  printf("File name is missing in the url inserted\n");
  return -3;
}

connection->path = malloc(len2);
memcpy(connection->path, &buf2, len2);
connection->path_len = len2;

connection->filename = malloc(len1 - 1);
memcpy(connection->filename, &buf1[1], len1 - 1);
connection->filename_len = len1 - 1;

return 0;
}

int terminate(ftpConnection *connection){
  free(connection->user);
  free(connection->pass);
  free(connection->host);
  free(connection->port);
  free(connection->path);
  free(connection->filename);
  return 0;
}