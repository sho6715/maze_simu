#pragma once
// *************************************************************************
//   ���{�b�g��	�F �d���x�[�V�b�NDC�}�E�X�A�T���V���C��
//   �T�v		�F �T���V���C����search2�w�b�_�t�@�C��
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato			�V�K�i�t�@�C���̃C���N���[�h�j
// *************************************************************************/
// ���d�R���p�C���}�~
#ifndef _SEARCH_H
#define _SEARCH_H

//**************************************************
// �C���N���[�h�t�@�C���iinclude�j
//**************************************************
#include "common.h"
#include "typedefine.h"						// ��`
#include "map_cmd.h"

#include "Maze.h"

//**************************************************
// ��`�idefine�j
//**************************************************
/* ���H�T�C�Y */
/*#define GOAL_MAP_X_def					( 7 )				// �S�[����X��搔�i�������j [���]
#define GOAL_MAP_Y_def					( 7 )				// �S�[����Y��搔�i�c�����j [���]
#define MAP_X_SIZE					( 16 )				// ���H��X��搔�i�������j [���]
#define MAP_Y_SIZE					( 16 )				// ���H��Y��搔�i�c�����j [���]

#define MAP_X_SIZE_REAL				( 16 )					// ���H�̎�X��搔�i�������j [���]
#define MAP_Y_SIZE_REAL				( 16 )					// ���H�̎�Y��搔�i�c�����j [���]
*/
//**************************************************
// �񋓑́ienum�j
//**************************************************
/* �T�����@ */
typedef enum {
	CONTOUR_SYSTEM = 0,			// ������MAP�@
	MAP_SEARCH_TYPE_MAX,
}enMAP_SEARCH_TYPE;

/* �T�����@ */
typedef enum {
	SEARCH = 0,			// �T��
	BEST_WAY,			// �ŒZ
	MAP_ACT_MODE_MAX,
}enMAP_ACT_MODE;

/* �T������ */
typedef enum {
	SEARCH_TURN = 0,		// ���M�n����T��
	SEARCH_SURA,		// �X�����[���T��
	SEARCH_SKEW,		// �΂ߒT��
	SEARCH_RETURN,
	SEARCH_MAX,
}enSEARCH_MODE;

/*�@���ʁ@*/
/*
typedef enum {
	NORTH = 0,
	EAST,
	SOUTH,
	WEST,
	MAP_HEAD_DIR_MAX,
}enMAP_HEAD_DIR;
*/


//**************************************************
// �\���́istruct�j
//**************************************************


//**************************************************
// �O���[�o���ϐ�
//**************************************************
extern PUBLIC UCHAR		g_sysMap[MAP_Y_SIZE][MAP_X_SIZE];		///< ���H���
extern PUBLIC USHORT	us_cmap[MAP_Y_SIZE][MAP_X_SIZE];		///< ������ �f�[�^

extern PUBLIC UCHAR		GOAL_MAP_X;					//�S�[�����W�ύX�v���O�����p��
extern PUBLIC UCHAR		GOAL_MAP_Y;					//�S�[�����W�ύX�v���O�����p��

//**************************************************
// �v���g�^�C�v�錾�i�t�@�C�����ŕK�v�Ȃ��̂����L�q�j
//**************************************************
PUBLIC void MAP_init(void);
PUBLIC void MAP_Goal_init(void);
PUBLIC void MAP_Goal_change_x(void);
PUBLIC void MAP_Goal_change_y(void);
PUBLIC void MAP_showLog(void);
PUBLIC void MAP_clearMap(void);
PUBLIC void MAP_setPos(UCHAR uc_x, UCHAR uc_y, enMAP_HEAD_DIR en_dir);
PUBLIC void MAP_searchGoal(UCHAR uc_trgX, UCHAR uc_trgY, enMAP_ACT_MODE en_type, enSEARCH_MODE en_search);
PUBLIC void MAP_makeContourMap(UCHAR uc_goalX, UCHAR uc_goalY, enMAP_ACT_MODE en_type);
PUBLIC void MAP_actGoalLED(void);

PUBLIC void MAP_ClearMapData(void);

PUBLIC void MAP_showcountLog(void);


PUBLIC void Simu_searchGoal(
	UCHAR 			uc_trgX, 		///< [in] �ڕWx���W
	UCHAR 			uc_trgY, 		///< [in] �ڕWy���W 
	enMAP_ACT_MODE 	en_type, 		///< [in] �T�����@
	enSEARCH_MODE	en_search 		///< [in] �T�����@
);

#endif //_SEARCH_H