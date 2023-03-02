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
// D8        | PORTB |  38         |  Chip enable signal              (CS)    |
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
// LCD SPI pins and control lines on Seeeduino:
//  ARD      | Port  | Display pin |  Function - SPI                          |
//-----------+-------+-------------+------------------------------------------+
// 3.3V/5V   |       |             |  POWER 3.3V                              |
// GND       |       |             |  GROUND                                  |
//-----------+-------+-------------+------------------------------------------+
// D8        | PORTB |  37         |  Data/Command                    (DC)    |
// D9        | PORTB |  09         |  Reset                           (Reset) |
// D10       | PORTB |  38         |  Chip select                     (CS)    |
// D11       | PORTB |  34         |  SPI data input                  (SDA)   |
// D13       | PORTB |  36         |  Serial clock                    (SCK)   |
//-----------+-------+-------------+------------------------------------------+
// Interface Selection
// IM2 | IM1 | IM0 |  Interface mode  |
//-----+-----+-----+------------------+
// 0   | 1   | 1   |  8-bit parallel  |
// 1   | 1   | 1   |  4-wire SPI      |
//-----+-----+-----+------------------+

#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include <SD.h>

// set the resolution to match the display being used
#define HRES 480
#define VRES 320

// set the interface to be used
#define interface_SPI 1
#define interface_PARALLEL 0

#if interface_PARALLEL

// define the Arduino pin locations as per the parallel 8080 table above
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

#endif

#if interface_SPI

// define the Arduino pin locations as per the SPI table above
#define DC (8)
#define RESET (9)
#define CS (10)
#define MOSI (11)
#define SCK (13)
// define clear and set commands for each of the pins
#define CLR_DC (PORTB &= ~(0x01))
#define SET_DC (PORTB |= (0x01))
#define CLR_RESET (PORTB &= ~(0x02))
#define SET_RESET (PORTB |= (0x02))
#define CLR_CS (PORTB &= ~(0x04))
#define SET_CS (PORTB |= (0x04))
#define CLR_MOSI (PORTB &= ~(0x08))
#define SET_MOSI (PORTB |= (0x08))
#define CLR_SCK (PORTB &= ~(0x20))
#define SET_SCK (PORTB |= (0x20))

#endif

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

#if interface_PARALLEL
// send a command via the parallel interface
void write_command(uint8_t command)
{
	// select the LCD command register
	CLR_DC;
	// select the LCD controller
	CLR_CS;
	// transfer command via datapins
	datapins = command;
	// clear the write register
	CLR_WR;
	// set the write register
	SET_WR;
	// deselect the LCD controller
	SET_CS;
}

// send data via the parallel interface
void write_data(uint8_t data)
{
	// select the LCD data register
	SET_DC;
	// select the LCD controller
	CLR_CS;
	// transfer data via datapins
	datapins = data;
	// clear the write register
	CLR_WR;
	// set the write register
	SET_WR;
	// deselect the LCD controller
	SET_CS;
}
#endif

#if interface_SPI
// send a command via the serial interface
void write_command(uint8_t command)
{
	// select the LCD command register
	CLR_DC;
	// select the LCD controller
	CLR_CS;
	// transfer command via SPI
	SPI.transfer(command);
	// deselect the LCD controller
	SET_CS;
}

// send data via the serial interface
void write_data(uint8_t data)
{
	// select the LCD data register
	SET_DC;
	// select the LCD controller
	CLR_CS;
	// transfer data via SPI
	SPI.transfer(data);
	// deselect the LCD controller
	SET_CS;
}
#endif

// display initialization sequence
void initialize_display()
{
	// issue a reset to the display to ensure it is properly initialized
	SET_RESET;
	delay(1);
	CLR_RESET;
	delay(10);
	SET_RESET;
	delay(120);

	// positive gamma control
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

	// negative gamma control
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

	// power control
	write_command(ILI9488_PCTRL1);
	write_data(0x18);
	write_data(0x16);
	write_command(ILI9488_PCTRL2);
	write_data(0x41);

	// VCOM control
	write_command(ILI9488_VCOMCTRL);
	write_data(0x00);
	write_data(0x1E);
	write_data(0x80);

	// memory access control
	write_command(ILI9488_MADCTL);
	write_data(0x40); // page address order 1

	// interface pixel format
	write_command(ILI9488_COLMOD);
	write_data(0x06); // 18bits/pixel (MCU)

	// frame rate control
	write_command(ILI9488_FRMCTR);
	write_data(0xB0);

	// display inversion control
	write_command(ILI9488_INVTR);
	write_data(0x02);

	// set image function
	write_command(ILI9488_SETIMG);
	write_data(0x00);

	// adjust control 3
	write_command(ILI9488_ADJCTRL3);
	write_data(0xA9);
	write_data(0x51);
	write_data(0x2C);
	write_data(0x82);

	// display function control
	write_command(ILI9488_DISCTRL);
	write_data(0x02);
	write_data(0x02);
	write_data(0x3B);

	// column address set
	write_command(ILI9488_CASET);
	write_data(0x00);
	write_data(0x00);
	write_data(0x01);
	write_data(0x3F);

	// page address set
	write_command(ILI9488_PAGESET);
	write_data(0x00);
	write_data(0x00);
	write_data(0x01);
	write_data(0xDF);

	// write the current column and page address to RAM
	write_command(ILI9488_RAMWR);

	// sleep out function
	write_command(ILI9488_SLPOUT);
	delay(120); // it is required to wait 120ms before sending the next command

	// turn the display on
	write_command(ILI9488_DISPON);
}

// set the position on the display to start transferring data to
void setPosition(uint16_t x, uint16_t y)
{
	write_command(ILI9488_CASET); // column address set function
	write_data(x >> 8);
	write_data(x & 0x00FF); // the x axis will increment according to the variable passed through
	write_data(0x01);
	write_data(0x3F);

	write_command(ILI9488_PAGESET); // page address set function
	write_data(y >> 8);
	write_data(y & 0x00FF); // the y axis will increment according to the variable passed through
	write_data(0x01);
	write_data(0xDF);

	write_command(ILI9488_RAMWR); // write the current position to RAM
}

// fill LCD with 4 different colored bars
void ColorBars()
{
	uint16_t number = 4;			   // 4 bars
	uint16_t barHeight = 480 / number; // divide the colors into equal heights
	int i;
	int j;

	setPosition(0, 0);
	for (i = 0; i < barHeight; i++) // set the vertical limit as the height of one bar
	{
		for (j = 0; j < 320; j++)
		{
			write_data(0xff); // choose the R-G-B code to transfer to the display
			write_data(0xff);
			write_data(0xff);
		}
	}

	setPosition(0, barHeight); // update the position to start from the end of the first bar
	for (i = 0; i < barHeight; i++)
	{
		for (j = 0; j < 320; j++)
		{
			write_data(0xDC);
			write_data(0xD0);
			write_data(0xFF);
		}
	}

	setPosition(0, (barHeight * 2)); // update the position to start from the end of the second bar
	for (i = 0; i < barHeight; i++)
	{
		for (j = 0; j < 320; j++)
		{
			write_data(0x00);
			write_data(0xFF);
			write_data(0xFF);
		}
	}

	setPosition(0, (barHeight * 3)); // update the position to start from the end of the third bar
	for (i = 0; i < barHeight; i++)
	{
		for (j = 0; j < 320; j++)
		{
			write_data(0x40);
			write_data(0x19);
			write_data(0x35);
		}
	}
}

// fill LCD with a single color
void fill_LCD(uint8_t R, uint8_t G, uint8_t B)
{
	int i, j;
	setPosition(0, 0); // set the start position to be 0,0
	// traverse the entire display
	for (i = 0; i < 480; i++)
	{
		for (j = 0; j < 320; j++)
		{
			write_data(R);
			write_data(G);
			write_data(B);
		}
	}
}

void show_BMPs_in_root(void)
{
	// these are the colors pulled from the uSD card
	uint8_t R;
	uint8_t G;
	uint8_t B;

	File root_dir;
	root_dir = SD.open("/");
	if (0 == root_dir)
	{
		Serial.println("show_BMPs_in_root: Can't open \"root\"");
		return;
	}
	File bmp_file;

	while (1)
	{
		bmp_file = root_dir.openNextFile();
		if (0 == bmp_file)
		{
			// no more files, break out of while()
			// root_dir will be closed below.
			break;
		}
		// skip directories (what about volume name?)
		if (0 == bmp_file.isDirectory())
		{
			// the file name must include ".BMP"
			if (0 != strstr(bmp_file.name(), ".BMP"))
			{
				Serial.println("size=");
				Serial.print(bmp_file.size()); // print the size of the bmp

				// the BMP must be exactly 460854 long 
				if (460854 == bmp_file.size())
				{
					// jump over the BMP header, this value is a default
					bmp_file.seek(54);

					// since we are limited in memory, we can not send 
					// 320 * 3 = 960 bytes as is. therefore, split this into four 
					// chunks of 80 pixels each of 80 * 3 = 240 bytes
					
					// making this static speeds it up slightly (10ms)
					static uint8_t fourth_of_a_line[80 * 3]; // set the size of the array to 240
					for (uint16_t line = 0; line < 480; line++) // traverse the display in the y direction
					{
						// BMPs store data lowest line first -- bottom up
						setPosition(0, (479 - line));
						// move through the 4 chunks of 80 pixels
						for (uint8_t line_section = 0; line_section < 4; line_section++)
						{
							// get a fourth of the line and store it in the previously defined array
							bmp_file.read(fourth_of_a_line, 80 * 3);

							// now write this fourth to the TFT
							SPI_send_pixels(80 * 3, fourth_of_a_line);
							// do this until the entire amount of pixels has been sent
						}
					}
				}
			}
		}
		// release the BMP file handle
		bmp_file.close();
		delay(1000);
	}
	// release the root directory file handle
	root_dir.close();
}

void SPI_send_pixels(uint16_t byte_count, uint8_t *data_ptr)
{
	uint8_t subpixel;

	// select the LCD data register
	SET_DC;
	// select the LCD controller
	CLR_CS;

	// load the first byte
	subpixel = *data_ptr;

	while (byte_count)
	{
		// delay(1);
		// send the byte out by writing to the SPI data register 
		SPDR = subpixel;
		// whilst transmistting;
		data_ptr++; // point to next byte
		subpixel = *data_ptr; // load the next byte
		byte_count--; // count the byte increment

		// now that we have done all we can do, wait for the transfer to finish
		while (!(SPSR & _BV(SPIF))); // _BV is defined in the pre-processor and expands to (1 << (SPIF))
	}

	// deselect the LCD controller
	SET_CS;
}

void setup()
{
#if interface_PARALLEL
	// set up port directions
	DDRD = 0xFF; // pins 0-7 set as outputs
	DDRB = 0x1F; // pins 8-12 set as outputs

	// configure the pins in a reasonable starting state
	datapins = 0;
	SET_RD;
	SET_WR;
	SET_CS;
	CLR_DC;
	//
#endif

#if interface_SPI
	Serial.begin(115200);
	// set up port directions
	DDRB = 0x3F; // pins 8-13 set as outputs
	DDRC = 0xFF;
	SD.begin(14); // setup the SD card on pin 14

	// configure the pins in a reasonable starting state
	SET_DC;
	SET_CS;
	CLR_MOSI;
	CLR_SCK;

	// begin SPI with the following settings
	SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
	SPI.begin();
#endif

	initialize_display(); // call the display initialization function
}

// set the demo to 1 to try it out
#define showcolor_demo 0
#define colorbars_demo 0
#define show_BMPs_demo 1
// more demos will be added soon!

void loop()
{
#if BMP_demo
	show_BMPs_in_root();
#endif

#if colorbars_demo
	ColorBars();
	delay(5000);
#endif

#if showcolor_demo
	// colors are sent in the R-G-B format
	fill_LCD(0xDC, 0xD0, 0xFF);
	delay(5000);
	fill_LCD(0x88, 0x08, 0x08);
	delay(5000);
	fill_LCD(0x40, 0x19, 0x35);
	delay(5000);
	fill_LCD(0x00, 0xFF, 0xFF);
	delay(5000);
#endif

	delay(10000);
}
