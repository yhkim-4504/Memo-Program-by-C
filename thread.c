#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main_data = 0;

// ������ �Լ�
void *t_function(void *data)
{
    pid_t pid;            // process id
    pthread_t tid;        // thread id
 
    pid = getpid();
    tid = pthread_self();
 
    char* thread_name = (char*)data;
    int i = 0;
 
    while (1)   // 0,1,2 ������ loop �����ϴ�.
    {
        // �Ѱܹ��� ������ �̸��� 
        // ���� process id �� thread id �� �Բ� ���
        printf("[%s] pid:%u, tid:%x --- %d, maindata : %d\n", 
            thread_name, (unsigned int)pid, (unsigned int)tid, i, main_data);
        i++;
        sleep(1);  // 1�ʰ� ���
    }
}
 
int main()
{
    pthread_t p_thread[2];
    int thr_id;
    int status;
    int i;
    char p1[] = "thread_1";   // 1�� ������ �̸�
    char p2[] = "thread_2";   // 2�� ������ �̸�
    char pM[] = "thread_m";   // ���� ������ �̸�
 
 
    sleep(1);  // 2�� ����� ������ ����
 
    // �� 1�� ������ ����
    // ������ ������ �Լ��� t_function
    // t_function �� �Ű������� p1 �� �ѱ��.  
    thr_id = pthread_create(&p_thread[0], NULL, t_function, (void *)p1);
 
    // pthread_create() ���� ���������� �����尡 �����Ǹ� 0 �� ���ϵ˴ϴ�
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
 
    // �� 2�� ������ ����
    thr_id = pthread_create(&p_thread[1], NULL, t_function, (void *)p2);
    if (thr_id < 0)
    {
        perror("thread create error : ");
        exit(0);
    }
 
    // �� main() �Լ������� �����忡�� ���ư��� �ִ� ������ �Լ� ����
    //t_function((void *)pM);
    for(i=0; i<10; i++)
    {
    	main_data++;
    	sleep(1);
 	}
    // ������ ���Ḧ ��ٸ���. 
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);
 
    printf("���� ���� �ɱ��?\n");
 
    return 0;
}

