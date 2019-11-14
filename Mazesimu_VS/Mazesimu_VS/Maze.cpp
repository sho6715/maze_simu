/**
 * @file Maze_show.cpp
 * @brief
 * @author sho6715
 * @date 2019.11.13
 */

#include "Maze.h"

PUBLIC UCHAR	g_trgtMap[MAP_Y_SIZE][MAP_X_SIZE];		///< 正解迷路

void mazeC::readfile(std::string file_path)
{
	std::ifstream ifs(file_path.c_str());
	std::string str;
	std::ifstream size_ifs(file_path.c_str());
	std::string size;

	if (ifs.fail()) {
		std::cerr << "can't open " << file_path << std::endl;
	}

	getline(size_ifs, size);
	sizeX = ((int)size.length() - 1) / 4;
	sizeY = sizeX;

	char ch_top;
	char ch_side;

	// read maze map
	for (int y = sizeY * 2; y >= 0; y--) {//偶数と奇数で判別して南北壁入力と東西壁入力をする
		getline(ifs, str);
		for (int x = 0; x < sizeX; x++) {
			ch_top = str[x * 4 + 2];
			switch (ch_top) {
			case 'G':
				if (goal_map_x > x) goal_map_x = x;
				if (goal_map_y > y / 2) goal_map_y = y / 2;

				ch_side = str[x * 4];
				if (ch_side == '|') {
					g_trgtMap[y / 2][x] = (g_trgtMap[y / 2][x] | 0x88);
					if (x != 0) {
						g_trgtMap[y / 2][x - 1] = (g_trgtMap[y / 2][x - 1] | 0x22);
					}
				}
				break;

			case '-':

				if (y / 2 != sizeY) {
					g_trgtMap[y / 2][x] = (g_trgtMap[y / 2][x] | 0x44);
					if (y / 2 != 0) {
						g_trgtMap[y / 2 - 1][x] = (g_trgtMap[y / 2 - 1][x] | 0x11);
					}
				}
				else {
					g_trgtMap[y / 2 - 1][x] = (g_trgtMap[y / 2 - 1][x] | 0x11);
				}

				break;
			default:
				ch_side = str[x * 4];
				if (ch_side == '|') {
					g_trgtMap[y / 2][x] = (g_trgtMap[y / 2][x] | 0x88);
					if (x != 0) {
						g_trgtMap[y / 2][x - 1] = (g_trgtMap[y / 2][x - 1] | 0x22);
					}
				}
				break;
			}
			g_trgtMap[y / 2][sizeX - 1] = (g_trgtMap[y / 2][sizeX - 1] | 0x22);
			g_trgtMap[y / 2][x] = (g_trgtMap[y / 2][x] | 0xf0);

		}
		//		std::cout << "\n";
	}

}

void mazeC::show_countmap(void){
	for (int y = sizeY - 1; y >= 0; y--) {
		for (int x = 0; x < sizeX; x++) {
			printf("%x", (g_trgtMap[y][x] & 0x0f));
		}
		printf("\n");
	}
}


void mazeC::maze_show(void)
{
	SHORT	x, y;
	CHAR	c_data;
	printf("     ");
	for (x = 0; x < sizeX; x++) {
		printf("+---");
	}
	printf("+\n\r");

	for (y = sizeY*2-1; y >= 0; y--) {
		if (y % 2 == 0) {
			printf("     ");
		}
		else {
			printf("   %2d", y/2);
		}
		for (x = 0; x < sizeX*2; x++) {
			c_data = (UCHAR)g_trgtMap[y/2][x/2];
			if (y % 2 == 0) {
				if (x % 2 == 0) {
					if ((c_data & 0x04) == 0) {
						printf("+   ");
					}
					else {
						printf("+---");
					}
				}
			}

			else {
				if (x % 2 == 0) {
					if ((c_data & 0x08) == 0) {
						printf(" ");
					}
					else {
						printf("|");
					}
				}
				else {//ここに座標と方向をみて表記するプログラムを入れて実際には動かす
					printf(" ");
					printf(" ");//ここを引数に座標をとってやればええ
					printf(" ");
				}
			}
		}
		printf("|");

		printf("\n\r");
	}

	printf("     ");
	for (x = 0; x < sizeX; x++) {
		printf(" %2d ", x % 10);
	}
	printf("\n\r");

}