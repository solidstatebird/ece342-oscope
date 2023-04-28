#include <Arduino.h>

#include <ADC.h>
#include <AnalogBufferDMA.h>

time_t timer = 0;
time_t timer_end = 0;

const int channel_1_pin = A4;
const int channel_2_pin = A5;

ADC *adc = new ADC(); // adc object
const uint16_t buffer_size = 1600;//32767; //breaks if this is more than 1/2 the limit (32,767) also fails if too small - 1600 seems to be around the minimum

DMAMEM static volatile uint16_t __attribute__((aligned(32)))
adc0_buffer1[buffer_size];
// DMAMEM static volatile uint16_t __attribute__((aligned(32)))
// adc0_buffer2[buffer_size];
DMAMEM static volatile uint16_t __attribute__((aligned(32)))
adc1_buffer1[buffer_size];
// DMAMEM static volatile uint16_t __attribute__((aligned(32)))
// adc1_buffer2[buffer_size];

// specifying only one buffer automatically enabes stopping on completion
AnalogBufferDMA adc0_abdma(adc0_buffer1, buffer_size);
AnalogBufferDMA adc1_abdma(adc1_buffer1, buffer_size);
// AnalogBufferDMA adc0_abdma(adc0_buffer1, buffer_size, adc0_buffer2, buffer_size);
// AnalogBufferDMA adc1_abdma(adc1_buffer1, buffer_size, adc1_buffer2, buffer_size);

void processBuffers(int16_t *, int16_t *);

void setup()
{
    while (!Serial && millis() < 5000)
        ; // Wait 5 seconds for serial connection

    pinMode(channel_1_pin, INPUT_DISABLE);
    pinMode(channel_2_pin, INPUT_DISABLE);

    adc->adc0->setAveraging(0);
    adc->adc0->setResolution(12);
    adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
    adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);

    adc->adc1->setAveraging(0);
    adc->adc1->setResolution(12);
    adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
    adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);

    // startSynchronizedContinuous omits waiting for calibration
    adc->adc0->wait_for_cal();
    adc->adc1->wait_for_cal();


    // setup DMA Channels
    adc0_abdma.init(adc, ADC_0);
    adc1_abdma.init(adc, ADC_1);

    Serial.println("end setup");

    // Start the DMA operation
    adc->startSynchronizedContinuous(channel_1_pin, channel_2_pin);
    timer = micros();
}

void loop()
{
    static bool data_ready = false;
    static int16_t channel_1_data[buffer_size];
    static int16_t channel_2_data[buffer_size];

    if (adc0_abdma.interrupted() && adc1_abdma.interrupted()) 
    {
        Serial.println();
        Serial.print("Sample rate: ");
        Serial.print(1 / ((float)(micros() - timer) / buffer_size));
        Serial.println(" Msps");

        processBuffers(channel_1_data, channel_2_data);
        timer = micros();
        data_ready = true;

        // static int count = 100;
        // if (count > 5) while(1);
        // count++;
        delay(1000);
        
    }

    if (data_ready)
    {
        for (uint16_t i = 0; i < buffer_size; i++)
        {
            Serial.print(channel_1_data[i]);
            Serial.print(',');
            Serial.println(channel_2_data[i]);
        }
        data_ready = false;
    }
}

void processBuffers(int16_t* out1, int16_t* out2)
{
    volatile uint16_t *in0 = adc0_abdma.bufferLastISRFilled();
    volatile uint16_t *in1 = adc1_abdma.bufferLastISRFilled();

    // use if switching buffers
    // volatile uint16_t size0 = adc0_abdma.bufferCountLastISRFilled();
    // volatile uint16_t *end_data = data + analogBuffer->bufferCountLastISRFilled();

    arm_dcache_delete((void *)in0, sizeof(adc0_buffer1));
    arm_dcache_delete((void *)in1, sizeof(adc0_buffer1));

    for (uint16_t i = 0; i < buffer_size; i++)
    {
        out1[i] = ((int)in0[i] - 2047) * ((3.3 * 1000) / 4096);
        out2[i] = ((int)in1[i] - 2047) * ((3.3 * 1000) / 4096);
    }

    adc0_abdma.clearInterrupt();
    adc1_abdma.clearInterrupt();
    //restart DMA
    adc0_abdma.clearCompletion();
    adc1_abdma.clearCompletion();
}