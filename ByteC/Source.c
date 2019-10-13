#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>

int getStringLength(char* string);

typedef struct TextFormat
{
	unsigned char  stringLength;
	unsigned char  countOfLetters;
	int   sumOfLetterCodes;
	unsigned char* string;

}Format;

int main(void)
{
	#pragma region variables
	char* locale = setlocale(LC_ALL, "");
	unsigned char* line;

	FILE* InputTXTFileThread;
	const char* InputPath = "input.txt";
	int			InputLinesLenght = 0;
	int			MaxLettersInLine = 128;

	FILE* OutputBinFileThread;
	const char* OutputPath = "output.bin";
	const char* TestBin = "test.bin";

	Format* Lines;
	#pragma endregion

	printf("\t����� 1. ������ � �������� ����\n");

	if ((InputTXTFileThread = fopen(InputPath, "r")) == NULL)
	{
		printf("���� %s �� ����������\n", InputPath);
		return 1;
	}
	else
	{
		printf("���� %s ������\n\n", InputPath);
	}

	if ((OutputBinFileThread = fopen(OutputPath, "w")) == NULL)
	{
		printf("���� %s �� ����������\n", OutputPath);
		return 1;
	}
	else
	{
		printf("���� %s ������\n\n", OutputPath);
	}

	line = (unsigned char*)malloc(sizeof(unsigned char) * MaxLettersInLine);
	while (fgets(line, MaxLettersInLine, InputTXTFileThread) != NULL)
	{
		printf("\n������ �%d:\t%s", InputLinesLenght, line);
		InputLinesLenght++;

		int LineLength = getStringLength(line);
		if (line[LineLength - 1] == '\0' || line[LineLength - 1] == '\n')
		{
			LineLength--;
		}

		int countOfLetters = 0;
		int sumOfLetterCodes = 0;
		Format outputFormat;
		size_t result;

		for (int i = 0; i < LineLength; i++)
		{
			int LetterCode = (int)(unsigned char)line[i];
			sumOfLetterCodes += LetterCode;
			if ((LetterCode >= 65) && (LetterCode <= 90) || (LetterCode >= 97) && (LetterCode <= 122) ||
				(LetterCode >= 128) && (LetterCode <= 175) ||
				(LetterCode >= 224) && (LetterCode <= 241))
				countOfLetters++;
		}
		outputFormat.countOfLetters = (unsigned char)countOfLetters;
		outputFormat.stringLength = (unsigned char)LineLength;
		outputFormat.sumOfLetterCodes = sumOfLetterCodes;
		outputFormat.string = line;

		result = fwrite(&outputFormat.stringLength, sizeof(char), 1, OutputBinFileThread);
		if (result != 1)
		{
			printf("�� ������� �������� ����� ������ = %d � %s\n", (int)outputFormat.stringLength, OutputPath);
			break;
		}

		result = fwrite(&outputFormat.countOfLetters, sizeof(char), 1, OutputBinFileThread);
		if (result != 1)
		{
			printf("�� ������� �������� ���������� ���� � ������ = %d � %s\n", (int)outputFormat.countOfLetters, OutputPath);
			break;
		}

		result = fwrite(&outputFormat.sumOfLetterCodes, sizeof(outputFormat.sumOfLetterCodes), 1, OutputBinFileThread);
		if (result != 1)

		{
			printf("�� ������� �������� ����� ����� �������� = %d � %s\n", (int)outputFormat.sumOfLetterCodes, OutputPath);
			break;
		}

		result = fwrite(outputFormat.string, sizeof(char), outputFormat.stringLength, OutputBinFileThread);
		if (result != outputFormat.stringLength)
		{
			printf("�� ������� �������� ������ � %s\n", OutputPath);
			break;
		}
		if (line[LineLength] != '\n') printf("\n");
		printf("����� ������, ���������� ����, ����� ����� �������� � ������ �������� � %s\n", OutputPath);
	}
	printf("\n");

	fclose(InputTXTFileThread);
	printf("%s ������\n", InputPath);
	fclose(OutputBinFileThread);
	printf("%s ������\n", OutputPath);


	printf("\n\n\t����� 2. ����� ��������� �����\n\n");

	if ((OutputBinFileThread = fopen(OutputPath, "r")) == NULL)
	{
		printf("���� %s �� ����������\n", OutputPath);
		return 1;
	}
	else
	{
		printf("���� %s ������\n\n", OutputPath);
	}

	fseek(OutputBinFileThread, 0, SEEK_END);
	printf("������ �����: %d ����\n", ftell(OutputBinFileThread));
	rewind(OutputBinFileThread);

	while (1)
	{
		Format inputFormat;
		size_t result = fread(&inputFormat.stringLength, sizeof(char), 1, OutputBinFileThread);
		if (result != 1)
		{
			break;
		}
		if (inputFormat.stringLength < 0) 
		{
			printf("[ERROR] ������ � �������� �����. ����� ������ ������ ���� > 0\n");
		}


		result = fread(&inputFormat.countOfLetters, sizeof(char), 1, OutputBinFileThread);
		if (result != 1)
		{
			printf("[ERROR] �� ������� ������� ���������� ���� ������ �� �����\n");
			break;
		}
		result = fread(&inputFormat.sumOfLetterCodes, sizeof(int), 1, OutputBinFileThread);
		if (result != 1)
		{
			printf("[ERROR] �� ������� ������� ����� ����� �������� ������ �� �����\n");
			break;
		}

		inputFormat.string = (char*)malloc(sizeof(char) * (int)inputFormat.stringLength);
		result = fread(inputFormat.string, sizeof(char), (int)inputFormat.stringLength, OutputBinFileThread);
		if (result != (int)inputFormat.stringLength)
		{
			printf("[WARNING] �� ������� ������� ��� ������ �� �����. ��������� %d ����\n", result);
			inputFormat.string[result - 1] = 0;
		}
		inputFormat.string[(int)inputFormat.stringLength] = 0;
		printf("����� ������= %d\t���������� ����= %d\t����� ����� ��������= %d\t������: %s\n", (int)inputFormat.stringLength, (int)inputFormat.countOfLetters, inputFormat.sumOfLetterCodes, inputFormat.string);
	}

	return 0;
}

int getStringLength(char* string)
{
	int index = 0;
	while (string[index] != NULL)
	{
		index++;
	}
	return index;
}
