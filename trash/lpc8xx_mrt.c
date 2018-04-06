
#include "LPC8xx.h"
#include "lpc8xx_nmi.h"
#include "lpc8xx_mrt.h"


volatile uint32_t mrt_counter[] = {0,0,0,0};
volatile uint8_t bzStatus = !PRESSED;
volatile uint8_t bcStatus = !PRESSED;

void mrtSet(uint32_t ms, uint8_t channel)
{
	LPC_MRT->Channel[channel].INTVAL = ms * 30000; // 30,000 cyles per ms
}

void mrtRestart(uint8_t channel)
{
	LPC_MRT->Channel[channel].INTVAL |= 0x1UL<<31;
}


void delayMs(uint32_t delayInMs)
{
  mrt_counter[DELAY_CHANNEL] = 0;
  LPC_MRT->Channel[DELAY_CHANNEL].INTVAL = 30000; // 30,000 cyles per ms
  LPC_MRT->Channel[DELAY_CHANNEL].INTVAL |= 0x1UL<<31;
  while (mrt_counter[DELAY_CHANNEL] < delayInMs) {}
}

void MRT_IRQHandler(void)
{  
	if ( LPC_MRT->Channel[DELAY_CHANNEL].STAT & MRT_STAT_IRQ_FLAG )
	{
		LPC_MRT->Channel[DELAY_CHANNEL].STAT = MRT_STAT_IRQ_FLAG;			/* clear interrupt flag */
		mrt_counter[DELAY_CHANNEL]++;
	}
	if ( LPC_MRT->Channel[BC_CHANNEL].STAT & MRT_STAT_IRQ_FLAG )
	{
		LPC_MRT->Channel[BC_CHANNEL].STAT = MRT_STAT_IRQ_FLAG;			/* clear interrupt flag */
		mrt_counter[BC_CHANNEL]++;
	}
	if ( LPC_MRT->Channel[BZ_CHANNEL].STAT & MRT_STAT_IRQ_FLAG )
	{
		LPC_MRT->Channel[BZ_CHANNEL].STAT = MRT_STAT_IRQ_FLAG;			/* clear interrupt flag */
		mrt_counter[BZ_CHANNEL]++;
	}

  return;
}

// four channels available 0-3
void mrtInit(uint32_t TimerInterval, uint8_t channel) 
{
	/* Enable clock to MRT and reset the MRT peripheral */
	LPC_SYSCON->SYSAHBCLKCTRL |= (0x1<<10);
	LPC_SYSCON->PRESETCTRL &= ~(0x1<<7);
	LPC_SYSCON->PRESETCTRL |= (0x1<<7);
	
	mrt_counter[channel] = 0;
	LPC_MRT->Channel[channel].INTVAL = TimerInterval;
	LPC_MRT->Channel[channel].INTVAL |= 0x1UL<<31;
	
	LPC_MRT->Channel[channel].CTRL = MRT_REPEATED_MODE|MRT_INT_ENA;
	
	/* Enable the MRT Interrupt */
#if NMI_ENABLED
	NVIC_DisableIRQ( MRT_IRQn );
	NMI_Init( MRT_IRQn );
#else
	NVIC_EnableIRQ(MRT_IRQn);
#endif
  return;
}
