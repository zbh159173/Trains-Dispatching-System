#ifndef IN_INIT
#define IN_INIT
#include<stdio.h>
#include<stdlib.h>


enum STATE {Unable, Pause, Wait, Stop, Notask_Run, Task_Run};
enum DIRECTION {CCW, CW};												        //counter-clockwise & clockwise
enum STRATEGY {FAFS, BTW};														//first-arrive-first-serve & by-the-way

typedef struct {
	int ID;
	char train_name[20];		 
	int start_time;		 	     
	int begin_pos_X;
	int begin_pos_Y;			 												//begin position
	int now_speed;			    
	enum STATE now_state;	 	
	enum DIRECTION now_direction;
	enum STRATEGY now_strategy;
	int now_pos_X;
	int now_pos_Y;				 												//now position
	int is_in_task;				 												//0:no task 1: task
	int passby_check_point;	                                                    //uneven & even
    int task_list[5];                                                           //used in BTW
    int fire_coin;               
    int now_station_ID;    
	int alternation_strategy;                                                   //0:"the faster train goes first" strategy
	                                                                            //1:"alternation" strategy 
																				//2:"manual operation" strategy 
} Train;

typedef struct StationPoint{
	int x;
	int y;
	int ID;
	int release_task;                                                           //destination station(if no task set as 0)
	struct StationPoint *nextdtr;
} StationPoint;					

typedef struct CheckPoint{      
	int x;
	int y; 
	int flag;                                                                   //0: inoccupied 1:occupied
	struct CheckPoint *nextctr;
} CheckPoint;

int rail_world[100][100];                                                       //100 * 100 railway world 

void init_rail_world(void);
void init_railway(int x1,int y1,int x2,int y2);
void init_checkpoint(void);
void init_station(int x,int y);  

typedef struct checkpoint_ctr_noun{                                             //checkpoint linklist headnode list 
	CheckPoint *head1;                                                          //1-2 cross railway checkpoint list headnode 
	CheckPoint *head2;            												//1-3 cross railway checkpoint list headnode
} checkpoint_ctr_noun;

checkpoint_ctr_noun *init_checkpoint_list(void);  								//get the checkpoint linklist headnode list

typedef struct station{                      									//station linklist headnode list
	StationPoint *head1;             											//railway 1 station linklist
	StationPoint *head2;            											//railway 2 station linklist
	StationPoint *head3;             											//railway 3 station linklist
} station;


station *init_station_node(void);   										    //get the station linklist headnode list 

typedef struct{
	station *sta;
	checkpoint_ctr_noun *ctr_noun;
} noun;

typedef struct {
	Train *ptrA;
	Train *ptrB;
	Train *ptrC;
	noun *non;
	int time_flagA;
	int time_flagB;
	int time_flagC;
	int flagA;																	//task_flag: 0-no 1-yes
	int flagB;
	int flagC;
} Train_List;

#endif
