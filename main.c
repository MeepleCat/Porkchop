#include <winsock2.h> 
#include <Windows.h>
#include <stdio.h>

int main() {
  WSADATA wsadata;
  WSAStartup(MAKEWORD(2, 2), &wsadata);
  SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in addr; 
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = 0;
  addr.sin_port = htons(8080); // port server is on 
  bind(s, (const struct sockaddr*)&addr, sizeof(addr));

  while(1) {
    listen(s , 10);
  
    SOCKET client = accept(s, 0, 0);

    char request[256] = {0};
    recv(client, request, 256, 0);

    // GET /[file] ...
    if(memcmp(request, "GET / ", 6) == 0) {
      FILE* f = fopen("index.html", "r");
      char buffer[256] = {0};
      fread(buffer, 1, 256, f);
      send(client, buffer, 256, 0);
    }
  }
}

/*(int main(void)
{
  printf("test...\n");

  CURL *curl;
 
  CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
  if(result != CURLE_OK)
    return (int)result;
 
  curl = curl_easy_init();
  if(1 == 2) {
  //if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://ssd.jpl.nasa.gov/api/horizons.api?format=json&COMMAND='399'&CENTER='500@10'&EPHEM_TYPE=VECTORS&START_TIME='2026-03-01'&STOP_TIME='2026-03-02'&STEP_SIZE='1h'");
    /* example.com is redirected, so we tell libcurl to follow redirection */
    //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");

    /* Perform the request, result gets the return code *//*
    result = curl_easy_perform(curl);
    /* Check for errors *//*
    if(result != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(result));
 
    /* always cleanup *//*
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}*/