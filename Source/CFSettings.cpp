/*
  ==============================================================================

    CFSettings.cpp
    Created: 12 Jun 2018 10:19:04pm
    Author:  Ben

  ==============================================================================
*/

#include "CFSettings.h"
#include "CFSettingsUI.h"

juce_ImplementSingleton(CFSettings)

CFSettings::CFSettings() :
	ControllableContainer("Crazyflie settings"),
	flightCC("Flight"),
	miscCC("Miscellaneous"),
	setupCC("Setup")
{
	saveAndLoadRecursiveData = true;

	addChildControllableContainer(&setupCC);
	autoConnect = setupCC.addBoolParameter("Auto connect", "If checked, detected drones will be automatically connected", false);
	analyzeAfterConnect = setupCC.addBoolParameter("Analyze after connect", "If checked, a health check will be performed after each connection", false);
	calibAfterConnect = setupCC.addBoolParameter("Calibrate after connect", "If checked and 'Analyze after connect' is unchecked, a calibration be performed after each connection", true);
	calibAfterAnalyze = setupCC.addBoolParameter("Calibrate after Analysis", "If checked, a calibration will be trigger after each analysis", true);
	
	addChildControllableContainer(&flightCC);
	takeOffTime = flightCC.addFloatParameter("Takeoff Time", "Time to take off", 3, 0, 10);
	takeOffMaxSpeed = flightCC.addFloatParameter("Takeoff Max Speed", "The vertical speed in m/s corresponding to the max curve value",1,0,6);
	addChildControllableContainer(&takeOffCurve);
	takeOffCurve.showUIInEditor = true;
	takeOffCurve.addItem(0, 1);
	takeOffCurve.addItem(1, 0);
	takeOffCurve.items[0]->setEasing(Easing::BEZIER);
	takeOffCurve.enableSnap->setValue(false);
	
	flightCC.addChildControllableContainer(&physicsCC);
	
	addChildControllableContainer(&miscCC);
	zIsVertical = miscCC.addBoolParameter("Z is vertical", "If checked, will use z as vertical axis", false);
}

CFSettings::~CFSettings()
{
}

PhysicsCC::PhysicsCC() :
	EnablingControllableContainer("Physics"),
	testMotion("Test Motion")
{
	saveAndLoadRecursiveData = true;

	mode = addEnumParameter("Mode", "Control Mode");
	mode->addOption("Direct", DIRECT)->addOption("Spring", SPRING)->addOption("Jerk", JERK);

	forceFactor = addFloatParameter("Flight Speed Factor", "Aggressiveness of the flight", 10,0,1000, false);
	frotFactor = addFloatParameter("Frottements", "", 5,1,50, false);
	
	maxSpeed = addFloatParameter("Max Speed", "Max speed in m/s", 1, 0.001f, 200, false);
	maxAcceleration = addFloatParameter("Max Acceleration", "Max speed in m/s/s", 1, 0.001f, 1000, false);
	maxJerk = addFloatParameter("Max Jerk", "Max speed in m/s/s/s", 1, 0.001f, 1000, false);
	jerkFactor = addFloatParameter("Jerk Factor", "multiply dist to point by that in jerk", 1, 1, 1000, false);

	simTime = addFloatParameter("Sim Time", "Time of the simulated curve in seconds", 5, 1, 20);

	addChildControllableContainer(&testMotion);
	testMotion.showUIInEditor = true;
	testMotion.addItem(0, 0); 
	testMotion.addItem(.2f, 1);
	testMotion.items[0]->setEasing(Easing::HOLD);
	testMotion.enableSnap->setValue(false);
}

PhysicsCC::~PhysicsCC()
{
}

void PhysicsCC::onContainerParameterChanged(Parameter * p)
{
	if (p == mode)
	{
		ControlMode cm = mode->getValueDataAsEnum<ControlMode>();
		forceFactor->setEnabled(cm == SPRING);
		frotFactor->setEnabled(cm == SPRING);

		maxSpeed->setEnabled(cm == JERK);
		maxAcceleration->setEnabled(cm == JERK);
		maxJerk->setEnabled(cm == JERK);
		jerkFactor->setEnabled(cm == JERK);
	}
}

InspectableEditor * PhysicsCC::getEditor(bool isRoot)
{
	return new PhysicsCCUI(this, isRoot);
}
