#ifndef __INIT_RESET_BUTTON__
#define __INIT_RESET_BUTTON__

#define BTN_PIN 35u

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
bool buttonClicked = false;

void IRAM_ATTR handleIntr()
{
  portENTER_CRITICAL_ISR(&mux);
  buttonClicked = true;
  portEXIT_CRITICAL_ISR(&mux);  
}

void initButton() {
  pinMode(BTN_PIN, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), handleIntr, HIGH);
}

#endif
