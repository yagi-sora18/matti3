#include"GameMainScene.h"
#include"DxLib.h"
#include"Stage.h"
#include"SceneManager.h"

/************************************

*�}�N����`

*************************************/

#define TIMELIMIT                    (3600 * 3)  //��������
#define NUMBER_IMAGE_MAX (10)    //�����摜��

/************************************

*�^��`

*************************************/



/************************************

*�ϐ���`

*************************************/
int GameScore;
int GameLevel;
int GameMission;
int GameTime;
int GameCount;   //����������Ȃ��悤�ɂ��邽�߂̃J�E���g
int ReStartFlag;


int NumberImage[NUMBER_IMAGE_MAX];     //�����p�摜

/************************************

*�v���g�^�C�v�錾

*************************************/

/************************************

*�Q�[�����C���摜:����������

*���@��:�Ȃ�

*�߂�l:�G���[���

*************************************/

int GameMainScene_Initialize(void)
{
	int ret = 0;
	int i;

	//�摜�Ǎ���
	LoadDivGraph("images/number.png", NUMBER_IMAGE_MAX, NUMBER_IMAGE_MAX, 1, 60, 120, NumberImage);


	//�X�e�[�W�@�\������
	ret = StageInitialize();
	

	//�G���[�`�F�b�N
	for (i = 0; i < NUMBER_IMAGE_MAX; i++)
	{
		if (NumberImage[i] == D_ERROR)
		{
			ret = D_ERROR;
			break;
		}
	}

	//�Q�[���v���C�����񂩂ǂ����H
	if (GameCount == 0)
	{
		GameScore = 0;          //�X�R�A�̏�����
		GameLevel = 1;          //�Q�[�����x���̏�����
		Set_StageMission(3);    //�~�b�V�����̏�����
		GameCount++;            //����̐ݒ�
	}
	else
	{
		GameLevel++;          //�Q�[�����x���̍X�V
		Set_StageMission(3);  //�~�b�V�����𑝂₷
	}
	GameTime = TIMELIMIT;     //�������Ԃ̏�����


	return ret;
}














