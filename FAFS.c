#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "FAFS.h"
#include "init.h"
#include "run.h"

/*FAFS_RUN: First-arrive-first-serve strategy*/
//the same as the BTW so the annotation has been omitted
void FAFS_RUN(Train_List *train_list, Train *ptr, noun *non, int *time_flag, int *flag)
{
	StationPoint *dtr;
	int i;
	CheckPoint *head;
	int time = 3;
	static int stop_flag;
	if(*time_flag < ptr->start_time) {
		(*time_flag)++;
		//goto jump;
	} else if (*time_flag == ptr->start_time) {
		ptr->now_state = Notask_Run;
		(*time_flag)++;
	}
//jump:
	if (ptr->now_state != Unable) {
		switch (ptr->now_state) {
			case Notask_Run:
				(*flag) = 1;
				switch (rail_world[ptr->now_pos_X][ptr->now_pos_Y]) {
					case 1:
						run(ptr);
						break;
					case 2:
						run(ptr);
						break;
					case 3:
						ptr->now_state = Wait;
						break;
					case 4:
						ptr->now_state = Stop;
						break;
				}
				break;
			case Task_Run:
				(*flag) = 2;
				switch (rail_world[ptr->now_pos_X][ptr->now_pos_Y]) {
					case 1:
						run(ptr);
						break;
					case 2:
						run(ptr);
						break;
					case 3:
						ptr->now_state = Wait;
						break;
					case 4:
						ptr->now_state = Stop;
						break;
				}
				break;
			case Wait:   
				if((ptr->passby_check_point) % 2 == 1) {
					head = pointer_match(ptr,non);
					checkpoint_flag_change(head);
					ptr->passby_check_point++;
					if ((*flag) == 1) {        
						ptr->now_state = Notask_Run;
						run(ptr);
						break;
					} else if ((*flag) == 2) { 
						ptr->now_state = Task_Run;
						run(ptr);
						break;
					}
				} else if((ptr->passby_check_point) % 2 == 0) {
					if(is_have_train(ptr,non) == 1) {
						break;
					}
					head = pointer_match(ptr,non);
					checkpoint_flag_change(head);
					ptr->passby_check_point++;
					if ((*flag) == 1) {      
						ptr->now_state = Notask_Run;
						run(ptr);
						break;
					} else if ((*flag) == 2) { 
						ptr->now_state = Task_Run;
						run(ptr);
						break;
					}
				}
				break;
			case Stop:   
				dtr = station_match(train_list, ptr);
				ptr->now_station_ID = dtr->ID;
				if(ptr->task_list[0] == 0) {
					switch (dtr->release_task) {
						case 0:    
							if (stop_flag < time) {
								stop_flag++;
								break;
							} else {
								stop_flag = 0;
								for (i = 1; i < 5; i++)
									ptr->task_list[i]=0;
								ptr->now_state = Notask_Run;
								ptr->now_station_ID = 0;
								run(ptr);
							}
							break;
						default :  
							ptr->task_list[0] = dtr->release_task;
							for (i = 1; i < 5; i++)
								ptr->task_list[i]=0;
							if (stop_flag < time) {
								stop_flag++;
								break;
							} else {
								stop_flag = 0;
								ptr->now_state = Task_Run;
								ptr->now_station_ID = 0;
								if(ptr->fire_coin > 4) {
									direction_found(train_list, ptr);
								}
								run(ptr);
							}
							break;
					}
					break;
				} else {      
					if(ptr->task_list[0] == dtr->ID) { 
						ptr->fire_coin++;
						if(dtr->release_task == 0) { 
							ptr->task_list[0] = 0;
							for (i = 1; i < 5; i++)
								ptr->task_list[i] = 0;
							if (stop_flag < time) {
								stop_flag++;
								break;
							} else {
								stop_flag = 0;
								ptr->now_state = Notask_Run;
								ptr->now_station_ID = 0;
								run(ptr);
							}
						} else {
							ptr->task_list[0] = dtr->release_task;
							for (i = 1; i < 5; i++)
								ptr->task_list[i]=0;
							if (stop_flag < time) {
								stop_flag++;
								break;
							} else {
								stop_flag = 0;
								ptr->now_state=Task_Run;
								ptr->now_station_ID = 0;
								if(ptr->fire_coin > 4){
									direction_found(train_list, ptr);
								}
								run(ptr);
							}
						}
					} else {
						if (stop_flag < time) {
							stop_flag++;
							break;
						} else {
							stop_flag = 0;
							ptr->now_state = Task_Run;
							ptr->now_station_ID = 0;
							if(ptr->fire_coin > 4){
								direction_found(train_list, ptr);
							}
							run(ptr);
						}
					}
					break;
				}
				break;
		}//switch
	}//if
}


