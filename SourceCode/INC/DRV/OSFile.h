#ifndef __OSFILE_H__
#define __OSFILE_H__

#define FILESYSTEM_AUTOFORMAT		FALSE

//Macro related to FLASH
#define PagePerClus	32
#define BytesPerPage	512

#define FILEMODE_READ		1
#define FILEMODE_WRITE		2
#define FILEMODE_CREATE		0x10

#define Disk_Size 		(16*1024*1024)		//Disk�Ŀռ��С(��Sector/PageΪ������λ)
#define Begin_Cluster		(1)				//
//#define Start_Cluster	(1)				//Cluster0��Root_Informationռ��
#define End_Cluster		(Disk_Size/BytesPerPage/PagePerClus+Begin_Cluster)

//Disk Infomation
#define BytesPerSector	512

//#define UNUSED_MARK		(0xFF)
//#define BAD_MARK		(0x00)
#define LAST_BLOCK		(0xFFFF)
#define OSFILE_DELETEMARK		0xe5

#define FILE_NO_FOUND	(512)
#define Block_Size		(32*1024)

#define BPB_Sector				(0)
#define RSD_Sector				1
#define Fat_Sector				(BPB_Sector+RSD_Sector)
#define FAT_NUM					2	//����������
#define Fat_Sector_Num			2	//������ռ�õ�������
#define Directory_BeginSector		(Fat_Sector+Fat_Sector_Num*FAT_NUM)

#define FileData_BeginSector	(2*32)	//�ļ����ݵ���ʼ����
#define Directory_Number	((FileData_BeginSector-Directory_BeginSector)*BytesPerSector/32)	//�ļ�Ŀ¼����

//�ļ�������������
#define Total_Sector		((End_Cluster-Begin_Cluster)*BytesPerPage*PagePerClus/BytesPerSector-FileData_BeginSector)

#define FILESYS_MAX_BUFFER_FILEROOT		1024


typedef struct{
	U8 Buffer[Block_Size];	//�ļ�������32*1024
	U32 fileCluster;		//�ļ���ǰ�Ĵص�λ��
	U32 filemode;			//���ļ���ģʽ
	U32 filebufnum;	//�ļ����������Ѿ���ȡ/д����ֽ���
	U32 fileCurpos;	//��д�ĵ�ǰλ��
	U32 filesize;	//�ļ��Ĵ�С
	int rootpos;	//�ļ�ϵͳ��Ŀ¼��λ�� by threewater
}FILE;

#define FAT12 12
#define FAT16 16
#define FAT32 32

#define HIGHBYTE(a)		((a>>8)&0xff)
#define LOWBYTE(a)		(a&0xff)


int initOSFile(void);
FILE* OpenOSFile(char filename[], U32 OpenMode);

//U8 OpenOSFileRead(FILE* pfile,char filename[]);
U32 ReadOSFile(FILE* pfile,U8* ReadBuffer, U32 nReadbyte);
U32 LineReadOSFile(FILE* pfile, char str[]);	//��ȡָ���ļ���һ��
U32 SeekOSFile(FILE* pfile ,U32 nCurPos);
U32 GetPusOSFile(FILE* pfile);
U8 DeleteOSFile(char filename[]);

U8 RenameOSFile(char fromname[], char toname[]);
U8 CopyOSFile(char srcfile[], char decfile[]);
int FindOSFile(char filename [ ]);

//U8 OpenOSFileWrite(FILE* pfile, char filename[]);
U8 WriteOSFile(FILE* pfile,U8* WriteBuffer, U32 nWriteyte);
int CloseOSFile(FILE* pfile);
//�õ�ָ��λ�õ��ļ�����������չ�������ļ�λ���Զ�����
U8 GetNextFileName(U32 *filepos,char filename[]);
//�г���ǰλ�ÿ�ʼ��һ��ָ����չ�����ļ������û�У��򷵻�FALSE
U8 ListNextFileName(U32 *filepos, char FileExName[],char filename[]);
void FormatFileName11(char outfilename [ ], char infilename [ ]);

int Init_FAT_Info(int AutoFormat);
unsigned int NextCluster(unsigned int CurrentCluster);
unsigned int AllocateCluster(unsigned int CurrentCluster);


/********************************************************************/
/*���Flash��MBR��Fat16�ṹ����,����ô˺������Ի�*/	
/*��																*/
/********************************************************************/
int Format_Fat12Media(void);
void WriteMBR2Flash(void);
void CreatFAT16(void);
void CreatDirectoryEntry(void);


void TestFAT_COPY(char filename1[],char filename2[]);
void TestFAT_CREATE(char filename[]);
void TestFAT_DELETE(char filename[]);
void TestFAT_READ(char filename[]);

#endif
