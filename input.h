#ifndef IN_INPUT
#define IN_INPUT

#include <stdio.h> 
#include "init.h"

void input_railway(FILE *fp);                        //input the related data to init the fundamental railways
void input_train(Train *ptr, FILE *fp);              //input the related data to init all trains
void input_stationpoint(void);                       //input the related data to init the stationpoint on the railways
void input_allthing (Train_List *train_list); //to set up the whole rail_world

#endif
