#include <stdio.h>  // fopen, fread, fprintf, fclose
#include <stdint.h> // uint32_t, size_t
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS

union header {
  struct {
    uint8_t jmpboot[3];
    char oemname[8];
    uint16_t bytes_per_sec;
    uint8_t sec_per_clus;
  };
  uint8_t padding[512];
} header;

int main() {
  FILE *stream;
  size_t read;
  int retval;

  stream = fopen("fatdisk", "r");
  if (stream == NULL) {
    fprintf(stderr, "Couldn't open fatdisk.\n");
    exit(EXIT_FAILURE);
  }

  read = fread(&header, sizeof(union header), 1, stream);
  if (read != 1) {
    fprintf(stderr, "Couldn't read volume header block.\n");
    exit(EXIT_FAILURE);
  }

  retval = fclose(stream);
  if (retval != 0) {
    fprintf(stderr, "Colund't close the file.. weird.\n");
    exit(EXIT_FAILURE);
  }

  fprintf(stdout, "jmpboot: %02x%02x%02x\n",
    header.jmpboot[0], header.jmpboot[1], header.jmpboot[2]);
  fprintf(stdout, "oemname: %s\n", header.oemname);
  fprintf(stdout, "bytes_per_sec: %d\n", header.bytes_per_sec);
  fprintf(stdout, "sec_per_clus: %d\n", header.sec_per_clus);

  exit(EXIT_SUCCESS);
}
