import Controller.dzn;
import LED.dzn;
import PWManager.dzn;
import Siren.dzn;
import Sensor.dzn;
import Timer.dzn;

component AlarmSystem {
	provides IController iController;
	requires ITimer iTimer;
	
	system {
		Controller controller;
		LED led;
		Siren siren;
		PWManager pwManager;
		Sensor sensor;
		RobustTimer rbTimer;
		
		iController <=> controller.iController;
		controller.iLed <=> led.iLed;
		controller.iPWManager <=> pwManager.iPWManager;
		controller.iTimer <=> rbTimer.iTimer;
		controller.iSiren <=> siren.iSiren;
		controller.iSensor <=> sensor.iSensor;
		rbTimer.ext_iTimer <=> iTimer;
	}
}
