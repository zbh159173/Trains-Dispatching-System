#include <stdio.h>
#include <windows.h>
#include "init.h"
#include "output.h"

/*output_time: output information per sec*/
void output_time(Train *ptr)
{
	char *state_str[6] = {"Unable", "Pause", "Wait", "Stop", "Notask_Run", "Task_Run"};
	char *direction_str[2] = {"CClockwise", "Clockwise"};
	char *strategy_str[2] = {"FAFS", "BTW"};
	printf("Name       (X, Y)    STATE     NOWSTATION        DIRECTION       STRATEGY	 FIRECOIN\n");
	printf("%-10s(%2d, %2d)   ", ptr->train_name, ptr->now_pos_X, ptr->now_pos_Y);
	printf("%-10s  %d\t\t  %-15s %-15s %d",state_str[ptr->now_state], ptr->now_station_ID ,direction_str[ptr->now_direction], strategy_str[ptr->now_strategy], ptr->fire_coin);
	if (ptr->now_strategy == 1) {
		int i;
		printf("       task_station ID list: ");
		for (i = 0; i < 5; i++) {
			printf("%d ", ptr->task_list[i]);
		}
	} else {
		printf("       task_station ID: %d ", ptr->task_list[0]);
	}

	putchar('\n');
	FILE *fw = fopen("result.txt", "a+");
	if (ptr->ID == 1) {
		fprintf(fw, "============================================================================================");
	}
	fprintf(fw, "\nName       (X, Y)    STATE     NOWSTATION        DIRECTION       STRATEGY	 FIRECOIN\n");
	fprintf(fw, "%-10s(%2d, %2d)   ", ptr->train_name, ptr->now_pos_X, ptr->now_pos_Y);
	fprintf(fw, "%-10s  %d\t\t  %-15s %-15s %d",state_str[ptr->now_state], ptr->now_station_ID ,direction_str[ptr->now_direction], strategy_str[ptr->now_strategy], ptr->fire_coin);
	if (ptr->now_strategy == 1) {
		int i;
		fprintf(fw, "       task_station ID list: ");
		for (i = 0; i < 5; i++) {
			fprintf(fw, "%d ", ptr->task_list[i]);
		}
	} else {
		fprintf(fw, "       task_station ID: %d ", ptr->task_list[0]);
	}
	fputc('\n', fw);
	fclose(fw);
}

/*output_put_railway_state: output the occupied railway*/
void output_pub_railway_state(int pub_flag)
{

	FILE *fw = fopen("result.txt", "a+");
	if (fw != NULL) {
		printf("The public railway:\n");
		if (pub_flag == 1221) {              //B 车占用 1 轨道，A 车占用 2 轨道
			printf("1:Train B        \t2:Train A       \n");
			fprintf(fw, "1:Train B        \t2:Train A       \n");
		} else if (pub_flag == 1223) {       //B 车占用 1 轨道，C 车占用 2 轨道
			printf("1:Train B        \t2:Train C       \n");
			fprintf(fw, "1:Train B        \t2:Train C       \n");
		} else if (pub_flag == 1123) {       //A 车占用 1 轨道, C 车占用 2 轨道
			printf("1:Train A        \t2:Train C       \n");
			fprintf(fw, "1:Train A        \t2:Train C       \n");
		} else if (pub_flag == 33) {         //两个轨道都没被占用
			printf("1:None           \t2:None          \n");
			fprintf(fw, "1:None           \t2:None          \n");
		} else if(pub_flag == 11) {         //A 车占用 1 轨道 , 2号轨道没被占用
			printf("1:Train A        \t2:None          \n");
			fprintf(fw, "1:Train A        \t2:None          \n");
		} else if(pub_flag == 12) {         //B 车占用 1 轨道 ，2号轨道没被占用
			printf("1:Train B        \t2:None          \n");
			fprintf(fw, "1:Train B        \t2:None          \n");
		} else if(pub_flag == 21) {         //A 车占用 2 轨道 ，1号轨道没被占用
			printf("1:None           \t2:Train A       \n");
			fprintf(fw, "1:None           \t2:Train A       \n");
		} else if(pub_flag == 23) {         //C 车占用 2 轨道 ，1号轨道没被占用
			printf("1:None           \t2:Train C       \n");
			fprintf(fw, "1:None           \t2:Train C       \n");
		}
		fprintf(fw, "============================================================================================\n");
	}
	fclose(fw);
}
