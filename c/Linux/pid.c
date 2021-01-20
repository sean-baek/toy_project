#include <stdio.h>
#include <unistd.h>

int main(void)
{
  pid_t pid;
  pid = fork(); //fork함수 실행(실패 시 -1, 성공 시 부모에게 새로운 pid, 자식에게 0이 반환

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
  //system("ps | grep (현재 파일이 있는 디렉토리 이름)");
  return 0;
}

