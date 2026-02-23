#include "types.h"
#include "stat.h"
#include "user.h"

static volatile int sink = 0;

void scheduling_test(int n, int t1, int t2, int t3);

void waste_cpu_time();

void run_base_tests(int t);

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

  run_base_tests(t);

  printf(1, "------ Runnning scheduling test: short ------\n");
  scheduling_test(10000000, 40, 20, 80);

  waste_cpu_time();

  printf(1, "------ Running scheduling test: long ------\n");
  scheduling_test(200050000, 40, 20, 80);

  waste_cpu_time();

  printf(1, "------ Runnning scheduling test: similar ticket counts ------\n");
  scheduling_test(10000000, 4, 6, 2);

  waste_cpu_time();

  printf(1, "testlottery: done\n");
  exit();
}

void run_base_tests(int t)
{
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
}

void waste_cpu_time()
{
  for (int k = 0; k < 200; k++)
    burn(200000);
}

void scheduling_test(int n, int t1, int t2, int t3)
{
  int pids[3];
  int tickets[3] = {t1, t2, t3};
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
