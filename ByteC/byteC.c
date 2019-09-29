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

	Format* Lines;
#pragma endregion

	printf("Часть 1. Запись в бинарный файл\n");

	if ((InputTXTFileThread = fopen(InputPath, "r")) == NULL)
	{
		printf("Файл %s не существует\n", InputPath);
		return 1;
	}
	else
	{
		printf("Файл %s открыт\n\n", InputPath);
	}

	if ((OutputBinFileThread = fopen(OutputPath, "w")) == NULL)
	{
		printf("Файл %s не существует\n", OutputPath);
		return 1;
	}
	else
	{
		printf("Файл %s открыт\n\n", OutputPath);
	}

	line = (unsigned char*)malloc(sizeof(unsigned char) * MaxLettersInLine);
	while (fgets(line, MaxLettersInLine, InputTXTFileThread) != NULL)
	{
		printf("\nСтрока №%d:\t%s", InputLinesLenght, line);
		InputLinesLenght++;

		int LineLength = getStringLength(line);
		int countOfLetters = 0;
		int sumOfLetterCodes = 0;
		Format outputFormat;
		size_t result;

		for (int i = 0; i < LineLength; i++)
		{
			int LetterCode = (int)(unsigned char)line[i];
			sumOfLetterCodes += LetterCode;
			if ((LetterCode >= 65) && (LetterCode <= 90) ||		// Eng UpCase
				(LetterCode >= 97) && (LetterCode <= 122) ||	// Eng LowerCase
				(LetterCode >= 128) && (LetterCode <= 175) ||	// Rus UpCase + LowerCase
				(LetterCode >= 224) && (LetterCode <= 241))		// Rus UpCase + LowerCase
				countOfLetters++;
		}
		outputFormat.countOfLetters = (unsigned char)countOfLetters;
		outputFormat.stringLength = (unsigned char)LineLength;
		outputFormat.sumOfLetterCodes = sumOfLetterCodes;
		outputFormat.string = line;

		result = fwrite(&outputFormat.stringLength, sizeof(char), 1, OutputBinFileThread);
		if (result != 1)
		{
			printf("Не удалось записать длину строки = %d в %s\n", (int)outputFormat.stringLength, OutputPath);
			break;
		}
		result = fwrite(&outputFormat.countOfLetters, sizeof(char), 1, OutputBinFileThread);
		if (result != 1)
		{
			printf("Не удалось записать количество букв в строке = %d в %s\n", (int)outputFormat.countOfLetters, OutputPath);
			break;
		}
		result = fwrite(&outputFormat.sumOfLetterCodes, sizeof(outputFormat.sumOfLetterCodes), 1, OutputBinFileThread);
		if (result != 1)
		{
			printf("Не удалось записать сумму кодов символов = %d в %s\n", (int)outputFormat.sumOfLetterCodes, OutputPath);
			break;
		}
		result = fwrite(outputFormat.string, sizeof(char), outputFormat.stringLength, OutputBinFileThread);
		if (result != outputFormat.stringLength)
		{
			printf("Не удалось записать строку в %s\n", OutputPath);
			break;
		}
		if (line[LineLength - 1] != '\n') printf("\n");
		printf("Длина строки, количество букв, сумма кодов символов и строка записаны в %s\n", OutputPath);
	}
	printf("\n");

	fclose(InputTXTFileThread);
	printf("%s закрыт\n", InputPath);
	fclose(OutputBinFileThread);
	printf("%s закрыт\n", OutputPath);


	printf("Часть 2. Вывод бинарного файла\n");
	if ((OutputBinFileThread = fopen(OutputPath, "r")) == NULL)
	{
		printf("Файл %s не существует\n", OutputPath);
		return 1;
	}
	else
	{
		printf("Файл %s открыт\n\n", OutputPath);
	}

	for (int i = 0; i < InputLinesLenght; i++)
	{
		Format inputFormat;
		size_t result = fread(&inputFormat.stringLength, sizeof(char), 1, OutputBinFileThread);
		if (result != 1) 
		{
			printf("Не удалось считать длину строки из файла\n");
		}
		result = fread(&inputFormat.countOfLetters, sizeof(char), 1, OutputBinFileThread);
		if (result != 1)
		{
			printf("Не удалось считать количество букв строки из файла\n");
		}
		result = fread(&inputFormat.sumOfLetterCodes, sizeof(int), 1, OutputBinFileThread);
		if (result != 1)
		{
			printf("Не удалось считать сумму кодов символов строки из файла\n");
		}

		inputFormat.string = (char*)malloc(sizeof(char) * (int)inputFormat.stringLength);
		result = fread(inputFormat.string, sizeof(char), (int)inputFormat.stringLength, OutputBinFileThread);
		if (result != (int)inputFormat.stringLength)
		{
			printf("Не удалось считать строку из файла\n");
		}
		inputFormat.string[(int)inputFormat.stringLength] = 0;
		printf("Длина строки= %d\tКоличество букв= %d\tСумма кодов символов= %d\tСтрока: %s\n", (int)inputFormat.stringLength, (int)inputFormat.countOfLetters, inputFormat.sumOfLetterCodes, inputFormat.string);
	}

	getchar();
	return 0;
}

int getStringLength(char* string)
{
	int index = 0;
	while(string[index] != NULL)
	{
		index++;
	}
	return index;
}