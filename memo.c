#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <process.h>

typedef struct DateTime {
	int year;
	int month;
	int day;
	int hour;
	int min;
} DateTime;

typedef struct Memo {
	DateTime created_time;
	char contents[256];
	DateTime alarm_time;
} Memo;

typedef struct Node {
	struct Node* next;
	Memo data;
} Node;

void alarm_thread(void* ptr);
void add_memo(Node* head, Memo data);
void modify_memo(Node* head, int index, Memo data);
void delete_memo(Node* head, int index);
void delete_all_memo(Node* head);
int count_memo(Node* head);
void print_memo(Memo memo);
void print_all_memo(Node* head);
void set_memo(Memo* memo, int year1, int month1, int day1, int hour1, int min1, char* contents,
	int year2, int month2, int day2, int hour2, int min2);
void save_memo(Node* head, char* filename);
void load_memo(Node* head, char* filename);
time_t get_left_time(DateTime dtime);

int main()
{
	int select, memo_num, memo_count;
	int year, month, day, hour, min;
	char temp_string[128];

	struct tm* t;
	time_t tnow = time(NULL);

	Memo temp_memo;
	Node* memo_head = (Node*)malloc(sizeof(Node));
	Node* curr;
	
	t = (struct tm*)localtime(&tnow);
	memo_head->next = NULL;

	printf("메모 관리 프로그램입니다.\n");

	_beginthread(alarm_thread, 0, (void*)memo_head);

	while (1)
	{
		printf("\n메모입력(1), 메모삭제(2), 메모수정(3), 메모확인(4) 메모저장(5) 메모불러오기(6) 프로그램 종료(7) 입력 : ");
		scanf("%d", &select);

		if (select == 1)
		{
			printf("메모 내용 입력 : ");
			scanf("%s", temp_string);

			printf("알람날짜 입력 (년 월 일) : ");
			scanf("%d %d %d", &year, &month, &day);

			printf("알람시 입력 (시 분) : ");
			scanf("%d %d", &hour, &min);

			tnow = time(NULL);
			t = (struct tm*)localtime(&tnow);

			set_memo(&temp_memo, (t->tm_year) + 1900, (t->tm_mon) + 1, t->tm_mday, t->tm_hour, t->tm_min,
				temp_string, year, month, day, hour, min);

			add_memo(memo_head, temp_memo);
			printf("%d번 메모가 입력되었습니다.\n", count_memo(memo_head));
		}

		else if (select == 2)
		{
			memo_count = count_memo(memo_head);

			if (memo_count == 0)
			{
				printf("삭제할 메모가 없습니다.\n");
				continue;
			}

			printf("삭제할 메모번호를 입력해주세요 : ");
			scanf("%d", &memo_num);

			if ((memo_num < 1) || (memo_num > memo_count))
			{
				printf("범위에 맞는 메모번호를 입력해주세요!\n");
				continue;
			}

			delete_memo(memo_head, memo_num);
			printf("%d번 메모를 삭제했습니다.\n", memo_num); 
		}

		else if (select == 3)
		{
			memo_count = count_memo(memo_head);

			if (memo_count == 0)
			{
				printf("수정할 메모가 없습니다.\n");
				continue;
			}

			while (1)
			{
				printf("수정할 메모번호를 입력해주세요 : ");
				scanf("%d", &memo_num);

				if ((memo_num < 1) || (memo_num > memo_count))
					printf("범위에 맞는 메모번호를 입력해주세요!\n");
				else
					break;
			}

			printf("메모 내용 입력 : ");
			scanf("%s", temp_string);

			printf("알람날짜 입력 (년 월 일) : ");
			scanf("%d %d %d", &year, &month, &day);

			printf("알람시 입력 (시 분) : ");
			scanf("%d %d", &hour, &min);

			tnow = time(NULL);
			t = (struct tm*)localtime(&tnow);
			set_memo(&temp_memo, (t->tm_year) + 1900, (t->tm_mon) + 1, t->tm_mday, t->tm_hour, t->tm_min,
				temp_string, year, month, day, hour, min);
			modify_memo(memo_head, memo_num, temp_memo);
			printf("%d번 메모가 수정되었습니다.\n", memo_num);
		}

		else if (select == 4)
		{
			print_all_memo(memo_head);
		}

		else if (select == 5)
		{
			save_memo(memo_head, "memo_data.bin");
		}
		else if (select == 6)
		{
			delete_all_memo(memo_head);
			load_memo(memo_head, "memo_data.bin");
		}

		else if (select == 7)
		{
			printf("프로그램을 종료합니다.\n");
			break;
		}

		else
		{
			printf("번호에 맞게 입력해주세요.\n");
			continue;
		}
	}

	curr = memo_head->next;
	while (curr != NULL)
	{
		Node* next = curr->next;
		free(curr);
		curr = next;
	}
	free(memo_head);


	return 0;
}

// 쓰레드 함수관련
void alarm_thread(void* head)
{
	while (1)
	{
		Node* curr = ((Node*)head)->next;
		while (curr != NULL)
		{
			DateTime alarm_time = curr->data.alarm_time;
			time_t left_time = get_left_time(alarm_time);
			if (left_time == 10)
			{
				printf("\n다음 일정이 10초 남았습니다!! : %s\n", curr->data.contents);
			}

			curr = curr->next;
		}
		_sleep(300);
	}
}

time_t get_left_time(DateTime dtime)
{
	struct tm fix_time;

	fix_time.tm_year = dtime.year - 1900;
	fix_time.tm_mon = dtime.month - 1;
	fix_time.tm_mday = dtime.day;
	fix_time.tm_hour = dtime.hour;
	fix_time.tm_min = dtime.min;
	fix_time.tm_sec = 0;
	fix_time.tm_isdst = 0;

	time_t utc_time = mktime(&fix_time);

	return utc_time - time(NULL);
}

// 메모 저장 및 불러오기 관련
void save_memo(Node* head, char* filename)
{
	FILE* fp = fopen(filename, "wb");
	Node* curr = head->next;

	while (curr != NULL)
	{
		fwrite(&(curr->data), sizeof(Memo), 1, fp);
		curr = curr->next;
	}

	fclose(fp);
}

void load_memo(Node* head, char* filename)
{
	FILE* fp = fopen(filename, "rb");
	Memo temp_memo;
	int fread_return;

	while (1)
	{
		fread_return = fread(&temp_memo, sizeof(Memo), 1, fp);
		if (fread_return == 0)
			break;
		add_memo(head, temp_memo);
	}
	fclose(fp);
}


// 메모 및 연결리스트 함수 관련
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

void add_memo(Node* head, Memo data)
{
	Node* curr = head;
	Node* new_node = (Node*)malloc(sizeof(Node));

	new_node->data = data;
	new_node->next = NULL;

	while (curr->next != NULL)
		curr = curr->next;

	curr->next = new_node;
}

void modify_memo(Node* head, int index, Memo data)
{
	Node* curr = head->next;
	int count = 1;

	while (count < index)
	{
		curr = curr->next;
		count++;
	}

	curr->data = data;
}

void delete_memo(Node* head, int index)
{
	Node* curr = head->next;
	Node* prev = head;
	int count = 1;

	while (count < index)
	{
		prev = curr;
		curr = curr->next;
		count++;
	}

	prev->next = curr->next;
	free(curr);
}

void delete_all_memo(Node* head)
{
	Node* curr = head->next;
	while (curr != NULL)
	{
		Node* next = curr->next;
		free(curr);
		curr = next;
	}
	head->next = NULL;
}

int count_memo(Node* head)
{
	int count = 0;
	Node* curr = head->next;

	while (curr != NULL)
	{
		count++;
		curr = curr->next;
	}

	return count;
}

// 메모출력함수 관련
void print_memo(Memo memo)
{
	printf("만든시간 : %04d년 %02d월 %02d일 %02d시 %02d분, 메모내용 : %s, 알람시간 : %04d년 %02d월 %02d일 %02d시 %02d분\n",
		memo.created_time.year, memo.created_time.month, memo.created_time.day,
		memo.created_time.hour, memo.created_time.min,
		memo.contents,
		memo.alarm_time.year, memo.alarm_time.month, memo.alarm_time.day,
		memo.alarm_time.hour, memo.alarm_time.min);
}

void print_all_memo(Node* head)
{
	int index = 1;
	Node* curr = head->next;
	
	if (curr == NULL)
	{
		printf("저장된 메모가 없습니다.\n");
		return;
	}

	while (curr != NULL)
	{
		printf("%d번 메모 : ", index++);
		print_memo(curr->data);
		curr = curr->next;
	}
	
}



