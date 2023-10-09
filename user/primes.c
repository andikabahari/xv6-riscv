#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int min = 2;

int main(int argc, char *argv[]) {
  int fd[2][2];
  pipe(fd[0]);

  int idx = 0;
  int num;
  for (int i = min; i < 35; i++) {
    write(fd[idx][1], &i, 4);
  }
  close(fd[idx][1]);

  while (fork() == 0) {
    if (read(fd[idx][0], &min, 4) != 0) {
      printf("prime %d\n", min);
    } else {
      exit(0);
    }

    pipe(fd[idx^1]);
    while (read(fd[idx][0], &num, 4) != 0) {
      if (num % min) {
        write(fd[idx^1][1], &num, 4);
      }
    }
    close(fd[idx^1][1]);
    idx = idx^1;
  }
  close(fd[idx][0]);
  
  wait(0);
  exit(0);
}