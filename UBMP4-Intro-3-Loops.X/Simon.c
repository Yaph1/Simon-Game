
/*==============================================================================
 Project: UBMP4-Simon-Starter-Game
 Date:    January 12, 2022
 
  A Simon-style pattern matching game starter framework with complete game play.
  Just add startup, winning, and losing sound and light effects.
 ==============================================================================*/
 
#include    "xc.h"              // Microchip XC8 compiler include file
#include    "stdint.h"          // Include integer definitions
#include    "stdbool.h"         // Include Boolean (true/false) definitions
 
#include    "UBMP4.h"           // Include UBMP4 constants and functions
 
// TODO Set linker ROM ranges to 'default,-0-7FF' under "Memory model" pull-down.
// TODO Set linker code offset to '800' under "Additional options" pull-down.
 
// Program constants
#define pressed 0               // Switch pressed logic state
#define timeOut 2400            // Idle shutdown time-out duration * 50ms (1200/min)
#define patternLength 10        // Total number of pattern steps to match
 
// Set up program variables
__persistent unsigned char randomSeed;  // Random seed updated from TMR0
unsigned char button;           // Button ID (0 = none, 1 = S1, etc.)
unsigned char pattern[patternLength];	// Pattern sequence storage
unsigned char step = 0;         // Current step (pattern sequence index) counter
unsigned char maxStep = 0;      // Highest step achieved by player
unsigned int idle = 0;          // In-game idle timer for shutdown
 
// Define program functions
extern int rand();              // Declarations for random number generation
extern void srand(unsigned int);
unsigned int newSeed;           // New random number generator seed values
unsigned char ticks;            // Random delay ticks

 
/*==============================================================================
 Interrupt function. Processes IOC interrupts and wakes game from nap/sleep.
 =============================================================================*/
 
void __interrupt() wake(void)
{
    di();                       // Disable interrupts
	if(IOCIF == 1 && IOCIE == 1)	// Check for IOC interrupt
	{
        IOCAF = 0;              // Clear IOC port A interrupt flag
        IOCBF = 0;              // Clear IOC port B interrupt flag
        IOCIF = 0;				// Clear IOC interrupt flag
	}
	else
    {
        IOCAN = 0b00001000;     // Other interrupt? Only allow IOC on pushbuttons
        IOCBN = 0b11110000;
		INTCON = 0b00001000;
    }
	return;
}
 
void nap(void)
{
    LED1 = 1;                   // Turn all LEDs off
    LATC = 0;
    IOCAF = 0;                  // Clear S1 IOC interrupt flag
    IOCBF = 0;                  // Clear S2-S5 IOC interrupt flags
    IOCIF = 0;                  // Clear global IOC interrupt flag
    ei();                       // Enable interrupts
    SLEEP();
 
    RESET();                    // Restart processor after wake-up
}
    
/*==============================================================================
 getButton function. Return the code of a pressed button. 0 = no button.
 =============================================================================*/
 
unsigned char getButton(void)       // Return code of pressed button or 0 if
{                                   // no buttons are pressed
    if(SW2 == pressed)
    {
        return(1);
    }
    else if(SW3 == pressed)
    {
        return(2);
    }
    else if(SW4 == pressed)
    {
        return(3);
    }
    else if(SW5 == pressed)
    {
        return(4);
    }
    else
    {
        return(0);
    }
}
 
/*==============================================================================
 LED function. Light one of the 4 LEDs. 0 = all LEDs off.
==============================================================================*/
 
void LED(unsigned char num)
{
    if(num == 1)
    {
        LED3 = 1;
    }
    else if(num == 2)
    {
        LED4 = 1;
    }
    else if(num == 3)
    {
        LED5 = 1;
    }
    else if(num == 4)
    {
        LED6 = 1;
    }
    else
    {
        LED3 = 0;
        LED4 = 0;
        LED5 = 0;
        LED6 = 0;
    }
}
 
/*==============================================================================
 Music note functions. Plays note for requested cycles of time period.
 =============================================================================*/
 
void noteE5(unsigned int per)   // Plays note E5 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(758);
    }
}
 
void noteCS6(unsigned int per)   // Plays note C#6 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(451);
    }
}
 
void noteA5(unsigned int per)   // Plays note A5 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(568);
    }
}
 
void noteE6(unsigned int per)   // Plays note E6 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(379);
    }
}

void noteE4(unsigned int per)   // Plays note E4 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(1516);
    }
}

void noteG4(unsigned int per)   // Plays note G4 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(1275);
    }
}

void noteC5(unsigned int per)   // Plays note C5 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(955);
    }
}

void noteD5(unsigned int per)   // Plays note D5 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(851);
    }
}

void noteG5(unsigned int per)   // Plays note G5 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(637);
    }
}

void noteB4(unsigned int per)   // Plays note B4 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(1012);
    }
}

void noteF5(unsigned int per)   // Plays note F5 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(715);
    }
}

void noteG3(unsigned int per)   // Plays note G3 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(2551);
    }
}

void noteC4(unsigned int per)   // Plays note C4 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(1911);
    }
}

void noteG6(unsigned int per)   // Plays note G6 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(637);
    }
}

void noteGS3(unsigned int per)   // Plays note GS3 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(2407);
    }
}

void noteEF4(unsigned int per)   // Plays note EF4 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(1607);
    }
}

void noteGS4(unsigned int per)   // Plays note GS4 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(1203);
    }
}

void noteEF5(unsigned int per)   // Plays note EF5 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(803);
    }
}

void noteGS5(unsigned int per)   // Plays note GS5 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(601);
    }
}

void noteDS5(unsigned int per)   // Plays note DS5 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(803);
    }
}

void noteAS3(unsigned int per)   // Plays note AS3 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(2145);
    }
}

void noteD4(unsigned int per)   // Plays note D4 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(1702);
    }
}

void noteF4(unsigned int per)   // Plays note F4 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(1431);
    }
}

void noteBF4(unsigned int per)   // Plays note BF4 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(1072);
    }
}

void noteBF5(unsigned int per)   // Plays note BF5 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(536);
    }
}

void noteC6(unsigned int per)   // Plays note C6 for requested number of cycles
{
    for(per; per != 0; per--)
    {
        BEEPER = !BEEPER;
        __delay_us(477);
    }
}
 
/*==============================================================================
 Sound functions. 
 =============================================================================*/
 
void sound(unsigned char num)   // Play selected note sound
{
	if(num == 1)
        noteE5(379);
	else if(num == 2)
		noteCS6(568);
	else if(num == 3)
		noteE6(758);
	else if(num == 4)
		noteA5(451);	
}
 
void startSound(void)           // Game start-up sound (mario 1up sound)
{

   noteE4(125);
   noteG4(150);
   noteE5(150);
   noteC5(150);
   noteD5(150);
   noteG5(150);
   
   
}
 
void win(void)                  // Game win function (mario complete level sound)
{
    noteG3(100);
    noteC4(100);
    noteE4(100);
    noteG4(100);
    noteC5(100);
    noteE5(100);
    __delay_ms(10);
    noteG6(600);
    __delay_ms(10);
    noteE5(600);
    __delay_ms(10);
    
    noteGS3(100);
    noteC4(100);
    noteEF4(100);
    noteGS4(100); 
    noteC5(100);
    noteEF5(100);
    __delay_ms(10);
    noteGS5(600);
    __delay_ms(10);
    noteDS5(600);
    __delay_ms(10);
    
    noteAS3(100);
    noteD4(100);
    noteF4(100);
    noteBF4(100);
    noteD5(100);
    noteF5(100);
    noteBF5(600);
    __delay_ms(10);
    noteBF5(300);
     __delay_ms(10);
    noteBF5(300);
     __delay_ms(10);
    noteBF5(300);
     __delay_ms(10);
    noteC6(1000);
    __delay_ms(25);

 
	nap();                      // Shutdown
}
 
void lose(void)                 // Pattern fail. Game lose function (mario death sound, shows score, turns off)
{
    noteB4(500);
    noteF5(250);
    __delay_ms(25);
    noteF5(250);
    __delay_ms(25);
    noteF5(250);
    __delay_ms(25);
    noteE5(300);
    noteD5(300);
    noteC5(500);
      
      maxStep = maxStep - 1;
      while(maxStep >= 10)               //Show score (tens)
      {
          LED(1);
          __delay_ms(500);
          LED(0);
          __delay_ms(500);
          maxStep = maxStep - 10;
      }     
      
         while(maxStep >= 5)            //Show score (fives)
      {
          LED(2);
          __delay_ms(500);
          LED(0);
          __delay_ms(500);
          maxStep = maxStep - 5;
      }  
      
         while(maxStep > 0)           //Show score (ones)
      {
          LED(3);
          __delay_ms(500);
          LED(0);
          __delay_ms(500);
          maxStep = maxStep - 1;
      }                         // Sounds, lights?                     
                                // Show score?
	nap();                      // Power down
}
 
/*==============================================================================
	Main program loop. The main() function is executed on power-up and reset.
==============================================================================*/
 
int main(void)
{
    OSC_config();               // Configure internal oscillator for 48 MHz
    UBMP4_config();             // Configure on-board UBMP4 I/O devices
    
    LED1 = 1;                   // Wait for a button press
    while(SW2 == !pressed && SW3 == !pressed && SW4 == !pressed && SW5 == !pressed)
    {
        ticks++;
    }
    LED1 = 0;                   // LED on, play start sound
    startSound();
    while(SW2 == pressed || SW3 == pressed || SW4 == pressed || SW5 == pressed);
 
    // Initialize the random number generator
    randomSeed = TMR0 ^ ticks;  // Generate random number from timer and ticks
	newSeed = (newSeed << 8 ) | randomSeed; // Mix previous seed with random sample
	srand(newSeed);             // Seed random number generator
    
	maxStep = 0;                // Reset user's max step
    
	while(1)					// Loop continuously until win, lose or shutdown
	{
        // Button and light test code
        
//		button = getButton();   // Button test code
//		LED(button);
//        sound(button);
//		LED(0);
//
//        // Activate bootloader if SW1 is pressed.
//        if(SW1 == 0)
//        {
//            RESET();
//        }
 
        // Add a new step and play the pattern for the user
        
		__delay_ms(1000);       // Delay before playing next pattern
		pattern[maxStep] = (rand() & 0b00000011) + 1;   // Pick next pattern step
		maxStep++;              // Increase step count and check if user won
		if(maxStep == (patternLength + 1))       
			win();
		for(step = 0; step != maxStep; step++)  // Play each pattern step
		{
			LED(pattern[step]);
			sound(pattern[step]);
			LED(0);
			__delay_ms(500);
		}`  6
		
        // Let user repeat the pattern, comparing each step to the saved pattern
        
		step = 0;               // Reset step count before user matching attempt
 
		while(step != maxStep)
		{
            idle = 0;               // Reset idle timer for each user guess
			button = getButton();   // Is a button pressed?
			while(button == 0)      // No, wait for button press
			{
				button = getButton();
                __delay_ms(50);
                idle++;     // Increment idle timer to check for shutdown
                if(idle == timeOut)
                {
                    nap();
                }
			}
			LED(button);        // Show button and play sound for each guess
			sound(button);
			LED(0);
			
			if(button != pattern[step])
			{
				__delay_ms(500);
				lose();         // Uh, oh. Button didn't match pattern
			}
			step++;             // Pattern matches! Go on to the next step
			
            idle = 0;           // Reset idle timer.
			while(getButton() != 0) // Wait for button release
            {
                __delay_ms(50);
                idle++;          // Increment idle timer to check for shutdown
                if(idle == timeOut)
                {
                    nap();
                }
            }
            
            randomSeed = TMR0;      // Update random seed from fast timer
		}
	}
}
 

