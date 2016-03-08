#ifndef __OSFILE_H__
#define __OSFILE_H__

#define FILESYSTEM_AUTOFORMAT		FALSE

//Macro related to FLASH
#define PagePerClus	32
#define BytesPerPage	512

#define FILEMODE_READ		1
#define FILEMODE_WRITE		2
#define FILEMODE_CREATE		0x10

#define Disk_Size 		(16*1024*1024)		//Disk的空间大小(以Sector/Page为基本单位)
#define Begin_Cluster		(1)				//
//#define Start_Cluster	(1)				//Cluster0被Root_Information占用
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
#define FAT_NUM					2	//两个分区表
#define Fat_Sector_Num			2	//分区表占用的扇区数
#define Directory_BeginSector		(Fat_Sector+Fat_Sector_Num*FAT_NUM)

#define FileData_BeginSector	(2*32)	//文件数据的起始扇区
#define Directory_Number	((FileData_BeginSector-Directory_BeginSector)*BytesPerSector/32)	//文件目录项数

//文件数据扇区总数
#define Total_Sector		((End_Cluster-Begin_Cluster)*BytesPerPage*PagePerClus/BytesPerSector-FileData_BeginSector)

#define FILESYS_MAX_BUFFER_FILEROOT		1024


typedef struct{
	U8 Buffer[Block_Size];	//文件缓冲区32*1024
	U32 fileCluster;		//文件当前的簇的位置
	U32 filemode;			//打开文件的模式
	U32 filebufnum;	//文件缓冲区中已经读取/写入的字节数
	U32 fileCurpos;	//读写的当前位置
	U32 filesize;	//文件的大小
	int rootpos;	//文件系统中目录的位置 by threewater
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
U32 LineReadOSFile(FILE* pfile, char str[]);	//读取指定文件的一行
U32 SeekOSFile(FILE* pfile ,U32 nCurPos);
U32 GetPusOSFile(FILE* pfile);
U8 DeleteOSFile(char filename[]);

U8 RenameOSFile(char fromname[], char toname[]);
U8 CopyOSFile(char srcfile[], char decfile[]);
int FindOSFile(char filename [ ]);

//U8 OpenOSFileWrite(FILE* pfile, char filename[]);
U8 WriteOSFile(FILE* pfile,U8* WriteBuffer, U32 nWriteyte);
int CloseOSFile(FILE* pfile);
//得到指定位置的文件名（包括扩展名），文件位置自动下移
U8 GetNextFileName(U32 *filepos,char filename[]);
//列出当前位置开始第一个指定扩展名的文件，如果没有，则返回FALSE
U8 ListNextFileName(U32 *filepos, char FileExName[],char filename[]);
void FormatFileName11(char outfilename [ ], char infilename [ ]);

int Init_FAT_Info(int AutoFormat);
unsigned int NextCluster(unsigned int CurrentCluster);
unsigned int AllocateCluster(unsigned int CurrentCluster);


/********************************************************************/
/*如果Flash的MBR和Fat16结构受损,则调用此函数可以恢*/	
/*复																*/
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
