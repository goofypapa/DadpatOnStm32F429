#include <includes.h>

__IO uint16_t ADC_ConvertedValue[PIEZOELECTRIC_ELEMENT_NOFCHANEL];

void PIEZOELECTRIC_ELEMENT_Init( void )
{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		DMA_InitTypeDef DMA_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
		RCC_AHB1PeriphClockCmd( PIEZOELECTRIC_ELEMENT_ADC_GPIO_CLK, ENABLE );
		RCC_APB2PeriphClockCmd( PIEZOELECTRIC_ELEMENT_ADC_CLK, ENABLE );
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	
		GPIO_InitStructure.GPIO_Pin = PIEZOELECTRIC_ELEMENT1_INT_GPIO_PIN |
																	PIEZOELECTRIC_ELEMENT2_INT_GPIO_PIN |
																	PIEZOELECTRIC_ELEMENT3_INT_GPIO_PIN |
																	PIEZOELECTRIC_ELEMENT4_INT_GPIO_PIN |
																	PIEZOELECTRIC_ELEMENT5_INT_GPIO_PIN;
	
		GPIO_Init( PIEZOELECTRIC_ELEMENT_ADC_GPIO_PORT, &GPIO_InitStructure );
		
		
		//init DMA
		RCC_AHB1PeriphClockCmd( PIEZOELECTRIC_ELEMENT_ADC_DMA_CLK, ENABLE );
		DMA_InitStructure.DMA_PeripheralBaseAddr = PIEZOELECTRIC_ELEMENT_ADC_CDR_ADDR;
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_ConvertedValue;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize = PIEZOELECTRIC_ELEMENT_NOFCHANEL;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOStatus_HalfFull;

		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_InitStructure.DMA_Channel = PIEZOELECTRIC_ELEMENT_ADC_DMA_Channel;
		
		DMA_Init( PIEZOELECTRIC_ELEMENT_ADC_DMA_STREAM, &DMA_InitStructure );
		DMA_Cmd( PIEZOELECTRIC_ELEMENT_ADC_DMA_STREAM, ENABLE );
		
		
		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
		ADC_CommonInit( &ADC_CommonInitStructure );
		
		
		//init ADC
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfConversion = PIEZOELECTRIC_ELEMENT_NOFCHANEL;
		ADC_Init( PIEZOELECTRIC_ELEMENT_ADC, &ADC_InitStructure );
		
		
		ADC_RegularChannelConfig( PIEZOELECTRIC_ELEMENT_ADC, PIEZOELECTRIC_ELEMENT1_INT_CHANNEL, 1, ADC_SampleTime_56Cycles );
		ADC_RegularChannelConfig( PIEZOELECTRIC_ELEMENT_ADC, PIEZOELECTRIC_ELEMENT2_INT_CHANNEL, 2, ADC_SampleTime_56Cycles );
		ADC_RegularChannelConfig( PIEZOELECTRIC_ELEMENT_ADC, PIEZOELECTRIC_ELEMENT3_INT_CHANNEL, 3, ADC_SampleTime_56Cycles );
		ADC_RegularChannelConfig( PIEZOELECTRIC_ELEMENT_ADC, PIEZOELECTRIC_ELEMENT4_INT_CHANNEL, 4, ADC_SampleTime_56Cycles );
		ADC_RegularChannelConfig( PIEZOELECTRIC_ELEMENT_ADC, PIEZOELECTRIC_ELEMENT5_INT_CHANNEL, 5, ADC_SampleTime_56Cycles );
		
		
		ADC_DMARequestAfterLastTransferCmd( PIEZOELECTRIC_ELEMENT_ADC, ENABLE );
		ADC_DMACmd( PIEZOELECTRIC_ELEMENT_ADC, ENABLE );
		ADC_Cmd( PIEZOELECTRIC_ELEMENT_ADC, ENABLE );
		ADC_SoftwareStartConv( PIEZOELECTRIC_ELEMENT_ADC );
		
}
