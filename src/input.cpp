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

void KnobIn::rotate(Display *display, Encoder knob)
{
    newPosition = knob.read() / 4;
    int diff = abs(newPosition - oldPosition);

    if ((newPosition - oldPosition) < 0)
    {
        diff = -1 * diff;
    }
    if (diff != 0)
    {
        if (display->mode == 1)
        {
            display->vscale1 = knobRange(display->vscale1 + diff, 0, 10);
        }
        else if (display->mode == 2)
        {
            display->vscale2 = knobRange(display->vscale2 + diff, 0, 10);
        }
        else if (display->mode == 3)
        {
            display->hscale = knobRange(display->hscale + diff, 0, 13);
        }
        else if (display->mode == 4)
        {
            display->runStop = knobRange(display->runStop + diff, 0, 1);
        }
        else if (display->mode == 5)
        {
            display->trigger = knobRange(display->trigger + diff, 0, 60);
        }
        else if (display->mode == 6)
        {
            display->triggerChannel = knobRange(display->triggerChannel + diff, 0, 1);
        }
        oldPosition = newPosition;
    }
}

void KnobIn::modeChange(Display *display)
{
    if (display->mode == 6)
    {
        display->mode = 1;
    }
    else
    {
        display->mode++;
    }
    delayMicroseconds(20);
}
