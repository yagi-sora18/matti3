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


















