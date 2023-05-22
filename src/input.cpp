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
    //newPosition = knob.read();
    //int diff = abs(newPosition - oldPosition) / 4;
    /*
    newPosition = knob.read() / 4;
    int diff = abs(newPosition - oldPosition);
    
    if ((newPosition - oldPosition) < 0) 
    {
        diff = -1 * diff;
    }
    */
    newPosition = knob.read();
    int diff = newPosition - oldPosition;
    if (diff != 0) {
        if (display->mode == 1) 
        {
            display->vscale1 = knobRange(display->vscale1 + diff, 4, 80);
        }
        else if (display->mode == 2) 
        {
            display->vscale2 = knobRange(display->vscale2 + diff, 4, 80);
        }
        else if (display->mode == 3) 
        {
            display->hscale = knobRange(display->hscale + diff, 4, 600);
        }
        else if (display->mode == 4) 
        {
            display->runStop = knobRange(display->runStop + diff, 0, 4);
        }
        else if (display->mode == 5) 
        {
            display->triggerChannel = knobRange(display->triggerChannel + diff, 0, 4);
        }
        else if (display->mode == 6) 
        {
            display->trigger = knobRange(display->trigger + diff, 4, 200);
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

