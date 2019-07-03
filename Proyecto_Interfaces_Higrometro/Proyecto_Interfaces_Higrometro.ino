
int value;
String n_riego;
void setup() {
   Serial.begin(115200);

}

void loop() {

   
   value= analogRead(A0);
   Serial.println(value);

   if(value<637){
    n_riego = String("Muy Alto");
    Serial.print("Nivel de Riego: ");
    Serial.println(n_riego);
   }
   if(value>=637 && value<734){
    n_riego = String("Alto");
    Serial.print("Nivel de Riego: ");
    Serial.println(n_riego);
   }
   if(value>=734 && value<832){
    n_riego = String("Bueno");
    Serial.print("Nivel de Riego: ");
    Serial.println(n_riego);
   }
    if(value>=832 && value<930){
    n_riego = String("Bajo");
    Serial.print("Nivel de Riego: ");
    Serial.println(n_riego);
   }
      if(value>=930 && value<=1024){
    n_riego = String("Muy Bajo");
    Serial.print("Nivel de Riego: ");
    Serial.println(n_riego);
   }

       delay(1000);

}
