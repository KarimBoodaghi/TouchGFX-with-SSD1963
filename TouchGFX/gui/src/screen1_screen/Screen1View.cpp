#include <gui/screen1_screen/Screen1View.hpp>
extern uint16_t Test_Counter;
Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent()
{
	Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", Test_Counter);
	textArea1.invalidate();
}
