#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>


//�z�Q�ƂŃo�O��̂Ŕ��
/*�@���ʁ@*/
typedef enum {
	NORTH = 0,
	EAST,
	SOUTH,
	WEST,
	MAP_HEAD_DIR_MAX,
}enMAP_HEAD_DIR;

/* ���H�T�C�Y */
#define GOAL_MAP_X_def					( 11 )				// �S�[����X��搔�i�������j [���]
#define GOAL_MAP_Y_def					( 11 )				// �S�[����Y��搔�i�c�����j [���]
#define MAP_X_SIZE					( 32 )				// ���H��X��搔�i�������j [���]
#define MAP_Y_SIZE					( 32 )				// ���H��Y��搔�i�c�����j [���]

#define MAP_X_SIZE_REAL				( 32 )					// ���H�̎�X��搔�i�������j [���]
#define MAP_Y_SIZE_REAL				( 32 )					// ���H�̎�Y��搔�i�c�����j [���]