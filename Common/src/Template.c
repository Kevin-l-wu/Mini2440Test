#include <stdio.h>
#include "Uart.h"
#include "Template.h"

Template Global_ascii_template[] = {
	{'A', TEMPLATE_A},
	{'B', TEMPLATE_B},
	{'C', TEMPLATE_C},
	{'D', TEMPLATE_D},
	{'E', TEMPLATE_E},
	{'F', TEMPLATE_F},
	{'G', TEMPLATE_G},
	{'H', TEMPLATE_H},
	{'I', TEMPLATE_I},
	{'J', TEMPLATE_J},
	{'K', TEMPLATE_K},
	{'L', TEMPLATE_L},
	{'M', TEMPLATE_M},
	{'N', TEMPLATE_N},
	{'O', TEMPLATE_O},
	{'P', TEMPLATE_P},
	{'Q', TEMPLATE_Q},
	{'R', TEMPLATE_R},
	{'S', TEMPLATE_S},
	{'T', TEMPLATE_T},
	{'U', TEMPLATE_U},
	{'V', TEMPLATE_V},
	{'W', TEMPLATE_W},
	{'X', TEMPLATE_X},
	{'Y', TEMPLATE_Y},
	{'Z', TEMPLATE_Z},
	{' ', TEMPLATE_SPACE},
};

void fill_template_color(unsigned short color, TEMPLATE8X16* ascii_template)
{
	unsigned index = 0;
	
	if(ascii_template != NULL && color != 0x00)
	{
		for(index = 0; index < (8 * 16); index++)
		{
			if((*ascii_template)[index] != 0x00)
			{
				(*ascii_template)[index] = color;
			}
			else
			{
				(*ascii_template)[index] = 0x00;
			}
		}
	}
}

unsigned get_template(unsigned char ascii_char, unsigned short color, TEMPLATE8X16** ascii_template)
{
	unsigned index = 0;
//	LogPrintf("In function get_template\n\n");
//	LogPrintf("&Global_ascii_template = 0x%x\n\n", &Global_ascii_template);
//	LogPrintf("Global_ascii_template = 0x%x\n\n", Global_ascii_template);
	
	for(index = 0; index < (sizeof(Global_ascii_template) / sizeof(Global_ascii_template[0])); index++)
	{
		if(Global_ascii_template[index].ascii_char == ascii_char)
		{
//			LogPrintf("index = %d\n\n", index);
//			LogPrintf("&(Global_ascii_template[index].ascii_template) = 0x%x\n\n", &(Global_ascii_template[index].ascii_template));
			*ascii_template = &(Global_ascii_template[index].ascii_template);
			break;
		}
	}
	
	fill_template_color(color, *ascii_template);

	return index;
}