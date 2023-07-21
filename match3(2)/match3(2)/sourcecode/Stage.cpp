#include"Stage.h"
#include"DxLib.h"
#include"InputControl.h"

/************************

* �}�N����`

*************************/

#define HEIGHT            (12)     //�u���b�N�z�u�T�C�Y(�����j
#define WIDTH             (12)     //�u���b�N�z�u�T�C�Y�i���j
#define BLOCKSIZE         (48)     //�u���b�N�T�C�Y
#define BLOCK_IMAGE_MAX   (10)     //�u���b�N�摜��

#define ITEM_MAX          (8)     //�A�C�e���ő吔


#define SELECT_CURSOR     (0)     
#define NEXT_CURSOR       (1)     
#define TMP_CURSOR        (2)     

/**************************************

*�^��`

**************************************/

typedef struct
{
	int flg;
	int x, y;
	int width, height;
	int iamge;
	int backup;
}T_Object;

typedef struct
{

	int x;
	int y;

}T_CURSOR;


enum
{

	E_NONE,
	E_ONCE,
	E_SECOND
};

/**************************************

*�ϐ��錾

**************************************/

T_Object Block[HEIGHT][WIDTH];          //�u���b�N�I�u�W�F�N�g�f�[�^
T_CURSOR Select[3];//�Z���N�g�J�[�\�����W
int Item[ITEM_MAX];
int ClickStatus;
int Stage_State;
int Stage_Mission;
int Stage_Score;
int ClearFlag;


int BlockImage[BLOCK_IMAGE_MAX];    //�u���b�N�摜
int StageImage;                         //�w�i�p�摜

int ClickSE;                        //�N���b�NSE
int FadeOutSE;                           //�t�F�[�h�A�E�gSE
int MoveBlockSE;                    //�u���b�N�ړ�SE

/******************************

*�v���g�^�C�v�錾

********************************/

int combo_check(int x, int y);
void combo_check_h(int y, int x, int* cnt, int* col);
void combo_check_w(int y, int x, int* cnt, int* col);
void save_Block(void);
void restore_block(void);


/******************************************

*�X�e�[�W����@�\�F����������

*�����F�Ȃ�
 
*�߂�l�F�G���[���

*******************************************/

int StageInitialize(void)
{
	int ret = 0;
	int i;

	//�摜�ǂݍ���
	LoadDivGraph("images/block.png", BLOCK_IMAGE_MAX, BLOCK_IMAGE_MAX, 1, BLOCKSIZE, BLOCKSIZE, BlockImage);


	//�����ǂݍ���
	ClickSE = LoadSoundMem("sounds/click_se.mp3");
	FadeOutSE = LoadSoundMem("sounds/fadeout_se.mp3");
	MoveBlockSE = LoadSoundMem("sounds/moveblock_se.mp3");

	//�u���b�N��������
	CreateBlock();


	ClickStatus = E_NONE;
	Stage_State = 0;
	Stage_Score = 0;
	ClearFlag = FALSE;


	for (i = 0; i < 3; i++)
	{
		Select[i].x = 0;
		Select[i].y = 0;
	}

	//�G���[�`�F�b�N
	for (i = 0; i < BLOCK_IMAGE_MAX; i++)
	{
		if (BlockImage[i] == -1)
		{
			ret = -1;
			break;
		}
	}

	if (StageImage == -1)
	{
		ret = -1;
	}
	if (FadeOutSE == -1)
	{
		ret = -1;
	}
	if (MoveBlockSE == -1)
	{
		ret = -1;
	}


	return ret;
}

/*********************************************

*�X�e�[�W����@�\�F�X�e�[�W�̕`��

*�����F�Ȃ�

*�߂�l�F�Ȃ�

**********************************************/

void StageDraw(void) {
	DrawGraph(0, 0, StageImage, FALSE);

	//�A�C�e���̎擾����`��
	for (int i = 0; i < ITEM_MAX; i++)
	{
		DrawRotaGraph(540, 245 + i * 30, 0.5f, 0, BlockImage[i + 1], TRUE, 0);
		DrawFormatString(580, 235 + i * 30, 0xffffff, "%3d", Item[i]);
	}
	//�u���b�N��`��
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (Block[i][j].flg == TRUE && Block[i][j].image != NULL)
			{
				DrawGraph(Block[i][j].x, Block[i][j].y,
					BlockImage[Block[i][j].image], TRUE);
			}
		}
	}

	//�I���u���b�N��`��
	DrawGraph(Select[SELECT_CURSOR].x * BLOCKSIZE, Select[SELECT_CURSOR].y *
		BLOCKSIZE, BlockImage[9], TRUE);
	if (ClickStatus != E_NONE)
	{
		DrawGraph(Select[NEXT_CURSOR].x * BLOCKSIZE, Select[NEXT_CURSOR].y *
			BLOCKSIZE, BlockImage[9], TRUE);
	}

	//�~�b�V������`��
	SetFontSize(20);
	DrawFormatString(590, 211, GetColor(255, 255, 255), "%3d", Stage_Mission);

	//�A�C�e���̎擾����`��
	for (int i = 0; i < ITEM_MAX; i++)
	{
		DrawRotaGraph(540, 245 + i * 30, 0.5f, 0, BlockImage[i + 1], TRUE, 0);
		DrawFormatString(580, 235 + i * 30, GetColor(255, 255, 255), "%3d",
			Item[i]);
	}
}


/*******************************************

*�X�e�[�W����@�\:�u���b�N��������
 
*���@��:�Ȃ�

* �߂�l:�Ȃ�

********************************************/

void CreateBlock(void)
{
	int Check = 0;
	int i, j;


	do

	{
		Check = 0;
		for (i = 0; i < HEIGHT; i++)
		{
			for (j = 0; j < WIDTH; j++)
			{
				if (j == 0 || j == WIDTH - 1 || i == HEIGHT - 1 || i == 0)
				{
					Block[i][j].flg = FLASE;
					Block[i][j].image = NULL;
				}
				else
				{
					Block[i][j].flg = TRUE;
					Block[i][j].x = (j - 1) * BLOCKSIZE;
					Block[i][j].y = (i - 1) * BLOCKSIZE;
					Block[i][j].width = BLOCKSIZE;
					Block[i][j].height = BLOCKSIZE;
					Block[i][j].image = GetRand(7) + 1;//1�`8�̗���
				}

			}
		}


		/*for(i = 1;i < HEIGHT - 1;i++)
		{
		for(j = 1;j < WIDTH - 1;j++)
		{
		if(Block[i][j].image == NULL)
		{
		Block[i][j].image = GetRand(7) + 1;
		}
	}
}*/

//�u���b�N�A���`�F�b�N
		for (i = 1; i, HEIGTH - 1; i++)
		{
			for (j = 1; j < WIDTH - 1; i++)
			{
				Check += combo_check(i, j);
			}
		}
	} while (Check != 0);


	for (i = 0; i < ITEM_MAX; i++)
	{
		Item[i] = 0;
	}
}

/*******************************

*�X�e�[�W����@�\:�u���b�N�I������
 
* ���@��:�Ȃ�
 
* �߂�l:�Ȃ�

********************************/

void SelectBlock(void)

{
	int TmpBlock;
	int Result;

	//�J�[�\�����W�̎擾
	Select[SELECT_CURSOR].x = GetMousePositionX() / BLOCKSIZE;
	Select[SELECT_CURSOR].y = GetMousePositionY() / BLOCKSIZE;

	//�I���u���b�N�͈̔͂𐧌�
	if (Select[SELECT_CURSOR].x < 0)
	{
		Select[SLECT_CURSOR].x = 0;
	}
	if (Select[SELECT_CURSOR].x > WIDTH - 3)
	{
		Select[SELECT_CURSOR].x = WIDTH - 3;
	}
	if (Select[SELECT_CURSOR].y < 0)
	{
		Select[SELECT_CURSOR].y = 0;
	}
	if (Select[SELECT_CURSOR].y > HEIGHT - 3)
	{
		Select[SELECT_CURSOR].y = HEIGHT - 3
	}

	//�N���b�N�Ńu���b�N��I��
	if (GetKeyFlg(MOUSE_INPUT_LEFT)) {

		//�N���b�N���ʉ�
		PlaySoundMem(ClickSE, DX_PLAYTYPE_BACK);


		if (ClickStatus == E_NONE) {
			Select[NEXT_CURSOR].x = Select[SELECT_CURSOR].x;
			Select[NEXT_CURSOR].y = Select[SELECT_CURSOR].y;
			ClickStatus = E_ONCE;
		}
		else if (ClickStatus == E_ONCE &&
			((abs(Select[NEXT_CURSOR].x - Select[SELECT_CURSOR].x)
				== 1 &&
				(abs(Select[NEXT_CURSOR].y - Select[SELECT_CURSOR].y)
					== 0)) ||
				(abs(Select[NEXT_CURSOR].x - Select[SELECT_CURSOR].x)
					== 0 &&
					(abs(Select[NEXT_CURSOR].y - Select[SELECT_CURSOR].y) ==
						1)))
		{
			Select[TMP_CURSOR].x = Select[SELECT_CURSOR].x;
				Select[TMP_CURSOR].y = Select[SELECT_CURSOR].y;
			ClickStatus = E_SECOND;
		}
	}

	//�I���u���b�N����������
	if (ClickStatus == E_SECOND)
	{
		TmpBlock = Block[Select[NEXT_CURSOR].y + 1][Select[NEXT_CURSOR].x +
			1].image;
		Block[Select[NEXT_CURSOR].y + 1][Select[NEXT_CURSOR].x + 1].image =
			Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image;
		Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image =
			TmpBlock;

		//�A�����R�ȏォ���ׂ�B
		Result = 0;
		Result += combo_check(Select[NEXT_CURSOR].y + 1,
			Select[NEXT_CURSOR].x + 1);
		Result += combo_check(Select[TMP_CURSOR].y + 1,
			Select[TMP_CURSOR].x + 1);

		//�A�����R�����Ȃ�I���u���b�N�����ɖ߂�
		if (Result == 0)
		{

			int TmpBlock = Block[Select[NEXT_CURSOR].y +
				1][Select[NEXT_CURSOR].x + 1].image;
			Block[Select[NEXT_CURSOR].y + 1][Select[NEXT_CURSOR].x +
				1].image = Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image;
			Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x +
				1].image = TmpBlock;
		}
		else
		{

			//�A�����R�ȏ�Ȃ�u���b�N�������u���b�N�ړ������ֈڍs����
			Stage_State = 1;
		}

		//���ɃN���b�N�ł���悤��ClockFlag���O�ɂ���
		ClickStatus = E_NONE;
	}
}

/**************************

*�X�e�[�W����@�\:�t�F�[�h�A�E�g����

* ���@��:�Ȃ�

*�߂�l:�Ȃ�

***************************/

void FadeOutBlock(void)
{
	static int BlendMode = 255;
	int i, j;

	//�t�F�[�h�A�E�g���ʉ�
	if (CheckSoundMem(FadeOutSE) == 0)
	{
		PlaySoundMem(FadeOutSE, DX_PLAYTYPE_BACK);
	}

	//�`�惂�[�h���A���t�@�u�����h�ɂ���
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, BlendMode);
	for (i = 1; i < HEIGHT - 1; i++)
	{
		for (j = 1; j < WIDTH - 1; j++)
		{
			if (Block[i][j].image == 0)
			{
				DrawGraph(Block[i][j].x, Block[i][j].y,
					BlockImage[Block[i][j].backup], TRUE);
			}
		}
	}

	//�`�惂�[�h���m�[�u�����h�ɂ���
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	BlendMode -= 5;


	if (BlendMode == 0)
	{
		BlendMode = 255;
		Stage_State = 2;
		StopSoundMem(FadeOutSE);
	}
}

/**************************

*�X�e�[�W����@�\:�u���b�N�ړ�����

* ���@��:�Ȃ�

*�߂�l:�Ȃ�

***************************/

void MoveBlock(void)
{
	int i, j, k;
	//�u���b�N�ړ����ʉ�
	PlaySoundMem(MoveBlockSE, DX_PLAYTYPE_BACK);


	//���ֈړ����鏈��
	for (i = 1; i < WIDTH - 1; i++)
	{
		for (j = 1; j < WIDTH - 1; i++)
		{
			if (Block[i][j].image == 0)
			{
				for (k = i; k > 0; k--)
				{
					Block[k][j].image = Block[k - 1][j].image;
					Block[k - 1][j].image = 0;
				}
			}
		}
	}

	//��̃u���b�N�𐶐����鏈��
	for (i = 1; i < HEIGHT - 1; i++)
	{
		for (j = 1; j < WIDTH - 1; j++)
		{
			if (Block[i][j].image == 0)
			{
				Block[i][j].image = GetRand(7) + 1;
			}
		}
	}

	//�A���`�F�b�N�ֈڍs����
	Stage_State = 3;
}

/**************************

*�X�e�[�W����@�\:�A���`�F�b�N����

* ���@��:�Ȃ�

*�߂�l:�Ȃ�

***************************/

void CheckBlock(void)
{
	int Result = 0;
	int i, j;


	//�u���b�N�A���`�F�b�N
	for (i = 1; i < HEIGHT - 1; i++)
	{
		Result += combo_check(i, j);
	}
}

//�A�����Ȃ��Ȃ�΃u���b�N�I����
//�����łȂ���΃u���b�N�ړ��ֈڍs���ĘA���`�F�b�N���p������
if(Result == 0)
{
	//�N���A�`�F�b�N�����ֈڍs����B
	Stage_State = 4;
}
else
{
	//�A�����R�ȏ�Ȃ�u���b�N�������u���b�N�ړ������ֈڍs����
	Stage_State = 1;
 }
}


/**************************

*�X�e�[�W����@�\:�N���A�����`�F�b�N����

* ���@��:�Ȃ�

*�߂�l:�Ȃ�

*���@�l:�N���A�����t���O���O�Ƃ��A�e�X�N�[���̍폜�u���b�N��

*�@�@�@�@�@�@���x�������������Ȃ�������`�F�b�N�����𒆒f���ăQ�[���𑱍s����B

***************************/

void CheckClear(void)
{



	int i;

	for (i = 0; i < ITEM_MAX; i++)
	{
		if (Item[i] >= Stage_Mission)
		{
			ClearFlag = TRUE;
			break;
		}
	}
	if (ClearFlag != TRUE)
	{
		Stage_State = 0;
	}
}

/**************************

*�X�e�[�W����@�\:�X�e�[�W�X�e�[�^�X���擾����

* ���@��:�Ȃ�

*�߂�l:�X�e�[�W�X�e�[�^�X���

***************************/

int Get_StageState(void)
{
	return Stage_State;
}

/**************************

*�X�e�[�W����@�\:�~�b�V�������擾����

* ���@��:�Ȃ�

*�߂�l:�~�b�V�������N���A���Ă��邩

***************************/

int Get_StageClearFlag(void)
{
	return ClearFlag;
}

/**************************

*�X�e�[�W����@�\:�~�b�V�������擾����

* ���@��:�Ȃ�

*�߂�l:�~�b�V�������N���A���Ă��邩

***************************/

int Get_StageScore(void)
{
	return Stage_Score;
}

/**************************

*�X�e�[�W����@�\:�~�b�V�������擾����

* ���@��:���~�b�V�����ɕK�v�Ȑ��l

*�߂�l:�Ȃ�

***************************/

void Set_StageMission(int mission)
{
	Stage_Mission += mission;
}


/**************************

*�X�e�[�W����@�\:�A���`�F�b�N����

*���@��1:�u���b�NY�}�X

*���@��2:�u���b�NX�}�X 

*�߂�l:�A���L��(0:����  1:�L��)

***************************/

int combo_check(int y, int x)
{
	int ret = FALSE;

	//�c�����`�F�b�N
	int CountH = 0;
	int ColorH = 0;
	save_block();
	combo_check_h(y, x & CountH, &ColorH);
	if (CountH < 3)
	{
		restore_block();      //3�����Ȃ�߂�
	}

	//�������̃`�F�b�N
	int CountW = 0;
	int ColorW = 0;
	save_block();
	combo_check_w(y, x, &CountW, &ColorW);
	if (CountW < 3)
	{
		restore_block();
	}

	//3�ȏ����ł��邩�H
	if ((CountH >= 3 || CountW >= 3))
	{
		if (CountH >= 3)
		{
			Item[ColorH - 1] += CountH;
			Stage_Score += CountH * 10;
		}
		if (CountW >= 3)
		{
			Item[Color - 1] += CountW;
			Stage_Score += CountW * 10;
		}
		ret = TRUE;
	}


	return ret;
}

/**************************

*�X�e�[�W����@�\:�A���`�F�b�N�����i�c�����j

* ���@��:�Ȃ�

*�߂�l:�A���L���i0:�����@�@1:�L��)

***************************/

void combo_check_h(int y,int x,int*cnt,int*col)
{
	int Color = 0;
	//�Ώۂ̃u���b�N���O�g�̏ꍇ��return�ŏ����𔲂���

	if (Block[y][x].image == 0)
	{
		return;
	}
	*col = Block[y][x].image;
	Color = Block[y][x].image;
	Block[y][x].image = 0;
	(*cnt)++;

	if (Block[y + 1][x].image == Color)
	{
		combo_check_h(y + 1, x, cnt, col);
	}







}






