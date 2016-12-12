#include<stdio.h>
#include<stdlib.h>
#include "init.h"

/*init_rail_world: initialize the railway world*/
void init_rail_world(void)       
{
	int i,j;
	for (i = 0; i < 100; i++) {
		for(j = 0; j < 100; j++) {
			rail_world[i][j] = 0;
		}
	}
}
/*init_railway: initialize a railway (upper left(x1, y1) lower right(x2, y2))*/
void init_railway(int x1, int y1, int x2, int y2)  
{
	int i = 1, j = 1;	
	for (i = x1; i <= x2; i++) {
		(rail_world[i][y1])++;
		(rail_world[i][y2])++;
	}
	for (j = y1 + 1; j < y2; j++) {
		(rail_world[x1][j])++;
		(rail_world[x2][j])++;
	}																	//2: cross railway point
}
/*init_checkpoint: initialize checkpoints*/
void init_checkpoint(void)
{
	FILE *fw = fopen("checkpoint.txt", "w+");
	FILE *fp2 = fopen("railway3.txt", "r");
	FILE *fp1 = fopen("railway2.txt", "r");
	int temppos[4], flag = 0;											//flag: limit the loop (number of railways)
	fscanf(fp1, "%d %d %d %d", &temppos[0], &temppos[1], &temppos[2], &temppos[3]);
	do {
		if (rail_world[temppos[0]][temppos[1]] == 2) {
			if (rail_world[temppos[0]-1][temppos[1]] == 1) {
				rail_world[temppos[0]-1][temppos[1]] = rail_world[temppos[0]][temppos[1]+1] = 3;
				rail_world[temppos[2]][temppos[1]+1] = rail_world[temppos[2]+1][temppos[1]] = 3;
				fprintf(fw, "%d %d %d %d %d %d %d %d ", temppos[0]-1, temppos[1], temppos[0], temppos[1]+1,
				        temppos[2], temppos[1]+1, temppos[2]+1, temppos[1]);
			} else if (rail_world[temppos[0]][temppos[1]-1] == 1) {
				rail_world[temppos[0]][temppos[1]-1] = rail_world[temppos[0]+1][temppos[1]] = 3;
				rail_world[temppos[0]][temppos[3]+1] = rail_world[temppos[0]+1][temppos[3]] = 3;
				fprintf(fw, "%d %d %d %d %d %d %d %d ", temppos[0], temppos[1]-1, temppos[0]+1, temppos[1],
				        temppos[0], temppos[3]+1, temppos[0]+1, temppos[3]);
			}
		} else if (rail_world[temppos[2]][temppos[3]] == 2) {
			if (rail_world[temppos[2]][temppos[3]+1] == 1) {
				rail_world[temppos[2]][temppos[3]+1] = rail_world[temppos[2]-1][temppos[3]] = 3;
				rail_world[temppos[2]][temppos[1]-1] = rail_world[temppos[2]-1][temppos[1]] = 3;
				fprintf(fw, "%d %d %d %d %d %d %d %d ", temppos[2], temppos[3]+1, temppos[2]-1, temppos[3],
				        temppos[2], temppos[1]-1, temppos[2]-1, temppos[1]);
			} else if ((rail_world[temppos[2]+1][temppos[3]] == 1)) {
				rail_world[temppos[2]+1][temppos[3]] = rail_world[temppos[2]][temppos[3]-1] = 3;
				rail_world[temppos[0]-1][temppos[3]] = rail_world[temppos[0]][temppos[3]-1] = 3;
				fprintf(fw, "%d %d %d %d %d %d %d %d ", temppos[2]+1, temppos[3], temppos[2], temppos[3]-1,
				        temppos[0]-1, temppos[3], temppos[0], temppos[3]-1);
			}
		}																		
		flag++;
		fscanf(fp2, "%d %d %d %d", &temppos[0], &temppos[1], &temppos[2], &temppos[3]);
	} while (flag < 2);			  
	fclose(fp1);
	fclose(fp2);
	fclose(fw);
}
/*init_station: initialize stations*/
void init_station(int x,int y)    
{
	rail_world[x][y] = 4;
}
/*init_checkpoint_list: get the checkpoint circle linklist head node struct*/
checkpoint_ctr_noun *init_checkpoint_list(void)  
{
	checkpoint_ctr_noun *ctr_noun;
	ctr_noun = (checkpoint_ctr_noun *)malloc(sizeof(checkpoint_ctr_noun));
	CheckPoint *head,*currentctr,*lastctr;
	FILE *fp = fopen("checkpoint.txt","r");
	if(fp == NULL)                     
		exit(-1);
	int x, y;
	int i = 0;
	while (i < 2) {
		int j = 0;
		head = (CheckPoint *)malloc(sizeof (CheckPoint));
		if (head != NULL) {
			head->nextctr = NULL;
			lastctr = head;
		}
		while (j <= 3) {
			currentctr = (CheckPoint *)malloc(sizeof (CheckPoint));
			fscanf(fp, "%d %d ", &x, &y);
			currentctr->x = x;
			currentctr->y = y;
			currentctr->flag = 0;
			currentctr->nextctr = NULL;
			lastctr->nextctr = currentctr;
			lastctr = currentctr;
			j++;
		}
		switch (i) {
			case 0:
				ctr_noun->head1 = head;
				break;
			case 1:
				ctr_noun->head2 = head;
				break;
		}
		i++;
	}
	fclose(fp);
	return ctr_noun;
}
/*init_station_node: get the station point link list head node struct*/
station *init_station_node(void)     
{
	int task[12];
	FILE *ftask = fopen("task.txt", "r");
	fscanf(ftask, "%d %d %d %d %d %d %d %d %d %d %d %d ", &task[0], &task[1], &task[2], &task[3], &task[4], &task[5], &task[6], &task[7], &task[8], &task[9], &task[10], &task[11]);
	int k = 0;
	station *sta;
	sta = (station *)malloc(sizeof(station));
	StationPoint *head,*currentdtr,*lastdtr;
	FILE *fp = fopen("station.txt","r");
	if(fp == NULL)                     
		exit(-1);
	int x, y;
	int i = 0;
	while(i < 3) {
		int j = 0;
		head = (StationPoint *)malloc(sizeof(StationPoint));
		if(head != NULL) {
			head->nextdtr = NULL;
			lastdtr = head;
		}
		while (j < 4) {
			currentdtr = (StationPoint *)malloc(sizeof (StationPoint));
			fscanf(fp,"%d %d ", &x, &y);
			currentdtr->x = x;
			currentdtr->y = y;
			currentdtr->ID = j + 1;
			currentdtr->release_task = task[k++];
			currentdtr->nextdtr = NULL;
			lastdtr->nextdtr = currentdtr;
			lastdtr = currentdtr;
			j++;
		}
		switch (i) {
			case 0:
				sta->head1 = head;
				break;
			case 1:
				sta->head2 = head;
				break;
			case 2:
				sta->head3 = head;
				break;
		}
		i++;
	}
	fclose(fp);
	fclose(ftask);
	return sta;
}


