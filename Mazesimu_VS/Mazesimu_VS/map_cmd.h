///#pragma once
//   ロボット名	： 電研ベーシックDCマウス、サンシャイン
//   概要		： サンシャインのHAL（ハードウエア抽象層）ファイル
//   注意		： なし
//   メモ		： なし
//   引数		： なし
//   返り値		： なし
// **************************    履    歴    *******************************
// 		v1.0		2018.06.28			sato			新規（ファイルのインクルード）
// *************************************************************************/
// 多重コンパイル抑止
#ifndef _MAP_CMD_H
#define _MAP_CMD_H

//**************************************************
// インクルードファイル（include）
//**************************************************
#include "common.h"
#include "typedefine.h"						// 定義
#include "search.h"


//**************************************************
// 定義（define）
//**************************************************

//**************************************************
// 列挙体（enum）
//**************************************************
/* コマンド走行の種類 */
typedef enum {
	MAP_DRIVE_TURN = 0,			// 超信地旋回
	MAP_DRIVE_SURA,				// スラローム
	MAP_DRIVE_SKEW,				// 斜め
	MAP_DRIVE_TYPE_MAX,
}enMAP_DRIVE_TYPE;


/* 走行コマンド */
typedef enum {

	STOP = 0,	// 停止コマンド

	/* 直進 */
	GO1,		// [  1]  1区間前進コマンド
	GO2,		// [  2]  2区間前進コマンド
	GO3,		// [  3]  3区間前進コマンド
	GO4,		// [  4]  4区間前進コマンド
	GO5,		// [  5]  5区間前進コマンド
	GO6,		// [  6]  6区間前進コマンド
	GO7,		// [  7]  7区間前進コマンド
	GO8,		// [  8]  8区間前進コマンド
	GO9,		// [  9]  9区間前進コマンド
	GO10,		// [ 10] 10区間前進コマンド
	GO11,		// [ 11] 11区間前進コマンド
	GO12,		// [ 12] 12区間前進コマンド
	GO13,		// [ 13] 13区間前進コマンド
	GO14,		// [ 14] 14区間前進コマンド
	GO15,		// [ 15] 15区間前進コマンド
	GO16,		// [ 16] 16区間前進コマンド
	GO17,		// [ 17] 17区間前進コマンド
	GO18,		// [ 18] 18区間前進コマンド
	GO19,		// [ 19] 19区間前進コマンド
	GO20,		// [ 20] 20区間前進コマンド
	GO21,		// [ 21] 21区間前進コマンド
	GO22,		// [ 22] 22区間前進コマンド
	GO23,		// [ 23] 23区間前進コマンド
	GO24,		// [ 24] 24区間前進コマンド
	GO25,		// [ 25] 25区間前進コマンド
	GO26,		// [ 26] 26区間前進コマンド
	GO27,		// [ 27] 27区間前進コマンド
	GO28,		// [ 28] 28区間前進コマンド
	GO29,		// [ 29] 29区間前進コマンド
	GO30,		// [ 30] 30区間前進コマンド
	GO31,		// [ 31] 31区間前進コマンド
	GO32,		// [ 32] 32区間前進コマンド
	GO33,		// [ 33] 33区間前進コマンド
	GO34,		// [ 34] 34区間前進コマンド
	GO35,		// [ 35] 35区間前進コマンド
	GO36,		// [ 36] 36区間前進コマンド
	GO37,		// [ 37] 37区間前進コマンド
	GO38,		// [ 38] 38区間前進コマンド
	GO39,		// [ 39] 39区間前進コマンド
	GO40,		// [ 40] 40区間前進コマンド
	GO41,		// [ 41] 41区間前進コマンド
	GO42,		// [ 42] 42区間前進コマンド
	GO43,		// [ 43] 43区間前進コマンド
	GO44,		// [ 44] 44区間前進コマンド
	GO45,		// [ 45] 45区間前進コマンド
	GO46,		// [ 46] 46区間前進コマンド
	GO47,		// [ 47] 47区間前進コマンド
	GO48,		// [ 48] 48区間前進コマンド
	GO49,		// [ 49] 49区間前進コマンド
	GO50,		// [ 50] 50区間前進コマンド
	GO51,		// [ 51] 51区間前進コマンド
	GO52,		// [ 52] 52区間前進コマンド
	GO53,		// [ 53] 53区間前進コマンド
	GO54,		// [ 54] 54区間前進コマンド
	GO55,		// [ 55] 55区間前進コマンド
	GO56,		// [ 56] 56区間前進コマンド
	GO57,		// [ 57] 57区間前進コマンド
	GO58,		// [ 58] 58区間前進コマンド
	GO59,		// [ 59] 59区間前進コマンド
	GO60,		// [ 60] 60区間前進コマンド
	GO61,		// [ 61] 61区間前進コマンド
	GO62,		// [ 62] 62区間前進コマンド
	GO63,		// [ 63] 63区間前進コマンド
	GO64,		// [ 64] 64区間前進コマンド
	GO65,		// [ 65] 65区間前進コマンド
	GO66,		// [ 66] 66区間前進コマンド
	GO67,		// [ 67] 67区間前進コマンド
	GO68,		// [ 68] 68区間前進コマンド
	GO69,		// [ 69] 69区間前進コマンド
	GO70,		// [ 70] 70区間前進コマンド
	GO71,		// [ 71] 71区間前進コマンド

	/* 超地信旋回 */
	R90,		// [ 72] 右90度右超信旋回コマンド
	L90,		// [ 73] 左90度左超信旋回コマンド
	R180,		// [ 74] 右180度超信旋回コマンド
	L180,		// [ 75] 左180度超信旋回コマンド

	/* スラローム */
	R90S,		// [ 76] 右90度旋回コマンド
	L90S,		// [ 77] 左90度旋回コマンド
	R90F,		// [ 78] 右高速90度旋回コマンド
	L90F,		// [ 79] 左高速90度旋回コマンド
	R180F,		// [ 80] 右高速180度旋回コマンド
	L180F,		// [ 81] 左高速180度旋回コマンド

	/* 斜め */
	NGO1,		// [ 82] 斜め 1区間前進コマンド
	NGO2,		// [ 83] 斜め 2区間前進コマンド
	NGO3,		// [ 84] 斜め 3区間前進コマンド
	NGO4,		// [ 85] 斜め 4区間前進コマンド
	NGO5,		// [ 86] 斜め 5区間前進コマンド
	NGO6,		// [ 87] 斜め 6区間前進コマンド
	NGO7,		// [ 88] 斜め 7区間前進コマンド
	NGO8,		// [ 89] 斜め 8区間前進コマンド
	NGO9,		// [ 90] 斜め 9区間前進コマンド
	NGO10,		// [ 91] 斜め10区間前進コマンド
	NGO11,		// [ 92] 斜め11区間前進コマンド
	NGO12,		// [ 93] 斜め12区間前進コマンド
	NGO13,		// [ 94] 斜め13区間前進コマンド
	NGO14,		// [ 95] 斜め14区間前進コマンド
	NGO15,		// [ 96] 斜め15区間前進コマンド
	NGO16,		// [ 97] 斜め16区間前進コマンド
	NGO17,		// [ 98] 斜め17区間前進コマンド
	NGO18,		// [ 99] 斜め18区間前進コマンド
	NGO19,		// [100] 斜め19区間前進コマンド
	NGO20,		// [101] 斜め20区間前進コマンド
	NGO21,		// [102] 斜め21区間前進コマンド
	NGO22,		// [103] 斜め22区間前進コマンド
	NGO23,		// [104] 斜め23区間前進コマンド
	NGO24,		// [105] 斜め24区間前進コマンド
	NGO25,		// [106] 斜め25区間前進コマンド
	NGO26,		// [107] 斜め26区間前進コマンド
	NGO27,		// [108] 斜め27区間前進コマンド
	NGO28,		// [109] 斜め28区間前進コマンド
	NGO29,		// [110] 斜め29区間前進コマンド
	NGO30,		// [111] 斜め30区間前進コマンド
	NGO31,		// [112] 斜め31区間前進コマンド
	NGO32,		// [113] 斜め32区間前進コマンド
	NGO33,		// [114] 斜め33区間前進コマンド
	NGO34,		// [115] 斜め34区間前進コマンド
	NGO35,		// [116] 斜め35区間前進コマンド
	NGO36,		// [117] 斜め36区間前進コマンド
	NGO37,		// [118] 斜め37区間前進コマンド
	NGO38,		// [119] 斜め38区間前進コマンド
	NGO39,		// [120] 斜め39区間前進コマンド
	NGO40,		// [121] 斜め40区間前進コマンド
	NGO41,		// [122] 斜め41区間前進コマンド
	NGO42,		// [123] 斜め42区間前進コマンド
	NGO43,		// [124] 斜め43区間前進コマンド
	NGO44,		// [125] 斜め44区間前進コマンド
	NGO45,		// [126] 斜め45区間前進コマンド
	NGO46,		// [127] 斜め46区間前進コマンド
	NGO47,		// [128] 斜め47区間前進コマンド
	NGO48,		// [129] 斜め48区間前進コマンド
	NGO49,		// [130] 斜め49区間前進コマンド
	NGO50,		// [131] 斜め50区間前進コマンド
	NGO51,		// [132] 斜め51区間前進コマンド
	NGO52,		// [133] 斜め52区間前進コマンド
	NGO53,		// [134] 斜め53区間前進コマンド
	NGO54,		// [135] 斜め54区間前進コマンド
	NGO55,		// [136] 斜め55区間前進コマンド
	NGO56,		// [137] 斜め56区間前進コマンド
	NGO57,		// [138] 斜め57区間前進コマンド
	NGO58,		// [139] 斜め58区間前進コマンド
	NGO59,		// [140] 斜め59区間前進コマンド
	NGO60,		// [141] 斜め60区間前進コマンド
	NGO61,		// [142] 斜め61区間前進コマンド
	NGO62,		// [143] 斜め62区間前進コマンド
	NGO63,		// [144] 斜め63区間前進コマンド
	NGO64,		// [145] 斜め64区間前進コマンド
	NGO65,		// [146] 斜め65区間前進コマンド
	NGO66,		// [147] 斜め66区間前進コマンド
	NGO67,		// [148] 斜め67区間前進コマンド
	NGO68,		// [149] 斜め68区間前進コマンド
	NGO69,		// [150] 斜め69区間前進コマンド
	NGO70,		// [151] 斜め70区間前進コマンド
	NGO71,		// [152] 斜め71区間前進コマンド

	RS45N,		// [153] 直進 -> 右45   -> 斜め
	LS45N,		// [154] 直進 -> 左45   -> 斜め
	RS135N,		// [155] 直進 -> 右135  -> 斜め
	LS135N,		// [156] 直進 -> 左135  -> 斜め
	RN45S,		// [157] 斜め -> 右45   -> 直進
	LN45S,		// [158] 斜め -> 左45   -> 直進
	RN135S,		// [159] 斜め -> 右135  -> 直進
	LN135S,		// [160] 斜め -> 左135  -> 直進
	RN90N,		// [161] 斜め -> 右90   -> 斜め
	LN90N,		// [162] 斜め -> 左90   -> 斜め

	CEND = 250,	// [250] コマンド終了

	MAP_CMD_MAX,
}enMAP_CMD;

//**************************************************
// 構造体（struct）
//**************************************************


//**************************************************
// グローバル変数
//**************************************************




//**************************************************
// プロトタイプ宣言（ファイル内で必要なものだけ記述）
//**************************************************
PUBLIC void MAP_makeCmdList(UCHAR uc_staX, UCHAR uc_staY, enMAP_HEAD_DIR en_staDir, UCHAR uc_endX, UCHAR uc_endY, enMAP_HEAD_DIR* en_endDir);
PUBLIC void MAP_makeSuraCmdList(void);
PUBLIC void MAP_makeSkewCmdList(void);
//PUBLIC void MAP_drive( enMAP_DRIVE_TYPE en_driveType );
PROTECTED void MAP_setCmdPos(UCHAR uc_x, UCHAR uc_y, enMAP_HEAD_DIR en_dir);

PUBLIC void MAP_showCmdLog(void);


#endif
