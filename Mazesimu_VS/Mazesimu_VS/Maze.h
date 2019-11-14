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

extern PUBLIC UCHAR	g_trgtMap[MAP_Y_SIZE][MAP_X_SIZE];		///< ������H

class mazeC
{
public:

	int sizeX;
	int sizeY;
	int goal_map_x = 32;	//�ύX�̂��ߍő�l���f�t�H���g�ɓ���Ă���
	int goal_map_y = 32;
//	UCHAR	g_trgtMap[MAP_Y_SIZE][MAP_X_SIZE];		///< ������H

	void readfile(std::string file_path);
	void show_countmap(void);
	void maze_show(void);

private:

};


#endif 
