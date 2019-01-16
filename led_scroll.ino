/*=====================================
 * 2d dev:  Now try to make a new array that holds the two 1-d arrays...
 */
//////////////////////
// Library Includes //
//////////////////////
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#include "memprint.h"

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

int8_t cursorX = 0;  // Cursor x position, initialize left
int8_t cursorY = 0;  // Cursor y position, initialize top

int16_t color=matrix.Color333(1,0,0);


#define BUFFER_SIZE 40
uint8_t row0[BUFFER_SIZE]={1,0,1,1,0,1,1,1,0,1,1,1,1,0};
uint8_t row1[BUFFER_SIZE]={1,0,1,1,0,1,1,1,0,1,1,1,1,0};
uint8_t row2[BUFFER_SIZE]={1,0,1,1,0,1,1,1,0,1,1,1,1,0};
uint8_t row3[BUFFER_SIZE]={1,0,1,1,0,1,1,1,0,1,1,1,1,0};
uint8_t row4[BUFFER_SIZE]={1,0,1,1,0,1,1,1,0,1,1,1,1,0};
uint8_t row5[BUFFER_SIZE]={1,0,1,1,0,1,1,1,0,1,1,1,1,0};
uint8_t row6[BUFFER_SIZE]={1,0,1,1,0,1,1,1,0,1,1,1,1,0};
uint8_t row7[BUFFER_SIZE]={1,0,1,1,0,1,1,1,0,1,1,1,1,0};

#define NUM_ROWS 2
uint8_t *rows[NUM_ROWS] = {row0, row1};

int current_buffer_column = 0;

//Window goes pixels 0 through 9.  All on top.
#define WINDOW_START 0
#define WINDOW_SIZE 32

void setup()
{
  matrix.begin();       // Initialize the matrix.

  Serial.begin(9600);   // Start serial

  memprint_bytes(row0);
  memprint_bytes(row1);
  memprint_bytes(rows);

  
}

void scroll_iteration( void )
{
  int8_t matrix_column;
  int8_t buffer_column;
  int8_t current_row;
  uint8_t *row_ptr;

  Serial.println("Enter scroll_iteration");
  
  // print, from left to right, the 10 characters from our current buffer column
  for (matrix_column = 0; matrix_column < WINDOW_SIZE; matrix_column++)
  {
    buffer_column = (matrix_column + current_buffer_column) % BUFFER_SIZE;

    Serial.print("buffer_column=");
    Serial.print(buffer_column);
    Serial.print(" matrix_column=");
    Serial.println(matrix_column);

    for (current_row = 0; current_row < NUM_ROWS; current_row++)
    {
      row_ptr = rows[current_row];

      Serial.print("pixel: ");
      Serial.print(matrix_column);
      Serial.print(",");
      Serial.println(current_row);

      //memprint_bytes(&current_row, 2);
      
      if (row_ptr[buffer_column])
      {
        matrix.drawPixel(matrix_column, current_row, color);
      }
      else
      {
        //yes, we do have to draw black here to erase anything previously drawn
        matrix.drawPixel(matrix_column, current_row, 0);
      }

      //memprint_bytes(&current_row, 2);
    }
    
  }

  // next time through we're gonna want the next line, so increment current_buffer_column
  current_buffer_column = (current_buffer_column + 1) % BUFFER_SIZE;
  
}

void loop()
{
   scroll_iteration();
   
   while (!Serial.available());
   char c = Serial.read();
   //delay(50);
   
}
