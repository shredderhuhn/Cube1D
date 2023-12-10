#include <controller.h>
#include <constants.h>

Status status;
Ctrl ctrl;

void calcController(int measpoint) {
  
  ctrl.error = -(ctrl.setpoint - measpoint);
  ctrl.errorTildeKM1 = ctrl.errorTildeK;
  ctrl.errorTildeK = (K0Z * ctrl.error) >> 10;
  ctrl.u1K = (ctrl.errorTildeK * K3Z) >> 0;
  ctrl.u2KM1 = ctrl.u2K;
  ctrl.u2K = ((ctrl.errorTildeKM1 * K1Z) >> 8) + ((ctrl.u2KM1 * K2Z) >> 8);
  ctrl.u = (ctrl.u1K + ctrl.u2K);
  //ctrl.dac0 = UDAC0;
  //ctrl.dac1 = constrain(ctrl.u + ctrl.offset + UDAC0, 0, 4095);

}

void resetController(void) {
    ctrl.setpoint = status.setpoint;
    ctrl.offset = status.offset;
    ctrl.error = 0;
    ctrl.errorTildeK = 0;
    ctrl.errorTildeKM1 = 0;
    ctrl.u1K = 0;
    ctrl.u2K = 0;
    ctrl.u2KM1 = 0;
    ctrl.u = 0;
    ctrl.v = 0;
    ctrl.tick = 1 << 31;
    ctrl.dir = false;
}

void initController(void) {
    ctrl.setpoint = status.setpoint;
    ctrl.offset = status.offset;
    ctrl.error = 0;
    ctrl.errorTildeK = 0;
    ctrl.errorTildeKM1 = 0;
    ctrl.u1K = 0;
    ctrl.u2K = 0;
    ctrl.u2KM1 = 0;
    ctrl.u = 0;
    ctrl.v = 0;
    ctrl.tick = 1 << 31;
    ctrl.dir = false;
}

void calcTick() {
  // Limit acc
  ctrl.u = constrain(ctrl.u,-MAXABSMOTORACC,MAXABSMOTORACC);
  // Calc and limit vel
  ctrl.v = ctrl.v + ((ctrl.u * TAZZ) >> 10);
  ctrl.v = constrain(ctrl.v,-MAXABSMOTORVEL,MAXABSMOTORVEL);
  // Calc ticks
  int temp = ctrl.v >> 3;
  ctrl.dir = temp > 0; 
  temp = abs(temp);
  if (!temp) temp++; // wenn 0 dann 1, um Div /0 zu vermeiden
  ctrl.tick = KSTEPZZ / temp;
}


void setOutputValues() {
  //analogWrite(DAC0,ctrl.dac0);
  //analogWrite(DAC1,ctrl.dac1);
}

void resetOutputValues() {
  //analogWrite(DAC1,0);
  //analogWrite(DAC0,0);
}

void setCurrent() {
  //ctrl.offset = status.offset;
  //ctrl.dac0 = UDAC0;
  //ctrl.dac1 = UDAC0 + ctrl.offset;
  //setOutputValues();
}

void resetCurrent() {
  //ctrl.offset = 0;
  //ctrl.dac1 = 0;
  //ctrl.dac0 = 0;
  //resetOutputValues();
}

void resetState(Status &status) {
  //status.offset = INITIALCURRENTOFFSET;
  status.setpoint = INITIALSETPOINT;
  status.state = 0;
  status.failure = 0;
}
