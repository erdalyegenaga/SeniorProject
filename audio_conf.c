/* Includes ------------------------------------------------------------------*/

#include <audio_conf.h>
#include <cs43l22.h>
/*----------------------------------------------------------------------------*/

/* Ping-Pong buffer used for audio play */
uint16_t 			audiobuff[BUFF_LEN]; // THE audio buffer

/* Initial Volume level (from 0 (Mute) to 100 (Max)) */
static uint8_t		volume = VOL;

static bool			sound = true;

/*------------------------------------------------------------------------------*/
void audio_init(void)
{
	/* Initialize Wave player (Codec, DMA, I2C) */
	if(BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_AUTO, VOL, SAMPLERATE) != 0)
	{
		Error_Handler();
	}

	/* Start playing Wave */
	BSP_AUDIO_OUT_Play((uint16_t*)&audiobuff[0], 2*BUFF_LEN); // size must be in bytes

}

/*------------------------------------------------------------------------------*/

/*--------------------------------
Callbacks implementation:
The callbacks prototypes are defined in the stm32f4_discovery_audio_codec.h file
and their implementation should be done in the user code if they are needed.
Below some examples of callback implementations.
--------------------------------------------------------*/

/**
 * @brief  Manages the DMA Half Transfer complete interrupt.
 * @param  None
 * @retval None
 */
void BSP_AUDIO_OUT_HalfTransfer_CallBack(void)
{
	BSP_LED_Off(LED_Orange); // stop breathing
	make_sound((uint16_t *)audiobuff, BUFF_LEN_DIV4);
	BSP_LED_On(LED_Orange); // breath again ! (cpu usage indicator)

}

/**
 * @brief  Manages the DMA transfer complete interrupt.
 * @param  None
 * @retval None
 */
void BSP_AUDIO_OUT_TransferComplete_CallBack(void)
{
	BSP_LED_Off(LED_Orange);
	make_sound((uint16_t *)(audiobuff + BUFF_LEN_DIV2), BUFF_LEN_DIV4);
	BSP_LED_On(LED_Orange);
}

/**
 * @brief  Manages the DMA FIFO error interrupt.
 * @param  None
 * @retval None
 */
void BSP_AUDIO_OUT_Error_CallBack(void)
{
	/* Stop the program with an infinite loop */
	while (1)
	{}

	/* Could also generate a system reset to recover from the error */
	/* .... */
}



void toggleSound(void)
{
	if ( ! sound )
	{
		//pitchGenResetPhase();
		BSP_AUDIO_OUT_SetVolume(volume);
		sound = true;
	}
	else
	{
		BSP_AUDIO_OUT_SetVolume(0);
		sound = false;
	}
}
//increase output DAC volume
void incVol(void)
{
	if (volume < MAXVOL)
	{
		volume++;
		BSP_AUDIO_OUT_SetVolume(volume);
	}
}
//decrease output DAC volume
void decVol(void)
{
	if (volume > 0)
	{
		volume--;
		BSP_AUDIO_OUT_SetVolume(volume);
	}
}
void Volume_set(uint8_t val)
{
	volume = (uint8_t)(MAXVOL/MIDI_MAX * val);
	BSP_AUDIO_OUT_SetVolume(volume);
}
