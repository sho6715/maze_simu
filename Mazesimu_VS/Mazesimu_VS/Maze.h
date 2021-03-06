#pragma once
/**
 * @file Maze_show.cpp
 * @brief
 * @author sho6715
 * @date 2019.11.13
 */

#ifndef _MAZE_H
#define _MAZE_H

#include "common.h"
#include "typedefine.h"
#include "search.h"
#include "map_cmd.h"

extern PUBLIC UCHAR	g_trgtMap[MAP_Y_SIZE][MAP_X_SIZE];		///< 正解迷路

class mazeC
{
public:

	int sizeX;
	int sizeY;
	int goal_map_x = 32;	//変更のため最大値をデフォルトに入れている
	int goal_map_y = 32;
	int goal_count = 0;

	void readfile(std::string file_path);
	void show_countmap(void);
	void maze_show_trgt(void);
private:

};

void direction_show(
	enMAP_HEAD_DIR	en_head,
	char x,
	char y,
	char mx,
	char my);
void maze_show_search(enMAP_HEAD_DIR	en_head, int mx, int my);


#endif 
