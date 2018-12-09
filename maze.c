#include "gba1.h"
#include "8x8ascii.h"

#define LCD_CHAR_WIDTH_EX	15		// width for character unit(2倍)
#define LCD_CHAR_HEIGHT_EX	10		// height for character unit(2倍)
#define CHAR_WIDTH_EX		16		// character width(2倍）
#define CHAR_HEIGHT_EX	    16		// character height(2倍）
#define MAZESIZE_X1			13  	// 迷路の幅(ステージ1）
#define MAZESIZE_Y1			13		// 迷路の高さ(ステージ1）
#define	MAZEPRINT_X1		2		// 迷路の描画開始x座標(ステージ1）
#define	MAZEPRINT_Y1		4		// 迷路の描画開始y座標(ステージ1）
#define MAZESIZE_X2			15  	// 迷路の幅(ステージ2）
#define MAZESIZE_Y2			15		// 迷路の高さ(ステージ2）
#define	MAZEPRINT_X2		1		// 迷路の描画開始x座標(ステージ2）
#define	MAZEPRINT_Y2		3		// 迷路の描画開始y座標(ステージ2）

/* プロトタイプ宣言 */
void locate(hword, hword);
void locate_ex(hword, hword);
void prints(byte*);
void prints_ex(byte*);
void print_ascii(byte);
void print_ascii_ex(byte);
void draw_point(hword, hword, hword);
hword mod(hword dividened, hword divisor);
hword div(hword dividened, hword divisor);
void draw_point(hword, hword, hword);
void display_time(hword);
hword dig1(hword, hword);
hword dig2(hword, hword);
hword rand(hword);
void print_rectangle(hword, hword);

/* グローバル変数定義 */
point p;
point pe;
hword randnum = 1; // 乱数初期値
hword mx = 0;
hword my = 0;
hword cx = 0;
hword cy = 0;

/* 迷路用の配列 */
unsigned char maze1[MAZESIZE_Y1][MAZESIZE_X1];

/* 穴掘り開始位置の候補座標を格納する配列 */
unsigned char MX1[MAZESIZE_X1];
unsigned char MY1[MAZESIZE_Y1];


/* 迷路用の配列 */
unsigned char maze2[MAZESIZE_Y2][MAZESIZE_X2];

/* 穴掘り開始位置の候補座標を格納する配列 */
unsigned char MX2[MAZESIZE_X2];
unsigned char MY2[MAZESIZE_Y2];


int main(void) {

	/* 画面初期化ルーチン */
	*((unsigned short *)IOBASE) = 0xF03;

	/* 開始画面描画 */
	locate_ex(3, 4);
	prints_ex("MAZE START");
	locate(18,18);
	prints("PUSH START");
	
	/* 変数定義 */
	hword key_in, flagST;

	/* ステージ1 */
	while (1) {
		/* キー入力 */
		key_in = ~(*(hword *)KEY_STATUS)&KEY_ALL;

		int i,j;
		if (key_in == KEY_START) {
			for (i = 0; i < 10; i++) {
				/* STSRT10回点滅 */
				locate_ex(3, 4);
				prints_ex("          ");
				locate_ex(3, 4);
				prints_ex("MAZE START");
			}
			/* 黒色で画面初期化 */
			for (i = 0; i < LCD_HEIGHT; i++) {
				for (j = 0; j < LCD_WIDTH; j++) {
					draw_point(j, i, BGR(0x00, 0x00, 0x00));
				}
			}
			flagST = 1;
			break;
		}
	}

	/* 迷路初期化(初めはすべて壁で1なら壁、0なら道) */
	for (my = 0; my < MAZESIZE_Y1; my++) {
		for (mx = 0; mx < MAZESIZE_X1; mx++) {
			maze1[my][mx] = 1;
		}
	}

	/* スタート地点 */
	maze1[0][1] = 0;
	/* ゴール地点 */
	maze1[MAZESIZE_Y1 - 1][MAZESIZE_X1 - 2] = 0;
	/* 穴掘り初期開始位置 */
	maze1[1][1] = 0;


	while (1) {
		int k = 0;
		/* 穴を掘れる場所を調べる（上下左右2マス先が壁かどうか調べる） */
		for (my = 1; my < MAZESIZE_Y1 - 1; my += 2) {
			for (mx = 1; mx < MAZESIZE_X1 - 1; mx += 2) {
				if (maze1[my][mx] == 0) {
					/* 左 */
					if (mx - 2 >= 0 && maze1[my][mx - 2] == 1) {
						MX1[k] = mx;
						MY1[k] = my;
						k++;
					}
					/* 右 */
					else if (mx + 2 < MAZESIZE_X1 && maze1[my][mx + 2] == 1) {
						MX1[k] = mx;
						MY1[k] = my;
						k++;
					}
					/* 上 */
					else if (my - 2 >= 0 && maze1[my - 2][mx] == 1) {
						MX1[k] = mx;
						MY1[k] = my;
						k++;
					}
					/* 下 */
					else if (my + 2 < MAZESIZE_Y1 && maze1[my + 2][mx] == 1) {
						MX1[k] = mx;
						MY1[k] = my;
						k++;
					}
					/* 迷路の端かどうか */
					else if (mx == MAZESIZE_X1 - 2 && my == MAZESIZE_Y1 - 2) {
						break;
					}
				}
			}
		}
		/* 掘れる穴の候補がない場合 */
		if (k == 0) {
			break;
		}
		else {
			/* 穴掘りの開始位置をランダムに決め、穴を掘る */
			mx = MX1[mod(rand(randnum), k)];
			my = MY1[mod(rand(randnum), k)];
			dig1(mx, my);
		}
	}
	
	/* 迷路描画 */
	for (my = 0; my < MAZESIZE_Y1; my++) {
		for (mx = 0; mx < MAZESIZE_X1; mx++) {
			locate(mx+MAZEPRINT_X1, my+MAZEPRINT_Y1);
			if (maze1[my][mx] == 0) {
				print_ascii(32);
			}
			else if (maze1[my][mx] == 1) {
				print_ascii(219);
			}
		}
	}

	locate(1, 2);
	prints("START(STAGE1)");
	locate(12, 18);
	prints("GOAL");

	int i;
	/* 迷路の淵描画 */
	for (i = MAZEPRINT_X1 * 8 - 1; i <= (MAZEPRINT_X1 + MAZESIZE_X1) * 8; i++) {
		draw_point(i, MAZEPRINT_Y1 * 8 - 1, BGR(0x1F, 0x1F,0x00));
		draw_point(i, (MAZEPRINT_Y1 + MAZESIZE_Y1) * 8, BGR(0x1F, 0x1F, 0x00));
	}
	for (i = MAZEPRINT_Y1 * 8 - 1; i <= (MAZEPRINT_Y1 + MAZESIZE_Y1) * 8; i++) {
		draw_point(MAZEPRINT_X1 * 8 - 1, i, BGR(0x1F, 0x1F, 0x00));
		draw_point((MAZEPRINT_X1 + MAZESIZE_X1) * 8, i, BGR(0x1F, 0x1F, 0x00));
	}

	/* 操作説明描画 */
	cx = 17, cy = 5;
	locate(cx, cy);
	prints("Please use");
	cx = 17, cy++;
	locate(cx, cy);
	prints("KEY_UP/DOWN");
	cx = 17, cy++;
	locate(cx, cy);
	prints("RIGHT/LEFT");
	
	volatile unsigned short time;
	hword timejust;

	locate(17, 15);
	prints("Time = ");
	
	/* タイマカウンタ設定レジスタ */
	*((unsigned short *)0x04000100) = 0xFF88;	// タイマ0 カウンタ設定(元クロック1/120~3秒)
	*((unsigned short *)0x04000104) = 0xFF88;	// タイマ1 カウンタ設定(元クロック1/120~2秒)
	*((unsigned short *)0x04000108) = 0xFF88;	// タイマ2 カウンタ設定(元クロック1/120秒)
	*((unsigned short *)0x0400010C) = 0x0000;	// タイマ3 カウンタ設定(元クロック1秒)

												/* タイマ制御レジスタ */
	*((unsigned short *)0x04000102) = 0x0000;	// タイマ0 制御設定（タイマOFF，割込みOFF，カスケードOFF，プリスケーラなし）
	*((unsigned short *)0x04000106) = 0x0084;	// タイマ1 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）
	*((unsigned short *)0x0400010A) = 0x0084;	// タイマ2 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）
	*((unsigned short *)0x0400010E) = 0x0084;	// タイマ3 制御設定（タイマON，割込みOFF，カスケードON，プリスケーラなし）

	/* 経路スタート地点描画 */
	mx = 3, my = 4;
	print_rectangle(mx, my);

	while (1) {

		/* タイマ3 カウント値読込み */
		time = *((unsigned short *)0x0400010C);
		time = time - timejust;

		/* キー入力 */
		key_in = ~(*(hword *)KEY_STATUS)&KEY_ALL;
		hword flag_key;

		/* 経路描画 */
		if (flagST == 1) {
			switch (key_in) {
			case KEY_UP:
				if (flag_key == 0 && my != MAZEPRINT_Y1 && maze1[my - MAZEPRINT_Y1 - 1][mx - MAZEPRINT_X1] == 0) {
					my--;
					*((unsigned short *)0x04000102) = 0x0080;
				}
				flag_key = 1;
				break;

			case KEY_DOWN:
				if (flag_key == 0 && my != MAZESIZE_Y1 - 1 + MAZEPRINT_Y1 && maze1[my - MAZEPRINT_Y1 + 1][mx - MAZEPRINT_X1] == 0) {
					my++;
					*((unsigned short *)0x04000102) = 0x0080;
				}
				flag_key = 1;
				break;

			case KEY_LEFT:
				if (flag_key == 0 && mx != MAZEPRINT_X1 && maze1[my - MAZEPRINT_Y1][mx - MAZEPRINT_X1 - 1] == 0) {
					mx--;
					*((unsigned short *)0x04000102) = 0x0080;
				}
				flag_key = 1;
				break;

			case KEY_RIGHT:
				if (flag_key == 0 && mx != MAZESIZE_X1 - 1 + MAZEPRINT_X1 && maze1[my - MAZEPRINT_Y1][mx - MAZEPRINT_X1 + 1] == 0) {
					mx++;
					*((unsigned short *)0x04000102) = 0x0080;
				}
				flag_key = 1;
				break;

			default:
				flag_key = 0;
				break;
			}
			/* 道なら赤で塗りつぶす */
			if (maze1[my-MAZEPRINT_Y1][mx- MAZEPRINT_X1] == 0) {
				print_rectangle(mx, my);
			}

			/* クリアした場合 */
			if (mx == 13 && my == 16) {
				locate(17, 14);
				prints("CREAR");
				*((unsigned short *)0x04000102) = 0x0000;
				locate(17, 17);
				prints("PUSH START");
				break;
			}

		}
			/* time 表示位置設定 */
			locate(21, 15);
			display_time(time);
		
	}

	/* ステージ2 */
	randnum = 15;
	mx = 0, my = 0;
	cx = 0, cy = 0;
	while (1) {
		/* キー入力 */
		key_in = ~(*(hword *)KEY_STATUS)&KEY_ALL;
		int i, j;
		if (key_in == KEY_START) {
			timejust += time;
			/* 黒色で画面初期化 */
			for (i = 0; i < LCD_HEIGHT; i++) {
				for (j = 0; j < LCD_WIDTH; j++) {
					draw_point(j, i, BGR(0x00, 0x00, 0x00));
				}
			}
			flagST = 1;
			break;
		}
	}

	/* 迷路初期化(初めはすべて壁で1なら壁、0なら道) */
	for (my = 0; my < MAZESIZE_Y2; my++) {
		for (mx = 0; mx < MAZESIZE_X2; mx++) {
			maze2[my][mx] = 1;
		}
	}
	/* スタート地点 */
	maze2[0][1] = 0;
	/* ゴール地点 */
	maze2[MAZESIZE_Y2 - 1][MAZESIZE_X2 - 2] = 0;
	/* 穴掘り初期開始位置 */
	maze2[1][1] = 0;


	while (1) {
		int k = 0;
		/* 穴を掘れる場所を調べる（上下左右2マス先が壁かどうか調べる） */
		for (my = 1; my < MAZESIZE_Y2 - 1; my += 2) {
			for (mx = 1; mx < MAZESIZE_X2 - 1; mx += 2) {
				if (maze2[my][mx] == 0) {
					/* 左 */
					if (mx - 2 >= 0 && maze2[my][mx - 2] == 1) {
						MX2[k] = mx;
						MY2[k] = my;
						k++;
					}
					/* 右 */
					else if (mx + 2 < MAZESIZE_X2 && maze2[my][mx + 2] == 1) {
						MX2[k] = mx;
						MY2[k] = my;
						k++;
					}
					/* 上 */
					else if (my - 2 >= 0 && maze2[my - 2][mx] == 1) {
						MX2[k] = mx;
						MY2[k] = my;
						k++;
					}
					/* 下 */
					else if (my + 2 < MAZESIZE_Y2 && maze2[my + 2][mx] == 1) {
						MX2[k] = mx;
						MY2[k] = my;
						k++;
					}
					/* 迷路の端かどうか */
					else if (mx == MAZESIZE_X2 - 2 && my == MAZESIZE_Y2 - 2) {
						break;
					}
				}
			}
		}
		/* 掘れる穴の候補がない場合 */
		if (k == 0) {
			break;
		}
		else {
			/* 穴掘りの開始位置をランダムに決め、穴を掘る */
			mx = MX2[mod(rand(randnum), k)];
			my = MY2[mod(rand(randnum), k)];
			dig2(mx, my);
		}
	}

	/* 迷路描画 */
	for (my = 0; my < MAZESIZE_Y2; my++) {
		for (mx = 0; mx < MAZESIZE_X2; mx++) {
			locate(mx + MAZEPRINT_X2, my + MAZEPRINT_Y2);
			if (maze2[my][mx] == 0) {
				print_ascii(32);
			}
			else if (maze2[my][mx] == 1) {
				print_ascii(219);
			}
		}
	}

	locate(0, 1);
	prints("START(STAGE2)");
	locate(13, 19);
	prints("GOAL");

	int l;
	/* 迷路の淵描画 */
	for (l = MAZEPRINT_X2 * 8 - 1; l <= (MAZEPRINT_X2 + MAZESIZE_X2) * 8; l++) {
		draw_point(l, MAZEPRINT_Y2 * 8 - 1, BGR(0x1F, 0x1F, 0x00));
		draw_point(l, (MAZEPRINT_Y2 + MAZESIZE_Y2) * 8, BGR(0x1F, 0x1F, 0x00));
	}
	for (l = MAZEPRINT_Y2 * 8 - 1; l <= (MAZEPRINT_Y2 + MAZESIZE_Y2) * 8; l++) {
		draw_point(MAZEPRINT_X2 * 8 - 1, l, BGR(0x1F, 0x1F, 0x00));
		draw_point((MAZEPRINT_X2 + MAZESIZE_X2) * 8, l, BGR(0x1F, 0x1F, 0x00));
	}

	/* 操作説明描画 */
	cx = 18, cy = 5;
	locate(cx, cy);
	prints("Please use");
	cx = 18, cy++;
	locate(cx, cy);
	prints("KEY_UP/DOWN");
	cx = 18, cy++;
	locate(cx, cy);
	prints("RIGHT/LEFT");

	locate(18, 15);
	prints("Time = ");

	/* 経路スタート地点描画 */
	mx = 2, my = 3;
	print_rectangle(mx, my);

	while (1) {

		/* タイマ3 カウント値読込み */
		time = *((unsigned short *)0x0400010C);
		time = time - timejust;

		/* キー入力 */
		key_in = ~(*(hword *)KEY_STATUS)&KEY_ALL;
		hword flag_key;

		/* 経路描画 */
		if (flagST == 1) {
			switch (key_in) {
			case KEY_UP:
				if (flag_key == 0 && my != MAZEPRINT_Y2 && maze2[my - MAZEPRINT_Y2 - 1][mx - MAZEPRINT_X2] == 0) {
					my--;
					*((unsigned short *)0x04000102) = 0x0080;
				}
				flag_key = 1;
				break;

			case KEY_DOWN:
				if (flag_key == 0 && my != MAZESIZE_Y2 - 1 + MAZEPRINT_Y2 && maze2[my - MAZEPRINT_Y2 + 1][mx - MAZEPRINT_X2] == 0) {
					my++;
					*((unsigned short *)0x04000102) = 0x0080;
				}
				flag_key = 1;
				break;

			case KEY_LEFT:
				if (flag_key == 0 && mx != MAZEPRINT_X2 && maze2[my - MAZEPRINT_Y2][mx - MAZEPRINT_X2 - 1] == 0) {
					mx--;
					*((unsigned short *)0x04000102) = 0x0080;
				}
				flag_key = 1;
				break;

			case KEY_RIGHT:
				if (flag_key == 0 && mx != MAZESIZE_X2 - 1 + MAZEPRINT_X2 && maze2[my - MAZEPRINT_Y2][mx - MAZEPRINT_X2 + 1] == 0) {
					mx++;
					*((unsigned short *)0x04000102) = 0x0080;
				}
				flag_key = 1;
				break;

			default:
				flag_key = 0;
				break;
			}
			/* 道なら赤で塗りつぶす */
			if (maze2[my - MAZEPRINT_Y2][mx - MAZEPRINT_X2] == 0) {
				print_rectangle(mx, my);

			}
			
			/* クリアした場合 */
			if (mx == 14 && my == 17) {
				locate(18, 14);
				prints("CREAR");
				*((unsigned short *)0x04000102) = 0x0000;
				locate(18, 17);
				prints("PUSH START");
				break;
			}

		}
		/* time 表示位置設定 */
		locate(22, 15);
		display_time(time);
	}

	while (1) {
		/* キー入力 */
		key_in = ~(*(hword *)KEY_STATUS)&KEY_ALL;

		int m, n;
		if (key_in == KEY_START) {
			/* 黒色で画面初期化 */
			for (m = 0; m < LCD_HEIGHT; m++) {
				for (n = 0; n < LCD_WIDTH; n++) {
					draw_point(n, m, BGR(0x00, 0x00, 0x00));
				}
			}
			locate_ex(3, 4);
			prints_ex("MAZE END");
			flagST = 1;
			break;
		}
	}
	return 0;
}

/* 文字座標(0, 0)～(29,19)の位置(cx, cy)を画面座標に変換 */
void locate(hword cx, hword cy) {
	if (cx < LCD_CHAR_WIDTH) {
		p.x = cx << 3;				/* x座標の指定 */
	}
	else {
		p.x = LCD_CHAR_WIDTH - 1;	/* xの最大値を設定 */
	}
	if (cy < LCD_CHAR_HEIGHT) {
		p.y = cy << 3;				/* y座標の指定 */
	}
	else {
		p.y = LCD_CHAR_HEIGHT - 1;	/* yの最大値を設定 */
	}
}


/* 文字座標(0, 0)～(14,9)の位置(cx, cy)を画面座標に変換 */
void locate_ex(hword cx, hword cy) {
	if (cx < LCD_CHAR_WIDTH_EX) {
		pe.x = cx << 4;				/* x座標の指定 */
	}
	else {
		pe.x = LCD_CHAR_WIDTH_EX - 1;	/* xの最大値を設定 */
	}
	if (cy < LCD_CHAR_HEIGHT_EX) {
		pe.y = cy << 4;				/* y座標の指定 */
	}
	else {
		pe.y = LCD_CHAR_HEIGHT_EX - 1;	/* yの最大値を設定 */
	}
}


/* 文字列表示関数 */
void prints(byte *str) {

	while (*str) {
		print_ascii(*str++);
		p.x = p.x + CHAR_WIDTH;			/* 表示位置を1文字右へ */
	}
}


/* 文字列表示関数（2倍） */
void prints_ex(byte *str) {

	while (*str) {
		print_ascii_ex(*str++);
		pe.x = pe.x + CHAR_WIDTH_EX;			/* 表示位置を1文字右へ */
	}
}


/* locateで設定された位置にASCIIコードascii_numに相当する文字を表示する関数（白） */
void print_ascii(byte ascii_num) {

	hword tx, ty;							/* 文字パターンの座標指定 */
	byte cbit;								/* ビット判定 */

	for (ty = 0; ty < 8; ty++) {				/* 文字パターンのy座標指定 */

		cbit = 0x80;						/* 判定ビットを8ビットの最上位にセット */

		for (tx = 0; tx < 8; tx++) {							/* 文字パターンのx座標指定 */
			if ((char8x8[ascii_num][ty] & cbit) == cbit) {			/* ビット位置の判定 */
				draw_point((p.x + tx), (p.y + ty), BGR(0x1F, 0x1F, 0x1F));	// 1なら白
			}
			else {
				draw_point((p.x + tx), (p.y + ty), BGR(0x00, 0x00, 0x00));	// 0なら黒
			}
			cbit = cbit >> 1;					/* 判定ビットを1ビット右へシフト */
		}
	}
}


/* locateで設定された位置にASCIIコードascii_numに相当する文字を表示する関数（2倍） */
void print_ascii_ex(byte ascii_num) {

	hword tx, ty,i,j;							/* 文字パターンの座標指定 */
	byte cbit;								/* ビット判定 */

	for (ty = 0; ty < 16; ty+=2) {				/* 文字パターンのy座標指定 */

		cbit = 0x80;						/* 判定ビットを8ビットの最上位にセット */

		for (tx = 0; tx < 16; tx+=2) {							/* 文字パターンのx座標指定 */
			if ((char8x8[ascii_num][ty-div(ty,2)] & cbit) == cbit) {			/* ビット位置の判定 */
				for (i = 0; i < 2; i++) {
					for (j = 0; j < 2; j++) {
						draw_point((pe.x + tx + j), (pe.y + ty + i), BGR(0x00, 0x00, 0x1F));	// 1なら青
					}
				}
			}
			else {
				for (i = 0; i < 2; i++) {
					for (j = 0; j < 2; j++) {
						draw_point((pe.x + tx + j), (pe.y + ty + i), BGR(0x00, 0x00, 0x00));	// 0なら黒
					}
				}
			}
			cbit = cbit >> 1;					/* 判定ビットを1ビット右へシフト */
		}
	}
}


/* 割り算の商を求める関数*/
hword div(hword dividened, hword divisor) {

	hword quotient = 0;			/* 商 */

	while (1) {
		if (dividened >= divisor) {
			dividened = dividened - divisor;
			quotient++;
		}
		else {
			break;
		}
	}
	return quotient;
}


/* 割り算の余りを求める関数*/
hword mod(hword dividened, hword divisor) {

	hword quotient = 0;			/* 商 */

	while (1) {
		if (dividened >= divisor) {
			dividened = dividened - divisor;
			quotient++;
		}
		else {
			break;
		}
	}
	return dividened;
}


/* 指定した座標(x, y) に指定した色colorの点を描画する関数 */
void draw_point(hword x, hword y, hword color) {

	hword *ptr;
	ptr = (hword *)VRAM;

	ptr = ptr + x + y * LCD_WIDTH;
	*ptr = color;

}

/* 時間を表示する関数 */
void display_time(hword val) {

	byte char_data[] = "0123456789";
	byte buf[9];
	hword tmp;
	int i;

	/* 入力値valの桁数設定 */
	i = 6;

	/* 文字列の最後にNULLコード挿入 */
	buf[i + 1] = 0;

	/* 最下位桁の文字（10で割った余り）を挿入し，入力値を10で割る */
	buf[6] = char_data[mod(val, 10)];
	val = div(val, 10);
	buf[5] = '.';
	buf[4] = char_data[mod(val, 10)];
	val = div(val, 10);
	buf[3] = char_data[mod(val, 6)];
	val = div(val, 6);
	buf[2] = ':';
	buf[1] = char_data[mod(val, 10)];
	val = div(val, 10);
	buf[0] = char_data[mod(val, 6)];
	val = div(val, 6);

	/* 文字列全体を表示 */
	prints(buf);

	return;
}


/* 乱数を求める関数 */
hword rand(hword randnum) {
	randnum = mod((randnum * 109 + 1021), 32768);
	return randnum;
}


/* 穴を掘る関数(ステージ1用） */
hword dig1(hword mx, hword my) {
	int dx, dy;
	int cnt = 0;

	/* 上下左右の4回ループ */
	while (cnt < 4) {
		dx = 0;
		dy = 0;
		switch (mod(rand(randnum)+cnt+4, 4)) {
			case 0:
				dx++;
				break;
			case 1:
				dx--;
				break;
			case 2:
				dy++;
				break;
			case 3:
				dy--;
				break;
		}
		/* 2マス先が迷路の範囲外または道の場合 */
		if (mx + dx*2 <= 0 || my + dy*2 <= 0 || mx + dx*2 >= MAZESIZE_X1 - 1 || my + dy*2 >= MAZESIZE_Y1 - 1 || maze1[my + dy*2][mx + dx*2] == 0) {
			cnt++;
		}
		/* 2マスずつ掘っていく */
		else if (maze1[my + dy * 2][mx + dx * 2] == 1) {
			maze1[my + dy][mx + dx] = 0;
			maze1[my + dy * 2][mx + dx * 2] = 0;
			mx = mx + dx * 2;
			my = my + dy * 2;
			cnt = 0;
			randnum = rand(randnum);

		}
	}
}

/* 穴を掘る関数(ステージ2用）*/
hword dig2(hword mx, hword my) {
	int dx, dy;
	int cnt = 0;

	/* 上下左右の4回ループ */
	while (cnt < 4) {
		dx = 0;
		dy = 0;
		switch (mod(rand(randnum) + cnt + 4, 4)) {
		case 0:
			dx++;
			break;
		case 1:
			dx--;
			break;
		case 2:
			dy++;
			break;
		case 3:
			dy--;
			break;
		}
		/* 2マス先が迷路の範囲外または道の場合 */
		if (mx + dx * 2 <= 0 || my + dy * 2 <= 0 || mx + dx * 2 >= MAZESIZE_X2 - 1 || my + dy * 2 >= MAZESIZE_Y2 - 1 || maze2[my + dy * 2][mx + dx * 2] == 0) {
			cnt++;
		}
		/* 2マスずつ掘っていく */
		else if (maze2[my + dy * 2][mx + dx * 2] == 1) {
			maze2[my + dy][mx + dx] = 0;
			maze2[my + dy * 2][mx + dx * 2] = 0;
			mx = mx + dx * 2;
			my = my + dy * 2;
			cnt = 0;
			randnum = rand(randnum);

		}
	}
}


/* 文字座標を塗りつぶす関数 */
void print_rectangle(hword mx, hword my) {
	int i, j;
	for (i = my * 8; i < my * 8 + 8; i++) {
		for (j = mx * 8; j < mx * 8 + 8; j++) {
			draw_point(j, i, BGR(0x1F, 0x00, 0x00));
		}
	}
}


