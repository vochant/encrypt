#include "venc.cpp"

int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		SetColor(12);
		printf("Error: No/Too many input files.\n");
		SetColor();
		return 1;
	}
	string tmp=argv[1];
	DecodeFile(tmp);
}