boolean xboxButtonValue = false;
boolean xboxButtonPressed = false;

void debounce() {
	if (!buttons[8].pressed()) {
		xboxButtonPressed = false;
	}
	else if ((buttons[8].pressed())&&(!xboxButtonPressed)) {
		xboxButtonValue = !xboxButtonValue;
		xboxButtonPressed = true;
		delay(5); //Make sure it doesn't falsely read values
	}
	else {
		xboxButtonPressed = true;
	}
}