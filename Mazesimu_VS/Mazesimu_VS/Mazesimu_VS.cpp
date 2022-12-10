// Mazesimu_VS.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "common.h"
#include "typedefine.h"
#include "search.h"
#include "map_cmd.h"
#include "Maze.h"
#include <algorithm>

#include <chrono>
#include <thread>

int main()
{
	enMAP_HEAD_DIR		en_endDir;
/*	int x;
	std::string ceil = "";

	std::cout << "start\n";

	ceil += "   ";
	for (x = 0; x < MAP_X_SIZE; x++) {
		ceil +="+---";
	}
	ceil += "+";
	std::cout << ceil << std::endl;
*/
//	const std::string file_path = "../mazedata/16MM2017CX.maze";
//	const std::string file_path = "../mazedata/16MM2018H_semi.maze";
	const std::string file_path = "../mazedata/32MM2018HX.maze";
//	const std::string file_path = "../mazedata/16MM2019H_East.maze";
//	const std::string file_path = "../mazedata/test20191125.maze";
//	const std::string file_path = "../mazedata/32MM2012HX.maze";

	mazeC maze_set;
	maze_set.readfile(file_path);

	maze_set.show_countmap();

	MAP_showLog();

	MAP_Goal_init();
	MAP_Goalsize(maze_set.goal_count);
	MAP_setPos(0, 0, NORTH);								// スタート位置
	MAP_makeContourMap(maze_set.goal_map_x, maze_set.goal_map_y, BEST_WAY);					// 等高線マップを作る

	MAP_showcountLog();
	std::cout << "\n";
	maze_set.maze_show_trgt();
	std::cout << "\n";
	MAP_init();
	MAP_Goalsize(1);
	
//	MAP_searchGoalKnown(maze_set.goal_map_x, maze_set.goal_map_y, SEARCH);

	Simu_searchGoal(maze_set.goal_map_x, maze_set.goal_map_y, SEARCH, SEARCH_SURA);

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	std::cout<<"RETURN_START\n";
	MAP_Goalsize(1);
	MAP_makeContourMap(0, 0, SEARCH);
	MAP_showcountLog();
	MAP_searchGoalKnown(0, 0, SEARCH);
	Simu_searchGoal( 0, 0, SEARCH, SEARCH_RETURN );

	MAP_Goal_init();
	MAP_makeContourMap(maze_set.goal_map_x, maze_set.goal_map_y, BEST_WAY);
	MAP_showcountLog();
	MAP_Goalsize(1);
	MAP_makeCmdList(0, 0, NORTH, maze_set.goal_map_x, maze_set.goal_map_y, &en_endDir);		// ドライブコマンド作成
	MAP_makeSuraCmdList();													// スラロームコマンド作成
	MAP_makeSkewCmdList();
	MAP_showCmdLog();
	printf("\n\ndijkstra\n");
	MAP_Goal_init();
	MAP_makeContourMap_dijkstra(maze_set.goal_map_x, maze_set.goal_map_y, BEST_WAY);
	MAP_showcountLog();
	MAP_Goalsize(1);
	
	MAP_makeCmdList_dijkstra(0, 0, NORTH, maze_set.goal_map_x, maze_set.goal_map_y, &en_endDir);		// ドライブコマンド作成
	MAP_makeSuraCmdList();													// スラロームコマンド作成
	MAP_makeSkewCmdList();
	MAP_showCmdLog();
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
