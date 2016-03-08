#include"..\ucos-ii\includes.h"
#include"..\ucos-ii\add\osaddition.h"
#include"..\inc\drv.h"
#include<string.h>
#include<math.h>

#pragma import(__use_no_semihosting_swi)

#define ZLG7289_CS				(0x20)	//GPB5
#define ADS7843_CTRL_START		0x80
#define ADS7843_GET_X			0x50
#define ADS7843_GET_Y			0x10
#define ADS7843_CTRL_12MODE	0x0
#define ADS7843_CTRL_8MODE	0x8
#define ADS7843_CTRL_SER		0x4
#define ADS7843_CTRL_DFR		0x0
#define ADS7843_CTRL_DISPWD	0x3	// Disable power down
#define ADS7843_CTRL_ENPWD	0x0	// enable power down
#define SIO_START	0x08	//SIO Start
#define ADS7843_PIN_CS		(1<<6)	//GPF6
#define ADS7843_CMD_X	(ADS7843_CTRL_START|ADS7843_GET_X|ADS7843_CTRL_12MODE|ADS7843_CTRL_DFR|ADS7843_CTRL_ENPWD)
//采样x轴电压值，数据为12位，参考电压输入模式为差分模式，允许省电模式
#define ADS7843_CMD_Y	(ADS7843_CTRL_START|ADS7843_GET_Y|ADS7843_CTRL_12MODE|ADS7843_CTRL_DFR|ADS7843_CTRL_ENPWD)

#define ZLG7289_ENABLE()		do{Zlg7289SIOBand=rSBRDR;	Zlg7289SIOCtrl=rSIOCON;	\
						rSIOCON=0x31;	rSBRDR=0xff;	rPDATB&=(~ZLG7289_CS);}while(0)
#define ZLG7289_DISABLE()		do{rPDATB|=ZLG7289_CS; rSBRDR=Zlg7289SIOBand; rSIOCON=Zlg7289SIOCtrl;}while(0)

#define ZLG7289_CMD_DATA0		0x80	//下载数据且按方式0 译码
#define ZLG7289_CMD_DATA1		0xC8	//下载数据且按方式1 译码
#define ZLG7289_CMD_DATA3		0x90	//下载数据但不译码


#define ZLG7289_CMD_HIDE		0x98	//消隐控制


const U32 StartGame_Button_ID 	 =  99;
const U32 NewGame_Button_ID 	 =  100;
const U32 Pause_Button_ID    	 =  101;
const U32 Left_Button_ID  		 =  102;
const U32 Right_Button_ID   	 =  103;
const U32 Down_Button_ID 		 =  104;
const U32 Rotate_Button_ID  	 =  105;
const U32 MainDraw_Wnd_ID  	 =  110;
const U32 PieceDraw_Wnd_ID    =  115;
const U32 ScoreDraw_Wnd_ID    =  120;
const U32 TimeDraw_Wnd_ID    =  125;
const int ROWCOUNT				 =	21;
const int COLCOUNT 				 =	10;
extern int C_TchScr_Xmax=1876;
extern int C_TchScr_Xmin=269;
extern int C_TchScr_Ymax=229;
extern int C_TchScr_Ymin=1725;//触摸屏返回电压值范围
extern int Zlg7289SIOBand;
extern int Zlg7289SIOCtrl;

int oldx=0, oldy=0;			//调整坐标用，这是老坐标位置

U16 ch1[]={'W','E','L','C','O','M','E',' ','T','O',' ','R','U','S','S','I','A',0};

U32  Chose_Color;
U32  COLOR[10][21];
U32 Rand_Color();
int Shape = 7;
PDC pdc;
structRECT		NewGame_Button_RECT,
				Pause_Button_RECT,
				Left_Button_RECT,
				Right_Button_RECT,
				Down_Button_RECT,
				Rotate_Button_RECT,
				MainDraw_Wnd_RECT,
				PieceDraw_Wnd_RECT,
				ScoreDraw_Wnd_RECT,
				TimeDraw_Wnd_RECT,
				StartGame_Button_RECT;

structRECT		*pNewGame_Button_RECT,
				*pPause_Button_RECT,
				*pLeft_Button_RECT,
				*pRight_Button_RECT,
				*pDown_Button_RECT,
				*pRotate_Button_RECT,
				*pMainDraw_Wnd_RECT,
				*pPieceDraw_Wnd_RECT,
				*pScoreDraw_Wnd_RECT,
				*pTimeDraw_Wnd_RECT,
                *pStartGame_Button_RECT;
int 			BoardArray[10][21];          //定义一个10x20的游戏区域

BOOLEAN			boardinitialized = FALSE,
				StillRunning     = TRUE;
BOOLEAN         Paused			 = FALSE,
				kludgeflag       = FALSE,
				forceredraw      = FALSE,
				GameOver		 = FALSE;
				
int             CurrentBlockX	 	 =0,
				CurrentBlockY	 	 =0,
				CurrentBlockColour	 =0,
				CurrentBlockShape,
				CurrentBlockAngle;

int             NextBlockX	 	 =0,
				NextBlockY	 	 =0,
				NextBlockColour	 =0,
				NextBlockShape,
				NextBlockAngle;

int             LastX,
				LastY,
				LastAngle,
				score = 0,
				highscore = 10,
				delay = 500,
				time = 0;
int BlockInfo[7][4][4][2]=
{
	{
		{{0,0},{1,0},{0,1},{1,1}},
		{{0,0},{1,0},{0,1},{1,1}},
		{{0,0},{1,0},{0,1},{1,1}},
		{{0,0},{1,0},{0,1},{1,1}}
	},
	{		
		{{0,0},{-1,0},{0,1},{0,-1}},
		{{0,0},{0,1},{1,0},{0,-1}},
		{{0,0},{0,1},{1,0},{-1,0}},
		{{0,0},{1,0},{0,-1},{-1,0}}
	},
	{
		{{0,0},{-1,0},{1,0},{2,0}},
		{{0,0},{0,-1},{0,1},{0,2}},
		{{0,0},{-1,0},{1,0},{2,0}},
		{{0,0},{0,-1},{0,1},{0,2}}
	},
	{
		{{0,0},{0,-1},{0,1},{1,1}},
		{{0,0},{1,0},{-1,0},{-1,1}},
		{{0,0},{0,1},{0,-1},{-1,-1}},
		{{0,0},{-1,0},{1,0},{1,-1}}
	},
	{
		{{0,0},{0,-1},{0,1},{-1,1}},
		{{0,0},{1,0},{-1,0},{-1,-1}},
		{{0,0},{0,1},{0,-1},{1,-1}},
		{{0,0},{-1,0},{1,0},{1,1}}
	},
	{
		{{0,0},{-1,0},{0,-1},{1,-1}},
		{{0,0},{0,-1},{1,0},{1,1}},
		{{0,0},{-1,0},{0,-1},{1,-1}},
		{{0,0},{0,-1},{1,0},{1,1}},
	},
	{
		{{0,0},{1,0},{0,-1},{-1,-1}},
		{{0,0},{0,1},{1,0},{1,-1}},
		{{0,0},{1,0},{0,-1},{-1,-1}},
		{{0,0},{0,1},{1,0},{1,-1}},
	}			
};
/***********************************/
int update(void);
int DrawMainFrame(PDC);
int SetScoreText(PDC);
int SetTimeText(PDC);
int dopaint(PDC);
int realdopaint(PDC);
BOOLEAN newGame(void);
BOOLEAN	pause(void);
BOOLEAN moveLeft(void);
BOOLEAN	moveRight(void);
BOOLEAN	drop(void);
BOOLEAN	rotateClockwise(void);

int CheckRow(PDC);
int makeNewPiece(void);
int DisplayPiece(PDC);
int getPieceColour(void);
int getPieceX(int);
int getPieceY(int);

extern   U8  isConfigsysLoad;
extern   U8  sysCONFIG[];
extern   U32 ConfigSYsdata[];



OS_STK Main_Stack[STACKSIZE*8]		={0,};
void Main_Task(void*Id);
#define Main_Task_Prio               12



OS_STK AutoDrop_Task_Stack[STACKSIZE*8]		={0,};
void AutoDrop_Task(void*Id);
#define AutoDrop_Task_Prio              20


OS_STK Audio_Task_Stack[STACKSIZE*8]		={0,};
void Audio_Task(void*Id);
#define Audio_Task_Prio              30

OS_STK Welcome_Stack[STACKSIZE*8]		={0,};
void Welcome_Task(void*Id);
#define Welcome_Task_Prio               40

OS_STK Led_Flash_Stack[STACKSIZE]		={0,};//LED闪烁任务
void Led_Flash_Task(void*Id);
#define Led_Flash_Prio               56

OS_EVENT *Nand_Rw_Sem;
OS_EVENT *Uart_Rw_Sem;


void C_TchScr_GetScrXY(int *x, int *y, U8 bCal)
{//获得触摸点坐标
	unsigned int temp;

	rPDATF&=~ADS7843_PIN_CS;//打开ADS7843

	SendSIOData(ADS7843_CMD_X);//发送读取x电压值控制字
	SendSIOData(0);//等待8个时钟节拍，因为完成一转换需要16个时钟
	temp=ReadSIOData();//读取采样值高8位
	SendSIOData(ADS7843_CMD_X);
	temp<<=8;
	temp|=ReadSIOData();//读取低8位并与以前高8位组成16位数据
	*x=(temp>>4);//去掉低4位，保留12位有效数据

	SendSIOData(0);
	temp=ReadSIOData();
	SendSIOData(ADS7843_CMD_X);
	temp<<=8;
	temp|=ReadSIOData();
	*x+=(temp>>4);

	SendSIOData(0);
	temp=ReadSIOData();
	SendSIOData(ADS7843_CMD_X);
	temp<<=8;
	temp|=ReadSIOData();
	*x+=(temp>>4);

	SendSIOData(0);
	temp=ReadSIOData();
	SendSIOData(ADS7843_CMD_Y);
	temp<<=8;
	temp|=ReadSIOData();
	*x+=(temp>>4);
	*x>>=2;		//采样4次取平均值
	//读取y电压值
	SendSIOData(0);
	temp=ReadSIOData();
	SendSIOData(ADS7843_CMD_Y);
	temp<<=8;
	temp|=ReadSIOData();
	*y=(temp>>4);

	SendSIOData(0);
	temp=ReadSIOData();
	SendSIOData(ADS7843_CMD_Y);
	temp<<=8;
	temp|=ReadSIOData();
	*y+=(temp>>4);
	
	SendSIOData(0);
	temp=ReadSIOData();
	SendSIOData(ADS7843_CMD_Y);
	temp<<=8;
	temp|=ReadSIOData();
	*y+=(temp>>4);

	SendSIOData(0);
	temp=ReadSIOData();
	SendSIOData(0);
	temp<<=8;
	temp|=ReadSIOData();
	*y+=(temp>>4);
	*y>>=2;		

	rPDATF|=ADS7843_PIN_CS;//关闭ADS7843

	if(bCal){//对采样结果进行转换
		*x=(*x-C_TchScr_Xmin)*LCDWIDTH/(C_TchScr_Xmax-C_TchScr_Xmin);
		*y=(*y-C_TchScr_Ymin)*LCDHEIGHT/(C_TchScr_Ymax-C_TchScr_Ymin);
	}
}

void Led_Flash_Task(void*Id)
{
	unsigned char led_state;
	Uart_Printf(0,"\n10");
	for(;;)
	{
		Led_Display(led_state);
		led_state=~led_state;
		OSTimeDly(250);
	}
}
void initOSGUI()
{
	initOSMessage();
	initOSList();
	initOSDC();
	initOSCtrl();
	initOSFile();
}

//void ARMTargetInit(void)
//{

//}

void Welcome(PDC dc)
{
	FillRect(dc,0,0,320,240,0,RED);
	FillRect(dc,80,60,240,180,0,BLUE);
	ShowBmp(dc,"UI.bmp", 0, 0);//显示位图
	ReDrawOSCtrl();
	Buffer_DrawRectFrame(dc,80,60,240,180);
	Buffer_DrawRectFrame(dc,83,63,238,178);

	Buffer_TextOut(dc,120,60+8*4 ,ch1,TRUE,FONTSIZE_SMALL);

}

int main(void)
{
	ARMTargetInit();
	OSInit();
	uHALr_ResetMMU();
	LCD_Init();
	LCD_printf("LCDinitialization is OK\n");
	LCD_printf("320x240 Text Mode\n");
	initOSGUI();
	LoadFont();
	LoadConfigSys();
   // Zlg7289_Reset();//zlg7289复位
	LCD_printf("Create task on uc/os-Ⅱ...\n");
	
	//Welcome(pdc);						//欢迎界面
	//OSTimeDly(2000);						//停留3.5秒时间
	//ClearScreen();

//	Init_Timer3(100,4,40000,20000);
//	INTS_OFF();//Disable interrupt in PSR
// 	SetISR_Interrupt(10, Timer3_ISR,0);
//    Open_INT_GLOBAL();
//    Open_INT(BIT_TIMER3);
//    INTS_ON();//Enable interrupt in PSR
//    Start_Timer3();
	
    OSTaskCreate(Main_Task,(void*)0,(OS_STK*)&Main_Stack[STACKSIZE*8-1],
				Main_Task_Prio);
	OSTaskCreate(AutoDrop_Task,(void*)0,(OS_STK*)&AutoDrop_Task_Stack[STACKSIZE*8-1],
				AutoDrop_Task_Prio);
	//OSTaskCreate(Audio_Task,(void*)0,(OS_STK*)&Audio_Task_Stack[STACKSIZE*8-1],
	//			Audio_Task_Prio);			
    //OSTaskCreate(Welcome_Task,(void*)0,(OS_STK*)&Welcome_Stack[STACKSIZE*8-1],
	//			Welcome_Task_Prio);
	OSTaskCreate(Led_Flash_Task,(void*)0,(OS_STK*)&Led_Flash_Stack[STACKSIZE-1],
				Led_Flash_Prio);
	OSAddTask_Init();
	
	LCD_printf("Starting uc/os-Ⅱ...\n");
	LCD_printf("Entering graph mode...\n");
	LCD_ChangeMode(DspGraMode);
	InitRtc();
	Nand_Rw_Sem=OSSemCreate(1);
	
	OSStart();
	return 0;
}

void onKey(int nkey,int fnkey)
{
	switch(nkey)
	{
		case 0:
		newGame();
		return;
		case 1:
		pause();
		return;
		case 8:
		moveLeft();
		return;
		case 10:
		moveRight();
		return;
		case 9:
		drop();
		return;
		case 5:
		rotateClockwise();
		return;
		case 7:
		if(delay<1000)
		delay+=50;
		Uart_Printf("delay+:%d\n",delay);
		return;
		case 3:
		if(delay>250)
		delay-=50;
		Uart_Printf("delay-:%d\n",delay);
		default:
		return;
	}
}
/***************************/
void Audio_Task(void*Id)
{
	Init_Audio(1);
	Init_IIS(1);
	Init_UDA1341();
	
	Play_WavFile("1.wav");
	Delay(900);
	time+=1;
	pdc->DrawRect=TimeDraw_Wnd_RECT;
	SetTimeText(pdc);
}
void Welcome_Task(void*Id)
{

	Delay(1000);
	time+=1;
}
/**************************/
void Main_Task(void*Id)
{
int outflag1=0;
	POSMSG pMsg;
	structPOINT 	Touch_Position,*pTouch_Position;
	ButtonCtrl  	NewGame_Button,Pause_Button,Left_Button,
					Right_Button,Down_Button,Rotate_Button;
	PButtonCtrl  	pNewGame_Button,pPause_Button,pLeft_Button,
					pRight_Button,pDown_Button,pRotate_Button;

	Wnd      MainDraw_Wnd,PieceDraw_Wnd,ScoreDraw_Wnd,TimeDraw_Wnd;
	PWnd     pMainDraw_Wnd,pPieceDraw_Wnd,pScoreDraw_Wnd,pTimeDraw_Wnd;
	
	char	NewGame_Button_Caption_8[]="Start";
	char	Pause_Button_Caption_8[]="Pause";
	char	Left_Button_Caption_8[]="L";
	char	Right_Button_Caption_8[]="R";			
	char	Down_Button_Caption_8[]="D";
	char	Rotate_Button_Caption_8[]="C";
	char	MainDraw_Wnd_Caption_8[]="Main Draw";
	char	PieceDraw_Wnd_Caption_8[]="Piece Draw";
	char	ScoreDraw_Wnd_Caption_8[]="Score Draw";
	char	Score_Caption_8[]="Score";
	char	TimeDraw_Wnd_Caption_8[]="Time Draw";
	char	Time_Caption_8[]="Time";						
	U16     Score_Caption_16[10];
	U16     Time_Caption_16[10];	
	U16     NewGame_Button_Caption_16[10];
	U16     Pause_Button_Caption_16[10];	
	U16     Left_Button_Caption_16[10];			
	U16     Right_Button_Caption_16[10];	
	U16    	Rotate_Button_Caption_16[10];
	U16     Down_Button_Caption_16[10];	
	U16     MainDraw_Wnd_Caption_16[20];				
	U16     PieceDraw_Wnd_Caption_16[20];			
	U16     ScoreDraw_Wnd_Caption_16[20];
	U16     TimeDraw_Wnd_Caption_16[20];
	
    Uart_Printf("Touch Begin");
    ClearScreen();
    
	pdc=CreateDC();
	SetDrawOrg(pdc, 0,0, &oldx, & oldy);    //校准原点坐标
	Welcome(pdc);						    //欢迎界面
	OSTimeDly(2000);						//停留2秒时间
	ClearScreen();
	newGame();
	pTouch_Position		=&Touch_Position;		
	pNewGame_Button		=&NewGame_Button;	
	pPause_Button		=&Pause_Button;	
	pLeft_Button		=&Left_Button;
	pRight_Button		=&Right_Button;		
	pDown_Button		=&Down_Button;
	pRotate_Button		=&Rotate_Button;	
	pMainDraw_Wnd		=&MainDraw_Wnd;
	pPieceDraw_Wnd		=&PieceDraw_Wnd;	
	pScoreDraw_Wnd		=&ScoreDraw_Wnd;
	pTimeDraw_Wnd		=&TimeDraw_Wnd;	
	
	pNewGame_Button_RECT	=&NewGame_Button_RECT;
	pPause_Button_RECT		=&Pause_Button_RECT;
	pLeft_Button_RECT		=&Left_Button_RECT;
	pRight_Button_RECT		=&Right_Button_RECT;
	pDown_Button_RECT  		=&Down_Button_RECT;
	pRotate_Button_RECT 	=&Rotate_Button_RECT;
	pMainDraw_Wnd_RECT		=&MainDraw_Wnd_RECT;
	pPieceDraw_Wnd_RECT 	=&PieceDraw_Wnd_RECT; 
	pScoreDraw_Wnd_RECT  	=&ScoreDraw_Wnd_RECT;
	pTimeDraw_Wnd_RECT  	=&TimeDraw_Wnd_RECT;
	
	strChar2Unicode(NewGame_Button_Caption_16,NewGame_Button_Caption_8);
	strChar2Unicode(Pause_Button_Caption_16,Pause_Button_Caption_8);
	strChar2Unicode(Left_Button_Caption_16,Left_Button_Caption_8);			
	strChar2Unicode(Right_Button_Caption_16,Right_Button_Caption_8);
	strChar2Unicode(Down_Button_Caption_16,Down_Button_Caption_8);
	strChar2Unicode(Rotate_Button_Caption_16,Rotate_Button_Caption_8);
	strChar2Unicode(MainDraw_Wnd_Caption_16,MainDraw_Wnd_Caption_8);
	strChar2Unicode(PieceDraw_Wnd_Caption_16,PieceDraw_Wnd_Caption_8);
	strChar2Unicode(ScoreDraw_Wnd_Caption_16,ScoreDraw_Wnd_Caption_8);
	strChar2Unicode(Score_Caption_16,Score_Caption_8);
	strChar2Unicode(TimeDraw_Wnd_Caption_16,TimeDraw_Wnd_Caption_8);
	strChar2Unicode(Time_Caption_16,Time_Caption_8);
	
	NewGame_Button_RECT.bottom=40;
	NewGame_Button_RECT.left=180;
	NewGame_Button_RECT.right=230;
	NewGame_Button_RECT.top=10;
	
	Pause_Button_RECT.bottom=40;
	Pause_Button_RECT.left=240;
	Pause_Button_RECT.right=290;
	Pause_Button_RECT.top=10;
	
	Left_Button_RECT.bottom=228;
	Left_Button_RECT.left=180;
	Left_Button_RECT.right=210;
	Left_Button_RECT.top=198;
	
	Right_Button_RECT.bottom=228;
	Right_Button_RECT.left=260;
	Right_Button_RECT.right=290;
	Right_Button_RECT.top=198;
	
	Down_Button_RECT.bottom=228;
	Down_Button_RECT.left=220;
	Down_Button_RECT.right=250;
	Down_Button_RECT.top=198;
	
	Rotate_Button_RECT.bottom=190;
	Rotate_Button_RECT.left=220;
	Rotate_Button_RECT.right=250;
	Rotate_Button_RECT.top=160;
	
	MainDraw_Wnd_RECT.bottom=219;
	MainDraw_Wnd_RECT.left=10;
	MainDraw_Wnd_RECT.right=129;
	MainDraw_Wnd_RECT.top=10;
	
	PieceDraw_Wnd_RECT.bottom=149;
	PieceDraw_Wnd_RECT.left=180;
	PieceDraw_Wnd_RECT.right=289;
	PieceDraw_Wnd_RECT.top=100 ;
	
	ScoreDraw_Wnd_RECT.bottom=69;
	ScoreDraw_Wnd_RECT.left=180;
	ScoreDraw_Wnd_RECT.right=289;
	ScoreDraw_Wnd_RECT.top=50;
	
	TimeDraw_Wnd_RECT.bottom=99;
	TimeDraw_Wnd_RECT.left=180;
	TimeDraw_Wnd_RECT.right=289;
	TimeDraw_Wnd_RECT.top=70;
	

	
	pNewGame_Button=CreateButton(	NewGame_Button_ID,pNewGame_Button_RECT,
									FONTSIZE_SMALL,CTRL_STYLE_3DDOWNFRAME,
									NewGame_Button_Caption_16,NULL);
									
	pPause_Button=CreateButton(		Pause_Button_ID,pPause_Button_RECT,
									FONTSIZE_SMALL,CTRL_STYLE_3DDOWNFRAME,
									Pause_Button_Caption_16,NULL);
									
	pLeft_Button=CreateButton(		Left_Button_ID,pLeft_Button_RECT,
									FONTSIZE_SMALL,CTRL_STYLE_3DDOWNFRAME,
									Left_Button_Caption_16,NULL);
									
	pRight_Button=CreateButton(		Right_Button_ID,pRight_Button_RECT,
									FONTSIZE_SMALL,CTRL_STYLE_3DDOWNFRAME,
									Right_Button_Caption_16,NULL);
	
	pDown_Button=CreateButton(		Down_Button_ID,pDown_Button_RECT,
									FONTSIZE_SMALL,CTRL_STYLE_3DDOWNFRAME,
									Down_Button_Caption_16,NULL);
									
	pRotate_Button=CreateButton(	Rotate_Button_ID,pRotate_Button_RECT,
									FONTSIZE_SMALL,CTRL_STYLE_3DDOWNFRAME,
									Rotate_Button_Caption_16,NULL);
	
	pMainDraw_Wnd=CreateWindow(		MainDraw_Wnd_ID,pMainDraw_Wnd_RECT,
									FONTSIZE_SMALL,WND_STYLE_MODE,
									MainDraw_Wnd_Caption_16,NULL);
	
	pPieceDraw_Wnd=CreateWindow(	PieceDraw_Wnd_ID,pPieceDraw_Wnd_RECT,
									FONTSIZE_SMALL,WND_STYLE_MODE,
									PieceDraw_Wnd_Caption_16,NULL);
									
	pScoreDraw_Wnd=CreateWindow(	ScoreDraw_Wnd_ID,pScoreDraw_Wnd_RECT,
									FONTSIZE_SMALL,WND_STYLE_MODE,
									ScoreDraw_Wnd_Caption_16,NULL);
    pTimeDraw_Wnd=CreateWindow(		TimeDraw_Wnd_ID,pTimeDraw_Wnd_RECT,
									FONTSIZE_SMALL,WND_STYLE_MODE,
									TimeDraw_Wnd_Caption_16,NULL);
	
	DrawButton(pNewGame_Button);
	DrawButton(pPause_Button);
	DrawButton(pLeft_Button);
	DrawButton(pRight_Button);
	DrawButton(pDown_Button);
	DrawButton(pRotate_Button);
	
	DrawWindow(pMainDraw_Wnd);
	DrawWindow(pPieceDraw_Wnd);
	DrawWindow(pScoreDraw_Wnd);
	DrawWindow(pTimeDraw_Wnd);
	

	
	pdc->DrawRect=ScoreDraw_Wnd_RECT;
	TextOut(pdc,190,60,Score_Caption_16,TRUE,FONTSIZE_SMALL);
	SetScoreText(pdc);
	
	pdc->DrawRect=TimeDraw_Wnd_RECT;
	TextOut(pdc,190,80,Time_Caption_16,TRUE,FONTSIZE_SMALL);
	SetTimeText(pdc);
	
	pdc->DrawRect=MainDraw_Wnd_RECT;
	DrawMainFrame(pdc);
		ZLG7289_ENABLE();//使zlg7289占有同步串口
		
		WriteSDIO(ZLG7289_CMD_DATA0|0);//数码管以方式0译码，第一个数码管
		WriteSDIO(highscore%10);
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|1);//数码管以方式1译码，第一个数码管亮
		WriteSDIO(highscore/10%10);
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|2);//数码管以方式2译码，第一个数码管亮
		WriteSDIO(highscore/100%10);
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|3);//数码管以方式3译码，第一个数码管亮
		WriteSDIO(highscore/1000%10);
		Delay(1);//延时
		
		
		WriteSDIO(ZLG7289_CMD_DATA0|4);//数码管以方式4译码，第一个数码管亮
		WriteSDIO(score%10);
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|5);//数码管以方式5译码，第一个数码管亮
		WriteSDIO(score/10%10);
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|6);//数码管以方式6译码，第一个数码管亮
		WriteSDIO(score/100%10);
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|7);//数码管以方式7译码，第一个数码管亮
		WriteSDIO(score/1000%10);
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_HIDE);//使个位数码管显示
		WriteSDIO(0xff);
		ZLG7289_DISABLE();
		
	for(;;)
	{
	pMsg=WaitMessage(0);
	switch(pMsg->Message)
	{
	case OSM_TOUCH_SCREEN:
		Touch_Position.x=pMsg->WParam&0xffff;
		Touch_Position.y=pMsg->WParam>>16;
		Uart_Printf("B_Touch_Position.x:%d\n",Touch_Position.x);
		Uart_Printf("B_Touch_Position.y:%d\n",Touch_Position.y);
		Touch_Position.x=0;
		Touch_Position.y=0;
		C_TchScr_GetScrXY(&Touch_Position.x,&Touch_Position.y,TRUE);
		Uart_Printf("Touch_Position.x:%d\n",Touch_Position.x);
		Uart_Printf("Touch_Position.y:%d\n",Touch_Position.y);
		if(IsInRect2(pNewGame_Button_RECT,pTouch_Position))
		{
			newGame();
		}else if(IsInRect2(pPause_Button_RECT,pTouch_Position))
		{
			pause();
		}else if(IsInRect2(pLeft_Button_RECT,pTouch_Position))
		{
			moveLeft();
		}else if(IsInRect2(pRight_Button_RECT,pTouch_Position))
		{
			moveRight();
		}else if(IsInRect2(pDown_Button_RECT,pTouch_Position))
		{
			drop();
		}else if(IsInRect2(pRotate_Button_RECT,pTouch_Position))
		{
			rotateClockwise();
		}
	case OSM_KEY:
		onKey(pMsg->WParam,pMsg->LParam);
		break;

		}
			ZLG7289_ENABLE();//使zlg7289占有同步串口
		
		WriteSDIO(ZLG7289_CMD_DATA0|0);//数码管以方式0译码，第一个数码管
		WriteSDIO(highscore%10);//显示个位
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|1);//数码管以方式0译码，第一个数码管亮
		WriteSDIO(highscore/10%10);//显示个位
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|2);//数码管以方式0译码，第一个数码管亮
		WriteSDIO(highscore/100%10);//显示个位
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|3);//数码管以方式0译码，第一个数码管亮
		WriteSDIO(highscore/1000%10);//显示个位
		Delay(1);//延时
		
		
		WriteSDIO(ZLG7289_CMD_DATA0|4);//数码管以方式0译码，第一个数码管亮
		WriteSDIO(score%10);//显示个位
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|5);//数码管以方式0译码，第一个数码管亮
		WriteSDIO(score/10%10);//显示个位
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|6);//数码管以方式0译码，第一个数码管亮
		WriteSDIO(score/100%10);//显示个位
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_DATA0|7);//数码管以方式0译码，第一个数码管亮
		WriteSDIO(score/1000%10);//显示个位
		Delay(1);//延时
		WriteSDIO(ZLG7289_CMD_HIDE);//使个位数码管显示
		WriteSDIO(0xff);
		
		ZLG7289_DISABLE();
		DeleteMessage(pMsg);
		OSTimeDly(100);
	}
	DestroyDC(pdc);
}
/*方块下落任务：自动下落、检测满行并消去*/
void AutoDrop_Task(void *Id)
{
	int i;
	while(StillRunning)
	{
		OSTimeDly(delay);
		if(!StillRunning)break;
		if(boardinitialized)
		{
			if(Paused) continue;
			if(CurrentBlockColour==0)
			{
				Chose_Color=Rand_Color();
				if(NextBlockColour==0)makeNewPiece();
				CurrentBlockColour=NextBlockColour;
				CurrentBlockX=NextBlockX;
				CurrentBlockY=NextBlockY;
				CurrentBlockShape=NextBlockShape;
				CurrentBlockAngle=NextBlockAngle;
				makeNewPiece();
			}else
			{
				i=-1;
				while(i++<3)
				if(CurrentBlockY+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][1]>=(ROWCOUNT-1)
				||BoardArray[CurrentBlockX+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0]][
				CurrentBlockY+1+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][1]]!=0)
				{
					i=-1;
					while(i++<3)
					{
						BoardArray[CurrentBlockX+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0]]
						[CurrentBlockY+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][1]]=CurrentBlockColour;
						
						COLOR[CurrentBlockX+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0]]
						[CurrentBlockY+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][1]]=Chose_Color;
						if(CurrentBlockY+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][1]<3)
						{
							Paused=TRUE;
							GameOver=TRUE;
							forceredraw=TRUE;
						}
					}
					CheckRow(pdc);
					
					pdc->DrawRect=ScoreDraw_Wnd_RECT;
					SetScoreText(pdc);
					
					if(delay>100) delay-=20;
					CurrentBlockColour=0;
					LastX=LastY=LastAngle=-1;
					forceredraw=TRUE;
					break;
				}
				CurrentBlockY++;}
				update();
			}else{
					dopaint(pdc);
				}
		}
}

int update()
{
	int x,y,i;
	char Game_Over_Caption_8[]="GameOver";
	char Game_Paused_Caption_8[]="GamePaused";
	U16 Game_Over_Caption_16[20];
	U16 Game_Paused_Caption_16[20];
	strChar2Unicode(Game_Over_Caption_16,Game_Over_Caption_8);
	strChar2Unicode(Game_Paused_Caption_16,Game_Paused_Caption_8);
	pdc->DrawRect=MainDraw_Wnd_RECT;
	if(!boardinitialized)
	{
		boardinitialized=TRUE;
		for(x=0;x<COLCOUNT;x++)
			for(y=0;y<ROWCOUNT;y++)
				BoardArray[x][y]=0;
				
		LastX=LastY=LastAngle=-1;
		kludgeflag=forceredraw=FALSE;
	}
	if(kludgeflag&&forceredraw)
	{
		kludgeflag=FALSE;
		realdopaint(pdc);
		return;
	}
	kludgeflag=forceredraw=FALSE;
	
	FillRect(pdc,20,10,119,209,GRAPH_MODE_NORMAL,WHITE);
	if(Paused)
	{
		if(GameOver)
		{
			if(score>highscore)
			highscore=score;
			TextOut(pdc,45,20,Game_Over_Caption_16,TRUE,FONTSIZE_SMALL);
		}else 
		{
			TextOut(pdc,45,20,Game_Paused_Caption_16,TRUE,FONTSIZE_SMALL);
			return;
		}
	}
		for(x=0;x<COLCOUNT;x++)
		for(y=0;y<ROWCOUNT;y++)
		{
			if(BoardArray[x][y]!=0)
			{
				FillRect(pdc,20+x*10,y*10,19+(x+1)*10,y*10+9,GRAPH_MODE_NORMAL,COLOR[x][y]);
			}
		}
		if(CurrentBlockColour!=0)
		{
			i=-1;
			while(i++<3)
			FillRect(pdc,20+(CurrentBlockX+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0])*10,(CurrentBlockY+BlockInfo[CurrentBlockShape][CurrentBlockAngle]
			[i][1])*10,19+(1+CurrentBlockX+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0])*10,(CurrentBlockY+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][1])*10+9,
			GRAPH_MODE_NORMAL,Chose_Color);
		}
}				
					
int DrawMainFrame(PDC pdc)
{
	int x,y;
	for(y=10;y<=210;y+=10)
		Draw3DRect(pdc,10,y,19,y+9,RGB(0,128,255),RGB(0,128,255));
	for(y=10;y<=210;y+=10)
		Draw3DRect(pdc,120,y,129,y+9,RGB(0,128,255),RGB(0,128,255));
	for(x=10;x<=120;x+=10)
		Draw3DRect(pdc,x,210,x+9,219,RGB(0,128,255),RGB(0,128,255));
}
int SetScoreText(PDC pdc)
{
	U16 str_score[10];
	Int2Unicode(score,str_score);
	TextOut(pdc,230,60,str_score,TRUE,FONTSIZE_SMALL);
}
int SetTimeText(PDC pdc)
{
	U16 str_time[10];
	Int2Unicode(time,str_time);
	TextOut(pdc,230,80,str_time,TRUE,FONTSIZE_SMALL);
}
int dopaint(PDC pdc)
{
	kludgeflag=TRUE;
	update();
}
int realdopaint(PDC pdc)
{
	int x,y,i;
	pdc->DrawRect=MainDraw_Wnd_RECT;
	FillRect(pdc,30,20,39,29,GRAPH_MODE_NORMAL,Chose_Color);
	
	if(LastX!=-1)
	{
		i=-1;
		while(i++<3)
		    FillRect(pdc,20+(LastX+BlockInfo[CurrentBlockShape][LastAngle][i][0])*10,
		    (LastY+BlockInfo[CurrentBlockShape][LastAngle][i][1])*10,
		    20+(1+LastX+BlockInfo[CurrentBlockShape][LastAngle][i][0])*10-1,
		    (1+LastY+BlockInfo[CurrentBlockShape][LastAngle][i][1])*10-1,GRAPH_MODE_NORMAL,Chose_Color);
	}
	if(CurrentBlockColour!=0)
	{	
		LastX=CurrentBlockX;
		LastY-CurrentBlockY;
		LastAngle=CurrentBlockAngle;
		
		i=-1;
		while(i++<3)
			FillRect(pdc,20+(CurrentBlockX+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0])*10,(CurrentBlockY+BlockInfo[CurrentBlockShape]
			[CurrentBlockAngle][i][1])*10,20+(1+CurrentBlockX+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0])*10-1,
			(1+CurrentBlockY+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][1])*10-1,GRAPH_MODE_NORMAL,Chose_Color);
	}
}
									
BOOLEAN pause()
{
	if(GameOver) return FALSE;
	Paused=!Paused;
	
	kludgeflag=forceredraw=TRUE;
	
	update();
	return TRUE;
}
					
BOOLEAN newGame()
{
	int x,y;
	if(boardinitialized)
	{
		for(x=0;x<COLCOUNT;x++)
			for(y=0;y<ROWCOUNT;y++)
				BoardArray[x][y]=0;		
		LastX = LastY = LastAngle = -1;
		GameOver = Paused = kludgeflag = FALSE;			
		delay = 500;
		forceredraw = TRUE;
	}
	score =0;
	CurrentBlockColour = NextBlockColour = score =0;
	pdc->DrawRect =ScoreDraw_Wnd_RECT;
	SetScoreText(pdc);
	update();
	return TRUE;
}		
BOOLEAN moveRight()
{
	int i= -1;
	if(boardinitialized&&!Paused)
	{
		while(i++<3)
		if(CurrentBlockX+1+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0]>COLCOUNT-1||
		BoardArray[CurrentBlockX+1+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0]]
		[CurrentBlockY+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][1]]!=0)
		return FALSE;
		CurrentBlockX++;
	}
	return TRUE;
}
BOOLEAN moveLeft()			
{
	int i =-1;
	if(boardinitialized&&!Paused)
	{
		while(i++<3)
		if(CurrentBlockX-1+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0]<0||
			BoardArray[CurrentBlockX-1+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0]]
			[CurrentBlockY+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][1]]!=0)
			return FALSE;
			CurrentBlockX=CurrentBlockX-1;
	}
	return TRUE;
}
BOOLEAN rotateClockwise()
{
	int i,NewBlockAngle;
	if(boardinitialized&&!Paused)
	{
		NewBlockAngle = CurrentBlockAngle+1;
		if(NewBlockAngle==4)NewBlockAngle =0;
		i=-1;
		while(i++<3)
		if(CurrentBlockX+BlockInfo[CurrentBlockShape][NewBlockAngle][i][0]>COLCOUNT-1||
		CurrentBlockX+BlockInfo[CurrentBlockShape][NewBlockAngle][i][0]<0||
		CurrentBlockY+BlockInfo[CurrentBlockShape][NewBlockAngle][i][1]>=ROWCOUNT||
		CurrentBlockY+BlockInfo[CurrentBlockShape][NewBlockAngle][i][1]<0||
		BoardArray[CurrentBlockX+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0]]
		[CurrentBlockY+BlockInfo[CurrentBlockShape][NewBlockAngle][i][1]]!=0)
		return FALSE;
		CurrentBlockAngle = NewBlockAngle;
	}
	return TRUE;
}
BOOLEAN drop()
{
	int i;
	if(boardinitialized&&!Paused)
	{
		while(TRUE)
		{
			i=-1;
			while(i++<3)
			if(CurrentBlockY+1+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][1]>=ROWCOUNT||
			BoardArray[CurrentBlockX+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][0]]
			[CurrentBlockY+1+BlockInfo[CurrentBlockShape][CurrentBlockAngle][i][1]]!=0)
			{
				return FALSE;
			}
			CurrentBlockY++;
		}
		score+=1;
	}
	return TRUE;
}

int CheckRow(PDC pdc)
{
	int i,row =ROWCOUNT,j,flag=0;
	while(row-->0)
	{
		i=-1;
		while(++i<10)
		{
			if(BoardArray[i][row]==0)
			{
				i=99;
				break;
			}
		}
		if(i==99)
		{
			continue;
		}
		j=row++;
		flag++;
		while(--j>0)
		{
			i=-1;
			while(i++<9)
			BoardArray[i][j+1]=BoardArray[i][j];
		}
		
		switch(flag){
		case 0:
			break;
		case 1:
			score+=1;
			break;
		case 2:
		    score+=3;
		    break;
		case 3:
			score+=5;
			break;
		case 4:
			score+=7;
			break;
		default:break;
		}
		pdc->DrawRect=ScoreDraw_Wnd_RECT;
		SetScoreText(pdc);
		forceredraw = TRUE;
		DisplayPiece(pdc);
	}
}
int makeNewPiece()
{
	NextBlockColour =1;
	if(NextBlockColour ==7)	NextBlockColour =1;
	NextBlockX=4;
	if(NextBlockX==8)		NextBlockX=4;			
	NextBlockY=1;
	Shape--;
	if(Shape<0)  Shape=7;
	NextBlockShape =Shape;
	if(NextBlockShape ==7) NextBlockShape =0;
	NextBlockAngle =0;
	if(NextBlockAngle ==4) NextBlockAngle =0;
	
	DisplayPiece(pdc);
}
int DisplayPiece(PDC pdc)
 
{
	int i=-1;
	pdc->DrawRect=PieceDraw_Wnd_RECT;
	FillRect(pdc,180,90,289,149,GRAPH_MODE_NORMAL,WHITE);
	
	while(i++<3)
	{
		FillRect(pdc,225+getPieceX(i)*10,110+getPieceY(i)*10,
			224+(getPieceX(i)+1)*10,109+(getPieceY(i)+1)*10,
			GRAPH_MODE_NORMAL,Chose_Color);
	}
}
int getPieceColour()
{
	return(RGB(0,0,0));
}
int getPieceX(int i)
{
	return(BlockInfo[NextBlockShape][NextBlockAngle][i][0]);
}
int getPieceY(int i)
{
	return(BlockInfo[NextBlockShape][NextBlockAngle][i][1]);
}	
U32 Rand_Color()
{
	int k;
	k=rand()%7+1;
	switch(k){
		case 1:
			return	BLACK;
			break;					
		case 2:
			return	RED;
			break;						
		case 3:
			return	YELLOW;
			break;					
		case 4:
			return	BROWN;
			break;					
		case 5:
			return	GREEN;
			break;			         
		case 6:
			return	BLUE;
			break;						
		case 7:
			return	CYAN;
			break;

		default:break;
		}
}			        
	
