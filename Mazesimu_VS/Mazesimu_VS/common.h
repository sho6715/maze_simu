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

