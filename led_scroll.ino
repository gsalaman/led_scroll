/*=====================================
 * Trying letter maps.
 */
//////////////////////
// Library Includes //
//////////////////////
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

/////////////////////
// Hardware Hookup //
/////////////////////
// R0, G0, B0, R1, G1, B1 should be connected to pins
// 2, 3, 4, 5, 6, and 7 respectively. Their pins aren't defined,
// because they're controlled directly in the library. These pins
// can be moved (somewhat):

// CLK be on PORTB!
// CLK can be moved but must remain on PORTB(8, 9, 10, 11, 12, 13)
#define CLK 11
#define LAT 10
#define OE  9

#define A   A0
#define B   A1
#define C   A2
#define D   A3 // Comment this line out if you're using a 32x16

////////////////////////////////////////////////
/////   Instantiate RGBmatrixPanel Class   /////
////////////////////////////////////////////////

/* ========== For 32x32 LED panels: ========== */
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false); // 32x32

#define WINDOW_START 0
#define WINDOW_SIZE 32

#define CHAR_MAP_WIDTH   6
#define CHAR_MAP_HEIGHT  8

const bool char_A[]=
{
  0,0,1,0,0,0,
  0,1,0,1,0,0,
  1,0,0,0,1,0,
  1,1,1,1,1,0,
  1,0,0,0,1,0,
  1,0,0,0,1,0,
  1,0,0,0,1,0,
  0,0,0,0,0,0,  
};

void print_char(const bool *c_map, int start_x, int start_y)
{
  int x_offset;
  int y_offset;
  int pixel_x; 
  int pixel_y;
  int offset_index;

  for (y_offset=0;y_offset<CHAR_MAP_HEIGHT;y_offset++)
  {
    for (x_offset=0;x_offset<CHAR_MAP_WIDTH;x_offset++)
    {
      pixel_x = start_x + x_offset;
      pixel_y = start_y + y_offset;

      offset_index = pixel_x + (pixel_y * CHAR_MAP_WIDTH);
      if (c_map[offset_index]) matrix.drawPixel(pixel_x, pixel_y, matrix.Color333(1,0,0));
      else matrix.drawPixel(pixel_x, pixel_y, 0);
    }
  }
}


void setup()
{
  matrix.begin();       // Initialize the matrix.

  Serial.begin(9600);   // Start serial  

  print_char(char_A, 0, 0);
  
}

void loop()
{
   
   //while (!Serial.available());
   //char c = Serial.read();
   //delay(50);
   
}
