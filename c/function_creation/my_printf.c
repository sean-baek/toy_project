#include <stdio.h>
#include <stdarg.h>

int my_printf(const char *fmt, ...)
{
    int ret;
    va_list ap; 
    
    //초기 포인터 지정.
    va_start(ap, fmt);
    
    //vprintf 함수를 호출하여 고정 인수 값을 넘겨 받고 ap가 가리키는 값을 넣어준 뒤 ret에 저장.
    ret = vprintf(fmt, ap);
    
    //해제
    va_end(ap);
    
    return ret;
}

int main(void)
{
    my_printf("%d %d\n", 10, 20);

    return 0;
}
