#include <Arduino.h>

#include <ADC.h>
#include <AnalogBufferDMA.h>

time_t timer = 0;

const int channel_1_pin = A4;
const int channel_2_pin = A5;

ADC *adc = new ADC(); // adc object
const uint32_t buffer_size = 1600;

DMAMEM static volatile uint16_t __attribute__((aligned(32)))
adc0_buffer1[buffer_size];
DMAMEM static volatile uint16_t __attribute__((aligned(32)))
adc0_buffer2[buffer_size];
DMAMEM static volatile uint16_t __attribute__((aligned(32)))
adc1_buffer1[buffer_size];
DMAMEM static volatile uint16_t __attribute__((aligned(32)))
adc1_buffer2[buffer_size];

// specifying only one buffer automatically enabes stopping on completion
AnalogBufferDMA adc0_abdma(adc0_buffer1, buffer_size);
// AnalogBufferDMA adc0_abdma(adc0_buffer1, buffer_size, adc0_buffer2, buffer_size);
// AnalogBufferDMA adc1_abdma(adc1_buffer1, buffer_size, adc1_buffer2, buffer_size);

void processBuffer(AnalogBufferDMA *analogBuffer);

void setup()
{
    Serial.begin(115200);
    while (!Serial && millis() < 5000)
        ; // Wait for serial for at 5 sec

    Serial.println("begin setup");

    pinMode(channel_1_pin, INPUT_DISABLE);
    pinMode(channel_2_pin, INPUT_DISABLE);
    adc->adc0->setAveraging(0);
    adc->adc0->setResolution(8);
    // adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::LOW_SPEED);
    // adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::LOW_SPEED);
    adc->adc0->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
    adc->adc0->setSamplingSpeed(ADC_SAMPLING_SPEED::HIGH_SPEED);

    // adc->adc1->setAveraging(0);
    // adc->adc1->setResolution(8);
    // adc->adc1->setConversionSpeed(ADC_CONVERSION_SPEED::HIGH_SPEED);
    // adc->adc1->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);

    // setup DMA Channels
    adc0_abdma.init(adc, ADC_0);
    // adc1_abdma.init(adc, ADC_1);

    // startSynchronizedContinuous omits waiting for calibration
    adc->adc0->wait_for_cal();
    // adc->adc1->wait_for_cal();
    Serial.println("end setup");
    // Start the DMA operation
    adc->adc0->startContinuous(channel_1_pin);
    // adc->adc1->startContinuous(channel_2_pin);

    // adc->startSynchronizedContinuous(channel_1_pin, channel_2_pin);
    timer = micros();
}

void loop()
{
    if (adc0_abdma.interrupted())
    {
        processBuffer(&adc0_abdma);
        Serial.println();
        Serial.print("Time elasped: ");
        Serial.print(micros() - timer);
        Serial.println(" us");

        // static int count = 100;
        // if (count > 5) while(1);
        // count++;
        delay(500);
        timer = micros();
        adc0_abdma.init(adc, ADC_0);
        adc->adc0->startContinuous(channel_1_pin);
    }
}

void processBuffer(AnalogBufferDMA *analogBuffer)
{
    volatile uint16_t *data = analogBuffer->bufferLastISRFilled();
    volatile uint16_t *end_data = data + analogBuffer->bufferCountLastISRFilled();
    uint16_t size = analogBuffer->bufferCountLastISRFilled();

    arm_dcache_delete((void *)data, sizeof(adc0_buffer1));

    for (unsigned int i = 0; i < size; i++)
    {
        Serial.println(data[i]);
    }

    analogBuffer->clearInterrupt();
}