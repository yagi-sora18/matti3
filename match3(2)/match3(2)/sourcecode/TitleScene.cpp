#include"TitleScene.h"
#include"DxLib.h"
#include"InputControl..h"
#include"SceneManager.h"

/*******************************
 
 *�}�N����`

********************************/

/*******************************

 *�^��`

********************************/


/*******************************

 *�ϐ��錾

********************************/

int TitleImage;
int TitleBGM;


/*******************************

 *�v���g�^�C�v�錾

********************************/


/*******************************

 *�^�C�g����ʁF����������
 * ���@���F�Ȃ�
 * �߂�l�F�G���[���

********************************/

int TitleScene_Initialize(void)
{
	int ret = 0;
	//�摜�ǂݍ��ݏ���
	TitleImage = LoadGraph("images/title.png");
	//�����ǂݍ��ݏ���
	TitleBGM = LoadSoundMen("sounds/title_bgm.mp3");
}

