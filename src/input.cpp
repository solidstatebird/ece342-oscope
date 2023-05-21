#include <input.h>

int KnobIn::knobRange(int position, int min, int max) 
{
    if (position < min) 
    {
        position = min;
    }
    else if (position > max) 
    {
        position = max;
    }
    return position;
}

void KnobIn::rotate(Display * display, Encoder knob) 
{
    newPosition = knob.read();
    int diff = abs(newPosition - oldPosition) / 4;
    if ((newPosition - oldPosition) < 0) 
    {
        diff = -1 * diff;
    }

    if (diff != 0) {
        if (display->mode == 1) 
        {
            display->vscale1 = knobRange(display->vscale1 + diff, 1, 20);
        }
        else if (display->mode == 2) 
        {
            display->vscale2 = knobRange(display->vscale2 + diff, 1, 20);
        }
        else if (display->mode == 3) 
        {
            display->hscale = knobRange(display->hscale + diff, 1, 150);
        }
        else if (display->mode == 4) 
        {
            display->runStop = knobRange(display->runStop + diff, 0, 1);
        }
        else if (display->mode == 5) 
        {
            display->triggerChannel = knobRange(display->triggerChannel + diff, 0, 1);
        }
        else if (display->mode == 6) 
        {
            display->trigger = knobRange(display->trigger + diff, 1, 50);
        }
        oldPosition = newPosition;
    }
}

void KnobIn::modeChange(Display * display) 
{
    if (display->mode == 6) 
    {
        display->mode = 1;
    }
    else 
    {
        display->mode++;
    }
    delayMicroseconds(50);
}

