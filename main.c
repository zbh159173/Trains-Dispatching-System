#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <process.h>
#include <ctype.h>
#include "init.h"
#include "input.h"
#include "run.h"
#include "FAFS.h"
#include "BTW.h"
#include "output.h"

int sign_a = 0;
int sign_b = 0;
int sign_c = 0;

/*print_tip: print some tips*/
int print_tip(void) {
	printf("\nt = %ld\n", clock() / 1000);
	printf("Please input the change as examples.[A/B/C][P/T/S/+/-] or [Q]: \n");
	printf("AP: Train A pauses. If A is pausing, A restarts.\n");
	printf("BT: Train B turns its direction. (CCW-->CW || CW --> CCW)\n");
	printf("C+: Train C speeds up. C-: Train C speeds down.\n");
	printf("AS: Train A's service strategy changes. (FAFS-->BTW || BTW-->FAFS)\n");
	printf("Q : Save Infomation and Quit in 5 sec.\n");
	return 0;
}
/*station_match_ID:*/
StationPoint *station_match_ID(Train_List *train_list, Train *ptr, char str_ctr[]) {
	StationPoint *dtr , *back;
	switch (ptr->ID) {
		case 1:
			back = train_list->non->sta->head1;
			while (train_list->non->sta->head1->nextdtr != NULL) {
				if (train_list->non->sta->head1->nextdtr->ID == (str_ctr[1]-'0')) {
					dtr = train_list->non->sta->head1->nextdtr;
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
				if (train_list->non->sta->head1->nextdtr->ID == (str_ctr[1]-'0')) {
					dtr = train_list->non->sta->head2->nextdtr;
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
				if (train_list->non->sta->head1->nextdtr->ID == (str_ctr[1]-'0')) {
					dtr = train_list->non->sta->head3->nextdtr;
					break;
				}
				train_list->non->sta->head3 = train_list->non->sta->head3->nextdtr;
			}
			train_list->non->sta->head3 = back;
			return dtr;
			break;
	}
}
/*catch_pubrailway_state: the information of occupied public railway(s)*/
int catch_pubrailway_state(Train_List *trList_tmp) {
	int  pub_railway_flag = 0;
	CheckPoint *ctr1 = trList_tmp->non->ctr_noun->head1;
	CheckPoint *ctr2 = trList_tmp->non->ctr_noun->head2;
	if (ctr1->nextctr->flag == 1 && ctr2->nextctr->flag == 1) {                 //two occupied public railways
		if (rail_world[trList_tmp->ptrB->now_pos_X ][trList_tmp->ptrB->now_pos_Y] == 2) {
			if (rail_world[trList_tmp->ptrA->now_pos_X ][trList_tmp->ptrA->now_pos_Y] == 2)
				pub_railway_flag = 1221;                                        //1221 == 1.B 2.A
			else pub_railway_flag = 1223;                                       //1223 == 1.B 2.C
		} else {
			pub_railway_flag = 1123;                                            //1123 == 1.A 2.C
		}
	}
	if (ctr1->nextctr->flag == 0 && ctr2->nextctr->flag == 0)
		pub_railway_flag = 33;                                                  //33 == two free public railways
	if (ctr1->nextctr->flag == 1 && ctr2->nextctr->flag == 0) {                 //1.Y 2.N
		if (rail_world[trList_tmp->ptrA->now_pos_X ][trList_tmp->ptrA->now_pos_Y] == 2)
			pub_railway_flag = 11;                                              //11 == 1.A 2.N
		else pub_railway_flag = 12;                                             //12 == 1.B 2.N
	}
	if (ctr1->nextctr->flag == 0 && ctr2->nextctr->flag == 1) {                 //1.N 2.Y
		if (rail_world[trList_tmp->ptrA->now_pos_X ][trList_tmp->ptrA->now_pos_Y] == 2)
			pub_railway_flag = 21;                                              //21 == 1.N 2.A
		else pub_railway_flag = 23;                                             //23 == 1.N 2.C
	}
	return pub_railway_flag;
}
/*input_create: Input thread create*/
unsigned _stdcall input_create(void *trList) {
	char str_ctr[20];
	Train_List *trList_tmp = (Train_List *)trList;
	print_tip();
	while (1) {
		scanf("%s", str_ctr);
		if (strlen(str_ctr) <= 3 && strlen(str_ctr) >= 2) {
			switch (str_ctr[0]) {
				case 'A':														//Train A
				case 'a': {
					switch (str_ctr[1]) {
						case 'P':
						case 'p': {												//pause
							enum STATE tempA;
							int tempspeedA;
							if (trList_tmp->ptrA->now_state != Pause) {
								tempA = trList_tmp->ptrA->now_state;
								tempspeedA = trList_tmp->ptrA->now_speed;
								trList_tmp->ptrA->now_speed = 0;
								trList_tmp->ptrA->now_state = Pause;
								printf("Train A--%s has been paused\n", trList_tmp->ptrA->train_name);
							} else {
								trList_tmp->ptrA->now_state = tempA;
								trList_tmp->ptrA->now_speed = tempspeedA;
								printf("Train A--%s has been restarted successfully\n", trList_tmp->ptrA->train_name);
							}
							break;
						} //case 'P' case 'p'
						case 'T':
						case 't': {												//changes the direction
							enum DIRECTION temp_dA;
							if (trList_tmp->ptrA->now_direction == CW) {
								trList_tmp->ptrA->now_direction = CCW;
								printf("Train A--%s's direction has been changed to CCW\n", trList_tmp->ptrA->train_name);
							} else {
								trList_tmp->ptrA->now_direction = CW;
								printf("Train A--%s's direction has been changed to CW\n", trList_tmp->ptrA->train_name);
							}
							break;
						} //case 'T' case 't'
						case 'S':												//changes the service strategy
						case 's': {
							(trList_tmp->ptrA->now_strategy == 0) ? (trList_tmp->ptrA->now_strategy = 1) : (trList_tmp->ptrA->now_strategy = 0);
							break;
						} //case 'S' case 's'
						case '+': {												//speed up
							if (trList_tmp->ptrA->now_speed < 5) {
								(trList_tmp->ptrA->now_speed)++;
								printf("Train A--%s's speed has been added to %d\n", trList_tmp->ptrA->train_name, trList_tmp->ptrA->now_speed);
							} else {
								printf("Train A--%s's speed has already been to the max 5\n", trList_tmp->ptrA->train_name);
							}
							break;
						} //case '+'
						case '-': {												//speed down
							if (trList_tmp->ptrA->now_speed > 0) {
								(trList_tmp->ptrA->now_speed)--;
								printf("The Train A--%s's speed has been minused to %d\n", trList_tmp->ptrA->train_name, trList_tmp->ptrA->now_speed);
							} else {
								printf("The Train A--%s's speed has already been to the min 0\n", trList_tmp->ptrA->train_name);
							}
							break;
						} //case '-'
						default: {												//add new task
							if(isdigit(str_ctr[1])) {
								StationPoint *dtr = station_match_ID(trList_tmp, trList_tmp->ptrA,str_ctr);
								if(dtr->release_task == 0) {
									if(isdigit(str_ctr[2])) {
										if((dtr->ID) != (str_ctr[2]-'0')) {
											dtr->release_task = str_ctr[2]-'0';
										} else
											printf("this station has already had a task.\n");
									} else {
										printf("Please input a correct command.\n");
									}
								} else {
									printf("this station is now unavailable to be assigned a new task.\n");
								}                                               //try to add a task
							} else {
								printf("Please input a correct command.\n");
							}                                                   //invalid command
							break;
						}
					} //default
					break;
				} //case A
				case 'B':														//ditto Train B
				case 'b': {
					switch (str_ctr[1]) {
						case 'P':
						case 'p': {
							enum STATE tempB;
							int tempspeedB;
							if (trList_tmp->ptrB->now_state != Pause) {
								tempB = trList_tmp->ptrB->now_state;
								tempspeedB = trList_tmp->ptrB->now_speed;
								trList_tmp->ptrB->now_speed = 0;
								trList_tmp->ptrB->now_state = Pause;
								printf("Train B--%s has been paused\n", trList_tmp->ptrB->train_name);
							} else {
								trList_tmp->ptrB->now_state = tempB;
								trList_tmp->ptrB->now_speed = tempspeedB;
								printf("Train B--%s has been restarted successfully\n", trList_tmp->ptrB->train_name);
							}
							break;
						}
						case 'T':
						case 't': {
							enum DIRECTION temp_dB;//方向
							if (trList_tmp->ptrB->now_direction == CW) {
								trList_tmp->ptrB->now_direction = CCW;
								printf("Train B--%s's direction has been changed to CCW\n", trList_tmp->ptrB->train_name);
							} else {
								trList_tmp->ptrB->now_direction = CW;
								printf("Train B--%s's direction has been changed to CW\n", trList_tmp->ptrB->train_name);
							}
							break;
						}
						case 'S':
						case 's': {
							(trList_tmp->ptrB->now_strategy == 0) ? (trList_tmp->ptrB->now_strategy = 1) : (trList_tmp->ptrB->now_strategy = 0);
							break;
						}
						case '+': {
							if (trList_tmp->ptrB->now_speed < 5) {
								(trList_tmp->ptrB->now_speed)++;
								printf("Train B--%s's speed has been added to %d\n", trList_tmp->ptrB->train_name, trList_tmp->ptrB->now_speed);
							} else {
								printf("Train B--%s's speed has already been to the max 5\n", trList_tmp->ptrB->train_name);
							}
							break;
						}
						case '-': {
							if (trList_tmp->ptrB->now_speed > 0) {
								(trList_tmp->ptrB->now_speed)--;
								printf("The Train B--%s's speed has been minused to %d\n", trList_tmp->ptrB->train_name, trList_tmp->ptrB->now_speed);
							} else {
								printf("The Train B--%s's speed has already been to the min 0\n", trList_tmp->ptrB->train_name);
							}
							break;
						}
						default: {
							if(isdigit(str_ctr[1])) {
								StationPoint *dtr = station_match_ID(trList_tmp, trList_tmp->ptrB,str_ctr);
								if(dtr->release_task == 0) {
									if(isdigit(str_ctr[2])) {
										if((dtr->ID) != (str_ctr[2]-'0')) {
											dtr->release_task = str_ctr[2]-'0';
										} else
											printf("this station has already had a task.\n");
									} else {
										printf("Please input a correct command.\n");
									}
								} else {
									printf("this station is now unavailable to be assigned a new task.\n");
								}
							} else {
								printf("Please input a correct command.\n");
							}
							break;
						}
					}
					break;
				} //case B
				case 'C':														//ditto Train C
				case 'c': {
					switch (str_ctr[1]) {
						case 'P':
						case 'p': {
							enum STATE tempC;
							int tempspeedC;
							if (trList_tmp->ptrC->now_state != Pause) {
								tempC = trList_tmp->ptrC->now_state;
								tempspeedC = trList_tmp->ptrC->now_speed;
								trList_tmp->ptrC->now_speed = 0;
								trList_tmp->ptrC->now_state = Pause;
								printf("Train C--%s has been paused\n", trList_tmp->ptrC->train_name);
							} else {
								trList_tmp->ptrC->now_state = tempC;
								trList_tmp->ptrC->now_speed = tempspeedC;
								printf("Train C--%s has been restarted successfully\n", trList_tmp->ptrC->train_name);
							}
							break;
						}
						case 'T':
						case 't': {
							enum DIRECTION temp_dC;//方向
							if (trList_tmp->ptrC->now_direction == CW) {
								trList_tmp->ptrC->now_direction = CCW;
								printf("Train C--%s's direction has been changed to CCW\n", trList_tmp->ptrC->train_name);
							} else {
								trList_tmp->ptrC->now_direction = CW;
								printf("Train C--%s's direction has been changed to CW\n", trList_tmp->ptrC->train_name);
							}
							break;
						}
						case 'S':
						case 's': {
							(trList_tmp->ptrC->now_strategy == 0) ? (trList_tmp->ptrC->now_strategy = 1) : (trList_tmp->ptrC->now_strategy = 0);
							break;
						}
						case '+': {
							if (trList_tmp->ptrC->now_speed < 5) {
								(trList_tmp->ptrC->now_speed)++;
								printf("Train C--%s's speed has been added to %d\n", trList_tmp->ptrC->train_name, trList_tmp->ptrC->now_speed);
							} else {
								printf("Train C--%s's speed has already been to the max 5\n", trList_tmp->ptrC->train_name);
							}
							break;
						}
						case '-': {
							if (trList_tmp->ptrC->now_speed > 0) {
								(trList_tmp->ptrC->now_speed)--;
								printf("The Train C--%s's speed has been minused to %d\n", trList_tmp->ptrC->train_name, trList_tmp->ptrC->now_speed);
							} else {
								printf("The Train C--%s's speed has already been to the min 0\n", trList_tmp->ptrC->train_name);
							}
							break;
						}
						default: {
							if(isdigit(str_ctr[1])) {
								StationPoint *dtr = station_match_ID(trList_tmp, trList_tmp->ptrC,str_ctr);
								if(dtr->release_task == 0) {
									if(isdigit(str_ctr[2])) {
										if((dtr->ID) != (str_ctr[2]-'0')) {
											dtr->release_task = str_ctr[2]-'0';
										} else
											printf("this station has already had a task.\n");
									} else {
										printf("Please input a correct command.\n");
									}
								} else {
									printf("this station is now unavailable to be assigned a new task.\n");
								}
							} else {
								printf("Please input a correct command.\n");
							}
							break;
						}
					}
					break;
				} //case C
				default: {
					printf("Please input a correct command.\n");
					break;
				} //default
			}
		}
		if(strlen(str_ctr) == 1) {
			switch (str_ctr[0]) {
				case 'A':
				case 'a':
					if (trList_tmp->ptrA->now_strategy == 0) {
						FAFS_RUN(trList_tmp, trList_tmp->ptrA,trList_tmp->non,&trList_tmp->time_flagA, &trList_tmp->flagA);
					} else {
						BTW_RUN(trList_tmp, trList_tmp->ptrA, trList_tmp->non, &trList_tmp->time_flagA, &trList_tmp->flagA);
					}
					CheckPoint *head = pointer_match(trList_tmp->ptrA,trList_tmp->non);
					checkpoint_flag_change(head);
					sign_a = 1;
					break;
				case 'B':
				case 'b':
					if (trList_tmp->ptrB->now_strategy == 0) {
						FAFS_RUN(trList_tmp, trList_tmp->ptrB, trList_tmp->non, &trList_tmp->time_flagB, &trList_tmp->flagB);
					} else {
						BTW_RUN(trList_tmp, trList_tmp->ptrB,trList_tmp->non, &trList_tmp->time_flagB, &trList_tmp->flagB);
					}
					head = pointer_match(trList_tmp->ptrA,trList_tmp->non);
					checkpoint_flag_change(head);
					sign_b = 1;
					break;
				case 'C':
				case 'c':
					if (trList_tmp->ptrC->now_strategy == 0) {
						FAFS_RUN(trList_tmp, trList_tmp->ptrC, trList_tmp->non, &trList_tmp->time_flagC, &trList_tmp->flagC);
					} else {
						BTW_RUN(trList_tmp, trList_tmp->ptrC,trList_tmp->non, &trList_tmp->time_flagC, &trList_tmp->flagC);
					}
					head = pointer_match(trList_tmp->ptrA,trList_tmp->non);
					checkpoint_flag_change(head);
					sign_c = 1;
					break;
				case 'Q':														//Quit
				case 'q': {
					printf("The program will exit in 5 sec.\n");
					Sleep(5000);
					printf("Infomation has saved sucessfully.\n");
					exit(0);
					break;
				} //case 'Q' case 'q'
				default :
					printf("please input a correct command.\n");
			}
		} else if(strlen(str_ctr) > 3) {
			printf("Invalid command! Please enter again\n");
		}
		printf("ip = %ld\n", clock());
	}
	_endthreadex(0);
	return 0;
}
/*run_create: trains' run thread create*/
unsigned _stdcall run_create(void *trList) {
	int count1 = 0;
	int count2 = 0;
	int sametime_flag_b = 0;
	int sametime_flag_c = 0;
	int pub_flag = 0;
	int alternation_flag = 0;
	char str_ctr[10];
	Train_List *trList_tmp = (Train_List *)trList;
	trList_tmp->time_flagA = trList_tmp->time_flagB = trList_tmp->time_flagC = 0;
	trList_tmp->flagA = trList_tmp->flagB = trList_tmp->flagC = 1;				//0:no task 1:task
	while (1) {																	//run as service strategy
		if(sign_a == 0) {
			if (trList_tmp->ptrA->now_strategy == 0) {
				FAFS_RUN(trList_tmp, trList_tmp->ptrA,trList_tmp->non,&trList_tmp->time_flagA, &trList_tmp->flagA);
			} else {
				BTW_RUN(trList_tmp, trList_tmp->ptrA, trList_tmp->non, &trList_tmp->time_flagA, &trList_tmp->flagA);
			}
		} else if(sign_a == 1)sign_a = 0;
		if(sign_b == 0) {
			if (trList_tmp->ptrB->now_strategy == 0) {
				FAFS_RUN(trList_tmp, trList_tmp->ptrB, trList_tmp->non, &trList_tmp->time_flagB, &trList_tmp->flagB);
			} else {
				BTW_RUN(trList_tmp, trList_tmp->ptrB,trList_tmp->non, &trList_tmp->time_flagB, &trList_tmp->flagB);
			}
		} else if(sign_b == 1)sign_b = 0;
		if(sign_c == 0) {
			if (trList_tmp->ptrC->now_strategy == 0) {
				FAFS_RUN(trList_tmp, trList_tmp->ptrC, trList_tmp->non, &trList_tmp->time_flagC, &trList_tmp->flagC);
			} else {
				BTW_RUN(trList_tmp, trList_tmp->ptrC,trList_tmp->non, &trList_tmp->time_flagC, &trList_tmp->flagC);
			}
		} else if(sign_c == 1)sign_c = 0;
		pub_flag = catch_pubrailway_state(trList_tmp);                          //public railway state flag
		if((trList_tmp->ptrA->now_state == Wait && trList_tmp->ptrA->passby_check_point == 0 && trList_tmp->ptrB->now_state == Wait && trList_tmp->ptrB->passby_check_point == 0) || (trList_tmp->ptrA->now_state == Wait && trList_tmp->ptrA->passby_check_point == 0 && trList_tmp->ptrC->now_state == Wait && trList_tmp->ptrC->passby_check_point == 0)) {
			alternation_flag = pointer_match_special(trList_tmp->ptrA,trList_tmp->non);
			if(alternation_flag == 2 && trList_tmp->ptrB->now_state == Wait) {
				if(trList_tmp->ptrA->alternation_strategy == 0) {                //"the faster train goes first" strategy
					if(trList_tmp->ptrA->now_speed >= trList_tmp->ptrB->now_speed) {
						if (trList_tmp->ptrA->now_strategy == 0) {
							FAFS_RUN(trList_tmp, trList_tmp->ptrA,trList_tmp->non,&trList_tmp->time_flagA, &trList_tmp->flagA);
						} else {
							BTW_RUN(trList_tmp, trList_tmp->ptrA, trList_tmp->non, &trList_tmp->time_flagA, &trList_tmp->flagA);
						}
						sign_a = 1;
					} else {
						if (trList_tmp->ptrB->now_strategy == 0) {
							FAFS_RUN(trList_tmp, trList_tmp->ptrB, trList_tmp->non, &trList_tmp->time_flagB, &trList_tmp->flagB);
						} else {
							BTW_RUN(trList_tmp, trList_tmp->ptrB,trList_tmp->non, &trList_tmp->time_flagB, &trList_tmp->flagB);
						}
						sign_b = 1;
					}
					output_time(trList_tmp->ptrA);
					output_time(trList_tmp->ptrB);
					output_time(trList_tmp->ptrC);
					output_pub_railway_state(pub_flag);							   //information output
				}
				if(trList_tmp->ptrA->alternation_strategy == 1) {                  //"alternation" strategy
					if((count1%2) == 0) {
						if (trList_tmp->ptrA->now_strategy == 0) {
							FAFS_RUN(trList_tmp, trList_tmp->ptrA,trList_tmp->non,&trList_tmp->time_flagA, &trList_tmp->flagA);
						} else {
							BTW_RUN(trList_tmp, trList_tmp->ptrA, trList_tmp->non, &trList_tmp->time_flagA, &trList_tmp->flagA);
						}
						sign_a = 1;
					}
					if((count1%2) == 1) {
						if (trList_tmp->ptrB->now_strategy == 0) {
							FAFS_RUN(trList_tmp, trList_tmp->ptrB, trList_tmp->non, &trList_tmp->time_flagB, &trList_tmp->flagB);
						} else {
							BTW_RUN(trList_tmp, trList_tmp->ptrB,trList_tmp->non, &trList_tmp->time_flagB, &trList_tmp->flagB);
						}
						sign_b = 1;
					}
					count1++;
					output_time(trList_tmp->ptrA);
					output_time(trList_tmp->ptrB);
					output_time(trList_tmp->ptrC);
					output_pub_railway_state(pub_flag);										//information output
				}
				if(trList_tmp->ptrA->alternation_strategy == 2) {                           //"manual operation" strategy
					printf("train A and train B has arrived the checkpoint at the same time.\n");
					printf("please choose one to go first.\n");
					if(sametime_flag_b == 0) {
						CheckPoint *head = pointer_match(trList_tmp->ptrA,trList_tmp->non);
						checkpoint_flag_change(head);
						sametime_flag_b++;
					}
					output_time(trList_tmp->ptrA);
					output_time(trList_tmp->ptrB);
					output_time(trList_tmp->ptrC);
					output_pub_railway_state(pub_flag);										//information output
				}
			} else if(alternation_flag == 3 && trList_tmp->ptrC->now_state == Wait) {
				if(trList_tmp->ptrA->alternation_strategy == 0) {
					if(trList_tmp->ptrA->now_speed >= trList_tmp->ptrB->now_speed) {
						if (trList_tmp->ptrA->now_strategy == 0) {
							FAFS_RUN(trList_tmp, trList_tmp->ptrA,trList_tmp->non,&trList_tmp->time_flagA, &trList_tmp->flagA);
						} else {
							BTW_RUN(trList_tmp, trList_tmp->ptrA, trList_tmp->non, &trList_tmp->time_flagA, &trList_tmp->flagA);
						}
						sign_a = 1;
					} else {
						if (trList_tmp->ptrB->now_strategy == 0) {
							FAFS_RUN(trList_tmp, trList_tmp->ptrC, trList_tmp->non, &trList_tmp->time_flagC, &trList_tmp->flagC);
						} else {
							BTW_RUN(trList_tmp, trList_tmp->ptrC,trList_tmp->non, &trList_tmp->time_flagC, &trList_tmp->flagC);
						}
						sign_c = 1;
					}
					output_time(trList_tmp->ptrA);
					output_time(trList_tmp->ptrB);
					output_time(trList_tmp->ptrC);
					output_pub_railway_state(pub_flag);										//information output
				}
				if(trList_tmp->ptrA->alternation_strategy == 1) {
					if((count2%2) == 0) {
						if (trList_tmp->ptrA->now_strategy == 0) {
							FAFS_RUN(trList_tmp, trList_tmp->ptrA,trList_tmp->non,&trList_tmp->time_flagA, &trList_tmp->flagA);
						} else {
							BTW_RUN(trList_tmp, trList_tmp->ptrA, trList_tmp->non, &trList_tmp->time_flagA, &trList_tmp->flagA);
						}
						sign_a = 1;
					}
					if((count2%2) == 1) {
						if (trList_tmp->ptrB->now_strategy == 0) {
							FAFS_RUN(trList_tmp, trList_tmp->ptrC, trList_tmp->non, &trList_tmp->time_flagC, &trList_tmp->flagC);
						} else {
							BTW_RUN(trList_tmp, trList_tmp->ptrC,trList_tmp->non, &trList_tmp->time_flagC, &trList_tmp->flagC);
						}
						sign_c = 1;
					}
					count2++;
					output_time(trList_tmp->ptrA);
					output_time(trList_tmp->ptrB);
					output_time(trList_tmp->ptrC);
					output_pub_railway_state(pub_flag);										//information output
				}
				if(trList_tmp->ptrA->alternation_strategy == 2) {  /////////记得调用 _stdcall input_create(void *trList)函数
					printf("train A and train C has arrived the checkpoint at the same time.\n");
					printf("please choose one to go first.\n");
					if(sametime_flag_c == 0) {
						CheckPoint *head = pointer_match(trList_tmp->ptrA,trList_tmp->non);
						checkpoint_flag_change(head);
						sametime_flag_c++;
					}
					output_time(trList_tmp->ptrA);
					output_time(trList_tmp->ptrB);
					output_time(trList_tmp->ptrC);
					output_pub_railway_state(pub_flag);										//information output
				}
			}
		} else {
			output_time(trList_tmp->ptrA);
			output_time(trList_tmp->ptrB);
			output_time(trList_tmp->ptrC);
			output_pub_railway_state(pub_flag);										//information output
		}
		print_tip();
		Sleep(950);																//sleep about 1 second
		system("cls");															//clean screen
	}
	_endthreadex(0);
	return 0;
}

int main(void) {
	HANDLE h_run;
	HANDLE h_input;

	printf("Welcome to the 1709 Train System (V2.0|CUI).\n");
	printf("Press Any Key to GO!\n");
	getchar(); 
	Train_List *train_list = (Train_List *) malloc(sizeof (Train_List));
	input_allthing(train_list);

	h_input = (HANDLE)_beginthreadex(NULL, 0, input_create, (void *)train_list, 0, NULL);
	h_run = (HANDLE)_beginthreadex(NULL, 0, run_create, (void *)train_list, 0, NULL);

	WaitForSingleObject(h_input, INFINITE);
	WaitForSingleObject(h_run, INFINITE);
	CloseHandle(h_input);
	CloseHandle(h_run);
	return 0;
}

/*Compile Successfully*/
