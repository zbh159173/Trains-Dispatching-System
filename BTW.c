#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "BTW.h"
#include "init.h"
#include "run.h"

/*BTW_RUN: By-the-way strategy*/
void BTW_RUN(Train_List *train_list, Train *ptr, noun *non, int* time_flag, int *flag)
{
	StationPoint *dtr;
	CheckPoint *head;
	static int stop_flag;														//stop counter
	static int unable_flag = 0;                                                 //train's sleep time counter
	int time = 3;                                                               //stop seconds was set
	if(*time_flag < ptr->start_time) {
		(*time_flag)++;  														//timer
		//goto jump;
	} else if (*time_flag == ptr->start_time) {
		ptr->now_state = Notask_Run;
		(*time_flag)++;
	}
//jump:
	if (ptr->now_state != Unable) {                                             //if the sleep time equals to counter so you can run
		switch (ptr->now_state) {
			case (Notask_Run):													//run
				*flag = 1;                                                      //put the flag to 1 to declare Notask_Run
				run(ptr);
				if (rail_world[ptr->now_pos_X][ptr->now_pos_Y] == 4) {			//a station
					ptr->now_state = Stop;
					break;
				}
				if (rail_world[ptr->now_pos_X][ptr->now_pos_Y] == 3) {			//a checkpoint
					ptr->now_state = Wait;
					break;
				}
				break;
			case (Stop):
				dtr = station_match(train_list, ptr);                           //find the station where the train has arrived
				ptr->now_station_ID = dtr->ID;
				if (*flag == 1 && dtr->release_task) {							//notask run and station has a task
					if (stop_flag < time) {
						stop_flag++;
						break;
					} else {
						stop_flag = 0;
						add_task(ptr, dtr->release_task);
						ptr->now_state = Task_Run;
						ptr->now_station_ID = 0;
						if(ptr->fire_coin > 4){
							direction_found(train_list, ptr);
						}
						break;
					}
				}																//notask run and station has a task
				if (*flag == 1 && !dtr->release_task) {
					if (stop_flag < time) {
						stop_flag++;
						break;
					} else {
						stop_flag = 0;
						ptr->now_state = Notask_Run;
						ptr->now_station_ID = 0;
						break;
					}
				} 																//notask run and station has no task
				if (*flag == 2 && dtr->release_task) { 							//task run and station has a task
					if (stop_flag < time) {
						stop_flag++;
						break;
					} else {
						stop_flag = 0;
						finish_task(ptr, dtr->ID);
						add_task(ptr, dtr->release_task);
						ptr->now_state = Task_Run;
						ptr->now_station_ID = 0;
						if(ptr->fire_coin > 4){
							direction_found(train_list, ptr);
						}
						break;
					}
				}																//task run and station has a task
				if (*flag == 2 && !dtr->release_task) {							//task run but this station has no task
					if (stop_flag < time) {
						stop_flag++;
						break;
					} else {
						int number = task_number(ptr);                          //the task number of the train
						stop_flag = 0;
						if (number > 1) {
							finish_task(ptr, dtr->ID);
							ptr->now_state = Task_Run;
							ptr->now_station_ID = 0;
							if(ptr->fire_coin > 4){
								direction_found(train_list, ptr);
							}
							break;
						} else {
							if (finish_task(ptr, dtr->ID)) {
								ptr->now_state = Notask_Run;
								ptr->now_station_ID = 0;
								break;
							} else {
								ptr->now_state = Task_Run;
								ptr->now_station_ID = 0;
								if(ptr->fire_coin > 4){
									direction_found(train_list, ptr);
								}
								break;
							}
						}
					}
					break;
				}
				break;
			case (Task_Run):
				*flag = 2;
				run(ptr);
				if (rail_world[ptr->now_pos_X][ptr->now_pos_Y] == 4) {
					ptr->now_state = Stop;
					break;
				}
				if (rail_world[ptr->now_pos_X][ptr->now_pos_Y] == 3) {
					ptr->now_state = Wait;
					break;
				}
				break;
			case Wait:  
				if ((ptr->passby_check_point) % 2 == 1) {                       //leave the checkpoint
					head = pointer_match(ptr,non);
					checkpoint_flag_change(head);
					ptr->passby_check_point++;
					if (*flag == 1) {    									    //Notask_Run
						ptr->now_state = Notask_Run;
						run(ptr);
						break;
					} else if (*flag == 2) {  									//Task_Run
						ptr->now_state = Task_Run;
						run(ptr);
						break;
					}
				} else if ((ptr->passby_check_point) % 2 == 0) {                //enter the checkpoint
					if (is_have_train(ptr,non) == 1) {                          //if there has already had a train
						break;
					}
					head = pointer_match(ptr,non);                              //find the linklist of this checkpoint
					checkpoint_flag_change(head);                               //change the flag of this linklist to declare there has a train
					ptr->passby_check_point++;
					if (*flag == 1) {   										//Notask_Run
						ptr->now_state = Notask_Run;
						run(ptr);
						break;
					} else if (*flag == 2) { 									//Task_Run
						ptr->now_state = Task_Run;
						run(ptr);
						break;
					}
				}
				break;
			//default:
			//	printf("The Train's state changing has made a mistake\n");
			//	return;
		}//switch
	}//if
}
