#include "GLCD.h"
#include "KBD.h"
#include <stdio.h>
#include "snake.h"
#include "poker.h"
#include "usbdmain.h"


// Images
#include "tree.h"
#include "grass.h"
#include "cat.h"
#include "firework.h"
#include "port.h"
#include "japan.h"

#define __FI        1   /* Font index 16x24 */
#define IMAGES      6

// Function prototypes
void Menu(void);
void MP3_Player(void);
void Photo_Gallery(void);
void Snake(void);
void Poker(void);
extern int audio_main (void);


//----------------------------------------------------------------------------------------------------
// MENU Function
//----------------------------------------------------------------------------------------------------
void Menu(void) {
  uint32_t key, last_key = 0xFF;
  int selected = 0;
  int exit_menu = 0;
	int i;
  while (!exit_menu) {
    GLCD_Clear(White);
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Blue);
		GLCD_DisplayString(0, 3, __FI, "COE718 - Rafael");
    GLCD_DisplayString(1, 8, __FI, "Menu");

    while (1) {
      // Draw menu options with background color depending on selection
      for (i = 0; i < 4; i++) {
        if (i == selected) {
          GLCD_SetBackColor(Green);
          GLCD_SetTextColor(White);
        } else {
          GLCD_SetBackColor(White);
          GLCD_SetTextColor(Blue);
        }

        switch (i) {
          case 0:
            GLCD_DisplayString(3, 0, __FI, "MP3 Player    ");
            break;
          case 1:
            GLCD_DisplayString(4, 0, __FI, "Photo Gallery ");
            break;
          case 2:
            GLCD_DisplayString(5, 0, __FI, "Snake         ");
            break;
					case 3:
            GLCD_DisplayString(6, 0, __FI, "Poker         ");
            break;
        }
      }

      key = get_button();
      if (key != last_key) {
        last_key = key;
        if (key == KBD_DOWN && selected < 3) {
          selected++;
        } else if (key == KBD_UP && selected > 0) {
          selected--;
        } else if (key == KBD_SELECT) {
          switch (selected) {
            case 0:
              MP3_Player();
              break;
            case 1:
              Photo_Gallery();
              break;
            case 2:
              Snake();
              break;
						case 3:
              Poker();
              break;
          }
          break; // Return to redraw menu after exiting sub-function
        }
      }

      
    }
  }
}


//----------------------------------------------------------------------------------------------------
// MP3 PLAYER Function
//----------------------------------------------------------------------------------------------------
void MP3_Player(void) {
  audio_main();
}


//----------------------------------------------------------------------------------------------------
// PHOTO GALLERY Function
//----------------------------------------------------------------------------------------------------
void Photo_Gallery(void) {
  int img = 0;
  uint32_t key, last_key = 0xFF;

  GLCD_Clear(White);
  GLCD_DisplayString(1, 0, __FI, "Photo Gallery");
  GLCD_DisplayString(2, 0, __FI, "LEFT/RIGHT to change");
  GLCD_DisplayString(3, 0, __FI, "Select to return");

  while (1) {
		
    // Show image
    
    switch (img) {
      case 0:
        GLCD_Bitmap(75, 100, TREE_WIDTH, TREE_HEIGHT, TREE_PIXEL_DATA);
        break;
      case 1:
        GLCD_Bitmap(75, 100, GRASS_WIDTH, GRASS_HEIGHT, GRASS_PIXEL_DATA);
				break;
			case 2:
				GLCD_Bitmap(75, 100, CAT_WIDTH, CAT_HEIGHT, CAT_PIXEL_DATA);
				break;
			case 3:
				GLCD_Bitmap(75, 100, PORT_WIDTH, PORT_HEIGHT, PORT_PIXEL_DATA);
				break;
			case 4:
				GLCD_Bitmap(75, 100, JAPAN_WIDTH, JAPAN_HEIGHT, JAPAN_PIXEL_DATA);
				break;
			case 5:
				GLCD_Bitmap(75, 100, FIREWORK_WIDTH, FIREWORK_HEIGHT, FIREWORK_PIXEL_DATA);
        break;
    }

    key = get_button();
    if (key != last_key) {
      last_key = key;
      if (key == KBD_RIGHT && img < IMAGES - 1) {
        img++;
      } else if (key == KBD_LEFT && img > 0) {
        img--;
      } else if (key == KBD_SELECT) {
        return; // Return to menu
      }
    }
    
  }
}

//----------------------------------------------------------------------------------------------------
// SNAKE Function
//----------------------------------------------------------------------------------------------------
void Snake(void) {
  
	snakegame();
  
}

//----------------------------------------------------------------------------------------------------
// SNAKE Function
//----------------------------------------------------------------------------------------------------
void Poker(void) {
  
	pokergame();
  
}

//----------------------------------------------------------------------------------------------------
// MAIN FUNCTION
//----------------------------------------------------------------------------------------------------
int main(void) {
  // Initialize hardware
  KBD_Init();
  GLCD_Init();
  GLCD_Clear(White);
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(1, 0, __FI, "System Start");


  // Launch main menu
  Menu();

  // Should never reach here
  while (1) {}
}
