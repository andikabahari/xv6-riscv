#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char buf[4];

  int p[2];
  pipe(p);

  int pid = fork();
  if (pid == 0) {
    read(p[0], buf, sizeof(buf));
    printf("%d: received %s\n", getpid(), buf);
    write(p[1], "pong", 4);
  } else if (pid > 0) {
    write(p[1], "ping", 4);
    wait(0);
    read(p[0], buf, sizeof(buf));
    printf("%d: received %s\n", getpid(), buf);
  } else {
    fprintf(2, "fork error\n");
  }
  close(p[0]);
  close(p[1]);

  exit(0);
}
