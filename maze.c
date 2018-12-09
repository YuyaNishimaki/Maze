#include "gba1.h"
#include "8x8ascii.h"

#define LCD_CHAR_WIDTH_EX	15		// width for character unit(2�{)
#define LCD_CHAR_HEIGHT_EX	10		// height for character unit(2�{)
#define CHAR_WIDTH_EX		16		// character width(2�{�j
#define CHAR_HEIGHT_EX	    16		// character height(2�{�j
#define MAZESIZE_X1			13  	// ���H�̕�(�X�e�[�W1�j
#define MAZESIZE_Y1			13		// ���H�̍���(�X�e�[�W1�j
#define	MAZEPRINT_X1		2		// ���H�̕`��J�nx���W(�X�e�[�W1�j
#define	MAZEPRINT_Y1		4		// ���H�̕`��J�ny���W(�X�e�[�W1�j
#define MAZESIZE_X2			15  	// ���H�̕�(�X�e�[�W2�j
#define MAZESIZE_Y2			15		// ���H�̍���(�X�e�[�W2�j
#define	MAZEPRINT_X2		1		// ���H�̕`��J�nx���W(�X�e�[�W2�j
#define	MAZEPRINT_Y2		3		// ���H�̕`��J�ny���W(�X�e�[�W2�j

/* �v���g�^�C�v�錾 */
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

/* �O���[�o���ϐ���` */
point p;
point pe;
hword randnum = 1; // ���������l
hword mx = 0;
hword my = 0;
hword cx = 0;
hword cy = 0;

/* ���H�p�̔z�� */
unsigned char maze1[MAZESIZE_Y1][MAZESIZE_X1];

/* ���@��J�n�ʒu�̌����W���i�[����z�� */
unsigned char MX1[MAZESIZE_X1];
unsigned char MY1[MAZESIZE_Y1];


/* ���H�p�̔z�� */
unsigned char maze2[MAZESIZE_Y2][MAZESIZE_X2];

/* ���@��J�n�ʒu�̌����W���i�[����z�� */
unsigned char MX2[MAZESIZE_X2];
unsigned char MY2[MAZESIZE_Y2];


int main(void) {

	/* ��ʏ��������[�`�� */
	*((unsigned short *)IOBASE) = 0xF03;

	/* �J�n��ʕ`�� */
	locate_ex(3, 4);
	prints_ex("MAZE START");
	locate(18,18);
	prints("PUSH START");
	
	/* �ϐ���` */
	hword key_in, flagST;

	/* �X�e�[�W1 */
	while (1) {
		/* �L�[���� */
		key_in = ~(*(hword *)KEY_STATUS)&KEY_ALL;

		int i,j;
		if (key_in == KEY_START) {
			for (i = 0; i < 10; i++) {
				/* STSRT10��_�� */
				locate_ex(3, 4);
				prints_ex("          ");
				locate_ex(3, 4);
				prints_ex("MAZE START");
			}
			/* ���F�ŉ�ʏ����� */
			for (i = 0; i < LCD_HEIGHT; i++) {
				for (j = 0; j < LCD_WIDTH; j++) {
					draw_point(j, i, BGR(0x00, 0x00, 0x00));
				}
			}
			flagST = 1;
			break;
		}
	}

	/* ���H������(���߂͂��ׂĕǂ�1�Ȃ�ǁA0�Ȃ瓹) */
	for (my = 0; my < MAZESIZE_Y1; my++) {
		for (mx = 0; mx < MAZESIZE_X1; mx++) {
			maze1[my][mx] = 1;
		}
	}

	/* �X�^�[�g�n�_ */
	maze1[0][1] = 0;
	/* �S�[���n�_ */
	maze1[MAZESIZE_Y1 - 1][MAZESIZE_X1 - 2] = 0;
	/* ���@�菉���J�n�ʒu */
	maze1[1][1] = 0;


	while (1) {
		int k = 0;
		/* �����@���ꏊ�𒲂ׂ�i�㉺���E2�}�X�悪�ǂ��ǂ������ׂ�j */
		for (my = 1; my < MAZESIZE_Y1 - 1; my += 2) {
			for (mx = 1; mx < MAZESIZE_X1 - 1; mx += 2) {
				if (maze1[my][mx] == 0) {
					/* �� */
					if (mx - 2 >= 0 && maze1[my][mx - 2] == 1) {
						MX1[k] = mx;
						MY1[k] = my;
						k++;
					}
					/* �E */
					else if (mx + 2 < MAZESIZE_X1 && maze1[my][mx + 2] == 1) {
						MX1[k] = mx;
						MY1[k] = my;
						k++;
					}
					/* �� */
					else if (my - 2 >= 0 && maze1[my - 2][mx] == 1) {
						MX1[k] = mx;
						MY1[k] = my;
						k++;
					}
					/* �� */
					else if (my + 2 < MAZESIZE_Y1 && maze1[my + 2][mx] == 1) {
						MX1[k] = mx;
						MY1[k] = my;
						k++;
					}
					/* ���H�̒[���ǂ��� */
					else if (mx == MAZESIZE_X1 - 2 && my == MAZESIZE_Y1 - 2) {
						break;
					}
				}
			}
		}
		/* �@��錊�̌�₪�Ȃ��ꍇ */
		if (k == 0) {
			break;
		}
		else {
			/* ���@��̊J�n�ʒu�������_���Ɍ��߁A�����@�� */
			mx = MX1[mod(rand(randnum), k)];
			my = MY1[mod(rand(randnum), k)];
			dig1(mx, my);
		}
	}
	
	/* ���H�`�� */
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
	/* ���H�̕��`�� */
	for (i = MAZEPRINT_X1 * 8 - 1; i <= (MAZEPRINT_X1 + MAZESIZE_X1) * 8; i++) {
		draw_point(i, MAZEPRINT_Y1 * 8 - 1, BGR(0x1F, 0x1F,0x00));
		draw_point(i, (MAZEPRINT_Y1 + MAZESIZE_Y1) * 8, BGR(0x1F, 0x1F, 0x00));
	}
	for (i = MAZEPRINT_Y1 * 8 - 1; i <= (MAZEPRINT_Y1 + MAZESIZE_Y1) * 8; i++) {
		draw_point(MAZEPRINT_X1 * 8 - 1, i, BGR(0x1F, 0x1F, 0x00));
		draw_point((MAZEPRINT_X1 + MAZESIZE_X1) * 8, i, BGR(0x1F, 0x1F, 0x00));
	}

	/* ��������`�� */
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
	
	/* �^�C�}�J�E���^�ݒ背�W�X�^ */
	*((unsigned short *)0x04000100) = 0xFF88;	// �^�C�}0 �J�E���^�ݒ�(���N���b�N1/120~3�b)
	*((unsigned short *)0x04000104) = 0xFF88;	// �^�C�}1 �J�E���^�ݒ�(���N���b�N1/120~2�b)
	*((unsigned short *)0x04000108) = 0xFF88;	// �^�C�}2 �J�E���^�ݒ�(���N���b�N1/120�b)
	*((unsigned short *)0x0400010C) = 0x0000;	// �^�C�}3 �J�E���^�ݒ�(���N���b�N1�b)

												/* �^�C�}���䃌�W�X�^ */
	*((unsigned short *)0x04000102) = 0x0000;	// �^�C�}0 ����ݒ�i�^�C�}OFF�C������OFF�C�J�X�P�[�hOFF�C�v���X�P�[���Ȃ��j
	*((unsigned short *)0x04000106) = 0x0084;	// �^�C�}1 ����ݒ�i�^�C�}ON�C������OFF�C�J�X�P�[�hON�C�v���X�P�[���Ȃ��j
	*((unsigned short *)0x0400010A) = 0x0084;	// �^�C�}2 ����ݒ�i�^�C�}ON�C������OFF�C�J�X�P�[�hON�C�v���X�P�[���Ȃ��j
	*((unsigned short *)0x0400010E) = 0x0084;	// �^�C�}3 ����ݒ�i�^�C�}ON�C������OFF�C�J�X�P�[�hON�C�v���X�P�[���Ȃ��j

	/* �o�H�X�^�[�g�n�_�`�� */
	mx = 3, my = 4;
	print_rectangle(mx, my);

	while (1) {

		/* �^�C�}3 �J�E���g�l�Ǎ��� */
		time = *((unsigned short *)0x0400010C);
		time = time - timejust;

		/* �L�[���� */
		key_in = ~(*(hword *)KEY_STATUS)&KEY_ALL;
		hword flag_key;

		/* �o�H�`�� */
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
			/* ���Ȃ�Ԃœh��Ԃ� */
			if (maze1[my-MAZEPRINT_Y1][mx- MAZEPRINT_X1] == 0) {
				print_rectangle(mx, my);
			}

			/* �N���A�����ꍇ */
			if (mx == 13 && my == 16) {
				locate(17, 14);
				prints("CREAR");
				*((unsigned short *)0x04000102) = 0x0000;
				locate(17, 17);
				prints("PUSH START");
				break;
			}

		}
			/* time �\���ʒu�ݒ� */
			locate(21, 15);
			display_time(time);
		
	}

	/* �X�e�[�W2 */
	randnum = 15;
	mx = 0, my = 0;
	cx = 0, cy = 0;
	while (1) {
		/* �L�[���� */
		key_in = ~(*(hword *)KEY_STATUS)&KEY_ALL;
		int i, j;
		if (key_in == KEY_START) {
			timejust += time;
			/* ���F�ŉ�ʏ����� */
			for (i = 0; i < LCD_HEIGHT; i++) {
				for (j = 0; j < LCD_WIDTH; j++) {
					draw_point(j, i, BGR(0x00, 0x00, 0x00));
				}
			}
			flagST = 1;
			break;
		}
	}

	/* ���H������(���߂͂��ׂĕǂ�1�Ȃ�ǁA0�Ȃ瓹) */
	for (my = 0; my < MAZESIZE_Y2; my++) {
		for (mx = 0; mx < MAZESIZE_X2; mx++) {
			maze2[my][mx] = 1;
		}
	}
	/* �X�^�[�g�n�_ */
	maze2[0][1] = 0;
	/* �S�[���n�_ */
	maze2[MAZESIZE_Y2 - 1][MAZESIZE_X2 - 2] = 0;
	/* ���@�菉���J�n�ʒu */
	maze2[1][1] = 0;


	while (1) {
		int k = 0;
		/* �����@���ꏊ�𒲂ׂ�i�㉺���E2�}�X�悪�ǂ��ǂ������ׂ�j */
		for (my = 1; my < MAZESIZE_Y2 - 1; my += 2) {
			for (mx = 1; mx < MAZESIZE_X2 - 1; mx += 2) {
				if (maze2[my][mx] == 0) {
					/* �� */
					if (mx - 2 >= 0 && maze2[my][mx - 2] == 1) {
						MX2[k] = mx;
						MY2[k] = my;
						k++;
					}
					/* �E */
					else if (mx + 2 < MAZESIZE_X2 && maze2[my][mx + 2] == 1) {
						MX2[k] = mx;
						MY2[k] = my;
						k++;
					}
					/* �� */
					else if (my - 2 >= 0 && maze2[my - 2][mx] == 1) {
						MX2[k] = mx;
						MY2[k] = my;
						k++;
					}
					/* �� */
					else if (my + 2 < MAZESIZE_Y2 && maze2[my + 2][mx] == 1) {
						MX2[k] = mx;
						MY2[k] = my;
						k++;
					}
					/* ���H�̒[���ǂ��� */
					else if (mx == MAZESIZE_X2 - 2 && my == MAZESIZE_Y2 - 2) {
						break;
					}
				}
			}
		}
		/* �@��錊�̌�₪�Ȃ��ꍇ */
		if (k == 0) {
			break;
		}
		else {
			/* ���@��̊J�n�ʒu�������_���Ɍ��߁A�����@�� */
			mx = MX2[mod(rand(randnum), k)];
			my = MY2[mod(rand(randnum), k)];
			dig2(mx, my);
		}
	}

	/* ���H�`�� */
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
	/* ���H�̕��`�� */
	for (l = MAZEPRINT_X2 * 8 - 1; l <= (MAZEPRINT_X2 + MAZESIZE_X2) * 8; l++) {
		draw_point(l, MAZEPRINT_Y2 * 8 - 1, BGR(0x1F, 0x1F, 0x00));
		draw_point(l, (MAZEPRINT_Y2 + MAZESIZE_Y2) * 8, BGR(0x1F, 0x1F, 0x00));
	}
	for (l = MAZEPRINT_Y2 * 8 - 1; l <= (MAZEPRINT_Y2 + MAZESIZE_Y2) * 8; l++) {
		draw_point(MAZEPRINT_X2 * 8 - 1, l, BGR(0x1F, 0x1F, 0x00));
		draw_point((MAZEPRINT_X2 + MAZESIZE_X2) * 8, l, BGR(0x1F, 0x1F, 0x00));
	}

	/* ��������`�� */
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

	/* �o�H�X�^�[�g�n�_�`�� */
	mx = 2, my = 3;
	print_rectangle(mx, my);

	while (1) {

		/* �^�C�}3 �J�E���g�l�Ǎ��� */
		time = *((unsigned short *)0x0400010C);
		time = time - timejust;

		/* �L�[���� */
		key_in = ~(*(hword *)KEY_STATUS)&KEY_ALL;
		hword flag_key;

		/* �o�H�`�� */
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
			/* ���Ȃ�Ԃœh��Ԃ� */
			if (maze2[my - MAZEPRINT_Y2][mx - MAZEPRINT_X2] == 0) {
				print_rectangle(mx, my);

			}
			
			/* �N���A�����ꍇ */
			if (mx == 14 && my == 17) {
				locate(18, 14);
				prints("CREAR");
				*((unsigned short *)0x04000102) = 0x0000;
				locate(18, 17);
				prints("PUSH START");
				break;
			}

		}
		/* time �\���ʒu�ݒ� */
		locate(22, 15);
		display_time(time);
	}

	while (1) {
		/* �L�[���� */
		key_in = ~(*(hword *)KEY_STATUS)&KEY_ALL;

		int m, n;
		if (key_in == KEY_START) {
			/* ���F�ŉ�ʏ����� */
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

/* �������W(0, 0)�`(29,19)�̈ʒu(cx, cy)����ʍ��W�ɕϊ� */
void locate(hword cx, hword cy) {
	if (cx < LCD_CHAR_WIDTH) {
		p.x = cx << 3;				/* x���W�̎w�� */
	}
	else {
		p.x = LCD_CHAR_WIDTH - 1;	/* x�̍ő�l��ݒ� */
	}
	if (cy < LCD_CHAR_HEIGHT) {
		p.y = cy << 3;				/* y���W�̎w�� */
	}
	else {
		p.y = LCD_CHAR_HEIGHT - 1;	/* y�̍ő�l��ݒ� */
	}
}


/* �������W(0, 0)�`(14,9)�̈ʒu(cx, cy)����ʍ��W�ɕϊ� */
void locate_ex(hword cx, hword cy) {
	if (cx < LCD_CHAR_WIDTH_EX) {
		pe.x = cx << 4;				/* x���W�̎w�� */
	}
	else {
		pe.x = LCD_CHAR_WIDTH_EX - 1;	/* x�̍ő�l��ݒ� */
	}
	if (cy < LCD_CHAR_HEIGHT_EX) {
		pe.y = cy << 4;				/* y���W�̎w�� */
	}
	else {
		pe.y = LCD_CHAR_HEIGHT_EX - 1;	/* y�̍ő�l��ݒ� */
	}
}


/* ������\���֐� */
void prints(byte *str) {

	while (*str) {
		print_ascii(*str++);
		p.x = p.x + CHAR_WIDTH;			/* �\���ʒu��1�����E�� */
	}
}


/* ������\���֐��i2�{�j */
void prints_ex(byte *str) {

	while (*str) {
		print_ascii_ex(*str++);
		pe.x = pe.x + CHAR_WIDTH_EX;			/* �\���ʒu��1�����E�� */
	}
}


/* locate�Őݒ肳�ꂽ�ʒu��ASCII�R�[�hascii_num�ɑ������镶����\������֐��i���j */
void print_ascii(byte ascii_num) {

	hword tx, ty;							/* �����p�^�[���̍��W�w�� */
	byte cbit;								/* �r�b�g���� */

	for (ty = 0; ty < 8; ty++) {				/* �����p�^�[����y���W�w�� */

		cbit = 0x80;						/* ����r�b�g��8�r�b�g�̍ŏ�ʂɃZ�b�g */

		for (tx = 0; tx < 8; tx++) {							/* �����p�^�[����x���W�w�� */
			if ((char8x8[ascii_num][ty] & cbit) == cbit) {			/* �r�b�g�ʒu�̔��� */
				draw_point((p.x + tx), (p.y + ty), BGR(0x1F, 0x1F, 0x1F));	// 1�Ȃ甒
			}
			else {
				draw_point((p.x + tx), (p.y + ty), BGR(0x00, 0x00, 0x00));	// 0�Ȃ獕
			}
			cbit = cbit >> 1;					/* ����r�b�g��1�r�b�g�E�փV�t�g */
		}
	}
}


/* locate�Őݒ肳�ꂽ�ʒu��ASCII�R�[�hascii_num�ɑ������镶����\������֐��i2�{�j */
void print_ascii_ex(byte ascii_num) {

	hword tx, ty,i,j;							/* �����p�^�[���̍��W�w�� */
	byte cbit;								/* �r�b�g���� */

	for (ty = 0; ty < 16; ty+=2) {				/* �����p�^�[����y���W�w�� */

		cbit = 0x80;						/* ����r�b�g��8�r�b�g�̍ŏ�ʂɃZ�b�g */

		for (tx = 0; tx < 16; tx+=2) {							/* �����p�^�[����x���W�w�� */
			if ((char8x8[ascii_num][ty-div(ty,2)] & cbit) == cbit) {			/* �r�b�g�ʒu�̔��� */
				for (i = 0; i < 2; i++) {
					for (j = 0; j < 2; j++) {
						draw_point((pe.x + tx + j), (pe.y + ty + i), BGR(0x00, 0x00, 0x1F));	// 1�Ȃ��
					}
				}
			}
			else {
				for (i = 0; i < 2; i++) {
					for (j = 0; j < 2; j++) {
						draw_point((pe.x + tx + j), (pe.y + ty + i), BGR(0x00, 0x00, 0x00));	// 0�Ȃ獕
					}
				}
			}
			cbit = cbit >> 1;					/* ����r�b�g��1�r�b�g�E�փV�t�g */
		}
	}
}


/* ����Z�̏������߂�֐�*/
hword div(hword dividened, hword divisor) {

	hword quotient = 0;			/* �� */

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


/* ����Z�̗]������߂�֐�*/
hword mod(hword dividened, hword divisor) {

	hword quotient = 0;			/* �� */

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


/* �w�肵�����W(x, y) �Ɏw�肵���Fcolor�̓_��`�悷��֐� */
void draw_point(hword x, hword y, hword color) {

	hword *ptr;
	ptr = (hword *)VRAM;

	ptr = ptr + x + y * LCD_WIDTH;
	*ptr = color;

}

/* ���Ԃ�\������֐� */
void display_time(hword val) {

	byte char_data[] = "0123456789";
	byte buf[9];
	hword tmp;
	int i;

	/* ���͒lval�̌����ݒ� */
	i = 6;

	/* ������̍Ō��NULL�R�[�h�}�� */
	buf[i + 1] = 0;

	/* �ŉ��ʌ��̕����i10�Ŋ������]��j��}�����C���͒l��10�Ŋ��� */
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

	/* ������S�̂�\�� */
	prints(buf);

	return;
}


/* ���������߂�֐� */
hword rand(hword randnum) {
	randnum = mod((randnum * 109 + 1021), 32768);
	return randnum;
}


/* �����@��֐�(�X�e�[�W1�p�j */
hword dig1(hword mx, hword my) {
	int dx, dy;
	int cnt = 0;

	/* �㉺���E��4�񃋁[�v */
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
		/* 2�}�X�悪���H�͈̔͊O�܂��͓��̏ꍇ */
		if (mx + dx*2 <= 0 || my + dy*2 <= 0 || mx + dx*2 >= MAZESIZE_X1 - 1 || my + dy*2 >= MAZESIZE_Y1 - 1 || maze1[my + dy*2][mx + dx*2] == 0) {
			cnt++;
		}
		/* 2�}�X���@���Ă��� */
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

/* �����@��֐�(�X�e�[�W2�p�j*/
hword dig2(hword mx, hword my) {
	int dx, dy;
	int cnt = 0;

	/* �㉺���E��4�񃋁[�v */
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
		/* 2�}�X�悪���H�͈̔͊O�܂��͓��̏ꍇ */
		if (mx + dx * 2 <= 0 || my + dy * 2 <= 0 || mx + dx * 2 >= MAZESIZE_X2 - 1 || my + dy * 2 >= MAZESIZE_Y2 - 1 || maze2[my + dy * 2][mx + dx * 2] == 0) {
			cnt++;
		}
		/* 2�}�X���@���Ă��� */
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


/* �������W��h��Ԃ��֐� */
void print_rectangle(hword mx, hword my) {
	int i, j;
	for (i = my * 8; i < my * 8 + 8; i++) {
		for (j = mx * 8; j < mx * 8 + 8; j++) {
			draw_point(j, i, BGR(0x1F, 0x00, 0x00));
		}
	}
}


