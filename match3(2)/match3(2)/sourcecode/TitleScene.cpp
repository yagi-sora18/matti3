#include"TitleScene.h"
#include"DxLib.h"
#include"InputControl..h"
#include"SceneManager.h"

/*******************************
 
 *マクロ定義

********************************/

/*******************************

 *型定義

********************************/


/*******************************

 *変数宣言

********************************/

int TitleImage;
int TitleBGM;


/*******************************

 *プロトタイプ宣言

********************************/


/*******************************

 *タイトル画面：初期化処理
 * 引　数：なし
 * 戻り値：エラー情報

********************************/

int TitleScene_Initialize(void)
{
	int ret = 0;
	//画像読み込み処理
	TitleImage = LoadGraph("images/title.png");
	//音源読み込み処理
	TitleBGM = LoadSoundMen("sounds/title_bgm.mp3");
}

