#include "venc.cpp"

string TargetSet[1024],ToSet[1024];
string Output="output.vnecr";
string Input="%%%";

int VersionIds[1024][2];

inline void ProgramVersion()
{
	printf("\n");
	printf("Vochant Encrypt(Standard) PR-0.1.1\n");
	printf("Encoder 0.1, Decoder 0.1\n");
	printf("Version Id 0:1\n");
	#ifdef _MSC_VER
	#ifdef __clang__
	printf("Compiler: Clang (Microsoft Visual-C++)\n");
	#else
	printf("Compiler: Microsoft(R) C-C++ Compiler\n");
	#endif
	#ifdef _M_X64
	printf("Target Archive: X64\n");
	#elif _M_IX86
	printf("Target Archive: X86\n");
	#elif _M_ARM64
	printf("Target Archive: ARM64\n");
	#elif _M_ARM
	printf("Target Archive: ARM\n");
	#elif _M_THUMB
	printf("Target Archive: Thumb\n");
	#else
	printf("Target Archive: Unknown\n");
	#endif
	#else
	#ifdef __GNUC__
	printf("Compiler: GNU Compiler Collection\n");
	#elif __clang__
	printf("Compiler: Clang (Original)\n");
	#else
	printf("Compiler: Unknown\n");
	#endif
	#ifdef __x86_64__
	printf("Target Archive: amd64\n");
	#elif __i386__
	printf("Target Archive: i386\n");
	#elif __aarch64__
	printf("Target Archive: aarch64\n");
	#elif __arm__
	printf("Target Archive: arm\n");
	#elif __thumb
	printf("Target Archive: thumb\n");
	#else
	printf("Target Archive: unknown\n");
	#endif
	#endif
	#ifdef _WIN32
	#ifdef _WIN64
	printf("System: Microsoft Windows\nTested Archive: x64\n");
	#else
	printf("System: Microsoft Windows\nTested Archive: x86\n");
	#endif
	#else
	#ifdef __APPLE__
	#include "TargetConditionals.h"
	#if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
	printf("System: iPhone Simulator\n");
	#elif TARGET_OS_IPHONE
	printf("System: iPhone\n");
	#elif TARGET_OS_MAC
	printf("System: macOS\n");
	#else
	printf("System: Unknown Apple System\n");
	#endif
	#elif BSD
	printf("System: BSD\n");
	#elif __unix__
	#ifdef __linux__
	#ifdef __ANDROID
	printf("System: Android\n");
	#else
	printf("System: Linux\n");
	#endif
	#else
	printf("System: Unix\n");
	#endif
	#else
	printf("System: Unknown\n");
	#endif
	#ifdef __LP64
	printf("Tested: 64 bits\n");
	#else
	printf("Tested: 32 or lower bits\n");
	#endif
	#endif
}

inline void ProgramHelp()
{
	printf("\n");
	printf("Usage: vect [options]\n");
	printf("Options:\n");
	printf("  -e                    Encrypt files.\n");
	printf("  --encode              Encrypt files.\n");
	printf("  --encrypt             Encrypt files.\n");
	printf("    -i file             Include a file to the list for encrypting.\n");
	printf("    --input file        Include a file to the list for encrypting.\n");
	printf("      -c version        Set the version of the encoder for this file's encrypting.\n");
	printf("      --coder version   Set the version of the encoder for this file's encrypting.\n");
	printf("      -n name           Set the output filename of this file.\n");
	printf("      --name name       Set the output filename of this file.\n");
	printf("    -o file             Set the filename of the output file.\n");
	printf("    --output file       Set the filename of the output file.\n");
	printf("    -D version          Set the default version of the encoder for any one's\n");
	printf("                        encrypting of these file without argument -c/--coder.\n");
	printf("    --default version   Set the default version of the encoder for any one's\n");
	printf("                        encrypting of these file without argument -c/--coder.\n");
	printf("\n");
	printf("  -d file               Decrypt a file.\n");
	printf("  --decode file         Decrypt a file.\n");
	printf("  --decrypt file        Decrypt a file.\n");
	printf("\n");
	printf("  -v                    See the version of your software.\n");
	printf("  --version             See the version of your software.\n");
	printf("  -h                    See the helps of this software.\n");
	printf("  --help                See the helps of this software.\n");
}

int main(int argc,char *argv[])
{
	int Count=0;
	int Filla=0,Fillb=1;
	bool bOutput=false;
	if(argc==1)
	{
		SetColor(14);
		printf("Warning: you should use this program by some args.\n");
		SetColor();
		ProgramVersion();
		ProgramHelp();
		return 0;
	}
	string Temp=argv[1],temp2;
	if(Temp=="-e"||Temp=="--encode"||Temp=="--encrypt")
	{
		for(int i=2;i<argc;i++)
		{
			Temp=argv[i];
			if(Temp=="-i"||Temp=="--input")
			{
				if(argc==i+1)
				{
					SetColor(12);
					printf("Error: Nothing is after an Add-Input-File method.\n");
					SetColor();
					return 1;
				}
				TargetSet[Count]=argv[i+1];
				VersionIds[Count][0]=-1;
				VersionIds[Count][1]=-1;
				ToSet[Count]=__GetFileName(argv[i+1]);
				Count++;
				i++;
			}
			else if(Temp=="-o"||Temp=="--output")
			{
				if(argc==i+1)
				{
					SetColor(12);
					printf("Error: Nothing is after Set-Output-File method.\n");
					SetColor();
					return 1;
				}
				if(bOutput)
				{
					SetColor(12);
					printf("Error: Only support single output file.\n");
					SetColor();
					return 1;
				}
				bOutput = true;
				Output = argv[i+1];
				i++;
			}
			else if(Temp=="-c"||Temp=="--coder")
			{
				if(argc<=i+2)
				{
					SetColor(12);
					printf("Error: Not enough arguments is after a Set-Output-Version method.\n");
					SetColor();
					return 1;
				}
				int ta,tb;
				_sscanf(argv[i+1],"%d",&ta);
				_sscanf(argv[i+2],"%d",&tb);
				string Sa=argv[i+1],Sb=argv[i+2];
				string Sc,Sd;
				char Se[1024];
				sprintf(Se,"%d",ta);
				Sc=Se;
				sprintf(Se,"%d",tb);
				Sd=Se;
				if(Sc!=Sa||Sd!=Sb)
				{
					SetColor(12);
					printf("Error: Not right arguments is after a Set-Output-Version method.\n");
					SetColor();
					return 1;
				}
				VersionIds[Count-1][0]=ta;
				VersionIds[Count-1][1]=tb;
				i+=2;
			}
			else if(Temp=="-n"||Temp=="--name")
			{
				if(argc==i+1)
				{
					SetColor(12);
					printf("Error: Nothing is after Set-Output-Name method.\n");
					SetColor();
					return 1;
				}
				ToSet[Count-1]=argv[i+1];
				i++;
			}
			else if(Temp=="-D"||Temp=="--default")
			{
				if(argc<=i+2)
				{
					SetColor(12);
					printf("Error: Not enough arguments is after a Set-Output-Default-Version method.\n");
					SetColor();
					return 1;
				}
				int ta,tb;
				_sscanf(argv[i+1],"%d",&ta);
				_sscanf(argv[i+2],"%d",&tb);
				string Sa=argv[i+1],Sb=argv[i+2];
				string Sc,Sd;
				char Se[1024];
				sprintf(Se,"%d",ta);
				Sc=Se;
				sprintf(Se,"%d",tb);
				Sd=Se;
				if(Sc!=Sa||Sd!=Sb)
				{
					SetColor(12);
					printf("Error: Not right arguments is after a Set-Output-Default-Version method.\n");
					SetColor();
					return 1;
				}
				Filla=ta;
				Fillb=tb;
				i+=2;
			}
			else
			{
				SetColor(12);
				printf("Error: Unknown argument.\n");
				SetColor();
				return 1;
			}
		}
		if(Count==0)
		{
			SetColor(12);
			printf("Error: No input files.\n");
			SetColor();
			return 1;
		}
		for(int aa=0;aa<Count;aa++)
		{
			if(VersionIds[aa][0]==-1)
			{
				VersionIds[aa][0]=Filla;
			}
			if(VersionIds[aa][1]==-1)
			{
				VersionIds[aa][1]=Fillb;
			}
		}
		return (int)EncodeFileSet(Count,TargetSet,Output,ToSet,VersionIds);
	}
	else if(Temp=="-d"||Temp=="--decode"||Temp=="--decrypt")
	{
		if(argc!=3)
		{
			SetColor(12);
			printf("Error: No/Too many files to decrypt.\n");
			SetColor();
			return 1;
		}
		Input=argv[2];
		return (int)DecodeFile(Input);
	}
	else if(Temp=="-v"||Temp=="--version")
	{
		ProgramVersion();
		return 0;
	}
	else if(Temp=="-h"||Temp=="--help")
	{
		ProgramHelp();
		return 0;
	}
	else
	{
		SetColor(12);
		printf("Error: Unknown method.\n");
		SetColor();
		return 1;
	}
}
