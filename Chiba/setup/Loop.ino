void loop() {

  digitalOutBinary(0);
  val1 = analogRead(A1);
  digitalOutBinary(3);
  val2 = analogRead(A1);
  float Rval = ((val1 + val2) / 2 ) * 1.5;
  float Rrc = 0;
  Rrc = a * Rrc + (1 - a) * (float)Rval;
  Serial.print('R');
  Serial.println(Rrc);

  digitalOutBinary(1);
  val1 = analogRead(A1);
  digitalOutBinary(4);
  val2 = analogRead(A1);
  float Gval = ( (val1 + val2) / 2) * 1.1;
  float Grc = 0;
  Grc = a * Grc + (1 - a) * (float)Gval;
  Serial.print('G');
  Serial.println(Grc);

  digitalOutBinary(2);
  val1 = analogRead(A1);
  digitalOutBinary(5);
  val2 = analogRead(A1);
  float Bval = ((val1 + val2) / 2);
  float Brc = 0;
  Brc = a * Brc + (1 - a) * (float)Bval;
  Serial.print('B');
  Serial.println(Brc);

  //delay(1000);
  
    if (Rrc > Grc) {
     if (Rrc > Brc) {
       Serial.println("RED");
       Stop();

       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(255, 0, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
         delay(500);
       }
       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(255, 0, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
         delay(500);
       }
       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(255, 0, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
       }
       delay(2000);
       forward();
       delay( 1000 );
       Stop();

     } else {
       Serial.println("WHITE");

       int i = 0;
       pixels.clear();
       while (i < 16) {
         pixels.setPixelColor(i, pixels.Color(0, 0, 0));
         pixels.show();
         delay(10);
         i++;

         digitalOutBinary(4);
         val = analogRead(A0);
         Serial.print("PSD");
         Serial.println(val);

         int PSDval = 0;
         if (PSDval < 300) {
           forward();
         } else {
           Stop();
           delay(1000);
           back();
           delay(300);
           Left() ;
           delay(800);
           Stop();
           delay(100);
           //back();
           delay(400);
           Stop();
           delay(100);
         }
       }
     }
    } else {
     if (Grc > Brc) {
       Serial.println("GREEN");
       Stop();
       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 255, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
         delay(500);
       }

       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 255, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
         delay(500);
       }
       {
         int i = 0;
         pixels.clear();
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 255, 0));
           pixels.show();
           delay(100);
           i++;
         }
         delay(1000);
       }
       {
         int i = 0;
         while (i < 16) {
           pixels.setPixelColor(i, pixels.Color(0, 0, 0));
           pixels.show();
           delay(10);
           i++;
         }
       }

       delay(2000);
       forward();
       delay( 1000 );
       Stop();


     } else {
       Serial.println("WHITE");
       int i = 0;
       pixels.clear();
       while (i < 16) {
         pixels.setPixelColor(i, pixels.Color(0, 0, 0));
         pixels.show();
         delay(10);
         i++;

         int PSDval = 0;
         digitalOutBinary(4);
         PSDval = analogRead(A0);
         Serial.print("PSD");
         Serial.println(val);


         if (PSDval < 300) {
           forward();
         } else {
           Stop();
           delay(1000);
           back();
           delay(300);
           Left() ;
           delay(800);
           Stop();
           delay(100);
           //back();
           delay(400);
           Stop();
           delay(100);
         }
       }
     }
    }
  
}
