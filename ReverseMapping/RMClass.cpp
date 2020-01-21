#include <Windows.h>
#include <gdiplus.h>
#include <iostream>
#include <vector>

using namespace std;

#define ORIGINAL_COLS 1024
#define ORIGINAL_ROWS 720
#define TARGET_COLS 800
#define TARGET_ROWS 480
#define uint unsigned int

#define MIN(X, Y) (X<Y)?X:Y
#define MAX(X, Y) (X>Y)?X:Y

#define LOOP_X(length) for(int x = 0; x<length; x++)
#define LOOP_Y(length) for(int y = 0; y<length; y++)

#define DEBUG 

int main() {
	/*Normal Map*/
	double param[4] = { 0.007715, 0.0256731, 0.0 };
	param[3] = 1 - param[0] - param[1] - param[2];

	double centerX = (double)ORIGINAL_COLS / 2;
	double centerY = (double)ORIGINAL_ROWS / 2;

	int d = MIN(centerX, centerY);

	uint *map = new uint[TARGET_COLS*TARGET_ROWS];
	memset(map, 0, sizeof(uint)*TARGET_COLS*TARGET_ROWS);
	
	LOOP_Y(ORIGINAL_ROWS) {
		LOOP_X(ORIGINAL_COLS) {
			double dx = (x - centerX) / d;
			double dy = (y - centerY) / d;

			double dr = sqrt(dx*dx + dy * dy);
			double sr = (param[0] * dr*dr*dr + param[1] * dr*dr + param[2] * dr + param[3]) *dr;
			double factor = abs(dr / sr);

			double srcXd = centerX + (dx*factor*d);
			double srcYd = centerY + (dy*factor*d);
			
			int nx = ceil(srcXd);
			int ny = ceil(srcYd);

			double dCol = (double)ORIGINAL_COLS / (TARGET_COLS / 2.);
			double dRow = (double)ORIGINAL_ROWS / (double)(TARGET_ROWS);

			if (nx < 0 || ny < 0 || nx >= ORIGINAL_COLS || ny >= ORIGINAL_ROWS || x % (int)dCol != 0 || y % (int)dRow != 0) continue;

			if (x < ORIGINAL_COLS - 80) {
				map[(int)(ny / dRow) * TARGET_COLS + (int)(nx / dCol)] = (ORIGINAL_COLS > ORIGINAL_ROWS) ? x * ORIGINAL_COLS + y : y * (ORIGINAL_ROWS + 1) + x;
			}
			if (x > 80) {
				map[(int)(ny / dRow) * TARGET_COLS + (int)(nx / dCol) + (TARGET_COLS / 2)] = (ORIGINAL_COLS > ORIGINAL_ROWS) ? x * ORIGINAL_COLS + y : y * (ORIGINAL_ROWS + 1) + x;
			}
		}
	}

#ifdef DEBUG
	LOOP_Y(TARGET_ROWS) {
		LOOP_X(TARGET_COLS) {
			int oriX = (ORIGINAL_COLS > ORIGINAL_ROWS) ? map[y*TARGET_COLS + x]/ORIGINAL_COLS : map[y*TARGET_COLS + x]/(ORIGINAL_ROWS+1);
			int oriY = (ORIGINAL_COLS > ORIGINAL_ROWS) ? map[y*TARGET_COLS + x] % ORIGINAL_COLS : map[y*TARGET_COLS + x] % (ORIGINAL_ROWS + 1);
			cout << "[" << oriX << ", " << oriY << "] ";
		}
		cout << endl;
	}
#endif

	getchar();
	return 0;
}