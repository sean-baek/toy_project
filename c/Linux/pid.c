#include <stdio.h>
#include <unistd.h>

int main(void)
{
  pid_t pid;
  pid = fork(); //fork�Լ� ����(���� �� -1, ���� �� �θ𿡰� ���ο� pid, �ڽĿ��� 0�� ��ȯ

  if(pid > 0)
  {
    printf("parent process %d\n", getpid());
  }

  else if(pid==0)
  {
    printf("child process %d\n",getpid());
    pid = fork();
    if(pid>0)
    {
      printf("child process %d\n", getpid());
    }

    else printf("grand child %d\n", getpid());
  }

  else printf("error\n");
  //system("ps | grep (���� ������ �ִ� ���丮 �̸�)");
  return 0;
}

