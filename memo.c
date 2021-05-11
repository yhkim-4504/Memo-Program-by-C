#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <process.h>

#define True 1
#define False 0

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

// For Thread & Alarm
void alarm_thread(void* ptr);
time_t get_left_time(DateTime dtime);

// For Memo & Linked List
void set_memo(Memo* memo, int year1, int month1, int day1, int hour1, int min1, char* contents,
	int year2, int month2, int day2, int hour2, int min2);
void add_memo(Node* head, Memo data);
void modify_memo(Node* head, int index, Memo data);
void delete_memo(Node* head, int index);
void delete_all_memo(Node* head);
int count_memo(Node* head);

// For Printing Memo
void print_memo(Memo memo);
void print_all_memo(Node* head);

// For Memo Save & Load
void save_memo(Node* head, char* filename);
void load_memo(Node* head, char* filename);


int main()
{
	int select, memo_num, memo_count;  // 메뉴선택, 메모번호선택, 총 메모개수
	int year, month, day, hour, min;  // 년, 월, 일, 시, 분
	char temp_string[256];  // 메모내용 입력 및 임시저장용

	struct tm* t;  // 현재 년,월,일,시,분을 얻기 위한 locatime 함수를 위한 tm 구조체
	time_t tnow;  // time(NULL) 반환받는 long long type 변수

	Memo temp_memo;  // 임시저장용 메모구조체
	Node* memo_head = (Node*)malloc(sizeof(Node));  // 연결리스트 head 정의
	Node* curr;  // 순환용 Node 포인터
	
	memo_head->next = NULL;  // 처음에 next를 NULL로 초기화

	printf("********* 메모 관리 프로그램 **********\n");

	_beginthread(alarm_thread, 0, (void*)memo_head);  // 알람기능 쓰레드 실행

	while (True)
	{
		printf("\n메모입력(1), 메모삭제(2), 메모수정(3), 메모확인(4) 메모저장(5) 메모불러오기(6) 프로그램 종료(7) 입력 : ");
		scanf("%d", &select);

		if (select == 1)
		{
			// 메모내용 입력
			printf("메모 내용 입력 : ");
			scanf("%s", temp_string);

			printf("알람날짜 입력 (년 월 일) : ");
			scanf("%d %d %d", &year, &month, &day);

			printf("알람시 입력 (시 분) : ");
			scanf("%d %d", &hour, &min);

			// 현재까지 지난 초(Time(NULL))을 localtime에 입력
			tnow = time(NULL);
			t = (struct tm*)localtime(&tnow);

			// temp_memo에 입력받은 값 설정
			set_memo(&temp_memo, (t->tm_year) + 1900, (t->tm_mon) + 1, t->tm_mday, t->tm_hour, t->tm_min,
				temp_string, year, month, day, hour, min);

			// memo 추가
			add_memo(memo_head, temp_memo);
			printf("%d번 메모가 입력되었습니다.\n", count_memo(memo_head));
		}

		else if (select == 2)
		{
			memo_count = count_memo(memo_head);  // 총매모개수

			if (memo_count == 0)  // 0개이면 삭제불가
			{
				printf("삭제할 메모가 없습니다.\n");
				continue;
			}

			printf("삭제할 메모번호를 입력해주세요 : ");
			scanf("%d", &memo_num);

			if ((memo_num < 1) || (memo_num > memo_count))  // 범위조건
			{
				printf("범위에 맞는 메모번호를 입력해주세요!\n");
				continue;
			}

			delete_memo(memo_head, memo_num);  // memo_num번의 메모삭제
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

			printf("수정할 메모번호를 입력해주세요 : ");
			scanf("%d", &memo_num);

			if ((memo_num < 1) || (memo_num > memo_count))
			{
				printf("범위에 맞는 메모번호를 입력해주세요!\n");
				continue;
			}

			// 수정할 메모내용 입력
			printf("메모 내용 입력 : ");
			scanf("%s", temp_string);

			printf("알람날짜 입력 (년 월 일) : ");
			scanf("%d %d %d", &year, &month, &day);

			printf("알람시 입력 (시 분) : ");
			scanf("%d %d", &hour, &min);

			// 현재시간 구하기
			tnow = time(NULL);
			t = (struct tm*)localtime(&tnow);

			// temp_memo에 임시저장
			set_memo(&temp_memo, (t->tm_year) + 1900, (t->tm_mon) + 1, t->tm_mday, t->tm_hour, t->tm_min,
				temp_string, year, month, day, hour, min);

			// memo_num번의 메모를 temp_memo로 수정
			modify_memo(memo_head, memo_num, temp_memo);
			printf("%d번 메모가 수정되었습니다.\n", memo_num);
		}

		else if (select == 4)
		{
			print_all_memo(memo_head);  // 전체메모 출력
		}

		else if (select == 5)
		{
			char* filename[128];
			printf("저장할 파일 이름을 입력해주세요 : ");
			scanf("%s", filename);
			save_memo(memo_head, filename);  // 메모저장
		}
		else if (select == 6)
		{
			char* filename[128];
			printf("불러올 파일 이름을 입력해주세요 : ");
			scanf("%s", filename);
			delete_all_memo(memo_head);  // 연결리스트 전체 삭제 후
			load_memo(memo_head, filename);  // 메모불러오기
		}

		else if (select == 7)
		{
			printf("프로그램을 종료합니다.\n");
			break;  // 반복문탈출
		}

		else
		{
			printf("번호에 맞게 입력해주세요.\n");
			continue;
		}
	}

	// 연결리스트 동적할당 전체해제
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
void alarm_thread(void* head)  // 알람기능 쓰레드
{
	while (1)
	{
		Node* curr = ((Node*)head)->next;  // 연결리스트 순환용 포인터
		while (curr != NULL)  // NULL이 아닐때까지
		{
			DateTime alarm_time = curr->data.alarm_time;  // 알람시간
			time_t left_time = get_left_time(alarm_time);  // 남은시간 계산
			
			// 남은시간 조건에 따라 알림
			if (left_time == 10)
			{
				printf("\n다음 일정이 10초 남았습니다!! : %s\n", curr->data.contents);
			}

			curr = curr->next;
		}
		_sleep(300);  // 0.3초 sleep
	}
}

time_t get_left_time(DateTime dtime)  // DateTime 구조체를 받아 현재시간과의 차이를 초로 반환
{
	// mktime을 이용해 utc_time으로 변환하기 위해 tm 구조체 선언
	struct tm fix_time;

	// 알람시간에 저장된 변수들 대입
	fix_time.tm_year = dtime.year - 1900;
	fix_time.tm_mon = dtime.month - 1;
	fix_time.tm_mday = dtime.day;
	fix_time.tm_hour = dtime.hour;
	fix_time.tm_min = dtime.min;
	fix_time.tm_sec = 0;
	fix_time.tm_isdst = 0;

	// mktime을 이용해 utc_time 반환(현재까지 지난 초)
	time_t utc_time = mktime(&fix_time);

	// 현재시간과의 차이값 반환
	return utc_time - time(NULL);
}

// 메모 저장 및 불러오기 관련
void save_memo(Node* head, char* filename)  // 모든 메모 저장
{
	FILE* fp = fopen(filename, "wb");  // 파일 열기
	Node* curr = head->next;  // 순환용 포인터

	// 순환하면서 Memo Data를 파일에 저장
	while (curr != NULL)
	{
		fwrite(&(curr->data), sizeof(Memo), 1, fp);
		curr = curr->next;
	}

	fclose(fp);
}

void load_memo(Node* head, char* filename)  // 모든 메모 로드
{
	FILE* fp = fopen(filename, "rb");  // 파일 열기
	Memo temp_memo;
	int fread_return;

	// 파일을 하나씩 읽으며 add_memo로 연결리스트에 추가
	while (True)
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
	int year2, int month2, int day2, int hour2, int min2)  // 메모설정함수
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

void add_memo(Node* head, Memo data)  // 메모 추가
{
	Node* curr = head;
	Node* new_node = (Node*)malloc(sizeof(Node));

	new_node->data = data;
	new_node->next = NULL;

	// 순환하며 마지막 Node로 이동
	while (curr->next != NULL)
		curr = curr->next;

	// 마지막 Node의 다음 Node에 new_node 할당
	curr->next = new_node;
}

void modify_memo(Node* head, int index, Memo data)  // 메모 수정
{
	Node* curr = head->next;
	int count = 1;

	// 받은 index 만큼 순환이동
	while (count < index)
	{
		curr = curr->next;
		count++;
	}

	// 새로운 데이터로 수정
	curr->data = data;
}

void delete_memo(Node* head, int index)  // 메모 삭제
{
	Node* curr = head->next;  // 순환용
	Node* prev = head;  // 이전 curr 저장
	int count = 1;

	// 받은 index 만큼 이동
	while (count < index)
	{
		prev = curr;
		curr = curr->next;
		count++;
	}

	prev->next = curr->next;  // 이전의 next를 curr의 next에 연결
	free(curr);  // curr은 할당해제
}

void delete_all_memo(Node* head)  // head를 제외한 모든 메모 삭제
{
	// 노드순환하며 모든 Node 삭제
	Node* curr = head->next;
	while (curr != NULL)
	{
		Node* next = curr->next;
		free(curr);
		curr = next;
	}
	
	// head Node의 next를 NULL로 다시 초기화
	head->next = NULL;
}

int count_memo(Node* head)  // 총 메모개수 계산
{
	int count = 0;
	Node* curr = head->next;

	// 순환하며 count++
	while (curr != NULL)
	{
		count++;
		curr = curr->next;
	}

	return count;
}

// 메모출력함수 관련
void print_memo(Memo memo)  // 메모출력
{
	printf("만든시간 : %04d년 %02d월 %02d일 %02d시 %02d분, 메모내용 : %s, 알람시간 : %04d년 %02d월 %02d일 %02d시 %02d분\n",
		memo.created_time.year, memo.created_time.month, memo.created_time.day,
		memo.created_time.hour, memo.created_time.min,
		memo.contents,
		memo.alarm_time.year, memo.alarm_time.month, memo.alarm_time.day,
		memo.alarm_time.hour, memo.alarm_time.min);
}

void print_all_memo(Node* head)  // 모든 메모 출력
{
	int index = 1;
	Node* curr = head->next;
	
	// head의 next가 NULL이면 저장된 메모가 없음
	if (curr == NULL)
	{
		printf("저장된 메모가 없습니다.\n");
		return;
	}

	// 순환하며 출력
	while (curr != NULL)
	{
		printf("%d번 메모 : ", index++);
		print_memo(curr->data);
		curr = curr->next;
	}
	
}
