#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>



//循環参照でバグるので避難
/*　方位　*/
typedef enum {
	NORTH = 0,
	EAST,
	SOUTH,
	WEST,
	MAP_HEAD_DIR_MAX,
}enMAP_HEAD_DIR;

/* 迷路サイズ */
#define GOAL_MAP_X_def					( 7 )				// ゴールのX区画数（横方向） [区画]
#define GOAL_MAP_Y_def					( 7 )				// ゴールのY区画数（縦方向） [区画]
#define MAP_X_SIZE					( 16 )				// 迷路のX区画数（横方向） [区画]
#define MAP_Y_SIZE					( 16 )				// 迷路のY区画数（縦方向） [区画]

#define MAP_X_SIZE_REAL				( 16 )					// 迷路の実X区画数（横方向） [区画]
#define MAP_Y_SIZE_REAL				( 16 )					// 迷路の実Y区画数（縦方向） [区画]