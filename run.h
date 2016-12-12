#ifndef IN_RUN
#define IN_RUN

#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "input.h"  
#include "init.h"

void move(int* x, int* y, int direction, int temppos[]);						//move the train 
void run(Train *ptr);															//trains run
int pointer_match_special(Train *ptr, noun *noun);								//to deal with a special situation when matching the correct head pointer
CheckPoint *pointer_match(Train *ptr, noun *noun);								//to match the correct head pointer(checkpoint_list)
int is_have_train(Train *ptr,noun *noun);										//judge the cross railway
void checkpoint_flag_change(CheckPoint *head);									//to change the symbol of the state of the public railway  
StationPoint * station_match(Train_List *train_list, Train *ptr);				//to match the correct head pointer(station_list)
void add_task(Train *ptr, int task);											//add a task
int task_number(Train *ptr);													//to statistics the quantity of the train's tasks
int finish_task(Train *ptr, int id);											//finish a task
void direction_found(Train_List *train_list, Train *ptr);						//find way automatically
#endif
