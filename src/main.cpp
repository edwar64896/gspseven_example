#include <Arduino.h>
#include <gspseven.h>
#include <gspstreamresponse.h>
#include <gspgrouped.h>
#include <gspswitch.h>
#include <gspquad.h>

void qdwn(int);
void qup(int);
void sw1(uint8_t);

char zstring[10];

gspSeven g1(7,6,9);
gspSeven g2(7,6,10);
gspSwitch q1sw(5,3,sw1,gspSwitch_MODE_PUSHBUTTON_RELEASE_COUNTER_AUTORESET_CB);
gspQuad q1(4,3,qdwn,qup);

volatile double z = 135.700 ;

void display() {

  dtostrf(z,3,3,zstring);
  g1.displayNums(String(zstring));


}

void sw1(uint8_t status) {
  g1.stopFlashing();
  switch(status) {
    case 1:
    g1.flashB();
    break;
    case 2:
    g1.flashA();
    break;

  }
}

void qdwn(int dif) {

  switch(q1sw.getCounterValue()) {
    case 1:
      z-=1.0f;
    break;
    case 2:
      z-=0.025f;
    break;
  }

  display();
  q1sw.counterAutoResetHoldoff();

}

void qup(int dif) {

  switch(q1sw.getCounterValue()) {
    case 1:
      z+=1.0f;
    break;
    case 2:
      z+=0.025f;
    break;
  }

  display();
  q1sw.counterAutoResetHoldoff(); 

}

void srA(String szParam) {
  z=szParam.toFloat();
  display();
}

void srB(String szParam) {
  //g2.displayNums(szParam);
}

void srC(String szParam) {
  Serial.println(szParam.charAt(0));
  switch(szParam.charAt(0)) {
    case '0':
      g1.flashA();
    break;
    case '1':
      g1.flashB();
    break;
    case '2':
      g2.flashA();
    break;
    case '3':
      g2.flashB();
    break;
  }
}

void srD(String szParam) {
  switch(szParam.charAt(0)) {
    case '0':
      g1.flashA(false);
    break;
    case '1':
      g1.flashB(false);
    break;
    case '2':
      g2.flashA(false);
    break;
    case '3':
      g2.flashB(false);
    break;
  }
}

void srE(String szParam) {
  g1.flash((uint8_t)szParam.toInt());
}

void srF(String szParam) {
  g1.flash((uint8_t)szParam.toInt(),false);
}

gspStreamResponse sr1("=A",7,srA);
gspStreamResponse sr2("=B",7,srB);
gspStreamResponse sr3("=C",1,srC);
gspStreamResponse sr4("=D",1,srD);
gspStreamResponse sr5("=E",1,srE);
gspStreamResponse sr6("=F",1,srF);

void setup() {

  Serial.begin(115200);
  // put your setup code here, to run once:

  gspGrouped::register_instance(&g1);
  gspGrouped::register_instance(&g2);
  gspGrouped::register_instance(&sr1);
  gspGrouped::register_instance(&sr2);
  gspGrouped::register_instance(&sr3);
  gspGrouped::register_instance(&sr4);
  gspGrouped::register_instance(&sr5);
  gspGrouped::register_instance(&sr6);
  gspGrouped::register_instance(&q1);
  gspGrouped::register_instance(&q1sw);

  gspQuad::startCheckAll();

  display();

}

void loop() {
  // put your main code here, to run repeatedly:
  gspSeven::checkAll();
  gspStreamResponse::checkAll();
  gspSwitch::checkAll();

  //Serial.println(String(zstring));

}