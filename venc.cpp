#ifndef __vochant_encrypt
#define __vochant_encrypt

#ifdef _MSC_VER
#define _sscanf sscanf_s
#else
#define _sscanf sscanf
#endif

#ifdef _MSC_VER
#ifndef __clang__

#pragma warning(disable : 4996)

#endif
#endif

#ifdef __GNUC__
#pragma GCC optimize(2)
#endif

#ifdef __clang__
#pragma GCC optimize(2)
#endif

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32

#include <windows.h>

// 1 = b, 2 = g, 4 = r

HANDLE hOut;

inline void SetColor(int __Color=7)
{
	SetConsoleTextAttribute(hOut,__Color);
}

#else //Linux and macOS

// 1 = r, 2 = g, 4 = b

inline void SetColor(int __Color=16)
{
	if(__Color==16)
	{
		printf("\033[0m");
		return;
	}
	printf("\033[%d;3%dm",(__Color&8)>>3,((__Color&1)<<2)|(__Color&2)|((__Color&4)>>2));
}

#endif

using namespace std;

string __BaseString="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
string __ValidString="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789`~!@#$%^&*()-=_+[]{}\\|;'\",./<>?";
string __ValidEncrypted="AcEgIkMoQsUwYbFjNrVzHpXdLt!~`0$#@2&^%4-)(6+_=8{][3|\\}7\"';1/.,9?><5*TlDxPhZvRnJfByWuSqOmKiGeCa";
string __Notused;

#define __BaseLength 65
#define __ValidLength 93
#define __ProxyLength 28

bool __ValidCharTag[128],__bInit;
int __BaseToValid[128],__ValidToBase[128];

char __EncryptStr[256],__DecryptStr[256];

int __BaseToInt[256],__ValidToInt[256];

string __Mask;

fstream __OutputStream;

inline void __InitSystem()
{
    #ifdef _WIN32
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);
    #endif
	srand(time(0));
	for(int i=0;i<__BaseLength;i++)
	{
		__BaseToInt[(int)__BaseString[i]]=i;
	}
	for(int i=0;i<__ValidLength;i++)
	{
		__ValidToInt[(int)__ValidString[i]]=i;
	}
	for(int i=0;i<__ValidLength;i++)
	{
		__EncryptStr[(int)__ValidString[i]]=__ValidEncrypted[i];
		__DecryptStr[(int)__ValidEncrypted[i]]=__ValidString[i];
	}
	__bInit=true;
}

inline void __EncryptMask()
{
	for(int i=0;i<__BaseLength;i++)
	{
		__Mask[i]=__EncryptStr[(int)__Mask[i]];
	}
}

inline void __DecryptMask()
{
	for(int i=0;i<__BaseLength;i++)
	{
		__Mask[i]=__DecryptStr[(int)__Mask[i]];
	}
}

inline void __GenerateMask()
{
	int __TargetChar,__FreeCounter;
	memset(__ValidCharTag,0,sizeof(__ValidCharTag));
	__Mask="";
	for(int i=0;i<__BaseLength;i++)
	{
		__TargetChar=rand()%(__ValidLength-i);
		__FreeCounter=0;
		for(int j=0;j<__ValidLength;j++)
		{
			if(__FreeCounter==__TargetChar&&!__ValidCharTag[j])
			{
				__BaseToValid[i]=j;
				__ValidToBase[j]=i;
				__ValidCharTag[j]=true;
				__Mask+=__ValidString[j];
				break;
			}
			if(!__ValidCharTag[j])
			{
				__FreeCounter++;
			}
		}
	}
	__Notused="";
	for(int i=0;i<__ValidLength;i++)
	{
		if(!__ValidCharTag[i])
		{
			__Notused+=__ValidString[i];
		}
	}
	// printf("%s\n%s %d\n",__Mask.c_str(),__Notused.c_str(),__Notused.length());
}

inline void __InitFromMask()
{
	memset(__ValidToBase,-1,sizeof(__ValidToBase));
	for(int i=0;i<__BaseLength;i++)
	{
		__BaseToValid[i]=__ValidToInt[(int)__Mask[i]];
		__ValidToBase[__ValidToInt[(int)__Mask[i]]]=i;
	}
}

int __EncoderReleaseBranch,__EncoderUpdate,__EncoderSubversion,__EncoderCompile;

struct __Result_T
{
	bool bSuccess;
	string strMessage;
};

typedef __Result_T __EncodeResult;
typedef __Result_T __DecodeResult;

inline string __GetFileCodec(string __FileName)
{
	string __Result="";
	int __Length=__FileName.length();
	for(int i=0;i<__Length;i++)
	{
		if(__FileName[i]=='.')
		{
			__Result="";
		}
		else
		{
			__Result+=__FileName[i];
		}
	}
	return __Result;
}

inline string __GetFileIndex(string __FileName)
{
	string __Result="",__Temp="";
	int __Length=__FileName.length();
	for(int i=0;i<__Length;i++)
	{
		if(__FileName[i]=='.')
		{
			__Result+=__Temp;
			__Temp="";
		}
		__Temp+=__FileName[i];
	}
	return __Result;
}

inline string __GetFileName(string __FileName)
{
	__FileName=__GetFileIndex(__FileName);
	string __Result="";
	int __Length=__FileName.length();
	for(int i=0;i<__Length;i++)
	{
		#ifdef _WIN32
		if(__FileName[i]=='\\')
		{
			__Result="";
		}
		#else
		if(__FileName[i]=='/')
		{
			__Result="";
		}
		#endif
		else if(__FileName[i]==' '||__FileName[i]=='\0'||__FileName[i]=='\t'||__FileName[i]=='\r'||__FileName[i]=='\n')
		{
			__Result+="_";
		}
		else
		{
			__Result+=__FileName[i];
		}
	}
	return __Result;
}

inline string __GetFileFullName(string __FileName)
{
	string __Result="";
	int __Length=__FileName.length();
	for(int i=0;i<__Length;i++)
	{
		#ifdef _WIN32
		if(__FileName[i]=='\\')
		{
			__Result="";
		}
		#else
		if(__FileName[i]=='/')
		{
			__Result="";
		}
		#endif
		else
		{
			__Result+=__FileName[i];
		}
	}
	return __Result;
}

namespace __Encoder
{
	__EncodeResult __prebuild(fstream &__OutputStream)
	{
		__EncodeResult __Result;
		if(!__bInit)
		{
			printf("Preparing System...");
			__InitSystem();
			printf("OK\n");
		}
		printf("Writing header...1/2");
		__OutputStream<<"Vencrypt?\n";
		printf("\b\b\b2/2");
		__OutputStream<<">Match-FileInfo\n:Change Generator-Display-Name\nVochant Encrypt CLI Tool Version 0.1.1 (Pre-Release)\n";
		printf("\b\b\bOK \n");
		__Result.bSuccess=true;
		__Result.strMessage="Done.\n";
		return __Result;
	}

	__EncodeResult __endfile(fstream &__OutputStream)
	{
		__EncodeResult __Result;
		__OutputStream<<">End-Matches\n!return-value 0\n";
		__Result.bSuccess=true;
		__Result.strMessage="Done.\n";
		return __Result;
	}

	__EncodeResult __pr_001(string __InputFileName,fstream &__OutputStream,string __OutputFileName)
	{
		string __ShortName=__GetFileName(__InputFileName);
		__EncodeResult __Result;
		#ifdef _WIN32
		string __Command="certutil -f -encode \""+__InputFileName+"\" \""+__ShortName+".tmp\">nul";
		#else
		string __Command="base64 \""+__InputFileName+"\">"+__ShortName+".tmp";
		#endif
		printf("Proccessing \"%s\"...1/5",__GetFileFullName(__InputFileName).c_str());
		system(__Command.c_str());
		#ifdef _WIN32
		__Command="del /f /s /q \""+__ShortName+".tmp\">nul";
		#else
		__Command="rm "+__ShortName+".tmp";
		#endif
		printf("\b\b\b2/5");
		fstream __InputStream;
		__InputStream.open(__ShortName+".tmp",ios::in);
		if(!__InputStream.good())
		{
			__Result.bSuccess=false;
			__Result.strMessage="\nError:File not found: \""+__ShortName+__GetFileCodec(__InputFileName)+"\"\n";
			return __Result;
		}
		__OutputStream<<">New-File\n";
		__OutputStream<<"!filetype "<<__GetFileCodec(__InputFileName)<<"\n!filename "<<__OutputFileName<<"\n";
		__OutputStream<<"!branch 0\n";
		__OutputStream<<"!version 0\n";
		printf("\b\b\b3/5");
		__GenerateMask();
		__OutputStream<<__Mask<<"\n";
		printf("\b\b\b4/5");
		char __In[256]="Nothing";
		string __ConvIn="Nothing";
		string __Out;
		bool __bSetup=false;
		int __Length;
		#ifdef _WIN32
		__InputStream.getline(__In,sizeof(__In));
		__ConvIn=__In;
		__Length=__ConvIn.length();
		while(__ConvIn!="-----END CERTIFICATE-----")
		{
			if(!__bSetup)
			{
				if(__ConvIn!="-----BEGIN CERTIFICATE-----")
				{
					__Result.bSuccess=false;
					__Result.strMessage="\nError: Maybe your Certificate Util cannot be used by this program.\nThe first line of the input file is not right.\nREQR: -----BEGIN CERTIFICATE-----\nYOUR: "+__ConvIn+"\n";
					return __Result;
				}
				__bSetup=true;
				__InputStream.getline(__In,sizeof(__In));
				__ConvIn=__In;
				__Length=__ConvIn.length();
				continue;
			}
			__Out="";
			for(int i=0;i<__Length;i++)
			{
				__Out+=__ValidString[__BaseToValid[__BaseToInt[(int)__In[i]]]];
			}
			__OutputStream<<__Out<<"\n";
			__InputStream.getline(__In,sizeof(__In));
			__ConvIn=__In;
			__Length=__ConvIn.length();
		}
		#else
		while(__InputStream>>__ConvIn)
		{
			__Length=__ConvIn.length();
			__Out="";
			for(int i=0;i<__Length;i++)
			{
				__Out+=__ValidString[__BaseToValid[__BaseToInt[(int)__ConvIn[i]]]];
			}
		}
		#endif
		__OutputStream<<":eof"<<"\n";
		printf("\b\b\b5/5");
		__InputStream.close();
		system(__Command.c_str());
		printf("\b\b\bOK \n");
		__Result.bSuccess=true;
		__Result.strMessage="\""+__InputFileName+"\": Done.\n";
		return __Result;
	}

	__EncodeResult __pr_002(string __InputFileName,fstream &__OutputStream,string __OutputFileName)
	{
		string __ShortName=__GetFileName(__InputFileName);
		__EncodeResult __Result;
		#ifdef _WIN32
		string __Command="certutil -f -encode \""+__InputFileName+"\" \""+__ShortName+".tmp\">nul";
		#else
		string __Command="base64 \""+__InputFileName+"\">"+__ShortName+".tmp";
		#endif
		printf("Proccessing \"%s\"...1/5",__GetFileFullName(__InputFileName).c_str());
		system(__Command.c_str());
		#ifdef _WIN32
		__Command="del /f /s /q \""+__ShortName+".tmp\">nul";
		#else
		__Command="rm "+__ShortName+".tmp";
		#endif
		printf("\b\b\b2/5");
		fstream __InputStream;
		__InputStream.open(__ShortName+".tmp",ios::in);
		if(!__InputStream.good())
		{
			__Result.bSuccess=false;
			__Result.strMessage="\nError:File not found: \""+__ShortName+__GetFileCodec(__InputFileName)+"\"\n";
			return __Result;
		}
		__OutputStream<<">New-File\n";
		__OutputStream<<"!filetype "<<__GetFileCodec(__InputFileName)<<"\n!filename "<<__OutputFileName<<"\n";
		__OutputStream<<"!branch 0\n";
		__OutputStream<<"!version 1\n";
		printf("\b\b\b3/5");
		__GenerateMask();
		__EncryptMask();
		__OutputStream<<__Mask<<"\n";
		printf("\b\b\b4/5");
		char __In[256]="Nothing";
		string __ConvIn="Nothing";
		string __Out;
		bool __bSetup=false;
		int __Length,__Random;
		int __Prevch=0,__Modify;
		#ifdef _WIN32
		__InputStream.getline(__In,sizeof(__In));
		__ConvIn=__In;
		__Length=__ConvIn.length();
		while(__ConvIn!="-----END CERTIFICATE-----")
		{
			if(!__bSetup)
			{
				if(__ConvIn!="-----BEGIN CERTIFICATE-----")
				{
					__Result.bSuccess=false;
					__Result.strMessage="\nError: Maybe your Certificate Util cannot be used by this program.\nThe first line of the input file is not right.\nREQR: -----BEGIN CERTIFICATE-----\nYOUR: "+__ConvIn+"\n";
					return __Result;
				}
				__bSetup=true;
				__InputStream.getline(__In,sizeof(__In));
				__ConvIn=__In;
				__Length=__ConvIn.length();
				continue;
			}
			__Random=(rand()%(__Length+1))-1;
			__Out="";
			if(__Random==-1)
			{
				__Out+=__Notused[rand()%__ProxyLength];
			}
			for(int i=0;i<__Length;i++)
			{
				__Modify=__BaseToInt[(int)__In[i]]-__Prevch;
				if(__Modify<0) __Modify+=65;
				__Prevch=__BaseToInt[(int)__In[i]];
				__Out+=__ValidString[__BaseToValid[__Modify]];
				if(i==__Random) __Out+=__Notused[rand()%__ProxyLength];
			}
			__OutputStream<<__Out<<"\n";
			__InputStream.getline(__In,sizeof(__In));
			__ConvIn=__In;
			__Length=__ConvIn.length();
		}
		#else
		while(__InputStream>>__ConvIn)
		{
			__Length=__ConvIn.length();
			__Random=(rand()%(__Length+1))-1;
			__Out="";
			if(__Random==-1)
			{
				__Out+=__Notused[rand()&__ProxyLength];
			}
			for(int i=0;i<__Length;i++)
			{
				__Modify=__BaseToInt[(int)__ConvIn[i]]-__Prevch;
				if(__Modify<0) __Modify+=65;
				__Prevch=__BaseToInt[(int)__ConvIn[i]];
				__Out+=__ValidString[__BaseToValid[__Modify]];
				if(i==__Random) __Out+=__Notused[rand()&__ProxyLength];
			}
			__OutputStream<<__Out<<"\n";
		}
		#endif
		__OutputStream<<":eof"<<"\n";
		printf("\b\b\b5/5");
		__InputStream.close();
		system(__Command.c_str());
		printf("\b\b\bOK \n");
		__Result.bSuccess=true;
		__Result.strMessage="\""+__InputFileName+"\": Done.\n";
		return __Result;
	}
}

bool EncodeFileSet(int __Count,string *__FileSet,string __OutputFileName,string *__DisplayFileNames,int GeneratorVersions[][2])
{
	__EncodeResult __Temp;
	__OutputStream.open(__OutputFileName,ios::out);
	__Temp=__Encoder::__prebuild(__OutputStream);
	SetColor(10);
	printf("%s",__Temp.strMessage.c_str());
	SetColor();
	for(int i=0;i<__Count;i++)
	{
		if(GeneratorVersions[i][0]==0)
		{
			if(GeneratorVersions[i][1]==0)
			{
				__Temp=__Encoder::__pr_001(__FileSet[i],__OutputStream,__DisplayFileNames[i]);
			}
			else if(GeneratorVersions[i][1]==1)
			{
				__Temp=__Encoder::__pr_002(__FileSet[i],__OutputStream,__DisplayFileNames[i]);
			}
			else
			{
				printf("Error: Pre-Release %d is too new! The maxium support of this version is Pre-Release 1.\n",GeneratorVersions[i][1]);
			}
		}
		else
		{
			SetColor(12);
			printf("Error: Branch %d is too new! The maxium support of this version is 0(Pre-Release).\n",GeneratorVersions[i][0]);
		}
		if(__Temp.bSuccess)
		{
			SetColor(10);
			printf("%s",__Temp.strMessage.c_str());
			SetColor();
		}
		else
		{
			SetColor(12);
			printf("%s",__Temp.strMessage.c_str());
			SetColor();
		}
	}
	__Temp=__Encoder::__endfile(__OutputStream);
	__OutputStream.close();
	SetColor(10);
	printf("%s",__Temp.strMessage.c_str());
	SetColor();
	return false;
}

namespace __Decoder
{
	__DecodeResult __pr_001(fstream &__InputStream,string __OutputFileName)
	{
		__DecodeResult __Result;
		fstream __OutputStream;
		string __ShortName=__GetFileName(__OutputFileName);
		__OutputStream.open(__ShortName+".tmp",ios::out);
		printf("Proccessing file \"%s\"...1/5",__OutputFileName.c_str());
		__InputStream>>__Mask;
		// printf("%s\n",__Mask.c_str());
		printf("\b\b\b2/5");
		__InitFromMask();
		printf("\b\b\b3/5");
		#ifdef _WIN32
		__OutputStream<<"-----BEGIN CERTIFICATE-----\n";
		#endif
		string __Temp="Nothing",__Out;
		int __Length;
		// for(int i=0;i<95;i++) printf("%d\n",__ValidToInt[i]);
		while(__Temp!=":eof")
		{
			__InputStream>>__Temp;
			if(__Temp==":eof")
			{
				continue;
			}
			__Length=__Temp.length();
			__Out="";
			for(int i=0;i<__Length;i++)
			{
				__Out+=__BaseString[__ValidToBase[__ValidToInt[(int)__Temp[i]]]];
			}
			__OutputStream<<__Out<<"\n";
		}
		#ifdef _WIN32
		__OutputStream<<"-----END CERTIFICATE-----\n";
		#endif
		__OutputStream.close();
		printf("\b\b\b4/5");
		#ifdef _WIN32
		string __Command="certutil -f -decode "+__ShortName+".tmp \""+__OutputFileName+"\">nul";
		#else
		string __Command="base64 -d "+__ShortName+".tmp>\""+__OutputFileName+"\"";
		#endif
		system(__Command.c_str());
		printf("\b\b\b5/5");
		#ifdef _WIN32
		__Command="del /f /s /q \""+__ShortName+".tmp\">nul";
		#else
		__Command="rm "+__ShortName+".tmp";
		#endif
		system(__Command.c_str());
		printf("\b\b\bOK \n");
		__Result.bSuccess=true;
		__Result.strMessage="Done.\n";
		return __Result;
	}

	__DecodeResult __pr_002(fstream &__InputStream,string __OutputFileName)
	{
		string __ShortName=__GetFileName(__OutputFileName);
		__DecodeResult __Result;
		fstream __OutputStream;
		__OutputStream.open(__ShortName+".tmp",ios::out);
		printf("Proccessing file \"%s\"...1/5",__OutputFileName.c_str());
		__InputStream>>__Mask;
		__DecryptMask();
		// printf("%s\n",__Mask.c_str());
		printf("\b\b\b2/5");
		__InitFromMask();
		printf("\b\b\b3/5");
		#ifdef _WIN32
		__OutputStream<<"-----BEGIN CERTIFICATE-----\n";
		#endif
		string __Temp="Nothing",__Out;
		int __Length;
		// for(int i=0;i<95;i++) printf("%d\n",__ValidToInt[i]);
		int __Sum=0;
		while(__Temp!=":eof")
		{
			__InputStream>>__Temp;
			if(__Temp==":eof")
			{
				continue;
			}
			__Length=__Temp.length();
			__Out="";
			for(int i=0;i<__Length;i++)
			{
				if(__ValidToBase[__ValidToInt[(int)__Temp[i]]]==-1)
				{
					continue;
				}
				__Sum+=__ValidToBase[__ValidToInt[(int)__Temp[i]]];
				if(__Sum>64) __Sum-=65;
				__Out+=__BaseString[__Sum];
			}
			__OutputStream<<__Out<<"\n";
		}
		#ifdef _WIN32
		__OutputStream<<"-----END CERTIFICATE-----\n";
		#endif
		__OutputStream.close();
		printf("\b\b\b4/5");
		#ifdef _WIN32
		string __Command="certutil -f -decode "+__ShortName+".tmp \""+__OutputFileName+"\">nul";
		#else
		string __Command="base64 -d "+__ShortName+".tmp>\""+__OutputFileName+"\"";
		#endif
		system(__Command.c_str());
		printf("\b\b\b5/5");
		#ifdef _WIN32
		__Command="del /f /s /q \""+__ShortName+".tmp\">nul";
		#else
		__Command="rm "+__ShortName+".tmp";
		#endif
		system(__Command.c_str());
		printf("\b\b\bOK \n");
		__Result.bSuccess=true;
		__Result.strMessage="Done.\n";
		return __Result;
	}

	__DecodeResult __mainexec(fstream &__InputStream)
	{
		__DecodeResult __Result;
		string __Head;
		printf("Decrypting file...\nDoing 1/3...\n");
		__InputStream>>__Head;
		if(__Head!="Vencrypt?")
		{
			__Result.bSuccess=false;
			__Result.strMessage="Error:The file header is not a Vencrypt Archive.\n";
			return __Result;
		}
		__InputStream>>__Head;
		bool hasDn=false;
		printf("Doing 2/3...\n");
		if(__Head!=">Match-FileInfo")
		{
			if(__Head!=">New-File"&&__Head!=">End-Matches")
			{
				SetColor(12);
				printf("Error: Unknown argument");
			}
			SetColor(14);
			printf("Warning: FileInfo is not seted, we cannot know your generator.\n");
			SetColor();
		}
		else
		{
			__InputStream>>__Head;
			while(__Head!=">New-File"&&__Head!=">End-Matches")
			{
				if(__Head==":Change")
				{
					__InputStream>>__Head;
					if(__Head=="Generator-Display-Name")
					{
						if(hasDn)
						{
							SetColor(12);
							printf("Error: You only can change the display name of your generator for 1 time.\n");
							SetColor();
						}
						else
						{
							hasDn=true;
							SetColor(3);
							char Buf[512],Temp1;
							__InputStream>>Temp1;
							__InputStream.getline(Buf,sizeof(Buf));
							printf("Note: Got the information about the generator of the file.\nThis file is generated by %c%s\n",Temp1,Buf);
							SetColor();
						}
					}
					else
					{
						SetColor(12);
						printf("Error: Undefined properties \"%s\"!\n",__Head.c_str());
						SetColor();
					}
				}
				else
				{
					SetColor(12);
					printf("Error: Undefined method \"%s\" in Match-FileInfo!\n",__Head.c_str());
					SetColor();
				}
				__InputStream>>__Head;
			}
			
		}
		string __FileType,__FileName;
		int __Branch,__Version;
		char __FileNameBuf[1024];
		__DecodeResult __Temp;
		printf("Doing 3/3...\n");
		while(__Head!=">End-Matches")
		{
			if(__Head==">New-File")
			{
				__InputStream>>__Head;
				__InputStream>>__FileType;
				__InputStream>>__Head;
				__InputStream.getline(__FileNameBuf,sizeof(__FileNameBuf));
				__FileName=__FileNameBuf;
				__FileName=__FileName.substr(1,__FileName.length()-1);
				__FileName+="."+__FileType;
				__InputStream>>__Head;
				__InputStream>>__Branch;
				__InputStream>>__Head;
				__InputStream>>__Version;
				if(__Branch==0)
				{
					if(__Version==0)
					{
						__Temp=__pr_001(__InputStream,__FileName);
						if(__Temp.bSuccess)
						{
							SetColor(10);
							printf("%s",__Temp.strMessage.c_str());
							SetColor();
						}
						else
						{
							SetColor(12);
							printf("%s",__Temp.strMessage.c_str());
							SetColor();
						}
					}
					else if(__Version==1)
					{
						__Temp=__pr_002(__InputStream,__FileName);
						if(__Temp.bSuccess)
						{
							SetColor(10);
							printf("%s",__Temp.strMessage.c_str());
							SetColor();
						}
						else
						{
							SetColor(12);
							printf("%s",__Temp.strMessage.c_str());
							SetColor();
						}
					}
					else
					{
						SetColor(12);
						printf("Error: Pre-Release %d is too new! The maxium support of this version is Pre-Release 1.\n",__Version);
						SetColor();
					}
				}
				else
				{
					SetColor(12);
					printf("Error: Branch %d is too new! The maxium support of this version is 0(Pre-Release).\n",__Branch);
					SetColor();
				}
			}
			else
			{
				SetColor(12);
				printf("Error: Undefined \"%s\"!\n",__Head.c_str());
				SetColor();
			}
			__InputStream>>__Head;
		}
		__Result.bSuccess=true;
		__Result.strMessage="Successfully to decrypt files.\n";
		return __Result;
	}
}

bool DecodeFile(string __InputFileName)
{
	if(!__bInit)
	{
		printf("Preparing...\n");
		__InitSystem();
		printf("Done.\n");
	}
	__DecodeResult __Temp;
	fstream __InputStream;
	__InputStream.open(__InputFileName,ios::in);
	if(!__InputStream.good()) return true;
	__Temp=__Decoder::__mainexec(__InputStream);
	SetColor(10);
	printf("%s",__Temp.strMessage.c_str());
	SetColor();
	__InputStream.close();
	return false;
}

#endif
