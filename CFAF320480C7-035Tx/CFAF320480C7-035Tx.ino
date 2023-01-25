//==============================================================================
//
//  CRYSTALFONTZ

//  The controller is a Ilitek ILI9488
//    https://www.crystalfontz.com/controllers/Ilitek/ILI9488/
//
//  Seeeduino v4.2, an open-source 3.3v capable Arduino clone.
//    https://www.seeedstudio.com/Seeeduino-V4.2-p-2517.html
//    https://github.com/SeeedDocument/SeeeduinoV4/raw/master/resources/Seeeduino_v4.2_sch.pdf
//
//==============================================================================
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>
//==============================================================================
// LCD parallel 8080 pins and control lines on Seeeduino:
//  ARD      | Port  | Display pin |  Function - 8080 Parallel                |
//-----------+-------+-------------+------------------------------------------+
// 3.3V/5V   |       |             |  POWER 3.3V                              |
// GND       |       |             |  GROUND                                  |
//-----------+-------+-------------+------------------------------------------+
// D8        | PORTB |  38         |  Chip Enable Signal              (CS)    |
// D9        | PORTB |  09         |  Reset                           (Reset) |
// D10       | PORTB |  37         |  Data/Command                    (DC)    |
// D11       | PORTB |  36         |  Write                           (WR)    |
// D12       | PORTB |  35         |  Read                            (RD)    |
//-----------+-------+-------------+------------------------------------------+
// Data Lines
//-----------+-------+-------------+------------------------------------------+
// D0        | PORTD |  31         |  DATA BUS LINE (DB0)                     |
// D1        | PORTD |  30         |  DATA BUS LINE (DB1)                     |
// D2        | PORTD |  29         |  DATA BUS LINE (DB2)                     |
// D3        | PORTD |  28         |  DATA BUS LINE (DB3)                     |
// D4        | PORTD |  27         |  DATA BUS LINE (DB4)                     |
// D5        | PORTD |  26         |  DATA BUS LINE (DB5)                     |
// D6        | PORTD |  25         |  DATA BUS LINE (DB6)                     |
// D7        | PORTD |  24         |  DATA BUS LINE (DB7)                     |
//-----------+-------+-------------+------------------------------------------+
//==============================================================================
// Interface Selection
// IM2 | IM1 | IM0 |  Interface mode  |
//-----+-----+-----+------------------+
// 0   | 1   | 1   |  8-bit parallel  |
// 1   | 1   | 1   |  4-wire SPI      |
//-----+-----+-----+------------------+

#include <Arduino.h>
#include <stdint.h>
//#include <SD.h>

// set the resolution to match the display being used
#define HRES 480
#define VRES 320

// define the Arduino pin locations as per the table above
#define CS (8)
#define RESET (9)
#define DC (10)
#define WR (11)
#define RD (12)
#define datapins (PORTD)

// define clear and set commands for each of the pins
#define CLR_CS (PORTB &= ~(0x01))
#define SET_CS (PORTB |= (0x01))
#define CLR_RESET (PORTB &= ~(0x02))
#define SET_RESET (PORTB |= (0x02))
#define CLR_DC (PORTB &= ~(0x04))
#define SET_DC (PORTB |= (0x04))
#define CLR_WR (PORTC &= ~(0x02))
#define SET_WR (PORTC |= (0x02))
#define CLR_RD (PORTC &= ~(0x04))
#define SET_RD (PORTC |= (0x04))

// ILI9488 defines
#define ILI9488_PGAMCTRL (0xE0)
#define ILI9488_NGAMCTRL (0xE1)
#define ILI9488_PCTRL1 (0xC0)
#define ILI9488_PCTRL2 (0xC1)
#define ILI9488_VCOMCTRL (0xC5)
#define ILI9488_MADCTL (0x36)
#define ILI9488_COLMOD (0x3A)
#define ILI9488_FRMCTR (0xB1)
#define ILI9488_INVTR (0xB4)
#define ILI9488_SETIMG (0xE9)
#define ILI9488_ADJCTRL3 (0xF7)
#define ILI9488_DISCTRL (0xB6)
#define ILI9488_CASET (0x2A)
#define ILI9488_PAGESET (0x2B)
#define ILI9488_SLPOUT (0x11)
#define ILI9488_DISPON (0x29)
#define ILI9488_RAMWR (0x2C)

// send a command via the parallel interface
void write_command(uint8_t command)
{
	// select the LCD's command register
	CLR_DC;
	// select the LCD controller
	CLR_CS;
	// transfer command via datapins
	datapins = command;
	// clear the write register
	CLR_WR;
	// set the write register
	SET_WR;
	// deselct the LCD controller
	SET_CS;
}

// send data via the parallel interface
void write_data(uint8_t data)
{
	// select the LCD's data register
	SET_DC;
	// select the LCD controller
	CLR_CS;
	// transfer data via datapins
	datapins = data;
	// clear the write register
	CLR_WR;
	// set the write register
	SET_WR;
	// deselct the LCD controller
	SET_CS;
}

// display initialization sequence
void initialize_display()
{
	SET_RESET;
	_delay_ms(1);
	CLR_RESET;
	_delay_ms(10);
	SET_RESET;
	_delay_ms(120);

	write_command(ILI9488_PGAMCTRL);
	write_data(0x00);
	write_data(0x04);
	write_data(0x0E);
	write_data(0x08);
	write_data(0x17);
	write_data(0x0A);
	write_data(0x40);
	write_data(0x79);
	write_data(0x4D);
	write_data(0x07);
	write_data(0x0E);
	write_data(0x0A);
	write_data(0x1A);
	write_data(0x1D);
	write_data(0x0F);

	write_command(ILI9488_NGAMCTRL);
	write_data(0x00);
	write_data(0x1B);
	write_data(0x1F);
	write_data(0x02);
	write_data(0x10);
	write_data(0x05);
	write_data(0x32);
	write_data(0x34);
	write_data(0x43);
	write_data(0x02);
	write_data(0x0A);
	write_data(0x09);
	write_data(0x33);
	write_data(0x37);
	write_data(0x0F);

	write_command(ILI9488_PCTRL1);
	write_data(0x18);
	write_data(0x16);

	write_command(ILI9488_PCTRL2);
	write_data(0x41);

	write_command(ILI9488_VCOMCTRL);
	write_data(0x00);
	write_data(0x1E);
	write_data(0x80);

	write_command(ILI9488_MADCTL);
	write_data(0x48);

	write_command(ILI9488_COLMOD);
	write_data(0x55); // 16bits/pixel

	write_command(ILI9488_FRMCTR);
	write_data(0xB0);

	write_command(ILI9488_INVTR);
	write_data(0x02);

	write_command(ILI9488_SETIMG);
	write_data(0x00);

	write_command(ILI9488_ADJCTRL3);
	write_data(0xA9);
	write_data(0x51);
	write_data(0x2C);
	write_data(0x82);

	write_command(ILI9488_DISCTRL);
	write_data(0x02);
	write_data(0x02);
	write_data(0x3B);

	write_command(ILI9488_CASET);
	write_data(0x00);
	write_data(0x00);
	write_data(0x01);
	write_data(0x3F);

	write_command(ILI9488_PAGESET);
	write_data(0x00);
	write_data(0x00);
	write_data(0x01);
	write_data(0xDF);

	write_command(ILI9488_SLPOUT);
	delay(120); // it is required to wait 120ms before sending the next command
	write_command(ILI9488_DISPON);
	write_command(ILI9488_RAMWR);
}

// fill screen with a single color
void show_single_color_on_display(unsigned int color)
{
	unsigned int i, j;

	for (j = 0; j < 480; j++)
	{
		for (i = 0; i < 320; i++)
		{
			write_data(color >> 8);
			write_data(color & 0xff);
		}
	}
}

void setup()
{
	// set up port directions
	DDRD = 0xFF;
	DDRB = 0x1F;

	// configure the pins in a reasonable starting state
	datapins = 0;
	SET_RD;
	SET_WR;
	SET_CS;
	CLR_DC;
	delay(1);

	initialize_display();
}

// more demos will be added soon!
#define showcolor_demo 1

void loop()
{
#if showcolor_demo
	show_single_color_on_display(0xF800);
	delay(5000);
	show_single_color_on_display(0x07E0);
	delay(5000);
	show_single_color_on_display(0x001F);
	delay(5000);
#endif
}

