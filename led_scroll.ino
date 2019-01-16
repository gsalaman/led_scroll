/*=====================================
 * Test #3. go 2-d (8 pixels high)
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

int8_t cursorX = 0;  // Cursor x position, initialize left
int8_t cursorY = 0;  // Cursor y position, initialize top

int16_t color=matrix.Color333(1,0,0);

#define PIXELS_PER_COLUMN 1
#define BUFFER_SIZE_COLUMNS 10
#define BUFFER_SIZE_PIXELS (BUFFER_SIZE_COLUMNS*PIXELS_PER_COLUMN)

uint8_t display_buffer[BUFFER_SIZE_PIXELS]=
{
  1,
  0,
  1,
  1,
  0,
  1, 
  1, 
  1
};

int current_buffer_column = 0;

#define WINDOW_START 0
#define WINDOW_SIZE  5

void setup()
{
  matrix.begin();       // Initialize the matrix.

  Serial.begin(9600);   // Start serial
 
}

void pause( void )
{
  #if 0
    while (!Serial.available());
    char c = Serial.read();
    #endif
}

void scroll_iteration( void )
{
  int matrix_column;
  int matrix_row;
  int buffer_column;
  int draw_pixel;
  int index;

  Serial.println("=======");
  Serial.print("Current Buffer Column: ");
  Serial.println(current_buffer_column);
  pause();
  
  // print, from left to right, the pixels from our current buffer column
  for (matrix_column = 0; matrix_column < WINDOW_SIZE; matrix_column++)
  {
    Serial.print("  matrix_column=");
    Serial.println(matrix_column);
    pause();
        
    buffer_column = (matrix_column + current_buffer_column) % BUFFER_SIZE_COLUMNS;
    Serial.print("  buffer_column=");
    Serial.println(   buffer_column);
    pause();
    
    for (matrix_row = 0; matrix_row < PIXELS_PER_COLUMN; matrix_row++)
    {
       Serial.print("    matrix_row=");
       Serial.println(matrix_row);
       pause();
       
       // optimize this later via ptr math rather than array
       index = (buffer_column * PIXELS_PER_COLUMN) + matrix_row;
       Serial.print("    index=");
       Serial.println(index);
       pause();
       
       draw_pixel = display_buffer[index];
       Serial.print("    draw_pixel=");
       Serial.println(draw_pixel);
       pause();

       if (draw_pixel)
       {
         Serial.print("Filling ");
         Serial.print(matrix_column);
         Serial.print(",");
         Serial.println(matrix_row);
         pause();
         matrix.drawPixel(matrix_column, matrix_row, color);
       }
       else
       {
         Serial.print("Clearing ");
         Serial.print(matrix_column);
         Serial.print(",");
         Serial.println(matrix_row);
         pause();

          //yes, we do have to draw black here to erase anything previously drawn
          matrix.drawPixel(matrix_column, matrix_row, 0);
       }

       
    }  // end of drawing a column
  }  // end of drawing rows

  // next time through we're gonna want the next line, so increment current_buffer_column
  current_buffer_column = (current_buffer_column + 1) % BUFFER_SIZE_COLUMNS;

  Serial.print("Current Buffer Column: ");
  Serial.println(current_buffer_column);
  Serial.println("+++++++");
  pause();
  
  
}

void loop()
{
   scroll_iteration();
   
   while (!Serial.available());
   char c = Serial.read();
   //delay(50);
   
}
