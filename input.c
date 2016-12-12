#include <stdio.h>
#include "init.h"
#include "input.h"

/*input_railway: set railways from files*/
void input_railway(FILE *fp)
{
	rewind(fp);
	int left_up_X, left_up_Y, right_down_X, right_down_Y;
	fscanf(fp, "%d %d %d %d", &left_up_X, &left_up_Y, &right_down_X, &right_down_Y);
	init_railway(left_up_X, left_up_Y, right_down_X, right_down_Y);
}
/*input_train: set trains from files*/
void input_train(Train *ptr, FILE *fp)
{
	rewind(fp);
	printf("请输入小火车名字：");
	scanf("%s", ptr->train_name);
	fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &ptr->ID, &ptr->begin_pos_X, &ptr->begin_pos_Y,
	       &ptr->now_speed, &ptr->now_state, &ptr->now_direction, &ptr->now_pos_X, &ptr->now_pos_Y,
	       &ptr->is_in_task, &ptr->passby_check_point,&ptr->now_strategy, &ptr->start_time,
		    &ptr->fire_coin,&ptr->now_station_ID,&ptr->alternation_strategy);
	int i;
	for (i = 0; i < 5; i++)
		ptr->task_list[i] = 0;
}
/*input_station: set stationpoint from the file*/
void input_stationpoint(void)
{
	FILE *fp = fopen("station.txt", "r");
	int i, j;
	rewind(fp);
	for (i = 0; i < 3; i++) { 													//3 railways
		int X, Y;					
		for (j = 0; j < 4; j++) { 												//4 stations per railway
			fscanf(fp, "%d%d ", &X, &Y);
			init_station(X, Y);
		}
	}
	fclose(fp);
}
/*input_allthing: init all things (called by main)*/
void input_allthing (Train_List *train_list)
{
	train_list->ptrA = (Train *)malloc(sizeof (Train));
	train_list->ptrB = (Train *)malloc(sizeof (Train));
	train_list->ptrC = (Train *)malloc(sizeof (Train));
	train_list->non = (noun *)malloc(sizeof (noun));
	train_list->non->sta = (station *)malloc(sizeof (station));
	train_list->non->ctr_noun = (checkpoint_ctr_noun *)malloc(sizeof (checkpoint_ctr_noun));
	init_rail_world();
	FILE *fpA = NULL;
	fpA = fopen("railway1.txt", "r");
	FILE *fpB = NULL;
	fpB = fopen("railway2.txt", "r");
	FILE *fpC = NULL;
	fpC = fopen("railway3.txt", "r");
	/*
	循环从弹出的文本框中把 railway 的顶角坐标读入 railway.txt 文件 
	循环第几次就读入第几个 railway 文件中 
	s[100] fprintf 
	*/
	input_railway(fpA);
	input_railway(fpB);
	input_railway(fpC);
	init_checkpoint();                    
	train_list->non->ctr_noun = init_checkpoint_list();
	/*
	调用 input_stationpoint 函数之前，应该首先弹出 3 次文本框，用户输入站点的 x，y 坐标 
	然后得把这些坐标读入到  station.txt 文件中 
	s[100] fprintf 
	*/ 
	input_stationpoint();                                    
	train_list->non->sta =  init_station_node();   //这两个函数都会用到 station.txt 函数 
	fclose(fpA);
	fclose(fpB);
	fclose(fpC);
	fpA = fopen("trainA.txt", "r");
	fpB = fopen("trainB.txt", "r");
	fpC = fopen("trainC.txt", "r");
	/*
	循环从弹出的文本框中把 train 的各类初始值读入 train.txt 文件 
	循环第几次就读入第几个 train 文件中 
	s[100] fprintf 
	*/ 
	input_train(train_list->ptrA, fpA);
	input_train(train_list->ptrB, fpB);
	input_train(train_list->ptrC, fpC);
	fclose(fpA);
	fclose(fpB);
	fclose(fpC);
}
