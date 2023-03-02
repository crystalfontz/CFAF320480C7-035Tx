# CFAF320480C7-035Tx Demonstration Code
This is Arduino sample code for the CFAF320480C7-035Tx family of displays. These displays are 3.5" TFTs which use the [Ilitek ILI9488](https://www.crystalfontz.com/controllers/Ilitek/ILI9488/) LCD controller. This LCD controller is capable of 8/9/16/18 bit parallel, 3 or 4-Wire SPI and DOTCLK RGB with SPI initialization. In order to interface with the displays, a 50 pin ZIF to header breakout board will be required.

## Connection Guide
```
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
```
## Display information
Here are links to our active displays:\
[CFAF320480C7-035TR](https://www.crystalfontz.com/product/cfaf320480c7035tr-320x480-resistive-touchscreen-tft-display)\
[CFAF320480C7-035TN](https://www.crystalfontz.com/product/cfaf320480c7035tn-320x480-3-5-inch-color-tft-display)

For more information about other TFT offerings, please see our full list [here](https://www.crystalfontz.com/c/tft-lcd-displays/25).
