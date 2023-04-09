#include "venc.cpp"

string TargetSet[1024],ToSet[1024];
string Output="output.vnecr";

int VersionSet[1024][2];

int main(int argc,char *argv[])
{
	int Count=argc-1;
	string tmp;
	if(Count==0)
	{
		SetColor(12);
		printf("Error: No input files.\n");
		SetColor();
		return 1;
	}
	if(Count==1)
	{
		tmp=argv[1];
		Output=__GetFileName(tmp)+".encrypted.txt";
	}
	else
	{
		Output="multi.encrypted.txt";
	}
	for(int i=1;i<=Count;i++)
	{
		TargetSet[i-1]=argv[i];
		VersionSet[i-1][0]=0;
		VersionSet[i-1][1]=1;
		ToSet[i-1]=__GetFileName(TargetSet[i-1]);
	}
	return (int)EncodeFileSet(Count,TargetSet,Output,ToSet,VersionSet);
}