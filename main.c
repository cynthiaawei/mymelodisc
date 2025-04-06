#include "myarrays.h"

short unsigned int **background_array;
int background_array_size;
short unsigned int **lyrics_array;
int lyrics_array_size;

// DEFINITIONS
#define PS2_BASE 0xFF200100
#define TIMER_BASE 0xFF202000
#define AUDIO_BASE 0xFF203040
	
int clock_rate = 100000000;
#define quarter_clock clock_rate / 4
typedef long long int64;

// INTERRUPT FUNCTIONS
static void handler(void) __attribute__((interrupt("machine")));
void setup_interrupts();
void set_itimer(void);
void itimer_ISR(void);
void keyboard_ISR();

// LOGIC FUNCTIONS
void key1_APT();
void key2_BeautyAndABeat();
void key3_BirdsOfAFeather();
void key4_GoodLuckBabe();
void key5_WhenIWasYourMan();
void space_PausePlay();
void esc_ReturnToMainScreen();
void change_backgound();


// GLOBAL VARIABLES
volatile int *PS2_ptr = (int *)PS2_BASE;
int key_pressed = 0; // value of last key pressed, ensure to set to 0 after action finished.
volatile int *PS2_control = (int *)PS2_BASE;
volatile int *timer_ptr = (int *) TIMER_BASE;
//volatile int *mtime_ptr = (int *)MTIME_BASE;
volatile int* audio_ptr = (int*)AUDIO_BASE;

// VGA SET-UP & FUNCTIONS
int pixel_buffer_start; 
volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
void plot_pixel(int x, int y, short int line_color);
void plot_image(int x, int y, short int img[]);
void plot_lyric(int x, int y, short int img[]);
void wait_for_v_sync();

short int Buffer1[240][512]; // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];

bool on_main_page = true;



//int *arrSONGS[] = {arrAPT, arrBAAB, arrBOAF, arrGLB, arrWIWYM};
int *arrSONGS[] = {arrAPT, arrBAAB, arrBOAF, arrGLB, arrWIWYM};
int arrSONGLENGTH[] = {304000, 600000, 296000, 560000, 616000};

int temp[] = {0x0, 0x0};

int playAudio = 0;
int audioCursor = 0; // where the audio has stopped playing (index number in array)
int shift = 16;
int audio_reset = 0;
int pauseLocation = 0;
int pauseSamples = 0;
int* samples = 0; //arrSONGS[0];
int samples_n = 0; //304000;

bool isAPT = false;
bool clear = false;

struct audio_t {
	volatile unsigned int control;
	volatile unsigned char rarc;
	volatile unsigned char ralc;
	volatile unsigned char warc;
	volatile unsigned char walc;
    volatile unsigned int ldata;
	volatile unsigned int rdata;
};
 
struct audio_t *const audiop = ((struct audio_t *)AUDIO_BASE);

void audio_playback_mono(int *samples, int n, int step, int replicate) {
    int i; // same as audioCursor
	shift = 16;

	if(playAudio == 1){
		audiop->control = 0x8; // clear the output FIFOs
		audiop->control = 0x0; // resume input conversion
		int clear = 0;

		for (i = 0; (i < n && !clear); i+=step) {
			//audioCursor = i;

			// output data if there is space in the output FIFOs
			for (int r=0; (r < replicate && !clear); r++) {
				while((audiop->warc == 0));
				audiop->ldata = samples[i] << shift;
				audiop->rdata = samples[i] << shift;
			}
		}
	}
	samples = temp;
	samples_n = 1;

		// if(i == n-1){ // audio finished playing
		//    playAudio = 0;
		//    //audioCursor = 0;
	    // }
	//}		    
}

#define GPIO_ptr ((volatile uint32_t *) 0xFF200070)
// MAIN
int main(void) {
	setup_interrupts();
	
    /* set front pixel buffer to Buffer 1 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer1; // first store the address in the  back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_v_sync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    /* set back pixel buffer to Buffer 2 */
    *(pixel_ctrl_ptr + 1) = (int) &Buffer2;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
	
	//pixel_buffer_start = *pixel_ctrl_ptr;
	plot_image(0,0,main_screen);
	wait_for_v_sync(); // swap front and back buffers on VGA vertical sync
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
	
	//int left, right, FIFOspace; // mic
	playAudio = 0; //temp
	samples = temp;
	samples_n = 1;
	audio_reset = *(audio_ptr);
    
    //GPIO
	*(GPIO_ptr+1) = 0xffffffff; // set direction to output
	*(GPIO_ptr) = 0x0; // set all to 0

  while (1) {
		//audio_playback_mono(samples, samples_n, 1, 1);
		
		
		int i; // same as audioCursor
		int replicate = 1, step = 1;
		//shift = 16;
		
		if(playAudio == 1){
			__asm__ volatile("nop");
			audiop->control = 0x8; // clear the output FIFOs
			audiop->control = 0x0; // resume input conversion
			int clear = 0;

			//__asm__ volatile("nop");
			for (audioCursor = 0; (audioCursor < samples_n && playAudio); audioCursor+=step) {
				//audioCursor = audioCursor;
				// output data if there is space in the output FIFOs
				for (int r=0; (r < replicate && playAudio); r++) {
					while(playAudio && (audiop->warc == 0));
					audiop->ldata = samples[audioCursor] << shift*playAudio;
					audiop->rdata = samples[audioCursor] << shift*playAudio;
				}
			}
            playAudio = 0;
			samples = temp;
			samples_n = 1;
		} 
		if(playAudio == 0){ // empty fifio
			__asm__ volatile("nop");
			__asm__ volatile("nop");
			
		    while(((*(audio_ptr+3)) != 0) && ((*(audio_ptr+2)) != 0)){ // condition for audio fifo clear
			    *(audio_ptr) = 0xC; // empty the fifo
            } 
            *(audio_ptr) = audio_reset;	
		}
    //*LEDR_ptr = counter; //random action in while loop
	//printf("%d", key_pressed); //checking last key pressed
    //*HEX3_HEX0_ptr=bit_codes[digit]; //display in decimal
  }
	//   FIFOspace = *(audio_ptr + 1);
	// 	if((FIFOspace&0xFF)>0){
	// 		// input channel != 0, load
	// 		left = *(audio_ptr+2);			// obtain input from mic
	// 		right = *(audio_ptr+3);
	// 		*(audio_ptr+2) = left;			// send output to speaker
	// 		*(audio_ptr+3) = right;
	// 	}
  
}


// INTERRUPT SET-UP
void setup_interrupts(){
	//  set_mtimer();
	set_itimer();

  	int mstatus_value, mtvec_value, mie_value;
  	mstatus_value = 0b1000;  // interrupt bit mask (mie)

  	// disableinterrupts
  	__asm__ volatile("csrc mstatus,%0" ::"r"(mstatus_value));
  	mtvec_value = (int)&handler;  // set trap address
  	__asm__ volatile("csrw mtvec,%0" ::"r"(mtvec_value));

  	// disable all interrupts that are currently enabled
  	__asm__ volatile("csrr %0,mie" : "=r"(mie_value));
  	__asm__ volatile("csrc mie,%0" ::"r"(mie_value));

  	// enable interrupt bits for specific hardware
	mie_value = 0x410000;  // PS2 & Interval: 0100 0001 0000 0000 0000 0000 = 0x410000
	// PS2 Port (IRQ #22) / Software Interrupt (IRQ #3) 
	// Interval Timer (IRQ #16) / 2nd Interval Timer (IRQ #17) / Machine Timer (IRQ #7)

  	// set interrupt enables
  	__asm__ volatile("csrs mie,%0" ::"r"(mie_value));

  	// enableNiosVinterrupts
  	__asm__ volatile("csrs mstatus,%0" ::"r"(mstatus_value));

  	// enable keyboard interrupts
  	__asm__ volatile("nop");
  	*(PS2_control + 1) = 0x400;  // bit 10 CE = 1
  	*(PS2_control + 1) = 1;      // bit 1 RE = 1

  	// cause a software interrupt
  	//*(mtime_ptr + 4) = 1;  

}

void set_itimer(void){	
	// set the interval timer period
	int load_val = clock_rate;
	*(timer_ptr + 0x2) = (load_val & 0xFFFF);
	*(timer_ptr + 0x3) = (load_val >> 16) & 0xFFFF;
	
	// start interval timer, enable its interrupts
	*(timer_ptr + 1) = 0x9; // STOP = 1, START = 1, CONT = 1, ITO = 1 - 1001
}


/*************************************************************************
 Trap Handler: determine cause of interrupt & call appropriate sub routine
 ************************************************************************/

void handler(void){
	int mcause_value; 
	__asm__ volatile("csrr %0, mcause" : "=r"(mcause_value)); // obtain mcause

	if(mcause_value == 0x80000016){ // keyboard
		keyboard_ISR();
	} else if (mcause_value == 0x80000010){ // interval timer
		itimer_ISR();
	}
	// software interrupt (mcause == 0x80000003)
}

void itimer_ISR(void){
	*timer_ptr = 0; // clear the interrupt
	change_backgound();
}

void keyboard_ISR() {
	int PS2_data, RVALID;
 	char byte1 = 0, byte2 = 0, byte3 = 0;

	PS2_data = *(PS2_ptr); // read the data register in the PS/2 port
	RVALID = PS2_data & 0x8000;  // extract the RVALID field
	
	if (RVALID) { // shift the next data byte into the display
    	byte1 = byte2;
    	byte2 = byte3;
    	byte3 = PS2_data & 0xFF; // byte3 contains the key pressed code
		
		key_pressed = byte3;
  	}
	
	switch(key_pressed){
		case 22:
			key1_APT();
            //*(audio_ptr) = 0x10; // leave it
			playAudio = 1;
            isAPT = true;
			samples = arrSONGS[0];
			samples_n = arrSONGLENGTH[0];
            *(audio_ptr) = audio_reset;
			//printf("changed");
			break;
		case 30:
			key2_BeautyAndABeat();
            //*(audio_ptr) = 0x10; // leave it
			playAudio = 1;
			samples = arrSONGS[1];
			samples_n = arrSONGLENGTH[1];
            *(audio_ptr) = audio_reset;
			break;
		case 38:
			key3_BirdsOfAFeather();
            //*(audio_ptr) = 0x10; // leave it
			playAudio = 1;
			samples = arrSONGS[2];
			samples_n = arrSONGLENGTH[2];
            *(audio_ptr) = audio_reset;
			break;
		case 37:
			key4_GoodLuckBabe();
            //*(audio_ptr) = 0x10; // leave it
			playAudio = 1;
			samples = arrSONGS[3];
			samples_n = arrSONGLENGTH[3];
            *(audio_ptr) = audio_reset;
			break;
		case 46:
			key5_WhenIWasYourMan();
            // *(audio_ptr) = 0x10; // leave it
			playAudio = 1;
			samples = arrSONGS[4];
			samples_n = arrSONGLENGTH[4];
            *(audio_ptr) = audio_reset;
			break;
		case 41:
			space_PausePlay();
			break;
		case 118:
			esc_ReturnToMainScreen();
			break;
		default:
	}
}

// some more global variables
int counter = 0;
int lyric_ind = 0;
int bg_ind = 0;
int *lyr_time;

int apt_lyr_time[8] = {16, 16, 16, 19, 13, 5, 13, 4};
int baab_lyr_time[11] = {8, 8, 7, 9, 16, 17, 19, 16, 32, 22, 15};
int boaf_lyr_time[6] = {16, 16, 8, 8, 8, 9};
int glb_lyr_time[7] = {23, 17, 17, 17, 17, 17, 35};
int wiwym_lyr_time[8] = {14, 16, 8, 8, 15, 16, 9, 9};

void change_backgound(){
	if(lyric_ind == lyrics_array_size){
		esc_ReturnToMainScreen();
		return;
	}
	
	plot_image(0,0,background_array[bg_ind]);
	if(lyrics_array[lyric_ind] != NULL) plot_lyric(0, 105, lyrics_array[lyric_ind]);
	wait_for_v_sync(); // swap front and back buffers on VGA vertical sync
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
	
	bg_ind = (bg_ind + 1) % background_array_size;
	if(counter >= lyr_time[lyric_ind]){ 
		lyric_ind++;
		counter = 0;
	}
	counter++;
}

void modify_timer_speed_and_start(int load_val){
	*(timer_ptr + 0x2) = (load_val & 0xFFFF);
	*(timer_ptr + 0x3) = (load_val >> 16) & 0xFFFF;
	*(timer_ptr + 1) = 0x7;
}

int calculate_startnum(int bpm){
	int timerFrequency = 100000000;
	double sec = (double)60/bpm;
	return (int)(sec*timerFrequency);
}
int GPIO_pin = 0;
void key1_APT(){
	// if we are not on main page, this button does not work
	if(!on_main_page) return;
	
	// timer settings
	modify_timer_speed_and_start(calculate_startnum(149)); 
	
	// vga settings
	background_array = apt_bg;
	background_array_size = 2;
	on_main_page = false;
	lyrics_array = apt_lyrics;
	lyrics_array_size = 8;
	lyr_time = apt_lyr_time;
    *(GPIO_ptr) = 0x1; // GPIO
	GPIO_pin = 0x1;
}

void key2_BeautyAndABeat(){
	// if we are not on main page, this button does not work
	if(!on_main_page) return;
	
	// timer settings
	modify_timer_speed_and_start(calculate_startnum(128));
	
	// vga settings
	background_array = baab_bg;
	background_array_size = 2;
	on_main_page = false;
	lyrics_array = baab_lyrics;
	lyrics_array_size = 11;
	lyr_time = baab_lyr_time;
	*(GPIO_ptr) = 0x2; // GPIO
	GPIO_pin = 0x2;
}

void key3_BirdsOfAFeather(){
	// if we are not on main page, this button does not work
	if(!on_main_page) return;
	
	// timer settings
	modify_timer_speed_and_start(calculate_startnum(105));
	
	// vga settings
	background_array = boaf_bg;
	background_array_size = 4;
	on_main_page = false;
	lyrics_array = boaf_lyrics;
	lyrics_array_size = 6;
	lyr_time = boaf_lyr_time;
	*(GPIO_ptr) = 0x4; // GPIO
	GPIO_pin = 0x4;
}

void key4_GoodLuckBabe(){
	// if we are not on main page, this button does not work
	if(!on_main_page) return;
	
	// timer settings
	modify_timer_speed_and_start(calculate_startnum(117));
	
	// vga settings
	background_array = glb_bg;
	background_array_size = 2;
	on_main_page = false;
	lyrics_array = glb_lyrics;
	lyrics_array_size = 7;
	lyr_time = glb_lyr_time;
	*(GPIO_ptr) = 0x3; // GPIO
	GPIO_pin = 0x3;
}

void key5_WhenIWasYourMan(){
	// if we are not on main page, this button does not work
	if(!on_main_page) return;
	
	// timer settings
	modify_timer_speed_and_start(calculate_startnum(73));
	
	// vga settings
	background_array = wiwym_bg;
	background_array_size = 2;
	on_main_page = false;
	lyrics_array = wiwym_lyrics;
	lyrics_array_size = 8;
	lyr_time = wiwym_lyr_time;
	*(GPIO_ptr) = 0x5; // GPIO
	GPIO_pin = 0x5;
}

bool paused = false;
bool key_was_pressed = false;
void space_PausePlay(){
    if (!key_was_pressed) { // if key was not previously pressed, we press (does nothing)
        key_was_pressed = true;
        return; // do not toggle on key press
    }
    
	key_was_pressed = false; // is now key release > we toggle
    if (!paused) {
        *(timer_ptr + 1) = 0x9;
        paused = true;
       *(GPIO_ptr) = 0x7;
    } else {
        *(timer_ptr + 1) = 0x7;
        paused = false;
       *(GPIO_ptr) = GPIO_pin;
    }
}

void esc_ReturnToMainScreen(){
	*(timer_ptr + 1) = 0x9;
	plot_image(0,0,main_screen);
	wait_for_v_sync(); // swap front and back buffers on VGA vertical sync
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
	on_main_page = true;
	lyric_ind = 0;
	bg_ind = 0;
	counter = 0;
    *(GPIO_ptr) = 0;
}

// VGA FUNCTIONS
void wait_for_v_sync(){
	volatile int* pixel_ctrl_ptr = (int*)0xFF203020;
	int status;
	*pixel_ctrl_ptr = 1; // start swap command
	status = *(pixel_ctrl_ptr + 3);
	while((status&0x01) != 0){ // wait until rendering finished
		status = *(pixel_ctrl_ptr + 3);
	}
}

void plot_pixel(int x, int y, short int line_color){
    volatile short int *one_pixel_address;
    one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);
    *one_pixel_address = line_color;
}

void plot_image(int x, int y, short int img[]) {
    for (int i = 0; i < 240; i++) {
        for (int j = 0; j < 320; j++) {
            plot_pixel(x + j, y + i, img[i * 320 + j]);
        }
    }
}
void plot_lyric(int x, int y, short int img[]) {
    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 320; j++) {
            plot_pixel(x + j, y + i, img[i * 320 + j]);
        }
    }
}