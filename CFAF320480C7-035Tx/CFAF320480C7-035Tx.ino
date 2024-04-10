//==============================================================================
//
//  CRYSTALFONTZ
//
//  The controller is a Ilitek ILI9488
//    https://www.crystalfontz.com/controllers/Ilitek/ILI9488/
//
//  Seeeduino, an open-source 3.3v capable Arduino clone.
//    https://www.crystalfontz.com/product/cfapn15062-seeeduino-arduino-clone-microprocessor
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
//
// Connection guide:
//
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
//==============================================================================
// Interface Selection
// IM2 | IM1 | IM0 |  Interface mode  |
//-----+-----+-----+------------------+
// 0   | 1   | 1   |  8-bit parallel  |
// 1   | 1   | 1   |  4-wire SPI      |
//-----+-----+-----+------------------+
//==============================================================================
// Resistive touchscreen connection (SPI only)
//  ARD      | Port  | Touchscreen pin |  Function                            |
//-----------+-------+-----------------+---------------- ---------------------+
// D16       | PORTC |  XL             |  Touch panel left            (XL)    |
// D17       | PORTC |  XR             |  Touch panel right           (XR)    |
// D18       | PORTC |  YD             |  Touch panel bottom          (YD)    |
// D19       | PORTC |  YU             |  Touch panel top             (YU)    |
//-----------+-------+-----------------+---------------- ---------------------+
// Capacitive touchscreen connection (SPI only)
//  ARD      | Port  | Touchscreen pin |  Function                            |
//-----------+-------+-----------------+---------------- ---------------------+
// 3.3V      |       |  3.3V           |  POWER 3.3V                          |
// GND       |       |  GND            |  GROUND                              |
//-----------+-------+-----------------+---------------- ---------------------+
// D0        | PORTD |  RST            |  Reset                       (Reset) |
// D17       | PORTC |  INT            |  Interrupt                   (INT)   |
// D18       | PORTC |  SDA            |  Serial data                 (SDA)   |
// D19       | PORTC |  SCK            |  Serial clock                (SCL)   |
//-----------+-------+-----------------+---------------- ---------------------+
//==============================================================================
// SD card connection (using CFA10112)
//  ARD      | Port  | Adapter pin |  Function                                |
//-----------+-------+-------------+------------------------------------------+
// 3.3V      |       |  3.3V       |  POWER 3.3V                              |
// GND       |       |  GND        |  GROUND                                  |
//-----------+-------+-------------+------------------------------------------+
// D11       | PORTB |  DI         |  Serial data in                  (DI)    |
// D12       | PORTB |  DO         |  Serial data out                 (DO)    |
// D13       | PORTB |  SCK        |  Serial clock                    (SCK)   |
// D14       | PORTC |  CS         |  Chip select                     (CS)    |
//-----------+-------+-------------+------------------------------------------+

#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

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
// define the capacitive touch pin locations
#define TCH_RES (0)
#define INTERRUPT (17)
#define I2C_SDA (18)
#define I2C_SCL (19)
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

// touch screen controller defines
#define FT_NOP_MODE (0x00)
#define FT_REG_NUM_FINGER (0x02)
#define FT_TP1_REG (0X03)
#define FT_TP2_REG (0X09)
#define FT_ID_G_LIB_VERSION (0xA1)
#define FT_ID_G_MODE (0xA4)
#define FT_ID_G_TRANSMISSION (0x80)
#define FT_ID_G_PERIODACTIVE (0x88)
#define NVM_ADDR_DATA_WR (0xD0)

// capacitive touchscreen demo (SPI only)
void sendCommand(uint8_t reg, uint8_t data)
{
	// start trasmission by sending address 0x38
	Wire.beginTransmission(0x38);
	// start reading at register 0x02
	Wire.write(reg);
	Wire.write(data);
	// send the data over
	Serial.println("return value: ");
	Serial.println(Wire.endTransmission());
}

void tchSetup()
{
	Wire.begin();
	Wire.setClock(400000);

	digitalWrite(TCH_RES, LOW);
	delay(200);
	digitalWrite(TCH_RES, HIGH);
	delay(200);

	// send device mode
	sendCommand(FT_NOP_MODE, 0x00);

	// ID_G_MODE set interrupt
	sendCommand(FT_ID_G_MODE, 0x00);
	//  0x00 - Polling Mode
	//  0x01 - Trigger Mode

	// touch threshold
	sendCommand(FT_ID_G_TRANSMISSION, 20);

	// reporting rate
	sendCommand(FT_ID_G_PERIODACTIVE, 12);
}

void touchDemo()
{
	// set the interrupt threshold
	int THRESHOLD = (2.6 * 1023) / 3.3; // 2.6V is the threshold in this case

	// draw a little X box
	for (int i = 0; i < 20; i++)
	{
		Fast_Horizontal_Line(301, i, 320, 0x00, 0x00, 0xFF);
	}
	drawLine(304, 2, 317, 18, 0xFF, 0xFF, 0xFF);
	drawLine(304, 18, 317, 2, 0xFF, 0xFF, 0xFF);

	// set the variables used in the demo
	uint8_t
		press1,
		release1, contact1, touchID1, w1, m1,
		press2, release2, contact2, touchID2, w2, m2;
	uint16_t
		x1,
		y1,
		x2, y2;

	// start touch demo
	while (1)
	{
		// check to see if the interrupt is toggled. since it's in polling mode, it will remain LOW
		// whenever there is an active touch. if it is in trigger mode, it will pulse
		if (analogRead(INTERRUPT) < THRESHOLD)
		{
			Serial.println("got a touch!");
			// start trasmission by sending address 0x38
			Wire.beginTransmission(0x38);
			// start reading at register 0x02
			Wire.write(0x02);
			// send the data over
			Wire.endTransmission();

			// read 16 bytes from address 0x38
			Wire.requestFrom(0x38, 16);

			// register 2 contains the number of points touched
			uint8_t
				pointsTouched;
			pointsTouched = Wire.read() & 0x0F;
			Serial.print("num of points touched:");
			Serial.println(pointsTouched);
			if (0 < pointsTouched)
			{

				// read the data
				((uint8_t *)(&x1))[1] = Wire.read();
				((uint8_t *)(&x1))[0] = Wire.read();
				((uint8_t *)(&y1))[1] = Wire.read();
				((uint8_t *)(&y1))[0] = Wire.read();
				w1 = Wire.read();
				m1 = Wire.read();

				// crunch the data
				press1 = ((x1 & 0xC000) == 0x0000) ? 1 : 0;
				release1 = ((x1 & 0xC000) == 0x4000) ? 1 : 0;
				contact1 = ((x1 & 0xC000) == 0x8000) ? 1 : 0;
				touchID1 = y1 & 0xE0000;
				x1 &= 0x0FFF;
				y1 &= 0x0FFF;

				// check for two points touched
				if (1 < pointsTouched)
				{
					((uint8_t *)(&x2))[1] = Wire.read();
					((uint8_t *)(&x2))[0] = Wire.read();
					((uint8_t *)(&y2))[1] = Wire.read();
					((uint8_t *)(&y2))[0] = Wire.read();
					w2 = Wire.read();
					m2 = Wire.read();

					press2 = ((x2 & 0xC000) == 0x0000) ? 1 : 0;
					release2 = ((x2 & 0xC000) == 0x4000) ? 1 : 0;
					contact2 = ((x2 & 0xC000) == 0x8000) ? 1 : 0;
					touchID2 = y2 & 0xE0000;
					x2 &= 0x0FFF;
					y2 &= 0x0FFF;
					drawPixel(x2, y2, 0x00, 0xFF, 0x00);
				}
				drawPixel(x1, y1, 0xFF, 0xFF, 0xFF);

				// break out of the loop if the little X box is clicked
				if ((x1 > 300 && y1 < 20) || (x2 > 300 && y2 < 20))
					break;
			}

#ifdef TOUCH_DEBUG
			// print out the data
			SerPrintFF(F("G = 0x%02X P = %d, (%5u,%5u) p=%3u r=%3u c=%3u ID=%3u"),
					   // gesture,
					   pointsTouched,
					   x1, y1,
					   press1,
					   release1,
					   contact1,
					   touchID1);
			SerPrintFF(F(" (%5u,%5u) p=%3u r=%3u c=%3u ID=%3u \n"),
					   x2, y2,
					   press2,
					   release2,
					   contact2,
					   touchID2);
#endif
		}
	}
}

void SerPrintFF(const __FlashStringHelper *fmt, ...)
{
	char
		tmp[128]; // resulting string limited to 128 chars
	va_list
		args;
	va_start(args, fmt);
	vsnprintf_P(tmp, 128, (const char *)fmt, args);
	va_end(args);
	Serial.print(tmp);
}

// resistive touchscreen demo (SPI only)
#define TS_XL (16)
#define TS_XR (17)
#define TS_YD (18)
#define TS_YU (19)

#define FIND_MIN_MAX 0

#if (FIND_MIN_MAX)
uint16_t Xmin = 1023;
uint16_t Xmax = 0;
uint16_t Ymin = 1023;
uint16_t Ymax = 0;
#else
// copied from the serial console window
uint16_t Xmin = 138;
uint16_t Xmax = 888;
uint16_t Ymin = 59;
uint16_t Ymax = 948;
#endif

uint8_t Read_Touch_Screen(uint16_t *x, uint16_t *y)
{
	// see if there is a touch.
	// let YU float, make YD tug high, drive X1 and X2 low.
	// read Y1, if it is near 3.3V (~1024), then the screen is not
	// touched. if it is near ground (~50) then the screen is
	// touched.
	uint16_t touched;
	pinMode(TS_YU, INPUT);
	digitalWrite(TS_YU, HIGH);
	pinMode(TS_YD, INPUT_PULLUP);
	digitalWrite(TS_YD, HIGH);
	pinMode(TS_XL, OUTPUT);
	digitalWrite(TS_XL, LOW);
	pinMode(TS_XR, OUTPUT);
	digitalWrite(TS_XR, LOW);
	touched = analogRead(TS_YU);

	// idle YD as an input
	pinMode(TS_YD, INPUT);
	if (touched < 512)
	{
		// we are touched.
		uint32_t X;
		uint32_t Y;

		// read X. set a gradient from 0v to 5v on X, then
		// read the Y line to get the X contact point.
		// pinMode(TS_YU,INPUT);    // already set
		// pinMode(TS_YD,INPUT);    // already set
		// pinMode(TS_XL,OUTPUT);   // already set
		digitalWrite(TS_XR, HIGH);
		// pinMode(TS_XR,OUTPUT);   // already set
		// digitalWrite(TS_XL,LOW); // already set
		X = analogRead(TS_YD); // could use YU

		// read Y. set a gradient from 0v to 3.3v on Y, then
		// read the X line to get the Y contact point.
		pinMode(TS_XL, INPUT);
		pinMode(TS_XR, INPUT);
		pinMode(TS_YU, OUTPUT);
		digitalWrite(TS_YU, HIGH);
		pinMode(TS_YD, OUTPUT);
		digitalWrite(TS_YD, LOW);
		Y = analogRead(TS_XL); // could use XR

		Serial.print("X: ");
		Serial.println(X);
		Serial.print("Y: ");
		Serial.println(Y);
		// idle the Y pins
		pinMode(TS_YU, INPUT);
		pinMode(TS_YD, INPUT);

		// calculate the pixel values, store in the user's pointers.
		*x = ((X - (uint32_t)Xmin) * 320) / ((uint32_t)Xmax - (uint32_t)Xmin);
		*y = 480 - ((Y - (uint32_t)Ymin) * 480) / ((uint32_t)Ymax - (uint32_t)Ymin);

		// return touched flag.
		return (1);
	}
	else
	{
		// not touched. idle the pins that were set to output
		// to detect the touch.
		pinMode(TS_XL, INPUT);
		pinMode(TS_XR, INPUT);
		return (0);
	}
}

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

// draw a pixel of color
void drawPixel(uint16_t x, uint16_t y, uint8_t R, uint8_t G, uint8_t B)
{
	setPosition(x, y);
	write_data(R);
	write_data(G);
	write_data(B);
}

// draws a line of a specified color from point [x0, y0] to [x1, y1]
// from: http://rosettacode.org/wiki/Bitmap/Bresenham's_line_algorithm#C
void drawLine(uint16_t x0, uint16_t y0,
			  uint16_t x1, uint16_t y1, uint8_t R, uint8_t G, uint8_t B)
{
	// general case
	if (y0 != y1)
	{
		// find the deltas and slopes
		int16_t dx = abs((int16_t)x1 - (int16_t)x0);
		int16_t sx = x0 < x1 ? 1 : -1;
		int16_t dy = abs((int16_t)y1 - (int16_t)y0);
		int16_t sy = y0 < y1 ? 1 : -1;
		int16_t err = (dx > dy ? dx : -dy) / 2;

		for (;;)
		{
			drawPixel(x0, y0, R, G, B);
			if ((x0 == x1) && (y0 == y1))
			{
				break;
			}
			int16_t e2 = err;
			if (e2 > -dx)
			{
				err -= dy;
				x0 = (uint16_t)((int16_t)x0 + sx);
			}
			if (e2 < dy)
			{
				err += dx;
				y0 = (uint16_t)((int16_t)y0 + sy);
			}
		}
	}
	else
	{
		// Optimized for this display
		Fast_Horizontal_Line(x0, y0, x1, R, G, B);
	}
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

void Fast_Horizontal_Line(uint16_t x0, uint16_t y, uint16_t x1, uint8_t R, uint8_t G, uint8_t B)
{
	if (x0 < x1)
		for (int i = x0; i < x1; i++)
		{
			drawPixel(i, y, R, G, B);
		}
	else
		for (int i = x1; i < x0; i++)
		{
			drawPixel(i, y, R, G, B);
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
					static uint8_t fourth_of_a_line[80 * 3];	// set the size of the array to 240
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
		data_ptr++;			  // point to next byte
		subpixel = *data_ptr; // load the next byte
		byte_count--;		  // count the byte increment

		// now that we have done all we can do, wait for the transfer to finish
		while (!(SPSR & _BV(SPIF)))
			; // _BV is defined in the pre-processor and expands to (1 << (SPIF))
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
	// capacitive touchscreen pin directions
	pinMode(INTERRUPT, INPUT);
	pinMode(TCH_RES, OUTPUT);
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
#define show_BMPs_demo 1 // SPI only
#define cap_touch_demo 0 // SPI only
#define res_touch_demo 0 // SPI only

void loop()
{
#if res_touch_demo
	uint16_t x;
	uint16_t y;
	fill_LCD(0x00, 0x00, 0x00);
	while (1)
	{
		if (Read_Touch_Screen(&x, &y))
		{
			// touch in upper left corner exits
			if ((x < 20) && (y < 20))
			{
				break;
			}
			// Otherwise draw
			drawPixel(x, y, 0xFF, 0xFF, 0xFF);
		}
		delay(10);
	}
#endif

#if cap_touch_demo
	fill_LCD(0x00, 0x00, 0x00);
	tchSetup();
	touchDemo();
#endif

#if show_BMPs_demo
	show_BMPs_in_root();
	while (1)
		;
#endif

#if colorbars_demo
	ColorBars();
	delay(5000);
#endif

#if showcolor_demo
	// colors are printed in the B-G-R format
	fill_LCD(0xDC, 0xD0, 0xFF);
	delay(5000);
	fill_LCD(0x88, 0x08, 0x08);
	delay(5000);
	fill_LCD(0x40, 0x19, 0x35);
	delay(5000);
	fill_LCD(0x00, 0xFF, 0xFF);
	delay(5000);
#endif
}
