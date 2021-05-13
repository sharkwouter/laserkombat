#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdio.h>
class Exception
{
public:
	Exception(const char* string) : text(string) { }
	static void Output(const char* string) {
		FILE* file = fopen("Output.txt", "a");
		fprintf( file, string);
		fprintf( file, "\r\n");
		fclose(file);
	}
	const char* what() {return text;}
private:
	const char* text;
};

#endif // EXCEPTION_H
