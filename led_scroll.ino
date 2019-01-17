/*=====================================
 * 2d dev:  Now try to make a new array that holds the two 1-d arrays...
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


#define BUFFER_SIZE 40
//uint8_t row0[BUFFER_SIZE]={1,1,1,1,0,1,1,1,0,1,1,1,1,0};
//uint8_t row1[BUFFER_SIZE]={0,1,1,1,0,1,1,1,0,1,1,1,1,0};

int current_buffer_column = 0;

//Window goes pixels 0 through 9.  All on top.
#define WINDOW_START 0
#define WINDOW_SIZE 32

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false); // 32x32

void setup()
{
  matrix.begin();       // Initialize the matrix.

  propagate_data();
  
  Serial.begin(9600);   // Start serial  
}

#if 0
void scroll_iteration( void )
{
  int matrix_column;
  int buffer_column;
  uint8_t *row0_ptr;
  uint8_t *row1_ptr;

  int16_t color;
  int16_t blank;

  color=matrix.Color333(1,0,0);
  blank=matrix.Color333(0,0,0);
  
  for (matrix_column = 0; matrix_column < WINDOW_SIZE; matrix_column++)
  {
    buffer_column = (matrix_column + current_buffer_column) % BUFFER_SIZE;

    row0_ptr = row0 + buffer_column;
    row1_ptr = row1 + buffer_column;

    if (*row0_ptr) matrix.drawPixel(matrix_column, 0, color);
    else matrix.drawPixel(matrix_column, 0, blank);    

    if (*row1_ptr) matrix.drawPixel(matrix_column, 1, color);
    else matrix.drawPixel(matrix_column, 1, blank);
    
  }

  // next time through we're gonna want the next line, so increment current_buffer_column
  current_buffer_column = (current_buffer_column + 1) % BUFFER_SIZE;
  
}
#endif


//  Okay, weird!  We seem to get memory corruption when this array is bigger than about 60 bytes
// (60 works, 70 does not)
#define BUFFER_COLUMNS 15
#define WINDOW_COLUMNS 10
#define WINDOW_ROWS     4
bool data[BUFFER_COLUMNS*WINDOW_ROWS]={0};

void propagate_data( void )
{
  int i;

  for (i=0; i<BUFFER_COLUMNS; i++)
  {
    if (i%2) data[i]=1;
  }
  for (i=BUFFER_COLUMNS; i<BUFFER_COLUMNS*2; i++)
  {
    if (i%3) data[i]=1;
  }

  for (i=BUFFER_COLUMNS*2; i<BUFFER_COLUMNS*3; i++)
  {
    if (i%4) data[i]=1;
  }

//  for (i=BUFFER_COLUMNS*7; i<BUFFER_COLUMNS*8; i++)
//  data[i]= 1;
}

void test_multiple_writes( void )
{
  int16_t color;
  int16_t blank;
  int x;  // window column
  int y;  // window row
  int buf_x_index;

  static int buffer_column=0;

  color = matrix.Color333(1,0,0);
  blank = 0;

  for (x=0; x<WINDOW_COLUMNS; x++)
  {
    for (y=0; y<WINDOW_ROWS; y++)
    {
      buf_x_index = (x+buffer_column) % BUFFER_COLUMNS;
      //buf_x_index = (x+buffer_column);
      
      if ( data[((y*BUFFER_COLUMNS)+buf_x_index)] ) matrix.drawPixel(x, y, color);
      else matrix.drawPixel(x, y, blank);
    }
  }
  
  buffer_column++;
  if (buffer_column == BUFFER_COLUMNS) buffer_column = 0;
}
void loop()
{
   test_multiple_writes();
   //scroll_iteration();
   
   while (!Serial.available());
   char c = Serial.read();
   //delay(50);
   
}
