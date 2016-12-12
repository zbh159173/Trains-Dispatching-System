#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "input.h"
#include "init.h"
#include "run.h"

/*run: trains run.*/
void run(Train *ptr)
{
	int speed = ptr->now_speed, i = 0;
	int x = ptr->now_pos_X, y = ptr->now_pos_Y;
	int direction = ((ptr->now_direction == CCW) ? 0 : 1);   					
	FILE *fp;
	switch (ptr->ID) {
		case 1:
			fp = fopen("railway1.txt", "r");
			break;
		case 2:
			fp = fopen("railway2.txt", "r");
			break;
		case 3:
			fp = fopen("railway3.txt", "r");
			break;
	}
	int temppos[4];                                         					//get the coordinate of the railway
	fscanf(fp, "%d %d %d %d", &temppos[0], &temppos[1], &temppos[2], &temppos[3]);
	while (i < speed) {
		move(&x, &y, direction, temppos);             
		ptr->now_pos_X = x;
		ptr->now_pos_Y = y;
		if (rail_world[ptr->now_pos_X][ptr->now_pos_Y] == 3) {
			ptr->now_state = Wait;
			break;
		}
		if (rail_world[ptr->now_pos_X][ptr->now_pos_Y] == 4) {
			ptr->now_state = Stop;
			break;
		}
		i++;
	}													
	fclose(fp);
}
/*move: move trains based on the postion and direction*/
void move(int *x, int *y, int direction, int temppos[])
{
	if (0 == direction) {														//CCW
		if (*x == temppos[0]) {
			if (*y != temppos[3]) {
				(*y)++;
			} else {
				(*x)++;
			}
		}																		
		else if (*y == temppos[3]) {											
			if (*x != temppos[2]) {
				(*x)++;
			} else {
				(*y)--;
			}
		}
		else if (*x == temppos[2]) {
			if (*y != temppos[1]) {
				(*y)--;
			} else {
				(*x)--;
			}
		} else if (*y == temppos[1]) {
			if (*x != temppos[0]) {
				(*x)--;
			} else {
				(*y)++;
			}
		}																		//CCW OK
	} else {																	//CW
		if (*x == temppos[0]) {
			if (*y != temppos[1]) {
				(*y)--;
			} else {
				(*x)++;
			}
		} else if (*y == temppos[1]) {
			if (*x != temppos[2]) {
				(*x)++;
			} else {
				(*y)++;
			}
		} else if (*x == temppos[2]) {
			if (*y != temppos[3]) {
				(*y)++;
			} else {
				(*x)--;
			}
		} else if (*y == temppos[3]) {
			if (*x != temppos[0]) {
				(*x)--;
			} else {
				(*y)--;
			}
		}
	}																			//CW OK
}
/*pointer_match_special:*/
int pointer_match_special(Train *ptr,noun *non)
{
	int flag = 2;         											
	CheckPoint *back;
	CheckPoint *ctr = non->ctr_noun->head2;
	back = ctr;
	while (ctr->nextctr != NULL) {
		if (ptr->now_pos_X == ctr->nextctr->x && ptr->now_pos_Y == ctr->nextctr->y)
			flag = 3;
		ctr = ctr->nextctr;
	}
	non->ctr_noun->head2 = back;
	return flag;
}
/*pointer_match:*/
CheckPoint *pointer_match(Train *ptr, noun *noun)
{
	CheckPoint *ctr;
	int flag;
	switch (ptr->ID) {
		case 1:
			flag = pointer_match_special(ptr,noun);
			if (flag == 2)
				ctr = noun->ctr_noun->head1;
			else if (flag == 3)
				ctr = noun->ctr_noun->head2;
			break;
		case 2:
			ctr = noun->ctr_noun->head1;
			break;
		case 3:
			ctr = noun->ctr_noun->head2;
			break;
	}
	return ctr;
}
/*is_have_train: judge the cross railway*/
int is_have_train(Train *ptr,noun *non)
{
	CheckPoint *ctr = pointer_match(ptr,non);
	if (ctr->nextctr->flag == 1)
		return 1;          														//occupied
	else return 0;         														//inoccupied
}
/*checkpoint_flag_change:*/
void checkpoint_flag_change(CheckPoint *head)
{
	if (head->nextctr->flag == 0) {
		head->nextctr->flag = 1;
	} else if(head->nextctr->flag == 1) {
		head->nextctr->flag = 0;
	}
}
/*station_match: */
StationPoint *station_match(Train_List *train_list, Train *ptr)
{
	StationPoint *dtr , *back;
	switch (ptr->ID) {
		case 1:
			back = train_list->non->sta->head1;
			while (train_list->non->sta->head1->nextdtr != NULL) {
				if ((train_list->non->sta->head1->nextdtr->x == ptr->now_pos_X) && (train_list->non->sta->head1->nextdtr->y == ptr->now_pos_Y)) {
					dtr=train_list->non->sta->head1->nextdtr;
					break;
				}
				train_list->non->sta->head1 = train_list->non->sta->head1->nextdtr;
			}
			train_list->non->sta->head1 = back;
			return dtr;
			break;
		case 2:
			back = train_list->non->sta->head2;
			while (train_list->non->sta->head2->nextdtr != NULL) {
				if ((train_list->non->sta->head2->nextdtr->x == ptr->now_pos_X) && (train_list->non->sta->head2->nextdtr->y == ptr->now_pos_Y)) {
					dtr=train_list->non->sta->head2->nextdtr;
					break;
				}
				train_list->non->sta->head2 = train_list->non->sta->head2->nextdtr;
			}
			train_list->non->sta->head2 = back;
			return dtr;
			break;
		case 3:
			back = train_list->non->sta->head3;
			while (train_list->non->sta->head3->nextdtr != NULL) {
				if ((train_list->non->sta->head3->nextdtr->x == ptr->now_pos_X) && (train_list->non->sta->head3->nextdtr->y == ptr->now_pos_Y)) {
					dtr=train_list->non->sta->head3->nextdtr;
					break;
				}
				train_list->non->sta->head3 = train_list->non->sta->head3->nextdtr;
			}
			train_list->non->sta->head3 = back;
			return dtr;
			break;
	}
}
/*add_task: add tasks.*/
void add_task(Train *ptr, int task)
{
	int i, flag = 0;
	for (i = 0; i < 5; i++) {
		if (ptr->task_list[i] == 0) {
			ptr->task_list[i] = task;
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		printf("Train %s's task list is full\n", ptr->train_name);
	}
}
/*task_number:*/
int task_number(Train *ptr)
{
	int i, flag = 0;
	for (i = 0; i < 5; i++) {
		if (ptr->task_list[i] != 0) {
			flag++;
		}
	}
	return flag;
}
/*finish_task: finish a task*/
int finish_task(Train *ptr, int id)
{
	int i, flag = 0;
	for (i = 0; i < 5; i++) {
		if (ptr->task_list[i] == id) {
			ptr->task_list[i] = 0;
			ptr->fire_coin++;
			flag = 1;
		}
	}
	return flag;
}
/*direction_found: find the way automatically*/
//only if the train's firecoin greater than 4
void direction_found(Train_List *train_list, Train *ptr)
{
	FILE *fp;
	int min = 2000;																//min distance
	int min_i = 0;
	int distance = 0;
	int left = 0;
	int i = 0;
	int flag = 0, j = 0;
	switch (ptr->ID) {
		case 1:
			fp = fopen("railway1.txt", "r");
			break;
		case 2:
			fp = fopen("railway2.txt", "r");
			break;
		case 3:
			fp = fopen("railway3.txt", "r");
			break;
	}
	int temppos[4];
	int all_distance_noun[4];
	int direction_noun[4];
	fscanf(fp, "%d %d %d %d ", &temppos[0], &temppos[1], &temppos[2], &temppos[3]);
	int all_distance = 2*(temppos[2]-temppos[0]+temppos[3]-temppos[1]);
	int x_back = ptr->now_pos_X, y_back = ptr->now_pos_Y;
	int x = ptr->now_pos_X, y = ptr->now_pos_Y;
	int direction, back_direction;  											//now direction
	direction = back_direction = ((ptr->now_direction == CCW) ? 0 : 1);
	StationPoint *back;
	switch (ptr->ID) {
		case 1:
			back = train_list->non->sta->head1;
			while(train_list->non->sta->head1->nextdtr != NULL) {
				flag = 0;
				for(j = 0; j < 5; j++) {
					if(ptr->task_list[j] == train_list->non->sta->head1->nextdtr->ID) {
						flag = 1;
						break;
					}
				}
				if (flag == 1) {
					while(x != train_list->non->sta->head1->nextdtr->x || y != train_list->non->sta->head1->nextdtr->y) {
						move(&x, &y, direction, temppos);
						distance++;
					}
					left = all_distance - distance;
					if (distance > left) {
						if(direction == 0)
							direction_noun[i] = 1;
						else
							direction_noun[i] = 0;
					} else {
						direction_noun[i] = direction;							//save direction
					}
					x = x_back;
					y = y_back;
					all_distance_noun[i] = ((distance < left) ? distance : left);//save distance
					i++;
					distance = 0;
					train_list->non->sta->head1 = train_list->non->sta->head1->nextdtr;
				} else if (flag == 0) {
					all_distance_noun[i] = 1000;
					direction_noun[i] = ((ptr->now_direction == CCW) ? 0 : 1);
					i++;
					train_list->non->sta->head1 = train_list->non->sta->head1->nextdtr;
				}
			}
			for (i = 0; i < 4; i++) {
				if (min > all_distance_noun[i]) {
					min = all_distance_noun[i];
					min_i = i;
				}
			}
			if(direction_noun[min_i] == 0)
				train_list->ptrA->now_direction = CCW;
			else if(direction_noun[min_i] == 1)
				train_list->ptrA->now_direction = CW;
			train_list->non->sta->head1 = back;
			fclose(fp);
			break;
		case 2:
			back = train_list->non->sta->head2;
			while (train_list->non->sta->head2->nextdtr != NULL) {
				flag = 0;
				for(j = 0; j < 5; j++) {
					if(ptr->task_list[j] == train_list->non->sta->head2->nextdtr->ID) {
						flag = 1;
						break;
					}
				}
				if (flag == 1) {
					while(x != train_list->non->sta->head2->nextdtr->x || y != train_list->non->sta->head2->nextdtr->y) {
						move(&x, &y, direction, temppos);
						distance++;
					}
					left = all_distance - distance;
					if (distance > left) {
						if(direction == 0)
							direction_noun[i] = 1;
						else
							direction_noun[i] = 0;
					} else {
						direction_noun[i] = direction;
					}
					x = x_back;
					y = y_back;
					all_distance_noun[i] = ((distance < left) ? distance : left);
					i++;
					distance = 0;
					train_list->non->sta->head2 = train_list->non->sta->head2->nextdtr;
				} else if (flag == 0) {
					all_distance_noun[i] = 1000;
					direction_noun[i] = ((ptr->now_direction == CCW) ? 0 : 1);
					i++;
					train_list->non->sta->head2 = train_list->non->sta->head2->nextdtr;
				}
			}
			for (i = 0; i < 4; i++) {
				if (min > all_distance_noun[i]) {
					min = all_distance_noun[i];
					min_i = i;
				}
			}
			if(direction_noun[min_i] == 0)
				train_list->ptrB->now_direction = CCW;
			else if(direction_noun[min_i] == 1)
				train_list->ptrB->now_direction = CW;
			train_list->non->sta->head2 = back;
			fclose(fp);
			break;
		case 3:
			back = train_list->non->sta->head3;
			while(train_list->non->sta->head3->nextdtr != NULL) {
				flag = 0;
				for(j = 0; j < 5; j++) {
					if(ptr->task_list[j] == train_list->non->sta->head3->nextdtr->ID) {
						flag = 1;
						break;
					}
				}
				if (flag == 1) {
					while(x != train_list->non->sta->head3->nextdtr->x || y != train_list->non->sta->head3->nextdtr->y) {
						move(&x, &y, direction, temppos);
						distance++;
					}
					left = all_distance - distance;
					if (distance > left) {
						if(direction == 0)
							direction_noun[i] = 1;
						else
							direction_noun[i] = 0;
					} else {
						direction_noun[i] = direction;
					}
					x = x_back;
					y = y_back;
					all_distance_noun[i] = ((distance < left) ? distance : left);
					i++;
					distance = 0;
					train_list->non->sta->head3 = train_list->non->sta->head3->nextdtr;
				} else if (flag == 0) {
					all_distance_noun[i] = 1000;
					direction_noun[i] = ((ptr->now_direction == CCW) ? 0 : 1);
					i++;
					train_list->non->sta->head3 = train_list->non->sta->head3->nextdtr;
				}
			}
			for (i = 0; i < 4; i++) {
				if (min > all_distance_noun[i]) {
					min = all_distance_noun[i];
					min_i = i;
				}
			}
			if(direction_noun[min_i] == 0)
				train_list->ptrC->now_direction = CCW;
			else if(direction_noun[min_i] == 1)
				train_list->ptrC->now_direction = CW;
			train_list->non->sta->head3 = back;
			fclose(fp);
			break;
	}
}
