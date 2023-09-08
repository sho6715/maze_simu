// *************************************************************************
//   ロボット名	： 電研ベーシックDCマウス、サンシャイン
//   概要		： サンシャインのsearch2ファイル
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2017.09.27			sato		新規（ファイルのインクルード）
// *************************************************************************/


//**************************************************
// インクルードファイル（include）
//**************************************************
#include "search.h"
#include "queue.h"

#include <chrono>
#include <thread>
//#include <queue>		//キューを使うようにするため

//**************************************************
// 定義（define）
//**************************************************
#define MAP_SMAP_MAX_VAL	( MAP_X_SIZE * MAP_Y_SIZE ) 			///< 等高線mapの最大値
#define MAP_SMAP_MAX_PRI_VAL	( MAP_SMAP_MAX_VAL * 4 )				///< 等高線mapの優先度最大値

#define MOVE_BACK_DIST		(0.3f)

//**************************************************
// 列挙体（enum）
//**************************************************

//**************************************************
// 構造体（struct）
//**************************************************
#define MAP_SMAP_MAX_VAL		( MAP_X_SIZE * MAP_Y_SIZE ) 			///< 等高線mapの最大値
#define MAP_SMAP_MAX_PRI_VAL	( MAP_SMAP_MAX_VAL * 4 )				///< 等高線mapの優先度最大値


//**************************************************
// グローバル変数
//**************************************************
PRIVATE enMAP_HEAD_DIR	en_Head;										///< マウスの進行方向 0:N 1:E 2:S 3:W
PRIVATE UCHAR		my;												///< マウスのＸ座標
PRIVATE UCHAR		mx;												///< マウスのＹ座標
PUBLIC USHORT		us_cmap[MAP_Y_SIZE][MAP_X_SIZE];				///< 等高線 データ
PUBLIC UCHAR		g_sysMap[MAP_Y_SIZE][MAP_X_SIZE];			///< 迷路情報
PUBLIC UCHAR		g_Map_direction[MAP_Y_SIZE][MAP_X_SIZE];			///< 迷路情報
PRIVATE FLOAT		f_MoveBackDist;									///< 壁当て動作で後退した距離[区画]
PRIVATE UCHAR		uc_SlaCnt = 0;									// スラローム連続回数
PRIVATE UCHAR		uc_back[MAP_Y_SIZE][MAP_X_SIZE];			// 迷路データ

PUBLIC UCHAR		GOAL_MAP_X;					//ゴール座標変更プログラム用ｘ
PUBLIC UCHAR		GOAL_MAP_Y;					//ゴール座標変更プログラム用ｙ

PUBLIC UCHAR		GOAL_SIZE;


//等高線マップを更新を止めるための移動区画規定変数
PRIVATE UCHAR		uc_max_x = GOAL_MAP_X_def;
PRIVATE UCHAR		uc_max_y = GOAL_MAP_Y_def;

//**************************************************
// プロトタイプ宣言（ファイル内で必要なものだけ記述）
//**************************************************
extern PUBLIC	UCHAR		dcom[];					// 超地信旋回用
extern PUBLIC	UCHAR		scom[];					// スラローム用
extern PUBLIC	UCHAR		tcom[];					// 斜め走行用

//TKR
/* 既知区間加速 */
typedef struct
{
	UCHAR	uc_StrCnt;
	BOOL	bl_Known;
}stMAP_KNOWN;

PRIVATE stMAP_KNOWN		st_known = { 0,FALSE };


// *************************************************************************
//   機能		： 迷路モジュールを初期化する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PUBLIC void MAP_init(void)
{
	UCHAR uc_dummy[MAP_Y_SIZE][MAP_X_SIZE];			// 迷路データ

	/* 座標、向き、迷路情報を初期化 */
	en_Head = NORTH;
	mx = 0;
	my = 0;
	MAP_clearMap();

	/* 走行用のパラメータ */
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;
}

// *************************************************************************
//   機能		： ゴール座標を初期化する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2019.08.05			sato		新規
// *************************************************************************/
PUBLIC void MAP_Goal_init(void)
{
	GOAL_MAP_X = GOAL_MAP_X_def;
	GOAL_MAP_Y = GOAL_MAP_Y_def;
}

// *************************************************************************
//   機能		： 迷路情報をクリアする
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PUBLIC void MAP_ClearMapData(void)
{
	/* 座標、向き、迷路情報を初期化 */
	en_Head = NORTH;
	mx = 0;
	my = 0;
	MAP_clearMap();

	/* 走行用のパラメータ */
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;


}

// *************************************************************************
//   機能		： 座標位置を設定する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PUBLIC void MAP_setPos(UCHAR uc_x, UCHAR uc_y, enMAP_HEAD_DIR en_dir)
{
	mx = uc_x;
	my = uc_y;
	en_Head = en_dir;

	MAP_setCmdPos(uc_x, uc_y, en_dir);//循環参照でバグ

}

// *************************************************************************
//   機能		： 迷路データを表示する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PUBLIC void MAP_showLog(void)
{
	SHORT	x, y;
	CHAR	c_data;

	/* ---------- */
	/*  通常迷路  */
	/* ---------- */
	printf("\n\r  /* ---------- */   ");
	printf("\n\r  /*  通常迷路  */   ");
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
//   機能		： 等高線マップを表示する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2019.11.13			sato		新規
// *************************************************************************/
PUBLIC void MAP_showcountLog(void)
{
	SHORT	x, y;
	USHORT	c_data;

	/* ---------- */
	/*  等高線迷路  */
	/* ---------- */
	printf("\n\r  /* ------------ */   ");
	printf("\n\r  /*  等高線迷路  */   ");
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
//   機能		： 迷路データをクリアする
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PUBLIC void MAP_clearMap(void)
{
	USHORT	x, y;
	UCHAR	uc_data;

	/* すべてのマップデータを未探索状態にする */
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
//   機能		： センサ情報から壁情報を取得する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PRIVATE UCHAR MAP_getWallData(void)
{
	UCHAR	 uc_wall;

	//	LED_offAll();			// debug

		// センサ情報から壁情報作成
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
	// マウスの進行方向にあわせてセンサデータを移動し壁データとする
	if (en_Head == EAST) {
		uc_wall = uc_wall >> 3;
	}
	else if (en_Head == SOUTH) {
		uc_wall = uc_wall >> 2;
	}
	else if (en_Head == WEST) {
		uc_wall = uc_wall >> 1;
	}

	//	探索済みフラグを立てる
	return (uc_wall | 0xf0);
}

// *************************************************************************
//   機能		： 壁情報を作成する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PRIVATE void MAP_makeMapData(void)
{
	UCHAR uc_wall;

	//	走行時の壁情報を迷路情報に書込
	if ((mx == 0) && (my == 0)) {
		uc_wall = 0xfe;
	}
	else {
		uc_wall = MAP_getWallData();
	}
	g_sysMap[my][mx] = uc_wall;

	//	隣の区間のＭＡＰデータも更新する
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
//   機能		： 等高線マップを作成する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PUBLIC void  MAP_makeContourMap(
	UCHAR uc_goalX, 			///< [in] ゴールX座標
	UCHAR uc_goalY, 			///< [in] ゴールY座標
	enMAP_ACT_MODE	en_type		///< [in] 計算方法（まだ未使用）
) {
	USHORT		x, y, i;		// ループ変数
	USHORT		uc_dase;		// 基準値
	USHORT		uc_new;			// 新値
	USHORT		uc_level;		// 等高線
	UCHAR		uc_wallData;	// 壁情報

	en_type = en_type;		// コンパイルワーニング回避（いずれ削除）

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER start, end;

	QueryPerformanceCounter(&start);

	/* 等高線マップを初期化する */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}
//	QueryPerformanceCounter(&start);
	/* 目標地点の等高線を0に設定 */
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

	/* 等高線マップを作成 */
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
					/* 探索走行 */
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
					/* 最短走行 */
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
	/* (x, y) がフィールド内か確認 */
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
	UCHAR uc_goalX, 			///< [in] ゴールX座標
	UCHAR uc_goalY, 			///< [in] ゴールY座標
	enMAP_ACT_MODE	en_type		///< [in] 計算方法（まだ未使用）
) {
	USHORT		x, y, i;		// ループ変数
	USHORT		uc_dase;		// 基準値
	USHORT		uc_new;			// 新値
	USHORT		uc_level;		// 等高線
	UCHAR		uc_wallData;	// 壁情報

	stPOSITION		st_pos;

	en_type = en_type;		// コンパイルワーニング回避（いずれ削除）

	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER start, end;
	QueryPerformanceCounter(&start);



	/* 等高線マップを初期化する */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}

	/* ステップの更新予約のキュー */
//	std::queue<stPOSITION> q;
//	QueryPerformanceCounter(&start);

	/* 目標地点の等高線を0に設定 */
	setStep(uc_goalX, uc_goalY, 0);
	st_pos.x = uc_goalX;
	st_pos.y = uc_goalY;
	st_pos.step = 0;

	enqueue(pQueue,st_pos);

	/* 等高線マップを作成 */
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

	/* すべてのマップデータを未探索状態にする */
	for (y = 0; y < MAP_Y_SIZE; y++) {
		for (x = 0; x < MAP_X_SIZE; x++) {
			uc_data = 0x00;
			g_Map_direction[y][x] = uc_data;
		}
	}

}

PUBLIC void  MAP_makeContourMap_dijkstra_modoki(
	UCHAR uc_goalX, 			///< [in] ゴールX座標
	UCHAR uc_goalY, 			///< [in] ゴールY座標
	enMAP_ACT_MODE	en_type		///< [in] 計算方法（まだ未使用）
) {
	USHORT		x, y, i;		// ループ変数
	USHORT		uc_dase;		// 基準値
	USHORT		uc_new;			// 新値
	USHORT		uc_level;		// 等高線
	UCHAR		uc_wallData;	// 壁情報

	en_type = en_type;		// コンパイルワーニング回避（いずれ削除）

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER start, end;

	QueryPerformanceCounter(&start);
	MAP_clearMap_direction();

	/* 等高線マップを初期化する */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL*4 - 1;
	}
//	QueryPerformanceCounter(&start);
	/* 目標地点の等高線を0に設定 */
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

	/* 等高線マップを作成 */
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
					/* 探索走行 */
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
					/* 最短走行 */
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
	UCHAR uc_goalX, 			///< [in] ゴールX座標
	UCHAR uc_goalY, 			///< [in] ゴールY座標
	enMAP_ACT_MODE	en_type		///< [in] 計算方法（まだ未使用）
) {
	USHORT		x, y, i;		// ループ変数
	USHORT		uc_dase;		// 基準値
	USHORT		uc_new;			// 新値
	USHORT		uc_level;		// 等高線
	UCHAR		uc_wallData;	// 壁情報

	stPOSITION		st_pos;

	en_type = en_type;		// コンパイルワーニング回避（いずれ削除）

	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);


	/* 等高線マップを初期化する */
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

	/* ステップの更新予約のキュー */
//	std::queue<stPOSITION> q;
//	QueryPerformanceCounter(&start);

	/* 目標地点の等高線を0に設定 */
//	setStep(uc_goalX, uc_goalY, 0);
	st_pos.x = uc_goalX;
	st_pos.y = uc_goalY;
	st_pos.step = 0;

	enqueue(pQueue,st_pos);

	/* 等高線マップを作成 */
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
//   機能		： マウスの進行方向を決定する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PRIVATE void MAP_calcMouseDir(
	enMAP_SEARCH_TYPE	en_calcType,	///< [in] 計算方法
	enMAP_HEAD_DIR* p_head			///< [out] 進行方向（戻り値）
) {
	UCHAR		uc_wall;				// 壁情報
	USHORT		us_base;				// 等高線優先度決定値
	USHORT		us_new;
	enMAP_HEAD_DIR	en_tmpHead;

	/* 方向計算 */
	// 等高線MAP法
	if (CONTOUR_SYSTEM == en_calcType) {
		// 周辺の4区画で一番目的地に近い移動方向を算出する。
		// ただし、移動できる一番近い区間が複数ある場合には、次の順で選択する。
		// ①未探索区間,直進 ②未探索区間,旋回 ③既探索区間,直進 ④既探索区間,旋回
		uc_wall = g_sysMap[my][mx];
		us_base = MAP_SMAP_MAX_PRI_VAL;					// 16[区画]×16[区画]×4[方向]

		/* 4方向を比較 */
		//	北方向の区画の確認
		if ((uc_wall & 1) == 0) {
			us_new = us_cmap[my + 1][mx] * 4 + 4;
			if ((g_sysMap[my + 1][mx] & 0xf0) != 0xf0) us_new = us_new - 2;
			if (en_Head == NORTH) us_new = us_new - 1;
			if (us_new < us_base) {
				us_base = us_new;
				en_tmpHead = NORTH;
			}
		}
		//	東方向の区画の確認
		if ((uc_wall & 2) == 0) {
			us_new = us_cmap[my][mx + 1] * 4 + 4;
			if ((g_sysMap[my][mx + 1] & 0xf0) != 0xf0) us_new = us_new - 2;
			if (en_Head == EAST) us_new = us_new - 1;
			if (us_new < us_base) {
				us_base = us_new;
				en_tmpHead = EAST;
			}
		}
		//	南方向の区画の確認
		if ((uc_wall & 4) == 0) {
			us_new = us_cmap[my - 1][mx] * 4 + 4;
			if ((g_sysMap[my - 1][mx] & 0xf0) != 0xf0) us_new = us_new - 2;
			if (en_Head == SOUTH) us_new = us_new - 1;
			if (us_new < us_base) {
				us_base = us_new;
				en_tmpHead = SOUTH;
			}
		}
		//	西方向の区画の確認
		if ((uc_wall & 8) == 0) {
			us_new = us_cmap[my][mx - 1] * 4 + 4;
			if ((g_sysMap[my][mx - 1] & 0xf0) != 0xf0) us_new = us_new - 2;
			if (en_Head == WEST) us_new = us_new - 1;
			if (us_new < us_base) {
				us_base = us_new;
				en_tmpHead = WEST;
			}
		}

		*p_head = (enMAP_HEAD_DIR)((en_tmpHead - en_Head) & 3);		// 移動方向
//		printf("進行方向%d\n", p_head);
	}
	// 制御方法指定なし
	else {
		*p_head = (enMAP_HEAD_DIR)0;
	}

}

// *************************************************************************
//   機能		： マウスの座標位置を更新する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PRIVATE void MAP_refMousePos(
	enMAP_HEAD_DIR 			en_head			///< [in] 進行方向
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
//   機能		： 次の区画に移動する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PRIVATE void MAP_moveNextBlock(
	enMAP_HEAD_DIR 	en_head,		///< [in] 相対進行方向（マウス進行方向を北としている）
	BOOL* p_type			///< [in] FALSE: １区間前進状態、TURE:半区間前進状態
) {
	*p_type = TRUE;
	f_MoveBackDist = 0;				// 移動距離を加算値クリア

	/* 動作 */
	switch (en_head) {

		/* そのまま前進 */
	case NORTH:
		*p_type = FALSE;
//		MOT_goBlock_Const(1);				// 1区画前進
		break;
		// 右に旋回する
	case EAST:
//		MOT_goBlock_FinSpeed(0.5, 0);		// 半区画前進
//			TIME_wait( MAP_TURN_WAIT );
//		MOT_turn(MOT_R90);									// 右90度旋回
//			TIME_wait( MAP_TURN_WAIT );
		break;
		// 左に旋回する
	case WEST:
//		MOT_goBlock_FinSpeed(0.5, 0);		// 半区画前進
//			TIME_wait( MAP_TURN_WAIT );
//		MOT_turn(MOT_L90);									// 右90度旋回
//			TIME_wait( MAP_TURN_WAIT );
		break;
		// 反転して戻る
	case SOUTH:
//		MOT_goBlock_FinSpeed(0.5, 0);		// 半区画前進
//			TIME_wait( MAP_TURN_WAIT );
//		MOT_turn(MOT_R180);									// 右180度旋回
//			TIME_wait( MAP_TURN_WAIT );

			/* 壁当て姿勢制御（後ろに壁があったらバック＋移動距離を加算する） */
		if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// 北を向いていて北に壁がある
			((en_Head == EAST) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// 東を向いていて東に壁がある
			((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// 南を向いていて南に壁がある
			((en_Head == WEST) && ((g_sysMap[my][mx] & 0x08) != 0)) 			// 西を向いていて西に壁がある
			) {
//			MOT_goHitBackWall();					// バックする
			f_MoveBackDist = MOVE_BACK_DIST;		// バックした分の移動距離[区画]を加算
//				TIME_wait( MAP_TURN_WAIT );				// 時間待ち
		}
		break;
	default:
		break;
	}

	/* 前進中にパワーリリース機能が働いてレジュームしなければならない */
/*	if( ( TRUE == DCMC_isPowerRelease() ) && ( en_head == NORTH ) ){

		MOT_goBack_Const( MOT_BACK_POLE );					// １つ前の柱まで後退
		MAP_makeMapData();									// 壁データから迷路データを作成			← ここでデータ作成をミスっている
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// 等高線MAP法で進行方向を算出			← 誤ったMAPを作成
		MAP_moveNextBlock(en_head, p_type);					// もう１度呼び出し（次の区画へ移動）
	}
	else{*/
	/* 進行方向更新 */
	en_Head = (enMAP_HEAD_DIR)((en_Head + en_head) & (MAP_HEAD_DIR_MAX - 1));
	//	}

}

// *************************************************************************
//   機能		： スラロームにて次の区画に移動する
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PRIVATE void MAP_moveNextBlock_Sura(
	enMAP_HEAD_DIR 	en_head,		///< [in] 相対進行方向（マウス進行方向を北としている）
	BOOL* p_type,			///< [in] FALSE: １区間前進状態、TURE:半区間前進状態
	BOOL			bl_resume		///< [in] FALSE: レジューム動作ではない、TURE:レジューム動作
) {
	*p_type = FALSE;
	f_MoveBackDist = 0;				// 移動距離を加算値クリア

	/* 動作 */
	switch (en_head) {

		// そのまま前進
	case NORTH:

		/* レジューム動作ではない */
		if (bl_resume == FALSE) {

//			MOT_goBlock_Const(1);					// 1区画前進
			uc_SlaCnt = 0;							// スラロームしていない
		}
		/* レジューム動作 */
		else {
//			MOT_goBlock_FinSpeed(1.0f, SEARCH_SPEED);		// 半区画前進(バックの移動量を含む)
			uc_SlaCnt = 0;										// スラロームしていない
		}
		break;

		// 右にスラロームする
	case EAST:
		if (uc_SlaCnt < 7) {
//			MOT_goSla(MOT_R90S, PARAM_getSra(SLA_90));	// 右スラローム
			uc_SlaCnt++;
		}
		else {
			/* 壁当て姿勢制御（左に壁があったらバック＋移動距離を加算する） */
			if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x08) != 0)) ||		// 北を向いていて西に壁がある
				((en_Head == EAST) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// 東を向いていて北に壁がある
				((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// 南を向いていて東に壁がある
				((en_Head == WEST) && ((g_sysMap[my][mx] & 0x04) != 0)) 			// 西を向いていて南に壁がある
				) {
//				MOT_goBlock_FinSpeed(0.5, 0);			// 半区画前進
//					TIME_wait( MAP_TURN_WAIT );
//				MOT_turn(MOT_R90);						// 右90度旋回
//					TIME_wait( MAP_TURN_WAIT );
				uc_SlaCnt = 0;
//				MOT_goHitBackWall();					// バックする
				f_MoveBackDist = MOVE_BACK_DIST;		// バックした分の移動距離[区画]を加算
//					TIME_wait( MAP_SLA_WAIT );				// 時間待ち
				*p_type = TRUE;							// 次は半区間（＋バック）分進める
			}
			else {
//				MOT_goSla(MOT_R90S, PARAM_getSra(SLA_90));	// 右スラローム
				uc_SlaCnt++;
			}
		}
		break;

		// 左にスラロームする
	case WEST:
		if (uc_SlaCnt < 7) {
//			MOT_goSla(MOT_L90S, PARAM_getSra(SLA_90));	// 左スラローム
			uc_SlaCnt++;
		}
		else {
			/* 壁当て姿勢制御（後ろに壁があったらバック＋移動距離を加算する） */
			if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// 北を向いていて東に壁がある
				((en_Head == EAST) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// 東を向いていて南に壁がある
				((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x08) != 0)) ||		// 南を向いていて西に壁がある
				((en_Head == WEST) && ((g_sysMap[my][mx] & 0x01) != 0)) 			// 西を向いていて北に壁がある
				) {
//				MOT_goBlock_FinSpeed(0.5, 0);		// 半区画前進
//					TIME_wait( MAP_TURN_WAIT );
//				MOT_turn(MOT_L90);					// 右90度旋回
//					TIME_wait( MAP_TURN_WAIT );
				uc_SlaCnt = 0;
//				MOT_goHitBackWall();					// バックする
				f_MoveBackDist = MOVE_BACK_DIST;		// バックした分の移動距離[区画]を加算
//					TIME_wait( MAP_SLA_WAIT );				// 時間待ち
				*p_type = TRUE;							// 次は半区間（＋バック）分進める
			}
			else {
//				MOT_goSla(MOT_L90S, PARAM_getSra(SLA_90));	// 左スラローム
				uc_SlaCnt++;
			}
		}
		break;

		// 反転して戻る
	case SOUTH:
//		MOT_goBlock_FinSpeed(0.5, 0);			// 半区画前進
//			TIME_wait( MAP_SLA_WAIT );
//		MOT_turn(MOT_R180);									// 右180度旋回
//			TIME_wait( MAP_SLA_WAIT );
		uc_SlaCnt = 0;

		/* 壁当て姿勢制御（後ろに壁があったらバック＋移動距離を加算する） */
		if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// 北を向いていて北に壁がある
			((en_Head == EAST) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// 東を向いていて東に壁がある
			((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// 南を向いていて南に壁がある
			((en_Head == WEST) && ((g_sysMap[my][mx] & 0x08) != 0)) 			// 西を向いていて西に壁がある
			) {
//			MOT_goHitBackWall();					// バックする
			f_MoveBackDist = MOVE_BACK_DIST;		// バックした分の移動距離[区画]を加算
//				TIME_wait( MAP_SLA_WAIT );				// 時間待ち
		}
		*p_type = TRUE;								// 次は半区間＋バック分進める
		break;

	default:
		break;
	}

	/* 前進中にパワーリリース機能が働いてレジュームしなければならない */
/*	if( ( TRUE == DCMC_isPowerRelease() ) && ( en_head == NORTH ) ){

		TIME_wait(1000);
		MOT_goBack_Const( MOT_BACK_POLE );					// １つ前の柱まで後退
		MAP_makeMapData();									// 壁データから迷路データを作成			← ここでデータ作成をミスっている
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// 等高線MAP法で進行方向を算出			← 誤ったMAPを作成
		MAP_moveNextBlock_Sura(en_head, p_type, TRUE );		// もう１度呼び出し（次の区画へ移動）
	}
	else{*/
	/* 進行方向更新 */
	en_Head = (enMAP_HEAD_DIR)((en_Head + en_head) & (MAP_HEAD_DIR_MAX - 1));
	//	}

}

// *************************************************************************
//   機能		： ゴール時の動作
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PRIVATE void MAP_actGoal(void)
{
//	MOT_goBlock_FinSpeed(0.5, 0);			// 半区画前進
//	TIME_wait(500);
//	MOT_turn(MOT_R180);										// 右180度旋回
//	TIME_wait(500);

	//	MAP_SaveMapData();						// 迷路情報のバックアップ
//	log_flag_off();
//	MAP_actGoalLED();

	en_Head = (enMAP_HEAD_DIR)((en_Head + 2) & (MAP_HEAD_DIR_MAX - 1));			//	進行方向更新

}

// *************************************************************************
//   機能		： ゴールの探索動作
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.09.27			sato		新規
// *************************************************************************/
PUBLIC void MAP_searchGoal(
	UCHAR 			uc_trgX, 		///< [in] 目標x座標
	UCHAR 			uc_trgY, 		///< [in] 目標y座標 
	enMAP_ACT_MODE 	en_type, 		///< [in] 探索方法
	enSEARCH_MODE	en_search 		///< [in] 探索方法
) {
	enMAP_HEAD_DIR	en_head = NORTH;
	BOOL		bl_type = TRUE;			// 現在位置、FALSE: １区間前進状態、TURE:半区間前進状態

//	SYS_setDisable( SYS_MODE );				// モード変更禁止

//	MOT_setTrgtSpeed(SEARCH_SPEED);		// 目標速度
//	MOT_setNowSpeed(0.0f);
	f_MoveBackDist = 0;
	uc_SlaCnt = 0;
	if (uc_trgX == GOAL_MAP_X && uc_trgY == GOAL_MAP_Y) {
		f_MoveBackDist = 0.3;
	}

//	log_flag_on();	//ログ関数スタート（大会時削除）

	/* 迷路探索 */
	while (1) {
		MAP_refMousePos(en_Head);								// 座標更新
		MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// 等高線マップを作る

		/* 超信地旋回探索 */
		if (SEARCH_TURN == en_search) {

			if (TRUE == bl_type) {
//				MOT_goBlock_FinSpeed(0.5 + f_MoveBackDist, SEARCH_SPEED);		// 半区画前進(バックの移動量を含む)
			}
			MAP_makeMapData();												// 壁データから迷路データを作成			← ここでデータ作成をミスっている
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// 等高線MAP法で進行方向を算出			← 誤ったMAPを作成

			/* 次の区画へ移動 */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
//				MAP_actGoal();										// ゴール時の動作
				break;
			}
			else {
				MAP_moveNextBlock(en_head, &bl_type);				// 次の区画へ移動								← ここで改めてリリースチェック＋壁再度作成＋等高線＋超信地旋回動作
			}
		}
		/* スラローム探索 */
		else if (SEARCH_SURA == en_search) {

			if (TRUE == bl_type) {

//				MOT_goBlock_FinSpeed(0.5 + f_MoveBackDist, SEARCH_SPEED);		// 半区画前進(バックの移動量を含む)
			}
			MAP_makeMapData();										// 壁データから迷路データを作成			← ここでデータ作成をミスっている
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// 等高線MAP法で進行方向を算出			← 誤ったMAPを作成

			/* 次の区画へ移動 */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
//				MAP_actGoal();										// ゴール時の動作
				break;
			}
			else {
				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE);	// 次の区画へ移動						← ここで改めてリリースチェック＋壁再度作成＋等高線＋超信地旋回動作
			}
		}

		/* 途中で制御不能になった */
//		if (SYS_isOutOfCtrl() == TRUE) {
//			CTRL_stop();
//			DCM_brakeMot(DCM_R);		// ブレーキ
//			DCM_brakeMot(DCM_L);		// ブレーキ

			/* 迷路関連を初期化 */
//			en_Head = NORTH;
//			mx = 0;
//			my = 0;
//			f_MoveBackDist = 0;

			// DCMCは下位モジュールで既にクリアと緊急停止を行っている。
//			break;
//		}
	}

//	TIME_wait(1000);
	//	SYS_setEnable( SYS_MODE );				// モード変更有効

}



/*シミュレータ用探索プログラム*/
PRIVATE void Simu_moveNextBlock(
	enMAP_HEAD_DIR 	en_head,		///< [in] 相対進行方向（マウス進行方向を北としている）
	BOOL* p_type			///< [in] FALSE: １区間前進状態、TURE:半区間前進状態
) {


	/* 動作 */
	switch (en_head) {

		/* そのまま前進 */
	case NORTH:
		//		MOT_goBlock_Const(1);				// 1区画前進
		break;
		// 右に旋回する
	case EAST:
		//		MOT_goBlock_FinSpeed(0.5, 0);		// 半区画前進
		//			TIME_wait( MAP_TURN_WAIT );
		//		MOT_turn(MOT_R90);									// 右90度旋回
		//			TIME_wait( MAP_TURN_WAIT );
		break;
		// 左に旋回する
	case WEST:
		//		MOT_goBlock_FinSpeed(0.5, 0);		// 半区画前進
		//			TIME_wait( MAP_TURN_WAIT );
		//		MOT_turn(MOT_L90);									// 右90度旋回
		//			TIME_wait( MAP_TURN_WAIT );
		break;
		// 反転して戻る
	case SOUTH:
		//		MOT_goBlock_FinSpeed(0.5, 0);		// 半区画前進
		//			TIME_wait( MAP_TURN_WAIT );
		//		MOT_turn(MOT_R180);									// 右180度旋回
		//			TIME_wait( MAP_TURN_WAIT );
		
		break;
	default:
		break;
	}

	/* 進行方向更新 */
	en_Head = (enMAP_HEAD_DIR)((en_Head + en_head) & (MAP_HEAD_DIR_MAX - 1));
	//	}

}


PRIVATE void Simu_makeMapData(void)
{
	UCHAR uc_wall;

	//	走行時の壁情報を迷路情報に書込
	if ((mx == 0) && (my == 0)) {
		uc_wall = 0xfe;
	}
	else {
		uc_wall = g_trgtMap[my][mx]|0xf0;
	}
	g_sysMap[my][mx] = uc_wall;

	//	隣の区間のＭＡＰデータも更新する
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
	USHORT		x, y, i;		// ループ変数
	USHORT		uc_dase;		// 基準値
	USHORT		uc_new;			// 新値
	USHORT		uc_level;		// 等高線
	UCHAR		uc_wallData;	// 壁情報

	/* 等高線マップを初期化する */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}
	/* 目標地点の等高線を0に設定 */
	us_cmap[0][0] = 0;

	/* 等高線マップを作成 */
	uc_dase = 0;
	do {
		uc_level = 0;
		uc_new = uc_dase + 1;
		for (y = 0; y < MAP_Y_SIZE; y++) {
			for (x = 0; x < MAP_X_SIZE; x++) {
				if ((us_cmap[uc_staY][uc_staX] != MAP_SMAP_MAX_VAL - 1) && (us_cmap[uc_staY][uc_staX] + 2 < uc_new))break;

				if (us_cmap[y][x] == uc_dase) {
					uc_wallData = g_sysMap[y][x];
					/* 探索走行 */
	
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

//TKRの既知区間を移植
// *************************************************************************
//   機能		： 進む区画方向が探索済みか未探索かを判定
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： TRUE:探索済み	FALSE:未探索
// **************************    履    歴    *******************************
// 		v1.0		2014.09.29			TKR			新規
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
//   機能		： 次の区画に移動する（既知区間加速）
//   注意		： なし
//   メモ		： なし
//   引数		： 相対進行方向（マウス進行方向を北としている）、前進状態（FALSE: １区間前進状態、TURE:半区間前進状態）
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2014.09.30			外川			新規
// *************************************************************************/
PRIVATE void MAP_moveNextBlock_acc(enMAP_HEAD_DIR en_head, BOOL* p_type)
{
	*p_type = FALSE;
//	f_MoveBackDist = 0;

	/* 動作 */
	switch (en_head) {

		/* そのまま前進 */
	case NORTH:
//		*p_type = FALSE;
//		printf("NORTH\n");
		if (MAP_KnownAcc() == FALSE) {					// 次に進む区画が未探索のとき
			if (st_known.bl_Known == TRUE){
				if (st_known.uc_StrCnt < 2) {
	//				MOT_goBlock_Const(1);					// 1区画の場合は等速のまま
				}
				else {
	//				printf("NORTH\n");
	//				printf("StrCnt%d\n", st_known.uc_StrCnt);
	//				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	//				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// 既知区間加速するときの目標速度	
	//				MOT_goBlock_FinSpeed((FLOAT)(st_known.uc_StrCnt - 1), MAP_SEARCH_SPEED);				// n区画前進
	//				MOT_setTrgtSpeed(MAP_SEARCH_SPEED);										// 目標速度をデフォルト値に戻す
				}
			}
//			MOT_goBlock_Const(1);
			st_known.uc_StrCnt = 0;
			st_known.bl_Known = FALSE;

		}
		else {

	//		std::cout << "既知区間加速中\n";
			st_known.uc_StrCnt++;			// 移動区画の加算
			st_known.bl_Known = TRUE;
		}

		break;

		/* 右に旋回する */
	case EAST:
	//	printf("EAST\n");
		if (st_known.bl_Known == TRUE) {		// 直線分を消化
			if (st_known.uc_StrCnt < 2) {
//				MOT_goBlock_Const(1);					// 1区画の場合は等速のまま
			}
			else {
//				printf("EAST\n");
	//			printf("StrCnt%d\n", st_known.uc_StrCnt);
//				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// 既知区間加速するときの目標速度	
//				MOT_goBlock_FinSpeed((FLOAT)(st_known.uc_StrCnt - 1), MAP_SEARCH_SPEED);				// n区画前進
//				MOT_setTrgtSpeed(MAP_SEARCH_SPEED);										// 目標速度をデフォルト値に戻す
			}
			st_known.uc_StrCnt = 0;		/////////////////////////////////////////
			st_known.bl_Known = FALSE;
		}

/*		if (uc_SlaCnt < MAP_SLA_NUM_MAX) {
			MOT_goSla(MOT_R90S, PARAM_getSra(SLA_90));	// 右スラローム
			uc_SlaCnt++;
		}
		else {
			MOT_goBlock_FinSpeed(0.5, 0);			// 半区画前進
			TIME_wait(MAP_TURN_WAIT);
			MOT_turn(MOT_R90);						// 右90度旋回
			TIME_wait(MAP_TURN_WAIT);
			uc_SlaCnt = 0;
*/			/* 壁当て姿勢制御（左に壁があったらバック＋移動距離を加算する） */
/*			if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x08) != 0)) ||		// 北を向いていて西に壁がある
				((en_Head == EAST) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// 東を向いていて北に壁がある
				((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// 南を向いていて東に壁がある
				((en_Head == WEST) && ((g_sysMap[my][mx] & 0x04) != 0)) 			// 西を向いていて南に壁がある
				) {
				MOT_goHitBackWall();					// バックする
				f_MoveBackDist = MOVE_BACK_DIST_SURA;	// バックした分の移動距離[区画]を加算
				TIME_wait(MAP_SLA_WAIT);				// 時間待ち
			}
			*p_type = TRUE;							// 次は半区間（＋バック）分進める
		}
*/		break;

		/* 左に旋回する */
	case WEST:
	//	printf("WEST\n");
		if (st_known.bl_Known == TRUE) {		// 直線分を消化
			if (st_known.uc_StrCnt < 2) {
//				MOT_goBlock_Const(1);					// 1区画の場合は等速のまま
			}
			else {
//				printf("WEST\n");
	//			printf("StrCnt%d\n", st_known.uc_StrCnt);
//				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//				MOT_setTrgtSpeed(MAP_KNOWN_ACC_SPEED);									// 既知区間加速するときの目標速度	
//				MOT_goBlock_FinSpeed((FLOAT)(st_known.uc_StrCnt - 1), MAP_SEARCH_SPEED);				// n区画前進
//				MOT_setTrgtSpeed(MAP_SEARCH_SPEED);										// 目標速度をデフォルト値に戻す
			}
			st_known.uc_StrCnt = 0;			//////////////////////////////////////
			st_known.bl_Known = FALSE;
		}

/*		if (uc_SlaCnt < MAP_SLA_NUM_MAX) {
			MOT_goSla(MOT_L90S, PARAM_getSra(SLA_90));	// 左スラローム
			uc_SlaCnt++;
		}
		else {

			MOT_goBlock_FinSpeed(0.5, 0);		// 半区画前進
			TIME_wait(MAP_TURN_WAIT);
			MOT_turn(MOT_L90);					// 左90度旋回
			TIME_wait(MAP_TURN_WAIT);
			uc_SlaCnt = 0;
*/			/* 壁当て姿勢制御（後ろに壁があったらバック＋移動距離を加算する） */
/*			if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// 北を向いていて東に壁がある
				((en_Head == EAST) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// 東を向いていて南に壁がある
				((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x08) != 0)) ||		// 南を向いていて西に壁がある
				((en_Head == WEST) && ((g_sysMap[my][mx] & 0x01) != 0)) 			// 西を向いていて北に壁がある
				) {
				MOT_goHitBackWall();					// バックする
				f_MoveBackDist = MOVE_BACK_DIST_SURA;	// バックした分の移動距離[区画]を加算
				TIME_wait(MAP_SLA_WAIT);				// 時間待ち
			}
			*p_type = TRUE;							// 次は半区間（＋バック）分進める
		}
*/		break;

		/* 反転して戻る */
	case SOUTH:
//		printf("SOUTH\n");
//		MOT_goBlock_FinSpeed(0.5, 0);			// 半区画前進
//		TIME_wait(MAP_SLA_WAIT);
//		MOT_turn(MOT_R180);									// 右180度旋回
//		TIME_wait(MAP_SLA_WAIT);
//		uc_SlaCnt = 0;

		/* 壁当て姿勢制御（後ろに壁があったらバック＋移動距離を加算する） */
/*		if (((en_Head == NORTH) && ((g_sysMap[my][mx] & 0x01) != 0)) ||		// 北を向いていて北に壁がある
			((en_Head == EAST) && ((g_sysMap[my][mx] & 0x02) != 0)) ||		// 東を向いていて東に壁がある
			((en_Head == SOUTH) && ((g_sysMap[my][mx] & 0x04) != 0)) ||		// 南を向いていて南に壁がある
			((en_Head == WEST) && ((g_sysMap[my][mx] & 0x08) != 0)) 			// 西を向いていて西に壁がある
			) {
			MOT_goHitBackWall();					// バックする
			f_MoveBackDist = MOVE_BACK_DIST_SURA;	// バックした分の移動距離[区画]を加算
			TIME_wait(MAP_SLA_WAIT);				// 時間待ち
		}
		*p_type = TRUE;								// 次は半区間＋バック分進める
*/		break;

	default:
		break;
	}

#ifndef POWER_RELESASE
	/* 進行方向更新 */
//	en_Head = (enMAP_HEAD_DIR)( (en_Head + en_head) & (MAP_HEAD_DIR_MAX-1) );
	en_Head = (enMAP_HEAD_DIR)(((UCHAR)en_Head + (UCHAR)en_head) & (MAP_HEAD_DIR_MAX - 1));
#else
	/* 前進中にパワーリリース機能が働いてレジュームしなければならない */
	if ((TRUE == DCMC_isPowerRelease()) && (en_head == NORTH)) {

		MOT_goBack_Const(MOT_BACK_POLE);					// １つ前の柱まで後退
		MAP_makeMapData();									// 壁データから迷路データを作成			← ここでデータ作成をミスっている
		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// 等高線MAP法で進行方向を算出			← 誤ったMAPを作成
		MAP_moveNextBlock(en_head, p_type);					// もう１度呼び出し（次の区画へ移動）
	}
	else {
		/* 進行方向更新 */
		en_Head = (enMAP_HEAD_DIR)((en_Head + en_head) & (MAP_HEAD_DIR_MAX - 1));
	}
#endif
}

PUBLIC void Simu_searchGoal(
	UCHAR 			uc_trgX, 		///< [in] 目標x座標
	UCHAR 			uc_trgY, 		///< [in] 目標y座標 
	enMAP_ACT_MODE 	en_type, 		///< [in] 探索方法
	enSEARCH_MODE	en_search		///< [in] 探索方法
) {
	enMAP_HEAD_DIR	en_head = NORTH;
	BOOL		bl_type = TRUE;			// 現在位置、FALSE: １区間前進状態、TURE:半区間前進状態
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



		/* 迷路探索 */
	while (1) {
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		LARGE_INTEGER start, end;
//		QueryPerformanceCounter(&start);

		MAP_refMousePos(en_Head);								// 座標更新
		printf("mx%d,my%d\n", mx, my);
//		MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// 等高線マップを作る
//		MAP_showcountLog();
//		maze_show_search(en_Head,mx,my);//map表記
		/* 超信地旋回探索 */
		if (SEARCH_TURN == en_search) {
			MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// 等高線マップを作る
			MAP_showcountLog();
			//			MAP_makeMapData();												// 壁データから迷路データを作成			← ここでデータ作成をミスっている
			Simu_makeMapData();//壁用のプログラムを作成			
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// 等高線MAP法で進行方向を算出			← 誤ったMAPを作成

			/* 次の区画へ移動 */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
				std::cout << "goal!!\n";
				MAP_actGoal();										// ゴール時の動作
				break;
			}
			else {
				//				MAP_moveNextBlock(en_head, &bl_type);				// 次の区画へ移動								← ここで改めてリリースチェック＋壁再度作成＋等高線＋超信地旋回動作
				Simu_moveNextBlock(en_head, &bl_type);//移動方向のプログラムを作成
			}
		}
		/* スラローム探索 */
		else if (SEARCH_SURA == en_search) {
//			MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// 等高線マップを作る
			MAP_makeContourMap_maltigoal(uc_trgX, uc_trgY, en_type);
//			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
//			MAP_showcountLog();
			//			MAP_makeMapData();										// 壁データから迷路データを作成			← ここでデータ作成をミスっている
			if (st_known.bl_Known != TRUE) {
	//			MAP_makeMapData();		// 壁データから迷路データを作成
				Simu_makeMapData();
	//			SPK_debug();
			}
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// 等高線MAP法で進行方向を算出			← 誤ったMAPを作成

			/* 次の区画へ移動 */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
				std::cout << "goal!!\n";
				MAP_actGoal();										// ゴール時の動作
				break;
			}
			else {
				//				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE);	// 次の区画へ移動						← ここで改めてリリースチェック＋壁再度作成＋等高線＋超信地旋回動作
				Simu_moveNextBlock(en_head, &bl_type);//移動方向のプログラムを作成

			}
		}
		/* 帰還探索 */
		else if (SEARCH_RETURN == en_search) {

//			MAP_makeMapData();										// 壁データから迷路データを作成			← ここでデータ作成をミスっている
			if (st_known.bl_Known != TRUE) {
	//			MAP_makeMapData();		// 壁データから迷路データを作成
				Simu_makeMapData();
	//			SPK_debug();
			}
//			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			
			MAP_makeReturnContourMap(uc_staX, uc_staY);
			Simu_searchCmdList(uc_staX, uc_staY, en_Head, uc_goalX, uc_goalX, &en_endDir);
			uc_trgX = Return_X;
			uc_trgY = Return_Y;
			MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// 等高線マップを作る
//			MAP_makeContourMap_kai(uc_trgX, uc_trgY, en_type);
//			MAP_showcountLog();
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);

			/* 次の区画へ移動 */
//			if ((us_cmap[my][mx] == 0) || ((g_sysMap[uc_trgY][uc_trgX] & 0xf0) == 0xf0)) {
				if ((mx == 0) && (my == 0)) {
					MAP_actGoal();
					break;
				}
//			}
			else {
				Simu_moveNextBlock(en_head, &bl_type);	// 次の区画へ移動						← ここで改めてリリースチェック＋壁再度作成＋等高線＋超信地旋回動作

			}
		}

//		QueryPerformanceCounter(&end);

//		double time = static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
//		printf("time %lf[ms]\n", time);

		maze_show_search(en_Head, mx, my);//map表記
//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

// *************************************************************************
//   機能		： 探索（既知区間加速）
//   注意		： なし
//   メモ		： なし
//   引数		： 目標x座標、目標y座標、探索方法
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2019.11.3			TKR			searchGoal関数から移植
// *************************************************************************/
PUBLIC void MAP_searchGoalKnown(UCHAR uc_trgX, UCHAR uc_trgY, enMAP_ACT_MODE en_type)
{
	enMAP_HEAD_DIR	en_head = NORTH;
	BOOL			bl_type = TRUE;			// 現在位置、FALSE: １区間前進状態、TURE:半区間前進状態
	int	c = 0;

//	MOT_setTrgtSpeed(MAP_SEARCH_SPEED);		// 目標速度
	//MOT_setNowSpeed(0.0f);

	/* ゴール座標目指すときは尻当て考慮 */
/*	if ((uc_trgX == GOAL_MAP_X) && (uc_trgY == GOAL_MAP_Y)) {
		f_MoveBackDist = MOVE_BACK_DIST;
	}
	else {
		f_MoveBackDist = 0;
	}

	uc_SlaCnt = 0;
*/
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	/* 迷路探索 */
	while (1) {
		MAP_refMousePos(en_Head);							// 座標更新
		MAP_makeContourMap(uc_trgX, uc_trgY, en_type);	// 等高線マップを作る

		/* ダミー壁挿入 */
	/*
		if( (uc_trgX == GOAL_MAP_X) && (uc_trgY == GOAL_MAP_Y) ){
			g_sysMap[0][0]	= 0x01;
			g_sysMap[0][1]	= 0x04;
		}
	*/
		if (TRUE == bl_type) {
//			MOT_goBlock_FinSpeed(0.5 + f_MoveBackDist, MAP_SEARCH_SPEED);		// 半区画前進(バックの移動量を含む)
//			f_MoveBackDist = 0;
		}

		// 既知区間加速するときは実行しない
		if (st_known.bl_Known != TRUE) {
//			MAP_makeMapData();		// 壁データから迷路データを作成
			Simu_makeMapData();
			c = 0;
//			SPK_debug();
		}
		else {
//			printf("count %d", c++);
		}

		MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);			// 等高線MAP法で進行方向を算出

		/* 次の区画へ移動 */
		if ((mx == uc_trgX) && (my == uc_trgY)) {

			/* ダミー壁削除 */
		/*
			if( (uc_trgX == GOAL_MAP_X) && (uc_trgY == GOAL_MAP_Y) ){
				g_sysMap[0][0]	&= ~0x01;
				g_sysMap[0][1]	&= ~0x04;
			}
		*/
			MAP_actGoal();		// ゴール時の動作
			return;				// 探索終了
		}
		else {
			MAP_moveNextBlock_acc(en_head, &bl_type);

		}
		maze_show_search(en_Head, mx, my);//map表記
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
	//stepを印付けに流用
	for(uint16_t i = 0;i<cell_count;i++){
		if((unknown_cell_data[i].x == mx)&&(unknown_cell_data[i].y == my)){
			unknown_cell_data[i].step = 1;
		}
	}
}

void  MAP_makeContourMap_maltigoal_return(
	UCHAR uc_goalX, 			///< [in] ゴールX座標
	UCHAR uc_goalY, 			///< [in] ゴールY座標
	enMAP_ACT_MODE	en_type		///< [in] 計算方法（まだ未使用）
) {
	USHORT		x, y, i;		// ループ変数
	USHORT		uc_dase;		// 基準値
	USHORT		uc_new;			// 新値
	USHORT		uc_level;		// 等高線
	UCHAR		uc_wallData;	// 壁情報

	stPOSITION		st_pos;

	en_type = en_type;		// コンパイルワーニング回避（いずれ削除）

	queue_t queue;
	queue_t* pQueue = &queue;

	initQueue(pQueue);


	/* 等高線マップを初期化する */
	for (i = 0; i < MAP_SMAP_MAX_VAL; i++) {
		us_cmap[i / MAP_Y_SIZE][i & (MAP_X_SIZE - 1)] = MAP_SMAP_MAX_VAL - 1;
	}
	Set_unknown_cell();
	MAP_showcountLog();

	/* ステップの更新予約のキュー */
//	std::queue<stPOSITION> q;
//	QueryPerformanceCounter(&start);

	/* 目標地点の等高線を0に設定 */
//	setStep(uc_goalX, uc_goalY, 0);
	for(uint16_t i = 0;i<cell_count;i++){
		if(unknown_cell_data[i].step == 0){
			st_pos.x = unknown_cell_data[i].x;
			st_pos.y = unknown_cell_data[i].y;
			st_pos.step = 0;
			enqueue(pQueue,st_pos);
		}
	}

	/* 等高線マップを作成 */
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
	UCHAR 			uc_trgX, 		///< [in] 目標x座標
	UCHAR 			uc_trgY, 		///< [in] 目標y座標 
	enMAP_ACT_MODE 	en_type, 		///< [in] 探索方法
	enSEARCH_MODE	en_search		///< [in] 探索方法
) {
	enMAP_HEAD_DIR	en_head = NORTH;
	BOOL		bl_type = TRUE;			// 現在位置、FALSE: １区間前進状態、TURE:半区間前進状態
	enMAP_HEAD_DIR		en_endDir;

	UCHAR uc_goalX;
	UCHAR uc_goalY;
	UCHAR uc_staX;
	UCHAR uc_staY;

		/* 迷路探索 */
	while (1) {
		MAP_refMousePos(en_Head);								// 座標更新
		printf("mx%d,my%d\n", mx, my);
//		MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// 等高線マップを作る
//		MAP_showcountLog();
//		maze_show_search(en_Head,mx,my);//map表記
		/* 超信地旋回探索 */
		if (SEARCH_TURN == en_search) {
			MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// 等高線マップを作る
			MAP_showcountLog();
			//			MAP_makeMapData();												// 壁データから迷路データを作成			← ここでデータ作成をミスっている
			Simu_makeMapData();//壁用のプログラムを作成			
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);						// 等高線MAP法で進行方向を算出			← 誤ったMAPを作成

			/* 次の区画へ移動 */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
				std::cout << "goal!!\n";
				MAP_actGoal();										// ゴール時の動作
				break;
			}
			else {
				//				MAP_moveNextBlock(en_head, &bl_type);				// 次の区画へ移動								← ここで改めてリリースチェック＋壁再度作成＋等高線＋超信地旋回動作
				Simu_moveNextBlock(en_head, &bl_type);//移動方向のプログラムを作成
			}
		}
		/* スラローム探索 */
		else if (SEARCH_SURA == en_search) {
//			MAP_makeContourMap(uc_trgX, uc_trgY, en_type);		// 等高線マップを作る
			Delete_unknown_cell(mx,my);
			MAP_makeContourMap_maltigoal_return(uc_trgX, uc_trgY, en_type);
//			MAP_makeContourMap_kai2(uc_trgX, uc_trgY, en_type);
//			MAP_showcountLog();
			//			MAP_makeMapData();										// 壁データから迷路データを作成			← ここでデータ作成をミスっている
			if (st_known.bl_Known != TRUE) {
	//			MAP_makeMapData();		// 壁データから迷路データを作成
				Simu_makeMapData();
			}
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// 等高線MAP法で進行方向を算出			← 誤ったMAPを作成

			/* 次の区画へ移動 */
//			if ((us_cmap[my][mx] == 0) || ((g_sysMap[uc_trgY][uc_trgX] & 0xf0) == 0xf0)) {
				if ((mx == 0) && (my == 0)) {
					MAP_actGoal();
					break;
				}
//			}
			else {
				//				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE);	// 次の区画へ移動						← ここで改めてリリースチェック＋壁再度作成＋等高線＋超信地旋回動作
				Simu_moveNextBlock(en_head, &bl_type);//移動方向のプログラムを作成

			}
		}


		maze_show_search(en_Head, mx, my);//map表記
		MAP_showcountLog();
//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}


PUBLIC void Simu_searchGoal_dijkstra(
	UCHAR 			uc_trgX, 		///< [in] 目標x座標
	UCHAR 			uc_trgY, 		///< [in] 目標y座標 
	enMAP_ACT_MODE 	en_type, 		///< [in] 探索方法
	enSEARCH_MODE	en_search		///< [in] 探索方法
) {
	enMAP_HEAD_DIR	en_head = NORTH;
	BOOL		bl_type = TRUE;			// 現在位置、FALSE: １区間前進状態、TURE:半区間前進状態
	enMAP_HEAD_DIR		en_endDir;

	UCHAR uc_goalX;
	UCHAR uc_goalY;
	UCHAR uc_staX;
	UCHAR uc_staY;

		/* 迷路探索 */
	while (1) {
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		LARGE_INTEGER start, end;

		MAP_refMousePos(en_Head);								// 座標更新
		printf("mx%d,my%d\n", mx, my);

		/* スラローム探索 */
		if (SEARCH_SURA == en_search) {
			MAP_makeContourMap_maltigoal(uc_trgX, uc_trgY, en_type);
			if (st_known.bl_Known != TRUE) {
				Simu_makeMapData();
			}
			MAP_calcMouseDir(CONTOUR_SYSTEM, &en_head);				// 等高線MAP法で進行方向を算出			← 誤ったMAPを作成

			/* 次の区画へ移動 */
//			if ((mx == uc_trgX) && (my == uc_trgY)) {
			if (us_cmap[my][mx] == 0) {
				std::cout << "goal!!\n";
				MAP_actGoal();										// ゴール時の動作
				break;
			}
			else {
//				MAP_moveNextBlock_Sura(en_head, &bl_type, FALSE);	// 次の区画へ移動						← ここで改めてリリースチェック＋壁再度作成＋等高線＋超信地旋回動作
				Simu_moveNextBlock(en_head, &bl_type);//移動方向のプログラムを作成

			}
		}

		maze_show_search(en_Head, mx, my);//map表記
//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}