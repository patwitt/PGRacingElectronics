#include <gui/screen1_screen/Screen1View.hpp>
#include <stdio.h>
#include <cstring>
#include <ecumaster.h>
#include <telemetry_data.h>

Screen1View::Screen1View()
{
	rpmArea.setWildcard(rpmAreaBuffer);
	gearArea.setWildcard(gearAreaBuffer);
	cltArea.setWildcard(cltAreaBuffer);
	battArea.setWildcard(battAreaBuffer);
	oilPressArea.setWildcard(oilPressAreaBuffer);

	setRPM(EcuData.rpm);
	setGear(telemetryData.gear);
	setClt(EcuData.clt);
	setBatt(EcuData.batt);
	setOilPress(EcuData.oilPress);
	alertBar1.setState(EBarState::Blank);
	alertBar2.setState(EBarState::Blank);
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
	//// Screen1View::tempBox1_1.redraw();
	//tempBox1_1.invalidate();
	static int a = 0;

	static uint16_t lastRPM = 0;
	if (EcuData.rpm != lastRPM)
	{
		setRPM(EcuData.rpm);
		lastRPM = EcuData.rpm;
		// if(EcuData.rpm <12000)
		gearArea.invalidate();
		//setGear(2);
	}
	static uint8_t lastGear = 0;
	if (telemetryData.gear != lastGear)
	{
		setGear(telemetryData.gear);
		lastGear = telemetryData.gear;
	}
	static float lastBatt = 0.0f;
	if (lastBatt != EcuData.batt)
	{
		setBatt(EcuData.batt);
		lastBatt = EcuData.batt;
	}
	static int16_t lastClt = 0;
	if (lastClt != EcuData.clt)
	{
		setClt(EcuData.clt);
		lastClt = EcuData.clt;
	}
	static float lastOilPressure = 0;
	if (lastOilPressure != EcuData.oilPress)
	{
		setOilPress(EcuData.oilPress);
		lastOilPressure = EcuData.oilPress;
	}

	if (EcuData.oilPress < 1.5f && EcuData.rpm > 1000)
	{
		static int lastChange = 0;
		if (tickCounter - lastChange > 15)
		{
			alertBar1.setText("LOW OIL PRESS\n");
			if (a == 0)
			{
				alertBar1.setState(EBarState::Critical1);
			}
			else
			{
				alertBar1.setState(EBarState::Critical2);
			}
			alertBar1.invalidate();
			a = 1 - a;
			lastChange = tickCounter;
		}
	}
	else if (telemetryData.gear == 7)
	{
		alertBar1.setState(EBarState::Warning);
		alertBar1.setText("GEAR UNKNOWN");
	}
	else
	{
		alertBar1.setState(EBarState::Blank);
	}

	//setClt(EcuData.clt);
	//setBatt(EcuData.batt);
	//alertBar1.invalidate();
	//alertBar1.setText("12345678912345\n");
	tickCounter++;
}

void Screen1View::setText(touchgfx::TextAreaWithOneWildcard textArea,
		touchgfx::Unicode::UnicodeChar *buffer, const char inText[])
{
	Unicode::snprintf(buffer, strlen(inText) + 1, inText);
	//textArea.resizeToCurrentText();
	textArea.invalidate();
}

void Screen1View::setRPM(int inRPM)
{
	char textBuffer[10];
	sprintf(textBuffer, "%d", inRPM);
	setText(rpmArea, rpmAreaBuffer, textBuffer);
}

void Screen1View::setBatt(float inBatt)
{
	char textBuffer[10];
	sprintf(textBuffer, "%.2f", inBatt);
	setText(battArea, battAreaBuffer, textBuffer);
}

void Screen1View::setOilPress(float inOilPress)
{
	char textBuffer[10];
	sprintf(textBuffer, "%.2f", inOilPress);
	setText(battArea, battAreaBuffer, textBuffer);
}

void Screen1View::setClt(int16_t inClt)
{
	char textBuffer[10];
	sprintf(textBuffer, "%i", inClt);
	setText(cltArea, cltAreaBuffer, textBuffer);
}

void Screen1View::setGear(int inGear)
{
	char textBuffer[10];
	if (inGear == 0)
	{
		sprintf(textBuffer, "%s", "N");
	}
	else if (inGear < 7)
	{
		sprintf(textBuffer, "%d", inGear);
	}
	setText(gearArea, gearAreaBuffer, textBuffer);
}

