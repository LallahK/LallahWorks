#include "bomb_core.h"

int phase_num = 0;

int post_result(char *student_input, int exploded);

const char *STUDENT_NUMBER = "12345678";
const char *BOMB_ID = "abcdefgh";
char *server_addr = "localhost:8080";

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  MemoryStruct *mem = (MemoryStruct *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

static char* make_phase_req_url(char *url) {
  sprintf(url, "%s/get_phase_info?bomb_id=%s&student_num=%s&phase_num=%d",
          server_addr, BOMB_ID, STUDENT_NUMBER, phase_num);
  return url;
}

static void sig_handler(int signo) {
    fprintf(stderr, "\nNo!\n");

    /* Explodes the bomb if the user closes the terminal. */
    if (signo == SIGHUP || signo == SIGTERM || signo == SIGQUIT) {
        explode_bomb("User closed terminal");
    }
}

void bomb_init() {
  signal(SIGINT, sig_handler);
  signal(SIGTSTP, sig_handler);
  signal(SIGHUP, sig_handler);
  signal(SIGTERM, sig_handler);
  signal(SIGQUIT, sig_handler);

  curl_global_init(CURL_GLOBAL_DEFAULT);
}

void bomb_cleanup() {
  curl_global_cleanup();
}

void bomb_exit() {
  bomb_cleanup();
  exit(0);
}

char *get_phase_info() {
  CURL *curl;
  CURLcode res;
  MemoryStruct chunk;

  char url[URL_BUF_SIZE] = {'\0'};
  char errbuf[ERR_BUF_SIZE] = {0};

  make_phase_req_url(url);

  chunk.memory = malloc(1);
  chunk.size = 0;

  curl = curl_easy_init();

  if (curl) {

    /* Set the options for the request */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)(&chunk));
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      free(chunk.memory);
      return NULL;
    }

    return chunk.memory;

  } else {
    free(chunk.memory);
    return NULL;
  }
}

void defuse_phase(char *student_input) {
  if (post_result(student_input, NOT_EXPLODED)) {
    printf("Well done! You have defused phase %d\n", phase_num);
  } else {
    printf("The server doesn't think the phase was defused successfully... BOOM!\n");
    bomb_exit();
  }
}

void explode_bomb(char *student_input) {
  post_result(student_input, EXPLODED);
  fprintf(stderr, "BOOM!\n");
  bomb_exit();
}

int post_result(char *student_input, int exploded) {
  CURL *curl;
  CURLcode res;
  MemoryStruct chunk;

  int success = 0;
  char url[URL_BUF_SIZE] = {'\0'};
  char data[DATA_BUF_SIZE] = {'\0'};
  char errbuf[ERR_BUF_SIZE] = {0};

  curl = curl_easy_init();

  char* escaped_input = curl_easy_escape(curl, student_input, 0);
  free(student_input);

  sprintf(url, "%s/post_result", server_addr);
  sprintf(data, "bomb_id=%s&student_num=%s&phase_num=%d&student_input=%s&exploded=%d",
          BOMB_ID,
          STUDENT_NUMBER,
          phase_num,
          escaped_input,
          exploded
          );


  chunk.memory = malloc(1);
  chunk.size = 0;

  curl_free(escaped_input);

  if (curl) {

    /* Set the options for the request */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)(&chunk));
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(data));


    res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
      success = !strcmp(chunk.memory, "SUCCESS");
      printf("Success is %d\n", success);
      printf("Response is %s\n", chunk.memory);
    }

    free(chunk.memory);

    return success;

  } else {
    free(chunk.memory);
    return success;
  }
}

char *read_line(void) {
  /* Is all this memory necessary? */
  char *line = malloc(MAX_LINE_LEN*sizeof(char));
  char *input= malloc(MAX_LINE_LEN*sizeof(char));
  char *secret = malloc(MAX_LINE_LEN*sizeof(char));
  int c;
  int ret;

  ret = fscanf(stdin, "%[^\n]", line);

  if (ret == 0) {
    freopen("/dev/tty", "rw", stdin);
    fscanf(stdin, "%[^\n]", line);
  }

  /* This was for 2019 phase 6. Might keep it in, because it's a pretty neat trick IMO */
  // ret = sscanf(line, "{%[^}]} %[^\n]", secret, input);
  // if (ret == 0) {
  //   /* Did not enter part of the phase 6 secret key */
  //   sscanf(line, "%[^\n]", input);
  // } else if (strlen(secret) < 20) {
  //   strcat(password, secret);
  // }

  // Delete this if you uncomment the above
  sscanf(line, "%[^n]", input);

  for (c = getchar(); !isspace(c) && c != EOF; c = getchar()) {}
  free(line);
  free(secret);
  return input;
}
