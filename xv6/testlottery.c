#include "types.h"
#include "stat.h"
#include "user.h"

static volatile int sink = 0;

void scheduling_test(int n);

static void burn(int n)
{
  for (int i = 0; i < n; i++)
    sink += i;
}

int main(int argc, char **argv)
{
  int t = 10;
  if (argc == 2)
    t = atoi(argv[1]);

  printf(1, "------ Base tests ------\n");
  if (settickets(0) == 0)
  {
    printf(1, "FAIL: settickets(0) should fail\n");
    exit();
  }
  if (t >= 1 && settickets(t) != 0)
  {
    printf(1, "FAIL: settickets(%d) should succeed\n", t);
    exit();
  }

  printf(1, "PASS: settickets validation\n");

  printf(1, "------ Scheduling test short ------\n");
  scheduling_test(10000000);

  for (int k = 0; k < 200; k++)
    burn(200000);

  printf(1, "------ Scheduling test long ------\n");
  scheduling_test(20005000);

  for (int k = 0; k < 200; k++)
    burn(200000);

  printf(1, "testlottery: done\n");
  exit();
}

void scheduling_test(int n)
{
  int pids[3];
  int tickets[3] = {40, 20, 80};
  int i;

  for (i = 0; i < 3; i++)
  {
    int pid = fork();
    if (pid < 0)
    {
      printf(1, "Fork failed\n");
      exit();
    }
    else if (pid == 0)
    {
      settickets(tickets[i]);
      sleep(100);
      burn(n);
      exit();
    }
    else
    {
      pids[i] = pid;
    }
  }

  for (int i = 0; i < 3; i++)
  {
    int finished_pid = wait();

    for (int j = 0; j < 3; j++)
    {
      if (pids[j] == finished_pid)
      {
        printf(1, "Parent: Child with %d tickets exited first among remaining.\n", tickets[j]);
      }
    }
  }
}
