#include <malloc.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

pthread_cond_t cond;
pthread_key_t key;
pthread_once_t once;

void key_create()
{
    pthread_key_create(&key,NULL);
}
char *getErr(int errCode)
{
    char *str;

    pthread_once(&once,key_create);
    str=pthread_getspecific(key);

    if (str==NULL)
    {
        str=malloc(sizeof(char)*15);
        pthread_setspecific(key,str);
    }


    if (errCode==0)
        strcpy(str,"Bad operation");
    else if (errCode==1)
        strcpy(str,"Bad operand");
    else if (errCode==2)
        strcpy(str,"Bad buffer size");
    else
        strcpy(str,"Bad error code");


    return str;
}

void *func(void *k)
{
    int *c=(int *)k;
    printf("%s %d\n",getErr(*c),*c);
}

int main()
{
    pthread_t tid[100];
    int k[100],status;


    for (int i=0;i<100;i++)
    {
        k[i]=i;
        pthread_create(&tid[i],NULL,func,(void *)&k[i]);
    }

    for (int i=0;i<100;i++)
    {
        pthread_join(tid[i],(void *)&status);
    }

    return 0;
}
