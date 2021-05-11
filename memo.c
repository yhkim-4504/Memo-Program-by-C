#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct DateTime{
	int year;
	int month;
	int day;
	int hour;
	int min;
} DateTime;
 

typedef struct Memo {
	DateTime created_time;
	char contents[128];
	DateTime alarm_time;
} Memo;

void print_memo(Memo memo);
void print_all_memo(Memo* memo, int max_index);
void set_memo(Memo* memo, int year1, int month1, int day1, int hour1, int min1, char* contents,
	int year2, int month2, int day2, int hour2, int min2);
	
int max = 0;

int main()
{
	int select, memo_num;
	int year, month, day, hour, min;
	char temp_string[128];
	struct tm* t;
	time_t tnow = time(NULL);
	t = (struct tm*)localtime(&tnow);
	Memo* memo_data = (Memo*)malloc(sizeof(Memo));
	
	printf("�޸� ���� ���α׷��Դϴ�.\n"); 
	
	
	while(1)
	{
		printf("\n�޸��Է�(1), �޸����(2), �޸����(3), �޸�Ȯ��(4) ���α׷� ����(5) �Է� : ");
		scanf("%d", &select);
		
		if(select == 1)
		{
			printf("�޸� ���� �Է� : ");
			scanf("%s", temp_string);
			
			printf("�˶���¥ �Է� (�� �� ��) : ");
			scanf("%d %d %d", &year, &month, &day);
			
			printf("�˶��� �Է� (�� ��) : ");
			scanf("%d %d", &hour, &min);
			
			tnow = time(NULL);
			t = (struct tm*)localtime(&tnow);
			
			memo_data = (Memo*)realloc(memo_data, ++max * sizeof(Memo));
			set_memo(&memo_data[max-1], (t->tm_year) + 1900, (t->tm_mon) + 1, t->tm_mday, t->tm_hour, t->tm_min,
				temp_string, year, month, day, hour, min);
			printf("%d�� �޸� �ԷµǾ����ϴ�.\n", max); 
		}
		else if(select == 3)
		{
			if(max == 0)
			{
				printf("������ �޸� �����ϴ�.\n");
				continue;
			}
			
			while(1)
			{
				printf("������ �޸��ȣ�� �Է����ּ��� : ");
				scanf("%d", &memo_num);
				
				if( (memo_num < 1) || (memo_num > max) )
					printf("������ �´� �޸��ȣ�� �Է����ּ���!\n");
				else
					break;
			}
			
			printf("�޸� ���� �Է� : ");
			scanf("%s", temp_string);
			
			printf("�˶���¥ �Է� (�� �� ��) : ");
			scanf("%d %d %d", &year, &month, &day);
			
			printf("�˶��� �Է� (�� ��) : ");
			scanf("%d %d", &hour, &min);
			
			tnow = time(NULL);
			t = (struct tm*)localtime(&tnow);
			set_memo(&memo_data[memo_num-1], (t->tm_year) + 1900, (t->tm_mon) + 1, t->tm_mday, t->tm_hour, t->tm_min,
				temp_string, year, month, day, hour, min);
			printf("%d�� �޸� �����Ǿ����ϴ�.\n", memo_num); 
		}
		else if(select == 4)
		{
			print_all_memo(memo_data, max);
		}
		else if(select == 5)
		{
			printf("���α׷��� �����մϴ�.\n");
			break; 
		}

	}
	
	free(memo_data);
 
    return 0;
}

void set_memo(Memo* memo, int year1, int month1, int day1, int hour1, int min1, char* contents,
	int year2, int month2, int day2, int hour2, int min2)
{
	memo->created_time.year = year1;
	memo->created_time.month = month1;
	memo->created_time.day = day1;
	memo->created_time.hour = hour1;
	memo->created_time.min = min1;
	strcpy(memo->contents, contents);
	memo->alarm_time.year = year2;
	memo->alarm_time.month = month2;
	memo->alarm_time.day = day2;
	memo->alarm_time.hour = hour2;
	memo->alarm_time.min = min2;
}

void print_all_memo(Memo* memo, int max_index)
{
	int i;
	printf("����� ��ü �޸��Դϴ�.\n");
	for(i=0; i<max_index; i++)
	{
		printf("%d�� �޸��Դϴ�. ", i+1);
		print_memo(memo[i]);
	}
}

void print_memo(Memo memo)
{
	printf("����ð� : %04d�� %02d�� %02d�� %02d�� %02d��, �޸𳻿� : %s, �˶��ð� : %04d�� %02d�� %02d�� %02d�� %02d��\n",
		memo.created_time.year, memo.created_time.month, memo.created_time.day,
		memo.created_time.hour, memo.created_time.min,
		memo.contents,
		memo.alarm_time.year, memo.alarm_time.month, memo.alarm_time.day,
		memo.alarm_time.hour, memo.alarm_time.min);
}

