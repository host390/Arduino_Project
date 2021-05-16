





void printNumb(int value, bool light1, int value2, bool light2) {
  static int outArr[4];
  static int segBit[11] { 
    0b11101011, // 0
    0b00101000, // 1
    0b10110011, // 2
    0b10111010, // 3
    0b01111000, // 4
    0b11011010, // 5
    0b11011011, // 6
    0b10101000, // 7
    0b11111011, // 8
    0b11111010, // 9
    0b00000000, // пусто
  };

  
  int intPart = value;
  int intPart2 = value2; // нужны все числа

  String intPar;
  String intPar2;
  if (intPart < 10) {
    intPar = (String)"0" + intPart;
  }  else {
    intPar = (String)intPart;
  }
  if (intPart2 < 10) {
    intPar2 = (String) "0" + intPart2;
  }  else {
    intPar2 = (String)intPart2;
  }

  String output = (String)intPar + intPar2;
  
//  Serial.println(output);
//  Serial.println(output.length());

//  String spaces = "     "; // а надо ли???
//  int digits = 4;
//  if (output.length() < digits) output = spaces.substring(0, digits - output.length()) + output;

  for (int g = 0; g < output.length(); g++) {
    String _char = output.substring(g, g + 1);
    int actualdigit = 0;
    actualdigit = _char.toInt();
    outArr[g] = segBit[actualdigit];
    
//    if (_char == " ") {
//      Serial.println("Пусто");
//    } 
//    else {
//      actualdigit = _char.toInt();
//    }
//    outArr[g] = segBit[actualdigit];
  }
  
  for (int n = 0; n < 4; n++) {
    digitalWrite(RCLK, LOW);
    shiftOut(SER, SRCLK, MSBFIRST, outArr[n]);
    digitalWrite(RCLK, HIGH);
    
    digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
    delay(5); // Дали паузу глазу воспринять
    digitalWrite(anodPins[n], HIGH); // Сняли напряжение с анода, чтобы переключение катодов не вызывали мерцание
  }
  
  
}


/*
//void printNumb(int value,bool light1, int value2, bool light2) {

void printNumb(int value, bool light1, int value2, bool light2) {
  static bool oneShow_flag = true;
  static bool blinkDischa_flag = false;
  
//  static int outArr[4];
  
  static bool segBit[11] {
    0b11101011, // 0
    0b00101000, // 1
    0b10110011, // 2
    0b10111010, // 3
    0b01111000, // 4
    0b11011010, // 5
    0b11011011, // 6
    0b10101000, // 7
    0b11111011, // 8
    0b11111010, // 9
    0b00000000, // пусто
  };
  
//if (blinking.isReady()) {
//  blinkDischa_flag = !blinkDischa_flag;
//}
  
    
//  if (lastValue != value || lastValue2 != value2) {
//    oneShow_flag = true;
//  }
  if (oneShow_flag) {
//    oneShow_flag = false;
//    lastValue = value;
//    lastValue2 = value2;
    
    //    if ((value > 99 || value < -99) ) { // может быть множество вариантов ошибки, которые решаются или и или и ...
    //      for (int e = 0; e < 4; e++) {
    //        outArr[e] = err[e];
    //      }
    //      return; // output in loop
    //    }

//    int intPart = (int)abs(value); // 1
//    int intPart2 = (int)abs(value2); // нужны все числа
    int intPart = value; // 2
    int intPart2 = value2; // нужны все числа

//    String intPart = value;
//    String intPart2 = value2;
//    String output = (String)value + value2;
    String intPar;
    String intPar2;
    if (intPart < 10) {
     intPar = (String)"0" + intPart;
    } else {
     intPar = (String)intPart;
    }
    if (intPart2 < 10) {
     intPar2 = (String) "0" + intPart2;
    } else {
     intPar2 = (String)intPart2;
    }
    
    String output = (String)intPar + intPar2;

//    Serial.println(output);
    
    String spaces = "     ";
    int digits = 4;
    if (output.length() < digits) output = spaces.substring(0, digits - output.length()) + output;

    for (int g = 0; g < output.length(); g++) {
      String _char = output.substring(g, g + 1);
      int actualdigit = 0;
      if (_char == " ") {
//        digits--;
      } 
//      else if (light1) {
//        
//      }
//      else if (_char.toInt() == 9) { // для моргания пока 9
//        actualdigit = 11;
       else {
        actualdigit = _char.toInt();
      }
      outArr[g] = segBit[actualdigit];
//      curr_outArr[g] = segBit[actualdigit];
    }

//    if (light1) { // хочу мигать
//      if (blinkDischa_flag) {
//        outArr[0] = curr_outArr[0];
//        outArr[1] = curr_outArr[1];
//      } else {
//        outArr[0] = segBit[11];
//        outArr[1] = segBit[11];
//      }
//    }
//
//    if (light2) { // хочу мигать
//      if (blinkDischa_flag) {
//        outArr[2] = curr_outArr[2];
//        outArr[3] = curr_outArr[3];
//      } else {
//        outArr[2] = segBit[11];
//        outArr[3] = segBit[11];
//      }
//    }

//    bitWrite(outArr[1], 2, 1); // точка для часов на 2 сегменте
//    if (blinkDischa_flag) { // мигание отдельным сегментом (.)
//      bitWrite(outArr[1], 2, 0); // точка для часов на 2 сегменте
//    } else {
//      bitWrite(outArr[1], 2, 1); // точка для часов на 2 сегменте
//    }

      
//      if (brinDots.isReady()) {
//        save_flag = false;
//      }
      
//      if (save_flag) { // если сохраняем то отобразить точки
//       bitWrite (outArr [0], 2, 1); // точка для часов на 2 сегменте
//       bitWrite (outArr [1], 2, 1); // точка для часов на 2 сегменте
//       bitWrite (outArr [2], 2, 1); // точка для часов на 2 сегменте
//       bitWrite (outArr [3], 2, 1); // точка для часов на 2 сегменте
//     } else {
//       bitWrite (outArr [0], 2, 0); // точка для часов на 2 сегменте
//       bitWrite (outArr [1], 2, 0); // точка для часов на 2 сегменте
//       bitWrite (outArr [2], 2, 0); // точка для часов на 2 сегменте
//       bitWrite (outArr [3], 2, 0); // точка для часов на 2 сегменте
//     }
//       
////        if (digits  > 2) {
////          bitWrite (outArr [1], 2, 1); // точка для часов на 2 сегменте
//        }
  }
  getDigits(light1, light2, blinkDischa_flag);
}



void getDigits (bool lt1, bool lt2, bool blin) { // функция вывода

//  if (blinking.isReady()) {
//    blinkDischa_flag = !blinkDischa_flag;
//  }
  
  for (int n = 0; n < 4; n++) {
    digitalWrite(RCLK, LOW);
    shiftOut(SER, SRCLK, MSBFIRST, outArr[n]);
    digitalWrite(RCLK, HIGH);

//    if ((lt1 && blin) && (lt2 && blin)) {
//      digitalWrite(anodPins[n], HIGH); // Подали напряжение на анод - все индикаторы загорелись
//    } else if ((lt1 && blin) || (lt2 && blin)) {
//      if (lt1) {
//        if (n == 0 || n == 1) {
//        digitalWrite(anodPins[0], HIGH); // Подали напряжение на анод - все индикаторы загорелись
//        digitalWrite(anodPins[1], HIGH); // Подали напряжение на анод - все индикаторы загорелись
//        } else{
//          digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
//        }
//      }
//      if (lt2) {
//        if (n == 2 || n == 3) {
//        digitalWrite(anodPins[2], HIGH); // Подали напряжение на анод - все индикаторы загорелись
//        digitalWrite(anodPins[3], HIGH); // Подали напряжение на анод - все индикаторы загорелись
//        } else{
//          digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
//        }
//      }
//    } else {
//      digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
//    }
    
//    if (lt2 && blin) {
//      if (n == 2 || n == 3) {
//        digitalWrite(anodPins[2], HIGH); // Подали напряжение на анод - все индикаторы загорелись
//        digitalWrite(anodPins[3], HIGH); // Подали напряжение на анод - все индикаторы загорелись
//      } else{
//        digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
//      }
//    }
    
    digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
    delay(5); // Дали паузу глазу воспринять ????
    digitalWrite(anodPins[n], HIGH); // Сняли напряжение с анода, чтобы переключение катодов не вызывали мерцание
  }
}

*/
