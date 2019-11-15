// Mazesimu_VS.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "common.h"
#include "typedefine.h"
#include "search.h"
#include "map_cmd.h"
#include "Maze.h"
#include <algorithm>

int main()
{
	enMAP_HEAD_DIR		en_endDir;
	std::cout << "start\n";
	const std::string file_path = "../mazedata/16MM2017CX.maze";
//	const std::string file_path = "../mazedata/16MM2018H_semi.maze";

	mazeC maze_set;
	maze_set.readfile(file_path);

	maze_set.show_countmap();

	MAP_showLog();

	MAP_Goal_init();
	MAP_setPos(0, 0, NORTH);								// スタート位置
	MAP_makeContourMap(maze_set.goal_map_x, maze_set.goal_map_y, BEST_WAY);					// 等高線マップを作る

	MAP_showcountLog();
	std::cout << "\n";
	maze_set.maze_show_trgt();
	std::cout << "\n";
	MAP_init();
	Simu_searchGoal(maze_set.goal_map_x, maze_set.goal_map_y, SEARCH, SEARCH_SURA);

/*	MAP_makeCmdList(0, 0, NORTH, goal_map_x, goal_map_y, &en_endDir);		// ドライブコマンド作成
	MAP_makeSuraCmdList();													// スラロームコマンド作成
	MAP_makeSkewCmdList();
	MAP_showCmdLog();
*/
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
