#include <gui/main_screen/MainView.hpp>
#include <stdio.h>
#include <cstring>
#include <ecumaster.h>
#include <telemetry_data.h>

MainView::MainView()
{
	rpmArea.setWildcard(rpmAreaBuffer);
	gearArea.setWildcard(gearAreaBuffer);
	cltArea.setWildcard(cltAreaBuffer);
	battArea.setWildcard(battAreaBuffer);
	oilPressArea.setWildcard(oilPressAreaBuffer);
	oilTempArea.setWildcard(oilTempAreaBuffer);

	setRPM(EcuData.rpm);
	setGear(telemetryData.gear);
	setClt(EcuData.clt);
	setBatt(EcuData.batt);
	setOilPress(EcuData.oilPress);
	setOilTemp(EcuData.oilTemp);
	alertBar1.setState(EBarState::Blank);
	alertBar2.setState(EBarState::Blank);
}

void MainView::setupScreen()
{
	MainViewBase::setupScreen();
}

void MainView::tearDownScreen()
{
	MainViewBase::tearDownScreen();
}

void MainView::handleTickEvent()
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
	static uint8_t lastOilPressure = 0;
	if (lastOilPressure != EcuData.oilPress)
	{
		if (EcuData.ain2 > 0.4f)
		{
			setOilPress(EcuData.oilPress);
		}
		else
		{
			setOilPress(-1.0f);
		}
		lastOilPressure = EcuData.oilPress;
	}

	static uint8_t lastOilTemperature = 0;
	if (lastOilTemperature != EcuData.oilTemp)
	{
		setOilTemp(EcuData.oilTemp);
		lastOilTemperature = EcuData.oilTemp;
	}

	if (EcuData.oilPress <= 1.5 && EcuData.rpm > 1000 && EcuData.ain2 > 0.4f)
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
	else if (telemetryData.steeringWheelAttached == 1)
	{
		alertBar1.setState(EBarState::Info);
		alertBar1.setText("STEERING WHEEL ATTACHED");
	}
	else
	{
		alertBar1.setState(EBarState::Blank);
	}

	if (telemetryData.isIntercomActive == 1)
	{
		alertBar2.setState(EBarState::Info);
		alertBar2.setText("INTERCOM ACTIVE");
	}

	//setClt(EcuData.clt);
	//setBatt(EcuData.batt);
	//alertBar1.invalidate();
	//alertBar1.setText("12345678912345\n");
	tickCounter++;
}

void MainView::setText(touchgfx::TextAreaWithOneWildcard textArea,
		touchgfx::Unicode::UnicodeChar *buffer, const char inText[])
{
	Unicode::snprintf(buffer, strlen(inText) + 1, inText);
	//textArea.resizeToCurrentText();
	textArea.invalidate();
}

void MainView::setRPM(int inRPM)
{
	char textBuffer[10];
	sprintf(textBuffer, "%d", inRPM);
	setText(rpmArea, rpmAreaBuffer, textBuffer);
}

void MainView::setBatt(float inBatt)
{
	char textBuffer[10];
	if (inBatt < 10.0f)
	{
		sprintf(textBuffer, " %.1f", inBatt);
	}
	else
	{
		sprintf(textBuffer, "%.1f", inBatt);
	}

	setText(battArea, battAreaBuffer, textBuffer);
}

void MainView::setOilPress(float inOilPress)
{
	char textBuffer[10];
	if (inOilPress == -1.0f)
	{
		sprintf(textBuffer, "%s", "---");
	}
	else
	{
		sprintf(textBuffer, "%.1f", inOilPress);
	}
	setText(oilPressArea, oilPressAreaBuffer, textBuffer);
}
void MainView::setOilTemp(uint8_t inOilTemp)
{
	char textBuffer[10];
	sprintf(textBuffer, "%i", inOilTemp);
	setText(oilTempArea, oilTempAreaBuffer, textBuffer);
}

void MainView::setClt(int16_t inClt)
{
	char textBuffer[10];
	sprintf(textBuffer, "%i", inClt);
	setText(cltArea, cltAreaBuffer, textBuffer);
}

void MainView::setGear(int inGear)
{
	char textBuffer[10];
	if (inGear == 0)
	{
		sprintf(textBuffer, "%s", "N");
		setText(gearArea, gearAreaBuffer, textBuffer);
	}
	else if (inGear < 7)
	{
		sprintf(textBuffer, "%d", inGear);
		setText(gearArea, gearAreaBuffer, textBuffer);
	}

}

