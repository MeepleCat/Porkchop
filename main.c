#include <winsock2.h> 
#include <Windows.h>
#include <stdio.h>
#include <string.h>

char* determineContentType(char* s, int i) {
  if(strncmp(6 + s + i, "html", 4) == 0) return "text/html";
  if(strncmp(6 + s + i, "js", 2) == 0) return "application/javascript";
  if(strncmp(6 + s + i, "wasm", 4) == 0) return "application/wasm";
  if(strncmp(6 + s + i, "ico", 3) == 0) return "image/x-icon";
  if(strncmp(6 + s + i, "css", 3) == 0) return "text/css";
  
  return "ERR";
}

int respond(SOCKET c, char* s) {
  if(memcmp(s, "GET /", 5) == 0) {
    int indexspace = 0;
    int indexdot = 0;

    while(indexspace <= 0) {
      if(s[5 + (indexspace * -1)] == ' ') indexspace *= -1;
      else if(s[5 + (indexspace) * -1] == '.' && indexdot <= 0) { 
        indexspace--; 
        indexdot *= -1;
      }
      else {
        indexspace--;
        if(indexdot <= 0) indexdot--;
      }
    }
    
    char* fname;
    FILE* f;
    char* code;
    int needsFreeing = 0;

    const char* ext = determineContentType(s, indexdot);  
    if(strncmp(ext, "ERR", 3) == 0) {
      code = "400 Bad Request";
      fname = "error.html";
      ext = "text/html";
      printf("ERR BAD REQUEST\n");
    }
    else {
      code = "200 OK";
      fname = malloc(indexspace + 1);
      needsFreeing = 1;
      strncpy(fname, s + 5, indexspace);
      fname[indexspace] = '\0';
    }

    f = fopen(fname, "rb");
    
    fseek(f, 0L, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buffer = (char*)malloc(size + 1);
    size_t bytesRead = fread(buffer, 1, size, f);
    fclose(f);

    char headers[256];
    int header_len = sprintf(headers,
            "HTTP/1.1 %s\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %zu"
            "Connection: close"
            "\r\n\r\n",
            code,
            ext,
            bytesRead);

    send(c, headers, header_len, 0);
    send(c, buffer, bytesRead, 0);

    if(needsFreeing) free(fname); free(buffer);

    printf("SENT\n-------------------------\n%s", headers);

    return 1;
  }
  else return -1;
}

int main() {
  WSADATA wsadata;
  WSAStartup(MAKEWORD(2, 2), &wsadata);
  SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in addr; 
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = 0;
  addr.sin_port = htons(36540); // port server is on 
  bind(s, (const struct sockaddr*)&addr, sizeof(addr));

  listen(s , 10);

  while(1) {  
    SOCKET client = accept(s, 0, 0);
    
    char request[1024] = {0};
    int bytesReceived = recv(client, request, sizeof(request) - 1, 0);
    if (bytesReceived <= 0) {
        printf("no bytes\n");
        closesocket(client);
        continue;
    }
    request[bytesReceived] = '\0';

    respond(client, request);
    closesocket(client);
  }
}