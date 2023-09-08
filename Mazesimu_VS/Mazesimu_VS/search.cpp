// *************************************************************************
//   ���{�b�g��	�F �d���x�[�V�b�NDC�}�E�X�A�T���V���C��
//   �T�v		�F �T���V���C����search2�t�@�C��
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2017.09.27			sato		�V�K�i�t�@�C���̃C���N���[�h�j
// *************************************************************************/


//**************************************************
// �C���N���[�h�t�@�C���iinclude�j
//**************************************************
#include "search.h"
#include "queue.h"

#include <chrono>
#include <thread>
//#include <queue>		//�L���[���g���悤�ɂ��邽��

//**************************************************
// ��`�idefine�j
//**************************************************
#define MAP_SMAP_MAX_VAL	( MAP_X_SIZE * MAP_Y_SIZE ) 			///< ������map�̍ő�l
#define MAP_SMAP_MAX_PRI_VAL	( MAP_SMAP_MAX_VAL * 4 )				///< ������map�̗D��x�ő�l

#define MOVE_BACK_DIST		(0.3f)

//**************************************************
// �񋓑́ienum�j
//**************************************************

//**************************************************
// �\���́istruct�j
//**************************************************
#define MAP_SMAP_MAX_VAL		( MAP_X_SIZE * MAP_Y_SIZE ) 			///< ������map�̍ő�l
#define MAP_SMAP_MAX_PRI_VAL	( MAP_SMAP_MAX_VAL * 4 )				///< ������map�̗D��x�ő�l


//**************************************************
// �O���[�o���ϐ�
//**************************************************
PRIVATE enMAP_HEAD_DIR	en_Head;										///< �}�E�X�̐i�s���� 0:N 1:E 2:S 3:W
PRIVATE UCHAR		my;												///< �}�E�X�̂w���W
PRIVATE UCHAR		mx;												///< �}�E�X�̂x���W
PUBLIC USHORT		us_cmap[MAP_Y_SIZE][MAP_X_SIZE];				///< ������ �f�[�^
PUBLIC UCHAR		g_sysMap[MAP_Y_SIZE][MAP_X_SIZE];			///< ���H���
PUBLIC UCHAR		g_Map_direction[MAP_Y_SIZE][MAP_X_SIZE];			///< ���H���
PRIVATE FLOAT		f_MoveBackDist;									///< �Ǔ��ē���Ō�ނ�������[���]
PRIVATE UCHAR		uc_SlaCnt = 0;									// �X�����[���A����
PRIVATE UCHAR		uc_back[MAP_Y_SIZE][MAP_X_SIZE];			// ���H�f�[�^

PUBLIC UCHAR		GOAL_MAP_X;					//�S�[�����W�ύX�v���O�����p��
PUBLIC UCHAR		GOAL_MAP_Y;					//�S�[�����W�ύX�v���O�����p��

PUBLIC UCHAR		GOAL_SIZE;


//�������}�b�v���X�V���~�߂邽�߂̈ړ����K��ϐ�
PRIVATE UCHAR		uc_max_x = GOAL_MAP_X_def;
PRIVATE UCHAR		uc_max_y = GOAL_MAP_Y_def;

//**************************************************
// �v���g�^�C�v�錾�i�t�@�C�����ŕK�v�Ȃ��̂����L�q�j
//**************************************************
extern PUBLIC	UCHAR		dcom[];					// ���n�M����p
extern PUBLIC	UCHAR		scom[];					// �X�����[���p
extern PUBLIC	UCHAR		tcom[];					// �΂ߑ��s�p

//TKR
/* ���m��ԉ��� */
typedef struct
{
	UCHAR	uc_StrCnt;
	BOOL	bl_Known;
}stMAP_KNOWN;

PRIVATE stMAP_KNOWN		st_known = { 0,FALSE };


// *************************************************************************
//   �@�\		�F ���H���W���[��������������
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PUBLIC void MAP_init(void)
{
	UCHAR uc_dummy[MAP_Y_SIZE][MAP_X_SIZE];			// ���H�f�[�^

	/* ���W�A�����A���H���������� */
	en_Head = NORTH;
	mx = 0;
	my = 0;
	MAP_clearMap();

	/* ���s�p�̃p�����[�^ */
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;
}

// *************************************************************************
//   �@�\		�F �S�[�����W������������
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2019.08.05			sato		�V�K
// *************************************************************************/
PUBLIC void MAP_Goal_init(void)
{
	GOAL_MAP_X = GOAL_MAP_X_def;
	GOAL_MAP_Y = GOAL_MAP_Y_def;
}

// *************************************************************************
//   �@�\		�F ���H�����N���A����
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PUBLIC void MAP_ClearMapData(void)
{
	/* ���W�A�����A���H���������� */
	en_Head = NORTH;
	mx = 0;
	my = 0;
	MAP_clearMap();

	/* ���s�p�̃p�����[�^ */
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;


}

// *************************************************************************
//   �@�\		�F ���W�ʒu��ݒ肷��
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PUBLIC void MAP_setPos(UCHAR uc_x, UCHAR uc_y, enMAP_HEAD_DIR en_dir)
{
	mx = uc_x;
	my = uc_y;
	en_Head = en_dir;

	MAP_setCmdPos(uc_x, uc_y, en_dir);//�z�Q�ƂŃo�O

}

// *************************************************************************
//   �@�\		�F ���H�f�[�^��\������
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PUBLIC void MAP_showLog(void)
{
	SHORT	x, y;
	CHAR	c_data;

	/* ---------- */
	/*  �ʏ���H  */
	/* ---------- */
	printf("\n\r  /* ---------- */   ");
	printf("\n\r  /*  �ʏ���H  */   ");
	printf("\n\r  /* ---------- */   ");

	printf("\n\r     ");
	for (x = 0; x < MAP_X_SIZE; x++) {
		printf("._");
	}
	printf(".\n\r");

	for (y = MAP_Y_SIZE - 1; y > -1; y--) {

		printf("   %2d", y);
		for (x = 0; x < MAP_X_SIZE; x++) {
			c_data = (UCHAR)g_sysMap[y][x];
			if ((c_data & 0x08) == 0) {
				printf(".");
			}
			else {
				printf("|");
			}
			if ((c_data & 0x04) == 0) {
				printf(" ");
			}
			else {
				printf("_");
			}
		}
		printf("|   ");

/*		for (x = 0; x < MAP_X_SIZE; x++) {
			c_data = g_sysMap[y][x];
			c_data = c_data >> 4;
			printf("%x", c_data);
		}
*/
		printf("\n\r");
	}

	printf("     ");
	for (x = 0; x < MAP_X_SIZE; x++) {
		printf("%2d", x % 10);
	}
	printf("\n\r");

}

// *************************************************************************
//   �@�\		�F �������}�b�v��\������
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2019.11.13			sato		�V�K
// *************************************************************************/
PUBLIC void MAP_showcountLog(void)
{
	SHORT	x, y;
	USHORT	c_data;

	/* ---------- */
	/*  ���������H  */
	/* ---------- */
	printf("\n\r  /* ------------ */   ");
	printf("\n\r  /*  ���������H  */   ");
	printf("\n\r  /* ------------ */   ");

	printf("\n\r");
//	for (x = 0; x < MAP_X_SIZE; x++) {
//		printf("._");
//	}
//	printf(".\n\r");

	for (y = MAP_Y_SIZE - 1; y > -1; y--) {

//		printf("   %2d", y);
		for (x = 0; x < MAP_X_SIZE; x++) {
			c_data = (USHORT)us_cmap[y][x];
			printf("%4d ", c_data);
/*			if ((c_data & 0x08) == 0) {
				printf(".");
			}
			else {
				printf("|");
			}
			if ((c_data & 0x04) == 0) {
				printf(" ");
			}
			else {
				printf("_");
			}
*/
		}
//		printf("|   ");

		/*		for (x = 0; x < MAP_X_SIZE; x++) {
					c_data = g_sysMap[y][x];
					c_data = c_data >> 4;
					printf("%x", c_data);
				}
		*/
		printf("\n\r");
	}


}

// *************************************************************************
//   �@�\		�F ���H�f�[�^���N���A����
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PUBLIC void MAP_clearMap(void)
{
	USHORT	x, y;
	UCHAR	uc_data;

	/* ���ׂẴ}�b�v�f�[�^�𖢒T����Ԃɂ��� */
	for (y = 0; y < MAP_Y_SIZE; y++) {
		for (x = 0; x < MAP_X_SIZE; x++) {
			uc_data = 0x00;
			if ((x == 0) && (y == 0)) uc_data = 0xfe;
			else if ((x == 1) && (y == 0)) uc_data = 0xcc;
			else if ((x == (MAP_X_SIZE - 1)) && (y == 0)) uc_data = 0x66;
			else if ((x == 0) && (y == (MAP_Y_SIZE - 1))) uc_data = 0x99;
			else if ((x == (MAP_X_SIZE - 1)) && (y == (MAP_Y_SIZE - 1))) uc_data = 0x33;
			else if (x == 0) uc_data = 0x88;
			else if (x == (MAP_X_SIZE - 1)) uc_data = 0x22;
			else if (y == 0) uc_data = 0x44;
			else if (y == (MAP_Y_SIZE - 1)) uc_data = 0x11;
			g_sysMap[y][x] = uc_data;
		}
	}

}

// *************************************************************************
//   �@�\		�F �Z���T��񂩂�Ǐ����擾����
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PRIVATE UCHAR MAP_getWallData(void)
{
	UCHAR	 uc_wall;

	//	LED_offAll();			// debug

		// �Z���T��񂩂�Ǐ��쐬
	uc_wall = 0;
/*	if (TRUE == DIST_isWall_FRONT()) {
		uc_wall = uc_wall | 0x11;
	}
	if (TRUE == DIST_isWall_L_SIDE()) {
		uc_wall = uc_wall | 0x88;
	}
	if (TRUE == DIST_isWall_R_SIDE()) {
		uc_wall = uc_wall | 0x22;
	}
*/
	// �}�E�X�̐i�s�����ɂ��킹�ăZ���T�f�[�^���ړ����ǃf�[�^�Ƃ���
	if (en_Head == EAST) {
		uc_wall = uc_wall >> 3;
	}
	else if (en_Head == SOUTH) {
		uc_wall = uc_wall >> 2;
	}
	else if (en_Head == WEST) {
		uc_wall = uc_wall >> 1;
	}

	//	�T���ς݃t���O�𗧂Ă�
	return (uc_wall | 0xf0);
}

// *************************************************************************
//   �@�\		�F �Ǐ����쐬����
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PRIVATE void MAP_makeMapData(void)
{
	UCHAR uc_wall;

	//	���s���̕Ǐ�����H���ɏ���
	if ((mx == 0) && (my == 0)) {
		uc_wall = 0xfe;
	}
	else {
		uc_wall = MAP_getWallData();
	}
	g_sysMap[my][mx] = uc_wall;

	//	�ׂ̋�Ԃ̂l�`�o�f�[�^���X�V����
	if (mx != (MAP_X_SIZE - 1)) {
		g_sysMap[my][mx + 1] = (g_sysMap[my][mx + 1] & 0x77) | 0x80 | ((uc_wall << 2) & 0x08);
	}
	if (mx != 0) {
		g_sysMap[my][mx - 1] = (g_sysMap[my][mx - 1] & 0xdd) | 0x20 | ((uc_wall >> 2) & 0x02);
	}
	if (my != (MAP_Y_SIZE - 1)) {
		g_sysMap[my + 1][mx] = (g_sysMap[my + 1][mx] & 0xbb) | 0x40 | ((uc_wall << 2) & 0x04);
	}
	if (my != 0) {
		g_sysMap[my - 1][mx] = (g_sysMap[my - 1][mx] & 0xee) | 0x10 | ((uc_wall >> 2) & 0x01);
	}

}

// *************************************************************************
//   �@�\		�F �������}�b�v���쐬����
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PUBLIC void  MAP_makeContourMap(
	UCHAR uc_goalX, 			///< [in] �S�[��X���W
	UCHAR uc_goalY, 			///< [in] �S�[��Y���W
	enMAP_ACT_MODE	en_type		///< [in] �v�Z���@�i�܂����g�p�j
) {
	USHORT		x, y, i;		// ���[�v�ϐ�
	USHORT		uc_dase;		// ��l
	USHORT		uc_new;			// �V�l
	USHORT		uc_level;		// ������
	UCHAR		uc_wallData;	// �Ǐ��

	en_type = en_type;		// �R���p�C�����[�j���O����i������폜�j

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER start, end;

	QueryPerformanceCounter(&start);

	/* �������}�b�v������������ */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}
//	QueryPerformanceCounter(&start);
	/* �ڕW�n�_�̓�������0�ɐݒ� */
	us_cmap[uc_goalY][uc_goalX] = 0;
	if (GOAL_SIZE == 4) {
		us_cmap[uc_goalY + 1][uc_goalX] = 0;
		us_cmap[uc_goalY][uc_goalX + 1] = 0;
		us_cmap[uc_goalY + 1][uc_goalX + 1] = 0;
	}
	else if (GOAL_SIZE == 9){
		us_cmap[uc_goalY+1][uc_goalX] = 0;
		us_cmap[uc_goalY][uc_goalX+1] = 0;
		us_cmap[uc_goalY+1][uc_goalX+1] = 0;
		us_cmap[uc_goalY+2][uc_goalX] = 0;
		us_cmap[uc_goalY+2][uc_goalX+1] = 0;
		us_cmap[uc_goalY][uc_goalX+2] = 0;
		us_cmap[uc_goalY+1][uc_goalX+2] = 0;
		us_cmap[uc_goalY+2][uc_goalX+2] = 0;
	}

	if (mx > uc_max_x)uc_max_x = mx;
	if (my > uc_max_y)uc_max_y = my;
	uc_max_x = 32;
	uc_max_y = 32;

	/* �������}�b�v���쐬 */
	uc_dase = 0;
	do {
		uc_level = 0;
		uc_new = uc_dase + 1;
		for (y = 0; y < MAP_Y_SIZE; y++) {
			if (uc_max_y+1 < y) break;
			for (x = 0; x < MAP_X_SIZE; x++) {
				if (us_cmap[y][x] == uc_dase) {
					uc_wallData = g_sysMap[y][x];
					if (uc_max_x+1 < x) break;
					/* �T�����s */
					if (SEARCH == en_type) {
						if (((uc_wallData & 0x01) == 0x00) && (y != (MAP_Y_SIZE - 1))) {
							if (us_cmap[y + 1][x] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y + 1][x] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x02) == 0x00) && (x != (MAP_X_SIZE - 1))) {
							if (us_cmap[y][x + 1] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y][x + 1] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x04) == 0x00) && (y != 0)) {
							if (us_cmap[y - 1][x] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y - 1][x] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x08) == 0x00) && (x != 0)) {
							if (us_cmap[y][x - 1] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y][x - 1] = uc_new;
								uc_level++;
							}
						}
					}
					/* �ŒZ���s */
					else {
						if (((uc_wallData & 0x11) == 0x10) && (y != (MAP_Y_SIZE - 1))) {
							if (us_cmap[y + 1][x] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y + 1][x] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x22) == 0x20) && (x != (MAP_X_SIZE - 1))) {
							if (us_cmap[y][x + 1] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y][x + 1] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x44) == 0x40) && (y != 0)) {
							if (us_cmap[y - 1][x] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y - 1][x] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x88) == 0x80) && (x != 0)) {
							if (us_cmap[y][x - 1] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y][x - 1] = uc_new;
								uc_level++;
							}
						}
					}
//					if ((x == mx) && (y == my))	break;
				}
			}
//			if ((x == mx) && (y == my))break;
		}
//		if ((x == mx) && (y == my)) break;
		uc_dase = uc_dase + 1;
	} while (uc_level != 0);


	QueryPerformanceCounter(&end);

	double time = static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
	printf("time %lf[ms]\n", time);

#if 0
	/* debug */
	for (x = 0; x < 4; x++) {

		for (y = 0; y < 4; y++) {
			us_Log[y][x][us_LogPt] = us_cmap[y][x];
		}
	}
	us_LogPt++;
#endif

}

void setStep(const int8_t x, const int8_t y, const uint16_t step) {
	/* (x, y) ���t�B�[���h�����m�F */
	if (x < 0 || y < 0 || x >= MAP_X_SIZE || y >= MAP_Y_SIZE) {
		std::cout << "referred to out of field: " << std::endl;
		return;
	}
	us_cmap[y][x] = step;
}

BOOL wallcheck(stPOSITION focus, char i) {
	if ((i == 0) && ((g_sysMap[focus.y][focus.x] & 0x01) == 0x01))
		return FALSE;
	else if ((i == 1) && ((g_sysMap[focus.y][focus.x] & 0x02) == 0x02))
		return FALSE;
	else if ((i == 2) && ((g_sysMap[focus.y][focus.x] & 0x04) == 0x04))
		return FALSE;
	else if ((i == 3) && ((g_sysMap[focus.y][focus.x] & 0x08) == 0x08))
		return FALSE;
	else
		return TRUE;
}

stPOSITION nextdirection(stPOSITION next, char i) {
	switch (i) {
		case 0:
			next.y += 1;
			break;
		case 1:
			next.x += 1;
			break;
		case 2:
			next.y -= 1;
			break;
		case 3:
			next.x -= 1;
			break;
	}

	return next;
}

PUBLIC void  MAP_makeContourMap_kai2(
	UCHAR uc_goalX, 			///< [in] �S�[��X���W
	UCHAR uc_goalY, 			///< [in] �S�[��Y���W
	enMAP_ACT_MODE	en_type		///< [in] �v�Z���@�i�܂����g�p�j
) {
	USHORT		x, y, i;		// ���[�v�ϐ�
	USHORT		uc_dase;		// ��l
	USHORT		uc_new;			// �V�l
	USHORT		uc_level;		// ������
	UCHAR		uc_wallData;	// �Ǐ��

	stPOSITION		st_pos;

	en_type = en_type;		// �R���p�C�����[�j���O����i������폜�j

	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER start, end;
	QueryPerformanceCounter(&start);



	/* �������}�b�v������������ */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}

	/* �X�e�b�v�̍X�V�\��̃L���[ */
//	std::queue<stPOSITION> q;
//	QueryPerformanceCounter(&start);

	/* �ڕW�n�_�̓�������0�ɐݒ� */
	setStep(uc_goalX, uc_goalY, 0);
	st_pos.x = uc_goalX;
	st_pos.y = uc_goalY;
	st_pos.step = 0;

	enqueue(pQueue,st_pos);

	/* �������}�b�v���쐬 */
	while (pQueue->flag != EMPTY) {
		const auto focus = dequeue(pQueue);
//		q.pop();
		const auto focus_step = focus.step;
		x = focus.x;
		y = focus.y;
		auto next = focus;
		uc_wallData = g_sysMap[y][x];

		if (((uc_wallData & 0x01) == 0x00) && (y != (MAP_Y_SIZE - 1))) {
			if (us_cmap[y + 1][x] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y + 1][x] = focus_step + 1;
				next.x = x;
				next.y = y + 1;
				enqueue(pQueue,next);
			}
		}
		if (((uc_wallData & 0x02) == 0x00) && (x != (MAP_X_SIZE - 1))) {
			if (us_cmap[y][x + 1] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y][x + 1] = focus_step + 1;
				next.x = x + 1;
				next.y = y;
				enqueue(pQueue, next);
			}
		}
		if (((uc_wallData & 0x04) == 0x00) && (y != 0)) {
			if (us_cmap[y - 1][x] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y - 1][x] = focus_step + 1;
				next.x = x;
				next.y = y - 1;
				enqueue(pQueue, next);
			}
		}
		if (((uc_wallData & 0x08) == 0x00) && (x != 0)) {
			if (us_cmap[y][x - 1] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y][x - 1] = focus_step + 1;
				next.x = x - 1;
				next.y = y;
				enqueue(pQueue, next);
			}
		}

	}



	QueryPerformanceCounter(&end);

	double time = static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
	printf("time %lf[ms]\n", time);

#if 0
	/* debug */
	for (x = 0; x < 4; x++) {

		for (y = 0; y < 4; y++) {
			us_Log[y][x][us_LogPt] = us_cmap[y][x];
		}
	}
	us_LogPt++;
#endif
}

PUBLIC void MAP_clearMap_direction(void)
{
	USHORT	x, y;
	UCHAR	uc_data;

	/* ���ׂẴ}�b�v�f�[�^�𖢒T����Ԃɂ��� */
	for (y = 0; y < MAP_Y_SIZE; y++) {
		for (x = 0; x < MAP_X_SIZE; x++) {
			uc_data = 0x00;
			g_Map_direction[y][x] = uc_data;
		}
	}

}

PUBLIC void  MAP_makeContourMap_dijkstra_modoki(
	UCHAR uc_goalX, 			///< [in] �S�[��X���W
	UCHAR uc_goalY, 			///< [in] �S�[��Y���W
	enMAP_ACT_MODE	en_type		///< [in] �v�Z���@�i�܂����g�p�j
) {
	USHORT		x, y, i;		// ���[�v�ϐ�
	USHORT		uc_dase;		// ��l
	USHORT		uc_new;			// �V�l
	USHORT		uc_level;		// ������
	UCHAR		uc_wallData;	// �Ǐ��

	en_type = en_type;		// �R���p�C�����[�j���O����i������폜�j

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER start, end;

	QueryPerformanceCounter(&start);
	MAP_clearMap_direction();

	/* �������}�b�v������������ */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL*4 - 1;
	}
//	QueryPerformanceCounter(&start);
	/* �ڕW�n�_�̓�������0�ɐݒ� */
	us_cmap[uc_goalY][uc_goalX] = 0;
	if (GOAL_SIZE == 4) {
		us_cmap[uc_goalY + 1][uc_goalX] = 0;
		us_cmap[uc_goalY][uc_goalX + 1] = 0;
		us_cmap[uc_goalY + 1][uc_goalX + 1] = 0;
	}
	else if (GOAL_SIZE == 9){
		us_cmap[uc_goalY+1][uc_goalX] = 0;
		us_cmap[uc_goalY][uc_goalX+1] = 0;
		us_cmap[uc_goalY+1][uc_goalX+1] = 0;
		us_cmap[uc_goalY+2][uc_goalX] = 0;
		us_cmap[uc_goalY+2][uc_goalX+1] = 0;
		us_cmap[uc_goalY][uc_goalX+2] = 0;
		us_cmap[uc_goalY+1][uc_goalX+2] = 0;
		us_cmap[uc_goalY+2][uc_goalX+2] = 0;
	}

	if (mx > uc_max_x)uc_max_x = mx;
	if (my > uc_max_y)uc_max_y = my;
	uc_max_x = 32;
	uc_max_y = 32;

	g_Map_direction[uc_goalY][uc_goalX] = 0xff;

	/* �������}�b�v���쐬 */
	uc_dase = 0;
	do {
		uc_level = 0;
		uc_new = uc_dase + 1;
		for (y = 0; y < MAP_Y_SIZE; y++) {
			if (uc_max_y+1 < y) break;
			for (x = 0; x < MAP_X_SIZE; x++) {
				if (us_cmap[y][x] == uc_dase) {
					uc_wallData = g_sysMap[y][x];
					if (uc_max_x+1 < x) break;
					/* �T�����s */
					if (SEARCH == en_type) {
						if (((uc_wallData & 0x01) == 0x00) && (y != (MAP_Y_SIZE - 1))) {
							if (us_cmap[y + 1][x] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y + 1][x] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x02) == 0x00) && (x != (MAP_X_SIZE - 1))) {
							if (us_cmap[y][x + 1] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y][x + 1] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x04) == 0x00) && (y != 0)) {
							if (us_cmap[y - 1][x] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y - 1][x] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x08) == 0x00) && (x != 0)) {
							if (us_cmap[y][x - 1] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y][x - 1] = uc_new;
								uc_level++;
							}
						}
					}
					/* �ŒZ���s */
					else {
						if (((uc_wallData & 0x11) == 0x10) && (y != (MAP_Y_SIZE - 1))) {
							if((g_Map_direction[y][x]&0x10) == 0x10){
								uc_new = uc_dase + 1;
							}else{
								uc_new = uc_dase + 2;
							}
							if (us_cmap[y + 1][x] > uc_new) {
								us_cmap[y + 1][x] = uc_new;
								g_Map_direction[y+1][x] |= 0x10;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x22) == 0x20) && (x != (MAP_X_SIZE - 1))) {
							if((g_Map_direction[y][x]&0x40) == 0x40){
								uc_new = uc_dase + 1;
							}else{
								uc_new = uc_dase + 2;
							}
							if (us_cmap[y][x + 1] > uc_new) {
								us_cmap[y][x + 1] = uc_new;
								g_Map_direction[y][x+1] |= 0x40;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x44) == 0x40) && (y != 0)) {
							if((g_Map_direction[y][x]&0x01) == 0x01){
								uc_new = uc_dase + 1;
							}else{
								uc_new = uc_dase + 2;
							}
							if (us_cmap[y - 1][x] > uc_new) {
								us_cmap[y - 1][x] = uc_new;
								g_Map_direction[y-1][x] |= 0x01;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x88) == 0x80) && (x != 0)) {
							if((g_Map_direction[y][x]&0x04) == 0x04){
								uc_new = uc_dase + 1;
							}else{
								uc_new = uc_dase + 2;
							}
							if (us_cmap[y][x - 1] > uc_new) {
								us_cmap[y][x - 1] = uc_new;
								g_Map_direction[y][x-1] |= 0x04;
								uc_level++;
							}
						}
					}
				}
				if(uc_dase != 4095)uc_level++;
			}
		}
		uc_dase = uc_dase + 1;
	} while (uc_level != 0);


	QueryPerformanceCounter(&end);

	double time = static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
	printf("time %lf[ms]\n", time);

#if 0
	/* debug */
	for (x = 0; x < 4; x++) {

		for (y = 0; y < 4; y++) {
			us_Log[y][x][us_LogPt] = us_cmap[y][x];
		}
	}
	us_LogPt++;
#endif

}

void  MAP_makeContourMap_maltigoal(
	UCHAR uc_goalX, 			///< [in] �S�[��X���W
	UCHAR uc_goalY, 			///< [in] �S�[��Y���W
	enMAP_ACT_MODE	en_type		///< [in] �v�Z���@�i�܂����g�p�j
) {
	USHORT		x, y, i;		// ���[�v�ϐ�
	USHORT		uc_dase;		// ��l
	USHORT		uc_new;			// �V�l
	USHORT		uc_level;		// ������
	UCHAR		uc_wallData;	// �Ǐ��

	stPOSITION		st_pos;

	en_type = en_type;		// �R���p�C�����[�j���O����i������폜�j

	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);


	/* �������}�b�v������������ */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}

	us_cmap[uc_goalY][uc_goalX] = 0;
	if (GOAL_SIZE == 4) {
		us_cmap[uc_goalY + 1][uc_goalX] = 0;
		us_cmap[uc_goalY][uc_goalX + 1] = 0;
		us_cmap[uc_goalY + 1][uc_goalX + 1] = 0;
	}
	else if (GOAL_SIZE == 9){
		us_cmap[uc_goalY+1][uc_goalX] = 0;
		us_cmap[uc_goalY][uc_goalX+1] = 0;
		us_cmap[uc_goalY+1][uc_goalX+1] = 0;
		us_cmap[uc_goalY+2][uc_goalX] = 0;
		us_cmap[uc_goalY+2][uc_goalX+1] = 0;
		us_cmap[uc_goalY][uc_goalX+2] = 0;
		us_cmap[uc_goalY+1][uc_goalX+2] = 0;
		us_cmap[uc_goalY+2][uc_goalX+2] = 0;
	}

	/* �X�e�b�v�̍X�V�\��̃L���[ */
//	std::queue<stPOSITION> q;
//	QueryPerformanceCounter(&start);

	/* �ڕW�n�_�̓�������0�ɐݒ� */
//	setStep(uc_goalX, uc_goalY, 0);
	st_pos.x = uc_goalX;
	st_pos.y = uc_goalY;
	st_pos.step = 0;

	enqueue(pQueue,st_pos);

	/* �������}�b�v���쐬 */
	while (pQueue->flag != EMPTY) {
		const auto focus = dequeue(pQueue);
//		q.pop();
		const auto focus_step = focus.step;
		x = focus.x;
		y = focus.y;
		auto next = focus;
		uc_wallData = g_sysMap[y][x];

		if (((uc_wallData & 0x01) == 0x00) && (y != (MAP_Y_SIZE - 1))) {
			if (us_cmap[y + 1][x] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y + 1][x] = focus_step + 1;
				next.x = x;
				next.y = y + 1;
				enqueue(pQueue,next);
			}
		}
		if (((uc_wallData & 0x02) == 0x00) && (x != (MAP_X_SIZE - 1))) {
			if (us_cmap[y][x + 1] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y][x + 1] = focus_step + 1;
				next.x = x + 1;
				next.y = y;
				enqueue(pQueue, next);
			}
		}
		if (((uc_wallData & 0x04) == 0x00) && (y != 0)) {
			if (us_cmap[y - 1][x] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y - 1][x] = focus_step + 1;
				next.x = x;
				next.y = y - 1;
				enqueue(pQueue, next);
			}
		}
		if (((uc_wallData & 0x08) == 0x00) && (x != 0)) {
			if (us_cmap[y][x - 1] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y][x - 1] = focus_step + 1;
				next.x = x - 1;
				next.y = y;
				enqueue(pQueue, next);
			}
		}

	}

}

// *************************************************************************
//   �@�\		�F �}�E�X�̐i�s���������肷��
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PRIVATE void MAP_calcMouseDir(
	enMAP_SEARCH_TYPE	en_calcType,	///< [in] �v�Z���@
	enMAP_HEAD_DIR* p_head			///< [out] �i�s�����i�߂�l�j
) {
	UCHAR		uc_wall;				// �Ǐ��
	USHORT		us_base;				// �������D��x����l
	USHORT		us_new;
	enMAP_HEAD_DIR	en_tmpHead;

	/* �����v�Z */
	// ������MAP�@
	if (CONTOUR_SYSTEM == en_calcType) {
		// ���ӂ�4���ň�ԖړI�n�ɋ߂��ړ��������Z�o����B
		// �������A�ړ��ł����ԋ߂���Ԃ���������ꍇ�ɂ́A���̏��őI������B
		// �@���T�����,���i �A���T�����,���� �B���T�����,���i �C���T�����,����
		uc_wall = g_sysMap[my][mx];
		us_base = MAP_SMAP_MAX_PRI_VAL;					// 16[���]�~16[���]�~4[����]

		/* 4�������r */
		//	�k�����̋��̊m�F
		if ((uc_wall & 1) == 0) {
			us_new = us_cmap[my + 1][mx] * 4 + 4;
			if ((g_sysMap[my + 1][mx] & 0xf0) != 0xf0) us_new = us_new - 2;
			if (en_Head == NORTH) us_new = us_new - 1;
			if (us_new < us_base) {
				us_base = us_new;
				en_tmpHead = NORTH;
			}
		}
		//	�������̋��̊m�F
		if ((uc_wall & 2) == 0) {
			us_new = us_cmap[my][mx + 1] * 4 + 4;
			if ((g_sysMap[my][mx + 1] & 0xf0) != 0xf0) us_new = us_new - 2;
			if (en_Head == EAST) us_new = us_new - 1;
			if (us_new < us_base) {
				us_base = us_new;
				en_tmpHead = EAST;
			}
		}
		//	������̋��̊m�F
		if ((uc_wall & 4) == 0) {
			us_new = us_cmap[my - 1][mx] * 4 + 4;
			if ((g_sysMap[my - 1][mx] & 0xf0) != 0xf0) us_new = us_new - 2;
			if (en_Head == SOUTH) us_new = us_new - 1;
			if (us_new < us_base) {
				us_base = us_new;
				en_tmpHead = SOUTH;
			}
		}
		//	�������̋��̊m�F
		if ((uc_wall & 8) == 0) {
			us_new = us_cmap[my][mx - 1] * 4 + 4;
			if ((g_sysMap[my][mx - 1] & 0xf0) != 0xf0) us_new = us_new - 2;
			if (en_Head == WEST) us_new = us_new - 1;
			if (us_new < us_base) {
				us_base = us_new;
				en_tmpHead = WEST;
			}
		}

		*p_head = (enMAP_HEAD_DIR)((en_tmpHead - en_Head) & 3);		// �ړ�����
//		printf("�i�s����%d\n", p_head);
	}
	// ������@�w��Ȃ�
	else {
		*p_head = (enMAP_HEAD_DIR)0;
	}

}

// *************************************************************************
//   �@�\		�F �}�E�X�̍��W�ʒu���X�V����
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PRIVATE void MAP_refMousePos(
	enMAP_HEAD_DIR 			en_head			///< [in] �i�s����
) {
	switch (en_head) {
	case NORTH:
		my = my + 1;
		break;
	case EAST:
		mx = mx + 1;
		break;
	case SOUTH:
		my = my - 1;
		break;
	case WEST:
		mx = mx - 1;
		break;
	default:
		break;
	}

}

// *************************************************************************
//   �@�\		�F ���̋��Ɉړ�����
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PRIVATE void MAP_moveNextBlock(
	enMAP_HEAD_DIR 	en_head,		///< [in] ���ΐi�s�����i�}�E�X�i�s������k�Ƃ��Ă���j
	BOOL* p_type			///< [in] FALSE: �P��ԑO�i��ԁATURE:����ԑO�i���
) {
	*p_type = TRUE;
	f_MoveBackDist = 0;				// �ړ����������Z�l�N���A

	/* ���� */
	switch (en_head) {

		/* ���̂܂ܑO�i */
	case NORTH:
		*p_type = FALSE;
//		MOT_goBlock_Const(1);				// 1���O�i
		break;
		// �E�ɐ��񂷂�
	case EAST:
//		MOT_goBlock_FinSpeed(0.5, 0);		// �����O�i
//			TIME_wait( MAP_TURN_WAIT );
//		MOT_turn(MOT_R90);									// �E90�x����
//			TIME_wait( MAP_TURN_WAIT );
		break;
		// ���ɐ��񂷂�
	case WEST:
//		MOT_goBlock_FinSpeed(0.5, 0);		// �����O�i
//			TIME_wait( MAP_TURN_WAIT );
//		MOT_turn(MOT_L90);									// �E90�x����
//			TIME_wait( MAP_TURN_WAIT );
		break;
		// ���]���Ė߂�
	case SOUTH:
//		MOT_goBlock_FinSpeed(0.5, 0);		// �����O�i
//			TIME_wait( MAP_TURN_WAIT );
//		MOT_turn(MOT_R180);									// �E180�x����
//			TIME_wait( MAP_TURN_WAIT );

			/* �Ǔ��Ďp������i���ɕǂ���������o�b�N�{�ړ����������Z����j */
		if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// �k�������Ă��Ėk�ɕǂ�����
			((en_Head == EAST) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// ���������Ă��ē��ɕǂ�����
			((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// ��������Ă��ē�ɕǂ�����
			((en_Head == WEST) && ((g_sysMap[my][mx] & 0x08) != 0)) 			// ���������Ă��Đ��ɕǂ�����
			) {
//			MOT_goHitBackWall();					// �o�b�N����
			f_MoveBackDist = MOVE_BACK_DIST;		// �o�b�N�������̈ړ�����[���]�����Z
//				TIME_wait( MAP_TURN_WAIT );				// ���ԑ҂�
		}
		break;
	default:
		break;
	}

	/* �O�i���Ƀp���[�����[�X�@�\�������ă��W���[�����Ȃ���΂Ȃ�Ȃ� */
/*	if( ( TRUE == DCMC_isPowerRelease() ) && ( en_head == NORTH ) ){

		MOT_goBack_Const( MOT_BACK_POLE );					// �P�O�̒��܂Ō��
		MAP_makeMapData();									// �ǃf�[�^������H�f�[�^���쐬			�� �����Ńf�[�^�쐬���~�X���Ă���
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// ������MAP�@�Ői�s�������Z�o			�� �����MAP���쐬
		MAP_moveNextBlock(en_head, p_type);					// �����P�x�Ăяo���i���̋��ֈړ��j
	}
	else{*/
	/* �i�s�����X�V */
	en_Head = (enMAP_HEAD_DIR)((en_Head + en_head) & (MAP_HEAD_DIR_MAX - 1));
	//	}

}

// *************************************************************************
//   �@�\		�F �X�����[���ɂĎ��̋��Ɉړ�����
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PRIVATE void MAP_moveNextBlock_Sura(
	enMAP_HEAD_DIR 	en_head,		///< [in] ���ΐi�s�����i�}�E�X�i�s������k�Ƃ��Ă���j
	BOOL* p_type,			///< [in] FALSE: �P��ԑO�i��ԁATURE:����ԑO�i���
	BOOL			bl_resume		///< [in] FALSE: ���W���[������ł͂Ȃ��ATURE:���W���[������
) {
	*p_type = FALSE;
	f_MoveBackDist = 0;				// �ړ����������Z�l�N���A

	/* ���� */
	switch (en_head) {

		// ���̂܂ܑO�i
	case NORTH:

		/* ���W���[������ł͂Ȃ� */
		if (bl_resume == FALSE) {

//			MOT_goBlock_Const(1);					// 1���O�i
			uc_SlaCnt = 0;							// �X�����[�����Ă��Ȃ�
		}
		/* ���W���[������ */
		else {
//			MOT_goBlock_FinSpeed(1.0f, SEARCH_SPEED);		// �����O�i(�o�b�N�̈ړ��ʂ��܂�)
			uc_SlaCnt = 0;										// �X�����[�����Ă��Ȃ�
		}
		break;

		// �E�ɃX�����[������
	case EAST:
		if (uc_SlaCnt < 7) {
//			MOT_goSla(MOT_R90S, PARAM_getSra(SLA_90));	// �E�X�����[��
			uc_SlaCnt++;
		}
		else {
			/* �Ǔ��Ďp������i���ɕǂ���������o�b�N�{�ړ����������Z����j */
			if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x08) != 0)) ||		// �k�������Ă��Đ��ɕǂ�����
				((en_Head == EAST) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// ���������Ă��Ėk�ɕǂ�����
				((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// ��������Ă��ē��ɕǂ�����
				((en_Head == WEST) && ((g_sysMap[my][mx] & 0x04) != 0)) 			// ���������Ă��ē�ɕǂ�����
				) {
//				MOT_goBlock_FinSpeed(0.5, 0);			// �����O�i
//					TIME_wait( MAP_TURN_WAIT );
//				MOT_turn(MOT_R90);						// �E90�x����
//					TIME_wait( MAP_TURN_WAIT );
				uc_SlaCnt = 0;
//				MOT_goHitBackWall();					// �o�b�N����
				f_MoveBackDist = MOVE_BACK_DIST;		// �o�b�N�������̈ړ�����[���]�����Z
//					TIME_wait( MAP_SLA_WAIT );				// ���ԑ҂�
				*p_type = TRUE;							// ���͔���ԁi�{�o�b�N�j���i�߂�
			}
			else {
//				MOT_goSla(MOT_R90S, PARAM_getSra(SLA_90));	// �E�X�����[��
				uc_SlaCnt++;
			}
		}
		break;

		// ���ɃX�����[������
	case WEST:
		if (uc_SlaCnt < 7) {
//			MOT_goSla(MOT_L90S, PARAM_getSra(SLA_90));	// ���X�����[��
			uc_SlaCnt++;
		}
		else {
			/* �Ǔ��Ďp������i���ɕǂ���������o�b�N�{�ړ����������Z����j */
			if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// �k�������Ă��ē��ɕǂ�����
				((en_Head == EAST) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// ���������Ă��ē�ɕǂ�����
				((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x08) != 0)) ||		// ��������Ă��Đ��ɕǂ�����
				((en_Head == WEST) && ((g_sysMap[my][mx] & 0x01) != 0)) 			// ���������Ă��Ėk�ɕǂ�����
				) {
//				MOT_goBlock_FinSpeed(0.5, 0);		// �����O�i
//					TIME_wait( MAP_TURN_WAIT );
//				MOT_turn(MOT_L90);					// �E90�x����
//					TIME_wait( MAP_TURN_WAIT );
				uc_SlaCnt = 0;
//				MOT_goHitBackWall();					// �o�b�N����
				f_MoveBackDist = MOVE_BACK_DIST;		// �o�b�N�������̈ړ�����[���]�����Z
//					TIME_wait( MAP_SLA_WAIT );				// ���ԑ҂�
				*p_type = TRUE;							// ���͔���ԁi�{�o�b�N�j���i�߂�
			}
			else {
//				MOT_goSla(MOT_L90S, PARAM_getSra(SLA_90));	// ���X�����[��
				uc_SlaCnt++;
			}
		}
		break;

		// ���]���Ė߂�
	case SOUTH:
//		MOT_goBlock_FinSpeed(0.5, 0);			// �����O�i
//			TIME_wait( MAP_SLA_WAIT );
//		MOT_turn(MOT_R180);									// �E180�x����
//			TIME_wait( MAP_SLA_WAIT );
		uc_SlaCnt = 0;

		/* �Ǔ��Ďp������i���ɕǂ���������o�b�N�{�ړ����������Z����j */
		if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// �k�������Ă��Ėk�ɕǂ�����
			((en_Head == EAST) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// ���������Ă��ē��ɕǂ�����
			((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// ��������Ă��ē�ɕǂ�����
			((en_Head == WEST) && ((g_sysMap[my][mx] & 0x08) != 0)) 			// ���������Ă��Đ��ɕǂ�����
			) {
//			MOT_goHitBackWall();					// �o�b�N����
			f_MoveBackDist = MOVE_BACK_DIST;		// �o�b�N�������̈ړ�����[���]�����Z
//				TIME_wait( MAP_SLA_WAIT );				// ���ԑ҂�
		}
		*p_type = TRUE;								// ���͔���ԁ{�o�b�N���i�߂�
		break;

	default:
		break;
	}

	/* �O�i���Ƀp���[�����[�X�@�\�������ă��W���[�����Ȃ���΂Ȃ�Ȃ� */
/*	if( ( TRUE == DCMC_isPowerRelease() ) && ( en_head == NORTH ) ){

		TIME_wait(1000);
		MOT_goBack_Const( MOT_BACK_POLE );					// �P�O�̒��܂Ō��
		MAP_makeMapData();									// �ǃf�[�^������H�f�[�^���쐬			�� �����Ńf�[�^�쐬���~�X���Ă���
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// ������MAP�@�Ői�s�������Z�o			�� �����MAP���쐬
		MAP_moveNextBlock_Sura(en_head, p_type, TRUE );		// �����P�x�Ăяo���i���̋��ֈړ��j
	}
	else{*/
	/* �i�s�����X�V */
	en_Head = (enMAP_HEAD_DIR)((en_Head + en_head) & (MAP_HEAD_DIR_MAX - 1));
	//	}

}

// *************************************************************************
//   �@�\		�F �S�[�����̓���
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PRIVATE void MAP_actGoal(void)
{
//	MOT_goBlock_FinSpeed(0.5, 0);			// �����O�i
//	TIME_wait(500);
//	MOT_turn(MOT_R180);										// �E180�x����
//	TIME_wait(500);

	//	MAP_SaveMapData();						// ���H���̃o�b�N�A�b�v
//	log_flag_off();
//	MAP_actGoalLED();

	en_Head = (enMAP_HEAD_DIR)((en_Head + 2) & (MAP_HEAD_DIR_MAX - 1));			//	�i�s�����X�V

}

// *************************************************************************
//   �@�\		�F �S�[���̒T������
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2018.09.27			sato		�V�K
// *************************************************************************/
PUBLIC void MAP_searchGoal(
	UCHAR 			uc_trgX, 		///< [in] �ڕWx���W
	UCHAR 			uc_trgY, 		///< [in] �ڕWy���W 
	enMAP_ACT_MODE 	en_type, 		///< [in] �T�����@
	enSEARCH_MODE	en_search 		///< [in] �T�����@
) {
	enMAP_HEAD_DIR	en_head = NORTH;
	BOOL		bl_type = TRUE;			// ���݈ʒu�AFALSE: �P��ԑO�i��ԁATURE:����ԑO�i���

//	SYS_setDisable( SYS_MODE );				// ���[�h�ύX�֎~

//	MOT_setTrgtSpeed(SEARCH_SPEED);		// �ڕW���x
//	MOT_setNowSpeed(0.0f);
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;
	if (uc_trgX == GOAL_MAP_X && uc_trgY == GOAL_MAP_Y) {
		f_MoveBackDist = 0.3;
	}

//	log_flag_on();	//���O�֐��X�^�[�g�i���폜�j

	/* ���H�T�� */
	while (1) {
		MAP_refMousePos(en_Head);								// ���W�X�V
		MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// �������}�b�v�����

		/* ���M�n����T�� */
		if (SEARCH_TURN == en_search) {

			if (TRUE == bl_type) {
//				MOT_goBlock_FinSpeed(0.5 + f_MoveBackDist, SEARCH_SPEED);		// �����O�i(�o�b�N�̈ړ��ʂ��܂�)
			}
			MAP_makeMapData();												// �ǃf�[�^������H�f�[�^���쐬			�� �����Ńf�[�^�쐬���~�X���Ă���
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// ������MAP�@�Ői�s�������Z�o			�� �����MAP���쐬

			/* ���̋��ֈړ� */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
//				MAP_actGoal();										// �S�[�����̓���
				break;
			}
			else {
				MAP_moveNextBlock(en_head, &bl_type);				// ���̋��ֈړ�								�� �����ŉ��߂ă����[�X�`�F�b�N�{�Ǎēx�쐬�{�������{���M�n���񓮍�
			}
		}
		/* �X�����[���T�� */
		else if (SEARCH_SURA == en_search) {

			if (TRUE == bl_type) {

//				MOT_goBlock_FinSpeed(0.5 + f_MoveBackDist, SEARCH_SPEED);		// �����O�i(�o�b�N�̈ړ��ʂ��܂�)
			}
			MAP_makeMapData();										// �ǃf�[�^������H�f�[�^���쐬			�� �����Ńf�[�^�쐬���~�X���Ă���
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// ������MAP�@�Ői�s�������Z�o			�� �����MAP���쐬

			/* ���̋��ֈړ� */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
//				MAP_actGoal();										// �S�[�����̓���
				break;
			}
			else {
				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE);	// ���̋��ֈړ�						�� �����ŉ��߂ă����[�X�`�F�b�N�{�Ǎēx�쐬�{�������{���M�n���񓮍�
			}
		}

		/* �r���Ő���s�\�ɂȂ��� */
//		if (SYS_isOutOfCtrl() == TRUE) {
//			CTRL_stop();
//			DCM_brakeMot(DCM_R);		// �u���[�L
//			DCM_brakeMot(DCM_L);		// �u���[�L

			/* ���H�֘A�������� */
//			en_Head = NORTH;
//			mx = 0;
//			my = 0;
//			f_MoveBackDist = 0;

			// DCMC�͉��ʃ��W���[���Ŋ��ɃN���A�Ƌً}��~���s���Ă���B
//			break;
//		}
	}

//	TIME_wait(1000);
	//	SYS_setEnable( SYS_MODE );				// ���[�h�ύX�L��

}



/*�V�~�����[�^�p�T���v���O����*/
PRIVATE void Simu_moveNextBlock(
	enMAP_HEAD_DIR 	en_head,		///< [in] ���ΐi�s�����i�}�E�X�i�s������k�Ƃ��Ă���j
	BOOL* p_type			///< [in] FALSE: �P��ԑO�i��ԁATURE:����ԑO�i���
) {


	/* ���� */
	switch (en_head) {

		/* ���̂܂ܑO�i */
	case NORTH:
		//		MOT_goBlock_Const(1);				// 1���O�i
		break;
		// �E�ɐ��񂷂�
	case EAST:
		//		MOT_goBlock_FinSpeed(0.5, 0);		// �����O�i
		//			TIME_wait( MAP_TURN_WAIT );
		//		MOT_turn(MOT_R90);									// �E90�x����
		//			TIME_wait( MAP_TURN_WAIT );
		break;
		// ���ɐ��񂷂�
	case WEST:
		//		MOT_goBlock_FinSpeed(0.5, 0);		// �����O�i
		//			TIME_wait( MAP_TURN_WAIT );
		//		MOT_turn(MOT_L90);									// �E90�x����
		//			TIME_wait( MAP_TURN_WAIT );
		break;
		// ���]���Ė߂�
	case SOUTH:
		//		MOT_goBlock_FinSpeed(0.5, 0);		// �����O�i
		//			TIME_wait( MAP_TURN_WAIT );
		//		MOT_turn(MOT_R180);									// �E180�x����
		//			TIME_wait( MAP_TURN_WAIT );
		
		break;
	default:
		break;
	}

	/* �i�s�����X�V */
	en_Head = (enMAP_HEAD_DIR)((en_Head + en_head) & (MAP_HEAD_DIR_MAX - 1));
	//	}

}


PRIVATE void Simu_makeMapData(void)
{
	UCHAR uc_wall;

	//	���s���̕Ǐ�����H���ɏ���
	if ((mx == 0) && (my == 0)) {
		uc_wall = 0xfe;
	}
	else {
		uc_wall = g_trgtMap[my][mx]|0xf0;
	}
	g_sysMap[my][mx] = uc_wall;

	//	�ׂ̋�Ԃ̂l�`�o�f�[�^���X�V����
	if (mx != (MAP_X_SIZE - 1)) {
		g_sysMap[my][mx + 1] = (g_sysMap[my][mx + 1] & 0x77) | 0x80 | ((uc_wall << 2) & 0x08);
	}
	if (mx != 0) {
		g_sysMap[my][mx - 1] = (g_sysMap[my][mx - 1] & 0xdd) | 0x20 | ((uc_wall >> 2) & 0x02);
	}
	if (my != (MAP_Y_SIZE - 1)) {
		g_sysMap[my + 1][mx] = (g_sysMap[my + 1][mx] & 0xbb) | 0x40 | ((uc_wall << 2) & 0x04);
	}
	if (my != 0) {
		g_sysMap[my - 1][mx] = (g_sysMap[my - 1][mx] & 0xee) | 0x10 | ((uc_wall >> 2) & 0x01);
	}

}

PUBLIC void MAP_Goalsize(int size)
{
	GOAL_SIZE= size;
	if (size = 4) {
		uc_max_x = uc_max_x + 1;
		uc_max_y = uc_max_y + 1;
	}
	else if (size = 9) {
		uc_max_x = uc_max_x + 2;
		uc_max_y = uc_max_y + 2;
	}
}

PUBLIC void  MAP_makeReturnContourMap(UCHAR uc_staX,UCHAR uc_staY)
{
	USHORT		x, y, i;		// ���[�v�ϐ�
	USHORT		uc_dase;		// ��l
	USHORT		uc_new;			// �V�l
	USHORT		uc_level;		// ������
	UCHAR		uc_wallData;	// �Ǐ��

	/* �������}�b�v������������ */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}
	/* �ڕW�n�_�̓�������0�ɐݒ� */
	us_cmap[0][0] = 0;

	/* �������}�b�v���쐬 */
	uc_dase = 0;
	do {
		uc_level = 0;
		uc_new = uc_dase + 1;
		for (y = 0; y < MAP_Y_SIZE; y++) {
			for (x = 0; x < MAP_X_SIZE; x++) {
				if ((us_cmap[uc_staY][uc_staX] != MAP_SMAP_MAX_VAL - 1) && (us_cmap[uc_staY][uc_staX] + 2 < uc_new))break;

				if (us_cmap[y][x] == uc_dase) {
					uc_wallData = g_sysMap[y][x];
					/* �T�����s */
	
						if (((uc_wallData & 0x01) == 0x00) && (y != (MAP_Y_SIZE - 1))) {
							if (us_cmap[y + 1][x] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y + 1][x] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x02) == 0x00) && (x != (MAP_X_SIZE - 1))) {
							if (us_cmap[y][x + 1] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y][x + 1] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x04) == 0x00) && (y != 0)) {
							if (us_cmap[y - 1][x] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y - 1][x] = uc_new;
								uc_level++;
							}
						}
						if (((uc_wallData & 0x08) == 0x00) && (x != 0)) {
							if (us_cmap[y][x - 1] == MAP_SMAP_MAX_VAL - 1) {
								us_cmap[y][x - 1] = uc_new;
								uc_level++;
							}
						}

				}
			}
		}
		uc_dase = uc_dase + 1;
	} while (uc_level != 0);

#if 0
	/* debug */
	for (x = 0; x < 4; x++) {

		for (y = 0; y < 4; y++) {
			us_Log[y][x][us_LogPt] = us_cmap[y][x];
		}
	}
	us_LogPt++;
#endif

}

//TKR�̊��m��Ԃ��ڐA
// *************************************************************************
//   �@�\		�F �i�ދ��������T���ς݂����T�����𔻒�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   �Ԃ�l		�F TRUE:�T���ς�	FALSE:���T��
// **************************    ��    ��    *******************************
// 		v1.0		2014.09.29			TKR			�V�K
// *************************************************************************/
PRIVATE BOOL MAP_KnownAcc(void) {

	BOOL	bl_acc = FALSE;
#if 0
	if ((g_sysMap[my][mx] & 0xf0) == 0xf0) {
		bl_acc = TRUE;
	}
	else {
		bl_acc = FALSE;
	}
#endif
#if 1
	switch (en_Head) {
	case NORTH:
		if ((g_sysMap[my + 1][mx] & 0xf1) == 0xf0) {
			bl_acc = TRUE;
		}

		break;

	case EAST:
		if ((g_sysMap[my][mx + 1] & 0xf2) == 0xf0) {
			bl_acc = TRUE;
		}
		break;

	case SOUTH:
		if ((g_sysMap[my - 1][mx] & 0xf4) == 0xf0) {
			bl_acc = TRUE;
		}
		break;

	case WEST:
		if ((g_sysMap[my][mx - 1] & 0xf8) == 0xf0) {
			bl_acc = TRUE;
		}
		break;

	default:
		break;
	}
#endif

	return	bl_acc;

}

// *************************************************************************
//   �@�\		�F ���̋��Ɉړ�����i���m��ԉ����j
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F ���ΐi�s�����i�}�E�X�i�s������k�Ƃ��Ă���j�A�O�i��ԁiFALSE: �P��ԑO�i��ԁATURE:����ԑO�i��ԁj
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2014.09.30			�O��			�V�K
// *************************************************************************/
PRIVATE void MAP_moveNextBlock_acc(enMAP_HEAD_DIR en_head, BOOL* p_type)
{
	*p_type = FALSE;
//	f_MoveBackDist = 0;

	/* ���� */
	switch (en_head) {

		/* ���̂܂ܑO�i */
	case NORTH:
//		*p_type = FALSE;
//		printf("NORTH\n");
		if (MAP_KnownAcc() == FALSE) {					// ���ɐi�ދ�悪���T���̂Ƃ�
			if (st_known.bl_Known == TRUE){
				if (st_known.uc_StrCnt < 2) {
	//				MOT_goBlock_Const(1);					// 1���̏ꍇ�͓����̂܂�
				}
				else {
	//				printf("NORTH\n");
	//				printf("StrCnt%d\n", st_known.uc_StrCnt);
	//				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	//				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ���m��ԉ�������Ƃ��̖ڕW���x	
	//				MOT_goBlock_FinSpeed((FLOAT)(st_known.uc_StrCnt - 1), MAP_SEARCH_SPEED);				// n���O�i
	//				MOT_setTrgtSpeed(MAP_SEARCH_SPEED);										// �ڕW���x���f�t�H���g�l�ɖ߂�
				}
			}
//			MOT_goBlock_Const(1);
			st_known.uc_StrCnt = 0;
			st_known.bl_Known = FALSE;

		}
		else {

	//		std::cout << "���m��ԉ�����\n";
			st_known.uc_StrCnt++;			// �ړ����̉��Z
			st_known.bl_Known = TRUE;
		}

		break;

		/* �E�ɐ��񂷂� */
	case EAST:
	//	printf("EAST\n");
		if (st_known.bl_Known == TRUE) {		// ������������
			if (st_known.uc_StrCnt < 2) {
//				MOT_goBlock_Const(1);					// 1���̏ꍇ�͓����̂܂�
			}
			else {
//				printf("EAST\n");
	//			printf("StrCnt%d\n", st_known.uc_StrCnt);
//				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ���m��ԉ�������Ƃ��̖ڕW���x	
//				MOT_goBlock_FinSpeed((FLOAT)(st_known.uc_StrCnt - 1), MAP_SEARCH_SPEED);				// n���O�i
//				MOT_setTrgtSpeed(MAP_SEARCH_SPEED);										// �ڕW���x���f�t�H���g�l�ɖ߂�
			}
			st_known.uc_StrCnt = 0;		/////////////////////////////////////////
			st_known.bl_Known = FALSE;
		}

/*		if (uc_SlaCnt < MAP_SLA_NUM_MAX) {
			MOT_goSla(MOT_R90S, PARAM_getSra(SLA_90));	// �E�X�����[��
			uc_SlaCnt++;
		}
		else {
			MOT_goBlock_FinSpeed(0.5, 0);			// �����O�i
			TIME_wait(MAP_TURN_WAIT);
			MOT_turn(MOT_R90);						// �E90�x����
			TIME_wait(MAP_TURN_WAIT);
			uc_SlaCnt = 0;
*/			/* �Ǔ��Ďp������i���ɕǂ���������o�b�N�{�ړ����������Z����j */
/*			if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x08) != 0)) ||		// �k�������Ă��Đ��ɕǂ�����
				((en_Head == EAST) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// ���������Ă��Ėk�ɕǂ�����
				((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// ��������Ă��ē��ɕǂ�����
				((en_Head == WEST) && ((g_sysMap[my][mx] & 0x04) != 0)) 			// ���������Ă��ē�ɕǂ�����
				) {
				MOT_goHitBackWall();					// �o�b�N����
				f_MoveBackDist = MOVE_BACK_DIST_SURA;	// �o�b�N�������̈ړ�����[���]�����Z
				TIME_wait(MAP_SLA_WAIT);				// ���ԑ҂�
			}
			*p_type = TRUE;							// ���͔���ԁi�{�o�b�N�j���i�߂�
		}
*/		break;

		/* ���ɐ��񂷂� */
	case WEST:
	//	printf("WEST\n");
		if (st_known.bl_Known == TRUE) {		// ������������
			if (st_known.uc_StrCnt < 2) {
//				MOT_goBlock_Const(1);					// 1���̏ꍇ�͓����̂܂�
			}
			else {
//				printf("WEST\n");
	//			printf("StrCnt%d\n", st_known.uc_StrCnt);
//				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// ���m��ԉ�������Ƃ��̖ڕW���x	
//				MOT_goBlock_FinSpeed((FLOAT)(st_known.uc_StrCnt - 1), MAP_SEARCH_SPEED);				// n���O�i
//				MOT_setTrgtSpeed(MAP_SEARCH_SPEED);										// �ڕW���x���f�t�H���g�l�ɖ߂�
			}
			st_known.uc_StrCnt = 0;			//////////////////////////////////////
			st_known.bl_Known = FALSE;
		}

/*		if (uc_SlaCnt < MAP_SLA_NUM_MAX) {
			MOT_goSla(MOT_L90S, PARAM_getSra(SLA_90));	// ���X�����[��
			uc_SlaCnt++;
		}
		else {

			MOT_goBlock_FinSpeed(0.5, 0);		// �����O�i
			TIME_wait(MAP_TURN_WAIT);
			MOT_turn(MOT_L90);					// ��90�x����
			TIME_wait(MAP_TURN_WAIT);
			uc_SlaCnt = 0;
*/			/* �Ǔ��Ďp������i���ɕǂ���������o�b�N�{�ړ����������Z����j */
/*			if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// �k�������Ă��ē��ɕǂ�����
				((en_Head == EAST) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// ���������Ă��ē�ɕǂ�����
				((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x08) != 0)) ||		// ��������Ă��Đ��ɕǂ�����
				((en_Head == WEST) && ((g_sysMap[my][mx] & 0x01) != 0)) 			// ���������Ă��Ėk�ɕǂ�����
				) {
				MOT_goHitBackWall();					// �o�b�N����
				f_MoveBackDist = MOVE_BACK_DIST_SURA;	// �o�b�N�������̈ړ�����[���]�����Z
				TIME_wait(MAP_SLA_WAIT);				// ���ԑ҂�
			}
			*p_type = TRUE;							// ���͔���ԁi�{�o�b�N�j���i�߂�
		}
*/		break;

		/* ���]���Ė߂� */
	case SOUTH:
//		printf("SOUTH\n");
//		MOT_goBlock_FinSpeed(0.5, 0);			// �����O�i
//		TIME_wait(MAP_SLA_WAIT);
//		MOT_turn(MOT_R180);									// �E180�x����
//		TIME_wait(MAP_SLA_WAIT);
//		uc_SlaCnt = 0;

		/* �Ǔ��Ďp������i���ɕǂ���������o�b�N�{�ړ����������Z����j */
/*		if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// �k�������Ă��Ėk�ɕǂ�����
			((en_Head == EAST) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// ���������Ă��ē��ɕǂ�����
			((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// ��������Ă��ē�ɕǂ�����
			((en_Head == WEST) && ((g_sysMap[my][mx] & 0x08) != 0)) 			// ���������Ă��Đ��ɕǂ�����
			) {
			MOT_goHitBackWall();					// �o�b�N����
			f_MoveBackDist = MOVE_BACK_DIST_SURA;	// �o�b�N�������̈ړ�����[���]�����Z
			TIME_wait(MAP_SLA_WAIT);				// ���ԑ҂�
		}
		*p_type = TRUE;								// ���͔���ԁ{�o�b�N���i�߂�
*/		break;

	default:
		break;
	}

#ifndef POWER_RELESASE
	/* �i�s�����X�V */
//	en_Head = (enMAP_HEAD_DIR)( (en_Head + en_head) & (MAP_HEAD_DIR_MAX-1) );
	en_Head = (enMAP_HEAD_DIR)(((UCHAR)en_Head + (UCHAR)en_head) & (MAP_HEAD_DIR_MAX - 1));
#else
	/* �O�i���Ƀp���[�����[�X�@�\�������ă��W���[�����Ȃ���΂Ȃ�Ȃ� */
	if ((TRUE == DCMC_isPowerRelease()) && (en_head == NORTH)) {

		MOT_goBack_Const(MOT_BACK_POLE);					// �P�O�̒��܂Ō��
		MAP_makeMapData();									// �ǃf�[�^������H�f�[�^���쐬			�� �����Ńf�[�^�쐬���~�X���Ă���
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// ������MAP�@�Ői�s�������Z�o			�� �����MAP���쐬
		MAP_moveNextBlock(en_head, p_type);					// �����P�x�Ăяo���i���̋��ֈړ��j
	}
	else {
		/* �i�s�����X�V */
		en_Head = (enMAP_HEAD_DIR)((en_Head + en_head) & (MAP_HEAD_DIR_MAX - 1));
	}
#endif
}

PUBLIC void Simu_searchGoal(
	UCHAR 			uc_trgX, 		///< [in] �ڕWx���W
	UCHAR 			uc_trgY, 		///< [in] �ڕWy���W 
	enMAP_ACT_MODE 	en_type, 		///< [in] �T�����@
	enSEARCH_MODE	en_search		///< [in] �T�����@
) {
	enMAP_HEAD_DIR	en_head = NORTH;
	BOOL		bl_type = TRUE;			// ���݈ʒu�AFALSE: �P��ԑO�i��ԁATURE:����ԑO�i���
	enMAP_HEAD_DIR		en_endDir;

	UCHAR uc_goalX;
	UCHAR uc_goalY;
	UCHAR uc_staX;
	UCHAR uc_staY;

	if (en_search == SEARCH_RETURN){
		uc_goalX = uc_trgX;
		uc_goalY = uc_trgY;
		uc_staX = mx;
		uc_staY = my;
		printf("mx%d,my%d\n", mx, my);
//		MAP_makeContourMap(uc_trgX, uc_trgY, en_type);
		MAP_makeContourMap_maltigoal(uc_trgX, uc_trgY, en_type);
		Simu_searchCmdList(uc_staX, uc_staY, en_Head, uc_goalX, uc_goalX, &en_endDir);
		uc_trgX = Return_X;
		uc_trgY = Return_Y;
		printf("goalx%d,goaly%d\n", Return_X, Return_Y);
		MAP_showcountLog();
	}



		/* ���H�T�� */
	while (1) {
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		LARGE_INTEGER start, end;
//		QueryPerformanceCounter(&start);

		MAP_refMousePos(en_Head);								// ���W�X�V
		printf("mx%d,my%d\n", mx, my);
//		MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// �������}�b�v�����
//		MAP_showcountLog();
//		maze_show_search(en_Head,mx,my);//map�\�L
		/* ���M�n����T�� */
		if (SEARCH_TURN == en_search) {
			MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// �������}�b�v�����
			MAP_showcountLog();
			//			MAP_makeMapData();												// �ǃf�[�^������H�f�[�^���쐬			�� �����Ńf�[�^�쐬���~�X���Ă���
			Simu_makeMapData();//�Ǘp�̃v���O�������쐬			
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// ������MAP�@�Ői�s�������Z�o			�� �����MAP���쐬

			/* ���̋��ֈړ� */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
				std::cout << "goal!!\n";
				MAP_actGoal();										// �S�[�����̓���
				break;
			}
			else {
				//				MAP_moveNextBlock(en_head, &bl_type);				// ���̋��ֈړ�								�� �����ŉ��߂ă����[�X�`�F�b�N�{�Ǎēx�쐬�{�������{���M�n���񓮍�
				Simu_moveNextBlock(en_head, &bl_type);//�ړ������̃v���O�������쐬
			}
		}
		/* �X�����[���T�� */
		else if (SEARCH_SURA == en_search) {
//			MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// �������}�b�v�����
			MAP_makeContourMap_maltigoal(uc_trgX, uc_trgY, en_type);
//			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
//			MAP_showcountLog();
			//			MAP_makeMapData();										// �ǃf�[�^������H�f�[�^���쐬			�� �����Ńf�[�^�쐬���~�X���Ă���
			if (st_known.bl_Known != TRUE) {
	//			MAP_makeMapData();		// �ǃf�[�^������H�f�[�^���쐬
				Simu_makeMapData();
	//			SPK_debug();
			}
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// ������MAP�@�Ői�s�������Z�o			�� �����MAP���쐬

			/* ���̋��ֈړ� */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
				std::cout << "goal!!\n";
				MAP_actGoal();										// �S�[�����̓���
				break;
			}
			else {
				//				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE);	// ���̋��ֈړ�						�� �����ŉ��߂ă����[�X�`�F�b�N�{�Ǎēx�쐬�{�������{���M�n���񓮍�
				Simu_moveNextBlock(en_head, &bl_type);//�ړ������̃v���O�������쐬

			}
		}
		/* �A�ҒT�� */
		else if (SEARCH_RETURN == en_search) {

//			MAP_makeMapData();										// �ǃf�[�^������H�f�[�^���쐬			�� �����Ńf�[�^�쐬���~�X���Ă���
			if (st_known.bl_Known != TRUE) {
	//			MAP_makeMapData();		// �ǃf�[�^������H�f�[�^���쐬
				Simu_makeMapData();
	//			SPK_debug();
			}
//			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			
			MAP_makeReturnContourMap(uc_staX, uc_staY);
			Simu_searchCmdList(uc_staX, uc_staY, en_Head, uc_goalX, uc_goalX, &en_endDir);
			uc_trgX = Return_X;
			uc_trgY = Return_Y;
			MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// �������}�b�v�����
//			MAP_makeContourMap_kai(uc_trgX, uc_trgY, en_type);
//			MAP_showcountLog();
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);

			/* ���̋��ֈړ� */
//			if ((us_cmap[my][mx] == 0) || ((g_sysMap[uc_trgY][uc_trgX] & 0xf0) == 0xf0)) {
				if ((mx == 0) && (my == 0)) {
					MAP_actGoal();
					break;
				}
//			}
			else {
				Simu_moveNextBlock(en_head, &bl_type);	// ���̋��ֈړ�						�� �����ŉ��߂ă����[�X�`�F�b�N�{�Ǎēx�쐬�{�������{���M�n���񓮍�

			}
		}

//		QueryPerformanceCounter(&end);

//		double time = static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
//		printf("time %lf[ms]\n", time);

		maze_show_search(en_Head, mx, my);//map�\�L
//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

// *************************************************************************
//   �@�\		�F �T���i���m��ԉ����j
//   ����		�F �Ȃ�
//   ����		�F �Ȃ�
//   ����		�F �ڕWx���W�A�ڕWy���W�A�T�����@
//   �Ԃ�l		�F �Ȃ�
// **************************    ��    ��    *******************************
// 		v1.0		2019.11.3			TKR			searchGoal�֐�����ڐA
// *************************************************************************/
PUBLIC void MAP_searchGoalKnown(UCHAR uc_trgX, UCHAR uc_trgY, enMAP_ACT_MODE en_type)
{
	enMAP_HEAD_DIR	en_head = NORTH;
	BOOL			bl_type = TRUE;			// ���݈ʒu�AFALSE: �P��ԑO�i��ԁATURE:����ԑO�i���
	int	c = 0;

//	MOT_setTrgtSpeed(MAP_SEARCH_SPEED);		// �ڕW���x
	//MOT_setNowSpeed(0.0f);

	/* �S�[�����W�ڎw���Ƃ��͐K���čl�� */
/*	if ((uc_trgX == GOAL_MAP_X) && (uc_trgY == GOAL_MAP_Y)) {
		f_MoveBackDist = MOVE_BACK_DIST;
	}
	else {
		f_MoveBackDist = 0;
	}

	uc_SlaCnt = 0;
*/
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	/* ���H�T�� */
	while (1) {
		MAP_refMousePos(en_Head);							// ���W�X�V
		MAP_makeContourMap(uc_trgX, uc_trgY, en_type);	// �������}�b�v�����

		/* �_�~�[�Ǒ}�� */
	/*
		if( (uc_trgX == GOAL_MAP_X) && (uc_trgY == GOAL_MAP_Y) ){
			g_sysMap[0][0]	= 0x01;
			g_sysMap[0][1]	= 0x04;
		}
	*/
		if (TRUE == bl_type) {
//			MOT_goBlock_FinSpeed(0.5 + f_MoveBackDist, MAP_SEARCH_SPEED);		// �����O�i(�o�b�N�̈ړ��ʂ��܂�)
//			f_MoveBackDist = 0;
		}

		// ���m��ԉ�������Ƃ��͎��s���Ȃ�
		if (st_known.bl_Known != TRUE) {
//			MAP_makeMapData();		// �ǃf�[�^������H�f�[�^���쐬
			Simu_makeMapData();
			c = 0;
//			SPK_debug();
		}
		else {
//			printf("count %d", c++);
		}

		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// ������MAP�@�Ői�s�������Z�o

		/* ���̋��ֈړ� */
		if ((mx == uc_trgX) && (my == uc_trgY)) {

			/* �_�~�[�Ǎ폜 */
		/*
			if( (uc_trgX == GOAL_MAP_X) && (uc_trgY == GOAL_MAP_Y) ){
				g_sysMap[0][0]	&= ~0x01;
				g_sysMap[0][1]	&= ~0x04;
			}
		*/
			MAP_actGoal();		// �S�[�����̓���
			return;				// �T���I��
		}
		else {
			MAP_moveNextBlock_acc(en_head, &bl_type);

		}
		maze_show_search(en_Head, mx, my);//map�\�L
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
//		printf("mx%d,my%d\n", mx, my);
	}
}

stPOSITION unknown_cell_data[1000];
uint16_t cell_count = 0;

void Search_unknown_cell(void){
	for(uint8_t y=0;y<MAP_Y_SIZE;y++){
		for(uint8_t x=0;x<MAP_X_SIZE;x++){
			if((g_sysMap[y][x] & 0xf0) != 0xf0){
				unknown_cell_data[cell_count].x = x;
				unknown_cell_data[cell_count].y = y;
				unknown_cell_data[cell_count].step = 0;
				cell_count++;
			}
		}
	}
	std::cout << "\ncell_count";
	std::cout << cell_count;
	std::cout <<"\n";
}

void Set_unknown_cell(){
	for(uint16_t i = 0;i<cell_count;i++){
		if(unknown_cell_data[i].step == 0){
			setStep(unknown_cell_data[i].x,unknown_cell_data[i].y,0);
		}
	}
}
void Delete_unknown_cell(uint8_t mx,uint8_t my){
	//step����t���ɗ��p
	for(uint16_t i = 0;i<cell_count;i++){
		if((unknown_cell_data[i].x == mx)&&(unknown_cell_data[i].y == my)){
			unknown_cell_data[i].step = 1;
		}
	}
}

void  MAP_makeContourMap_maltigoal_return(
	UCHAR uc_goalX, 			///< [in] �S�[��X���W
	UCHAR uc_goalY, 			///< [in] �S�[��Y���W
	enMAP_ACT_MODE	en_type		///< [in] �v�Z���@�i�܂����g�p�j
) {
	USHORT		x, y, i;		// ���[�v�ϐ�
	USHORT		uc_dase;		// ��l
	USHORT		uc_new;			// �V�l
	USHORT		uc_level;		// ������
	UCHAR		uc_wallData;	// �Ǐ��

	stPOSITION		st_pos;

	en_type = en_type;		// �R���p�C�����[�j���O����i������폜�j

	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);


	/* �������}�b�v������������ */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}
	Set_unknown_cell();
	MAP_showcountLog();

	/* �X�e�b�v�̍X�V�\��̃L���[ */
//	std::queue<stPOSITION> q;
//	QueryPerformanceCounter(&start);

	/* �ڕW�n�_�̓�������0�ɐݒ� */
//	setStep(uc_goalX, uc_goalY, 0);
	for(uint16_t i = 0;i<cell_count;i++){
		if(unknown_cell_data[i].step == 0){
			st_pos.x = unknown_cell_data[i].x;
			st_pos.y = unknown_cell_data[i].y;
			st_pos.step = 0;
			enqueue(pQueue,st_pos);
		}
	}

	/* �������}�b�v���쐬 */
	while (pQueue->flag != EMPTY) {
		const auto focus = dequeue(pQueue);
//		q.pop();
		const auto focus_step = focus.step;
		x = focus.x;
		y = focus.y;
		auto next = focus;
		uc_wallData = g_sysMap[y][x];

		if (((uc_wallData & 0x01) == 0x00) && (y != (MAP_Y_SIZE - 1))) {
			if (us_cmap[y + 1][x] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y + 1][x] = focus_step + 1;
				next.x = x;
				next.y = y + 1;
				enqueue(pQueue,next);
			}
		}
		if (((uc_wallData & 0x02) == 0x00) && (x != (MAP_X_SIZE - 1))) {
			if (us_cmap[y][x + 1] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y][x + 1] = focus_step + 1;
				next.x = x + 1;
				next.y = y;
				enqueue(pQueue, next);
			}
		}
		if (((uc_wallData & 0x04) == 0x00) && (y != 0)) {
			if (us_cmap[y - 1][x] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y - 1][x] = focus_step + 1;
				next.x = x;
				next.y = y - 1;
				enqueue(pQueue, next);
			}
		}
		if (((uc_wallData & 0x08) == 0x00) && (x != 0)) {
			if (us_cmap[y][x - 1] > focus_step + 1) {
				next.step = focus_step + 1;
				us_cmap[y][x - 1] = focus_step + 1;
				next.x = x - 1;
				next.y = y;
				enqueue(pQueue, next);
			}
		}

	}

}


PUBLIC void Simu_searchGoal_all(
	UCHAR 			uc_trgX, 		///< [in] �ڕWx���W
	UCHAR 			uc_trgY, 		///< [in] �ڕWy���W 
	enMAP_ACT_MODE 	en_type, 		///< [in] �T�����@
	enSEARCH_MODE	en_search		///< [in] �T�����@
) {
	enMAP_HEAD_DIR	en_head = NORTH;
	BOOL		bl_type = TRUE;			// ���݈ʒu�AFALSE: �P��ԑO�i��ԁATURE:����ԑO�i���
	enMAP_HEAD_DIR		en_endDir;

	UCHAR uc_goalX;
	UCHAR uc_goalY;
	UCHAR uc_staX;
	UCHAR uc_staY;

		/* ���H�T�� */
	while (1) {
		MAP_refMousePos(en_Head);								// ���W�X�V
		printf("mx%d,my%d\n", mx, my);
//		MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// �������}�b�v�����
//		MAP_showcountLog();
//		maze_show_search(en_Head,mx,my);//map�\�L
		/* ���M�n����T�� */
		if (SEARCH_TURN == en_search) {
			MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// �������}�b�v�����
			MAP_showcountLog();
			//			MAP_makeMapData();												// �ǃf�[�^������H�f�[�^���쐬			�� �����Ńf�[�^�쐬���~�X���Ă���
			Simu_makeMapData();//�Ǘp�̃v���O�������쐬			
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// ������MAP�@�Ői�s�������Z�o			�� �����MAP���쐬

			/* ���̋��ֈړ� */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
				std::cout << "goal!!\n";
				MAP_actGoal();										// �S�[�����̓���
				break;
			}
			else {
				//				MAP_moveNextBlock(en_head, &bl_type);				// ���̋��ֈړ�								�� �����ŉ��߂ă����[�X�`�F�b�N�{�Ǎēx�쐬�{�������{���M�n���񓮍�
				Simu_moveNextBlock(en_head, &bl_type);//�ړ������̃v���O�������쐬
			}
		}
		/* �X�����[���T�� */
		else if (SEARCH_SURA == en_search) {
//			MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// �������}�b�v�����
			Delete_unknown_cell(mx,my);
			MAP_makeContourMap_maltigoal_return(uc_trgX, uc_trgY, en_type);
//			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
//			MAP_showcountLog();
			//			MAP_makeMapData();										// �ǃf�[�^������H�f�[�^���쐬			�� �����Ńf�[�^�쐬���~�X���Ă���
			if (st_known.bl_Known != TRUE) {
	//			MAP_makeMapData();		// �ǃf�[�^������H�f�[�^���쐬
				Simu_makeMapData();
			}
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// ������MAP�@�Ői�s�������Z�o			�� �����MAP���쐬

			/* ���̋��ֈړ� */
//			if ((us_cmap[my][mx] == 0) || ((g_sysMap[uc_trgY][uc_trgX] & 0xf0) == 0xf0)) {
				if ((mx == 0) && (my == 0)) {
					MAP_actGoal();
					break;
				}
//			}
			else {
				//				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE);	// ���̋��ֈړ�						�� �����ŉ��߂ă����[�X�`�F�b�N�{�Ǎēx�쐬�{�������{���M�n���񓮍�
				Simu_moveNextBlock(en_head, &bl_type);//�ړ������̃v���O�������쐬

			}
		}


		maze_show_search(en_Head, mx, my);//map�\�L
		MAP_showcountLog();
//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


PUBLIC void Simu_searchGoal_dijkstra(
	UCHAR 			uc_trgX, 		///< [in] �ڕWx���W
	UCHAR 			uc_trgY, 		///< [in] �ڕWy���W 
	enMAP_ACT_MODE 	en_type, 		///< [in] �T�����@
	enSEARCH_MODE	en_search		///< [in] �T�����@
) {
	enMAP_HEAD_DIR	en_head = NORTH;
	BOOL		bl_type = TRUE;			// ���݈ʒu�AFALSE: �P��ԑO�i��ԁATURE:����ԑO�i���
	enMAP_HEAD_DIR		en_endDir;

	UCHAR uc_goalX;
	UCHAR uc_goalY;
	UCHAR uc_staX;
	UCHAR uc_staY;

		/* ���H�T�� */
	while (1) {
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		LARGE_INTEGER start, end;

		MAP_refMousePos(en_Head);								// ���W�X�V
		printf("mx%d,my%d\n", mx, my);

		/* �X�����[���T�� */
		if (SEARCH_SURA == en_search) {
			MAP_makeContourMap_maltigoal(uc_trgX, uc_trgY, en_type);
			if (st_known.bl_Known != TRUE) {
				Simu_makeMapData();
			}
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// ������MAP�@�Ői�s�������Z�o			�� �����MAP���쐬

			/* ���̋��ֈړ� */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
				std::cout << "goal!!\n";
				MAP_actGoal();										// �S�[�����̓���
				break;
			}
			else {
//				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE);	// ���̋��ֈړ�						�� �����ŉ��߂ă����[�X�`�F�b�N�{�Ǎēx�쐬�{�������{���M�n���񓮍�
				Simu_moveNextBlock(en_head, &bl_type);//�ړ������̃v���O�������쐬

			}
		}

		maze_show_search(en_Head, mx, my);//map�\�L
//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}