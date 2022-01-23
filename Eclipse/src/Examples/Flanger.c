//技术点需要写入手册的
//1. 循环的数组
//2. 定义长数组但是只用部分

#include <pdl_header.h>
#include <platform.h>
#include <utils.h>

// External library
#include "FM4_slider_interface.h"

// user definitions
#define SAMPLERATE 48000	// CODEC sampling frequency in Hz, 8000, 32000, 48000 or 96000
#define N 32001		// modulo delay of 24000 samples for 0.5 s delay at 48000 Hz sample frequency

// user global variables
float32_t buffer[N] = {0.0};	// (Modulo) buffer of N values, all initialized by 0.0 to avoid 0.5 s noise at output
int16_t bufferInd = 0;				// buffer index
extern struct FM4_slider_struct FM4_GUI; // For GUI

// prototypes
void I2S_HANDLER(void);

void I2S_HANDLER(void) {   /****** I2S Interruption Handler *****/
	union audio audioIO;

	float32_t x_n, y_n, delayed_input, gainedDelayed_input;
	//float32_t gainFactor = 0.7; //-1 to 1
	// Use GUI slide to adjust P_vals[0] from -1 to 1

	//gpio_toggle(TEST_PIN);
	gpio_set(TEST_PIN, HIGH);		// TestPin P10, 110 ns

	audioIO.audioSample = I2s_ReadRxFifo(&I2S0); // get one sample (left and right channel, 16 bit each), 175 ns

//	Process (nothing to do, just a loop)

// Calculate average of left and right sample - cast it to float32_t
	x_n = (float32_t)(audioIO.audio_ch[LEFT]/2 + audioIO.audio_ch[RIGHT]/2);

	delayed_input = buffer[bufferInd];							// 1.) read oldest value out of delay modulo buffer
	buffer[bufferInd] = x_n;								// 2.) overwrite eldest sample value by newest sample

    bufferInd = (++bufferInd)%(int16_t)FM4_GUI.P_vals[1];								// does the same as 3.) and 4.)

    gainedDelayed_input = delayed_input*FM4_GUI.P_vals[0];				// 5.) calculate delayed levelled value

    y_n = gainedDelayed_input + x_n;			// 7.) calculate sum of delayed and non-delayed levelled values

	audioIO.audio_ch[LEFT] = audioIO.audio_ch[LEFT] = (int16_t)y_n;	// write back audio signal to left and right channels

	I2s_WriteTxFifo(&I2S0, audioIO.audioSample); // send one sample, 175 ns

	gpio_set(TEST_PIN, LOW);		// TestPin P10, 110 ns
	gpio_set(LED_B, LOW);			// LED_B on
}

int main(void) {
	// Initialize platform resources: I2S, I2C and UART interfaces, CODEC, GPIO for LED, user button and test pin
	platform_init(BAUDRATE, SAMPLERATE, line_in, intr, I2S_HANDLER, NULL); // second pointer to interrupt handler only for DSTC needed

	// send a string to the terminal
	// writeUart0("Hello FM4 World!\r\n");

	while(1) {
		// Update slider parameters
		update_slider_parameters(&FM4_GUI);

		// update line in and head phone level through slider app
		setLIandHPlevels(&FM4_GUI);

		// toggle activity indicator
		gpio_set(LED_B, HIGH);		// LED_B off
	}
}
