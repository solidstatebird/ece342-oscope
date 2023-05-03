#include "sampler.h"

static ADC *adc = new ADC();       // adc object

DMAMEM static volatile uint16_t __attribute__((aligned(32)))
adc0_buffer1[BUFFER_SIZE];
// DMAMEM static volatile uint16_t __attribute__((aligned(32)))
// adc0_buffer2[BUFFER_SIZE];
DMAMEM static volatile uint16_t __attribute__((aligned(32)))
adc1_buffer1[BUFFER_SIZE];
// DMAMEM static volatile uint16_t __attribute__((aligned(32)))
// adc1_buffer2[BUFFER_SIZE];

// specifying only one buffer automatically enabes stopping on completion
AnalogBufferDMA adc0_abdma(adc0_buffer1, BUFFER_SIZE);
AnalogBufferDMA adc1_abdma(adc1_buffer1, BUFFER_SIZE);
// AnalogBufferDMA adc0_abdma(adc0_buffer1, BUFFER_SIZE, adc0_buffer2, BUFFER_SIZE);
// AnalogBufferDMA adc1_abdma(adc1_buffer1, BUFFER_SIZE, adc1_buffer2, BUFFER_SIZE);

void setupADC(uint8_t pin1, uint8_t pin2)
{
    pinMode(pin1, INPUT_DISABLE);
    pinMode(pin2, INPUT_DISABLE);

    adc->adc0->setAveraging(0);
    adc->adc0->setResolution(12);
    adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
    adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);

    adc->adc1->setAveraging(0);
    adc->adc1->setResolution(12);
    adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
    adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);

    // startSynchronizedContinuous omits waiting for calibration
    adc->adc0->wait_for_cal();
    adc->adc1->wait_for_cal();

    // setup DMA Channels
    adc0_abdma.init(adc, ADC_0);
    adc1_abdma.init(adc, ADC_1);

    // Start the DMA operation
    adc->startSynchronizedContinuous(pin1, pin2);
}

void processBuffers(int16_t *out1, int16_t *out2, float scale1, float scale2)
{
    volatile uint16_t *in0 = adc0_abdma.bufferLastISRFilled();
    volatile uint16_t *in1 = adc1_abdma.bufferLastISRFilled();

    // use if switching buffers
    // volatile uint16_t size0 = adc0_abdma.bufferCountLastISRFilled();
    // volatile uint16_t *end_data = data + analogBuffer->bufferCountLastISRFilled();

    arm_dcache_delete((void *)in0, sizeof(adc0_buffer1));
    arm_dcache_delete((void *)in1, sizeof(adc0_buffer1));

    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        out1[i] = ((int)in0[i] - 2047) * ((3.3 * 1000) / 4096) * scale1;
        out2[i] = ((int)in1[i] - 2047) * ((3.3 * 1000) / 4096) * scale2;
    }

    adc0_abdma.clearInterrupt();
    adc1_abdma.clearInterrupt();
    // restart DMA
    adc0_abdma.clearCompletion();
    adc1_abdma.clearCompletion();
}

bool DMA_completed()
{
    return adc0_abdma.interrupted() && adc1_abdma.interrupted();
}