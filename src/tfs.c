#include <stdio.h>  // fopen, fread, fprintf, fclose
#include <stdint.h> // uint32_t, size_t
#include <stdlib.h> // exit, EXIT_FAILURE, EXIT_SUCCESS

static inline uint32_t
from_big_endian(uint32_t value) {
  return ((value & 0xFF000000) >> 24) |
         ((value & 0x00FF0000) >> 8)  |
         ((value & 0x0000FF00) << 8)  |
         ((value & 0x000000FF) << 24);
}

union header {
  struct {
    uint32_t magic;
    char volname[16];
  };
  uint8_t padding[512];
} header;

int main() {
  FILE *stream;
  size_t read;
  int retval;

  stream = fopen("store.file", "r");
  if (stream == NULL) {
    fprintf(stderr, "Couldn't open store.file.\n");
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

  fprintf(stdout, "Magic number: %x\n", from_big_endian(header.magic));
  fprintf(stdout, "Volume name: %s\n", header.volname);

  exit(EXIT_SUCCESS);
}
