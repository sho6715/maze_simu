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
				goal_count++;

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


void direction_show(
	enMAP_HEAD_DIR	en_head,
	char x,
	char y,
	char mx,
	char my)
{
	if ((x == mx) && (y == my)) {
		switch (en_head) {

		case NORTH:
			std::cout << "^";
			break;
		case EAST:
			std::cout << ">";
			break;
		case WEST:
			std::cout << "<";
			break;
		case SOUTH:
			std::cout << "V";
			break;
		default:
			break;
		}
	}
	else {
		std::cout << " ";
	}
}

void maze_show_search(enMAP_HEAD_DIR en_head,int mx,int my)
{
//	system("cls");

	SHORT	x, y;
	CHAR	c_data;
	std::string wall_t = "";
	std::string floor = "";
	std::string wall = "";

	for (x = 0; x < MAP_X_SIZE; x++) {
		wall_t += "+---";
	}
	wall_t += "+";

	for (y = MAP_Y_SIZE - 1; y >= 0; y--) {
		floor = "";
		wall = "";
		for (x = 0; x < MAP_X_SIZE; x++) {
			c_data = (UCHAR)g_sysMap[y][x];
			if ((c_data & 0x08) == 0) {
				floor += " ";
			}
			else {
				floor += "|";
			}
			if ((x == mx) && (y == my)) {
				switch (en_head) {

				case NORTH:
					floor += " ^ ";
					break;
				case EAST:
					floor += " > ";
					break;
				case WEST:
					floor += " < ";
					break;
				case SOUTH:
					floor += " V ";
					break;
				default:
					break;
				}
			}
			else {
				floor += "   ";
			}
			if ((c_data & 0x04) == 0) {
				wall += "+   ";
			}
			else {
				wall += "+---";
			}
		}
		floor += "|";
		wall += "+";
		if (y == MAP_Y_SIZE - 1) {
			std::cout << wall_t << std::endl;
		}
		std::cout << floor << std::endl;
		std::cout << wall << std::endl;
	}
	std::cout << "mx " << mx << " my " << my << std::endl;

}

void mazeC::maze_show_trgt(void)
{
	SHORT	x, y;
	CHAR	c_data;
	printf("     ");
	for (x = 0; x < sizeX; x++) {
		printf("+---");
	}
	printf("+\n\r");

	for (y = sizeY * 2 - 1; y >= 0; y--) {
		if (y % 2 == 0) {
			printf("     ");
		}
		else {
			printf("   %2d", y / 2);
		}
		for (x = 0; x < sizeX * 2; x++) {
			c_data = (UCHAR)g_trgtMap[y / 2][x / 2];
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