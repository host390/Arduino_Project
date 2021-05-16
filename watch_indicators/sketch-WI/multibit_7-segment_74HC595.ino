


void printByte(uint8_t byte_1, uint8_t byte_2, uint8_t byte_3, uint8_t byte_4, bool light_on) {

  if (blinking.isReady()) {
    blinkDischa_flag = !blinkDischa_flag;
  }
  
  static uint8_t outArr[4];

//  #define _C 0b11000011 // ??? не знаю как это сделать
//  #define _L 0b01000011
//  #define _o 0b00011011
//  #define _c 0b00010011
//  #define _A 0b11111001
//  #define _r 0b00010001
//  #define _S 0b11011010
//  #define _t 0b01010011
//  #define _P 0b11110001
//  #define _i 0b00000001
//  #define _e 0b11110011

  //  for (uint8_t i; i < 4; i++) {
  //    outArr[i] = byte_1
  //  }

  outArr[0] = byte_1;
  outArr[1] = byte_2;
  outArr[2] = byte_3;
  outArr[3] = byte_4;

  if (light_on) {
    getDigits(outArr, true, true, blinkDischa_flag); // blinkDischa_flag
  } else {
    getDigits(outArr, false, false, false); // blinkDischa_flag
  }

//  getDigits(outArr, true, true, blinkDischa_flag); // blinkDischa_flag

  //  for (int n = 0; n < 4; n++) {
  //    digitalWrite(RCLK, LOW);
  //    shiftOut(SER, SRCLK, MSBFIRST, outArr[n]);
  //    digitalWrite(RCLK, HIGH);
  //    digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
  //    delay(5); // Дали паузу глазу воспринять
  //    digitalWrite(anodPins[n], HIGH); // Сняли напряжение с анода, чтобы переключение катодов не вызывали мерцание
  //  }
}


void displayClock(int value, bool light1, int value2, bool light2) {


// String 35% 36%
//static String spaces = "     ";
//static byte digits = 4;

//static String intPart;
//static String intPart2;
//static String output;
//static String _char;

static bool oneShow_flag = true;

// Str 28% 32%
static char output[10] = "";
//static char outChar[3] = "";

static byte actualdigit;
static uint8_t outArr[4];
static uint8_t segBit[10] {
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
};

  
if (blinking.isReady()) {
  blinkDischa_flag = !blinkDischa_flag;
}

  if (oneShow_flag) {
//    oneShow_flag = false;
//    lastValue = value;
//    lastValue2 = value2;
    
//    intPart = value;
//    intPart2 = value2;
    
    if (value < 10) { // если первое число < 10 - ведущий 0
      //intPart = "0" + intPart;
      strcpy(output + 0, "0");
      itoa(value, output + strlen(output), DEC);
    } else {
      itoa(value, output + 0, DEC);
    }
    if (value2 < 10) { // если 2-ое число < 10 - ведущий 0
//      intPart2 = "0" + intPart2;
      strcpy(output + strlen(output), "0");
      itoa(value2, output + strlen(output), DEC);
    }  else {
      itoa(value2, output + strlen(output), DEC);
    }

//    output = intPart + intPart2;
    
//    Serial.print("output: ");
//    Serial.println(output);
//    Serial.print("Длина строки: ");
//    Serial.println(strlen(output));

///*  ФОРМИРОВАНИЕ
//    String spaces = "     ";
//    int digits = 4;
//    if (output.length() < digits) output = spaces.substring(0, digits - output.length()) + output;

    for (int g = 0; g < strlen(output); g++) { // string output.length()
//      _char = output.substring(g, g + 1);

        char outChar[3] = ""; // не знаю приведёт ли это к чему-то плохому в большом цыкле
        strncat(outChar, output + g, 1); // Добавляет num символов из начала str2 к концу str1
//      actualdigit = _char.toInt();
        actualdigit = atoi(outChar);

        outArr[g] = segBit[actualdigit];
      
//        Serial.print("actualdigit: ");
//        Serial.println(actualdigit);
      
//      actualdigit = 0
//      if (_char == " ") { //??? когда оно может быть равно пробелу???
////        digits--;
//      } 
//      else if (light1) {
//      }
//      else if (_char.toInt() == 9) { // для моргания пока 9
//        actualdigit = 11;
//       else {
//        actualdigit = _char.toInt();
//      }


//      outArr[g] = segBit[actualdigit];
    }
    
  ///* МИГАНИЕ 1 ТОЧКОЙ
//    bitWrite(outArr[1], 2, 1); // точка для часов на 2 сегменте
//    if (blinkDischa_flag) { // мигание отдельным сегментом (.)
//      bitWrite(outArr[1], 2, 0); // точка для часов на 2 сегменте
//    } else {
//      bitWrite(outArr[1], 2, 1); // точка для часов на 2 сегменте
//    }
//  */

      ///* ЗАЖЕЧЬ ВСЕ ТОЧКИ 
      if (brinDots.isReady()) {
        save_flag = false;
      }
      if (save_flag) { // если сохраняем то отобразить точки
       for (int i = 0; i < 4; i++) {
        bitWrite (outArr [i], 2, 1); // точка для часов на 2 сегменте
       }
     } else {
       for (int i = 0; i < 4; i++) {
          bitWrite (outArr [i], 2, 0); // точка для часов на 2 сегменте
       }
     }
//     */

//        if (digits  > 2) {
//          bitWrite (outArr [1], 2, 1); // точка для часов на 2 сегменте
//        }


//*/

  }

  getDigits(outArr, light1, light2, blinkDischa_flag);
}

/* ВЫВОД FLOAT
void printFloatNumb(float value, int cauntMeteo) {
  static uint8_t outArr[4];
  static uint8_t segBit[14] {
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
    0b00010000, // -
    0b11110000, // * градус
    0b01011001, // h - высота
  };

  static int lastValueInt = 9999; // старая целая часть
  static int lastValueFrac = 9999; // старая дробная часть

  //  static bool pausa = false; // пауза на кд после смены значений

  static int newValue;

  static long int timerPres;

  static int intLength; // длина целой части

  static String output; // выводная строка

  static int fracInt = 7;

  static int digits;

  double fff;
  //  if (millis() - timerPres >= 250) {
  //    timerPres = millis();
  //    Serial.print("Последняя цыфра: ");
  //    Serial.println(newValue);
  //  }
  //  newValue = value * 10;
  //  newValue = newValue % 10; // последняя цыфра // это всё ломает блин

  int digits = 4; // У нас 4 разряда
  if (value < 0) digits = digits - 1; // Из-за минуса один символ убирается
  if (cauntMeteo == 2 || cauntMeteo == 3) digits = digits - 1; // у нас градус

  int intPart = (int)abs(value); // целая часть == 0
  if (intPart == 0) {
    digits = digits + 1;
  }
  if (intPart < 10 && digits != 2) { // если меньше 10 и не -
    intLength = 2;
  } else {
    intLength = ((String)intPart).length(); // Смотрим, сколько разрядов занимает целая часть
  }
  //  intLength = ((String)intPart).length(); // Смотрим, сколько разрядов занимает целая часть


  char valueChars[8]; // По нормальному float в String преобразовать нельзя, поэтому...
  dtostrf(value, 5, 2, valueChars); //... копируем данные float в массив chars[] //4 // 1
  String valueStr = valueChars;

  int fracIndex = valueStr.indexOf(".");                                        // Получаем индекс начала дробной части
  String fracPartStr = valueStr.substring(fracIndex + 1, valueStr.length());    // Выделяем цифры дробной части
  int fracDigits = digits - intLength; // На дробную часть у нас остается разрядов: digits-intLength
  fracPartStr = fracPartStr.substring(0, fracDigits);  // Вычленяем цифры, которые будут выводиться на дисплей после запятой
  int fracInt = fracPartStr.toInt();       // Переменная для проверки, нужно ли что-то выводить, если нули, то нет

  //  if (millis() - timerPres >= 250) { //!
  //    timerPres = millis();
  //    Serial.print("Последняя цыфра ввода: "); // дробная часть
  //    Serial.println(fracInt); // дробная часть
  //  }
  * /


  int lastNembF = modff(value, fff);

  Serial.println(lastNembF);

  if ((lastValueInt != intPart) || (lastValueFrac != fracInt)) { // если старое значение не равно новому, значит обновить //

    lastValueInt = intPart;
    lastValueFrac = fracInt;

    //    Serial.print("Последняя цыфра ввода: "); // дробная часть
    //    Serial.println(fracInt); // дробная часть

    //    Serial.print("lastValueInt: ");
    //    Serial.println(lastValueInt);
    //    Serial.print("lastValueFrac: ");
    //    Serial.println(lastValueFrac);
    /*
      int digits = 4; // У нас 4 разряда
      if (value < 0) digits = digits - 1; // Из-за минуса один символ убирается
      if (cauntMeteo == 2) digits = digits - 1; // у нас градус

      //    Serial.print("digits: ");
      //    Serial.println(digits);

      // Делим число на 2 группы - отдельно целую часть и дробную.
      int intPart = (int)abs(value); // целая часть
      int intLength = ((String)intPart).length(); // Смотрим, сколько разрядов занимает целая часть

      //    Serial.print("intLength: ");
      //    Serial.println(intLength);

      char valueChars[8]; // По нормальному float в String преобразовать нельзя, поэтому...
      dtostrf(value, 5, 2, valueChars); //... копируем данные float в массив chars[] //4 // 1
      String valueStr = valueChars;

      //    Serial.print("valueStr: "); // дробная часть
      //    Serial.println(valueStr); // дробная часть

      int fracIndex = valueStr.indexOf(".");                                        // Получаем индекс начала дробной части
      String fracPartStr = valueStr.substring(fracIndex + 1, valueStr.length());    // Выделяем цифры дробной части
      int fracDigits = digits - intLength; // На дробную часть у нас остается разрядов: digits-intLength
      fracPartStr = fracPartStr.substring(0, fracDigits);  // Вычленяем цифры, которые будут выводиться на дисплей после запятой
      int fracInt = fracPartStr.toInt();       // Переменная для проверки, нужно ли что-то выводить, если нули, то нет
  

    //      Serial.print("Последняя цыфра ввода: "); // дробная часть
    //      Serial.println(fracInt); // дробная часть

    // Собираем строку для отображения
    //    String output = (value < 0) ? "-" : "";
    if (intPart == 0 && fracInt == 0) {
    } else {
      output = (value < 0) ? "-" : "";
    }

    output += (String)intPart; // целая часть
    //  Serial.println(output);
    String outputFrac;
    if ((digits - intLength <= 0) || (fracInt == 0)) {
      if (digits == 2 && intLength == 2) { // если у нас минус и градус и 2 целых числа
        outputFrac = ""; // дробная часть убирается
      } else {
        outputFrac = ".0";
      }
    } else {
      outputFrac = (String)"." + fracPartStr;
    }
    //    String outputFrac = ((digits - intLength <= 0) || (fracInt == 0)) ? ".0" : ((String)"." + fracPartStr); // добавляем чать после запятой

    output += (String)outputFrac;
    //  Serial.println(output);
    if (cauntMeteo == 2) output += "*";
    if (cauntMeteo == 3) output += "h";

    // Дополняем символы спереди, если цифр слишком мало, например для "-1" делаем "  -1"
    String spaces = "     ";
    digits = 4;
    if (~output.indexOf(".")) digits += 1;
    if (output.length() < digits) output = spaces.substring(0, digits - output.length()) + output;

    //    Serial.print("output: ");
    //    Serial.println(output);

    /*
      // Формирум данные для показа:
      int dig = -1;
      for (int i = 0; i < output.length(); i++) {
      String _char = output.substring(i, i + 1);

      if (_char != ".") dig += 1; // Точка не занимает сегмент - увеличиваем на 1

      int actualdigit = 10; // По умолчанию пустой символ
      if ((_char == "-")) {
      actualdigit = 11;
      } else if (_char == "*") {
      actualdigit = 12;
      } else if (_char == "h") {
      actualdigit = 13;
      } else if (_char == ".") {
      }
      else if (_char == " ") {
      actualdigit = 0;
      }
      else {
      actualdigit = _char.toInt();
      }

      if (_char == ".") {
      //      arr[dig][7] = 1; // Если нужно - ставим точку
      bitWrite (outArr[dig], 2, 1); // Если нужно - ставим точку
      }
      else  {
      outArr[dig] = segBit[actualdigit];
      //      for (int n = 0; n <= 7; n++) {
      //        arr[dig][n] = seg[actualdigit][n];
      //      }
      }
      }


      }

      getDigits(outArr, false, false, false);

      }

      void printNumb(int value, bool light1, int value2, bool light2) {
      //  static bool blinkDischa_flag = true;
      static uint8_t outArr[4];
      static uint8_t segBit[11] {
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
      /*static int err[4] {
      //  0b00000000,
      //  0b11010011, // E
      //  0b00010001, // r
      //  0b00010001, // r
      };


    if (blinking.isReady()) {
      blinkDischa_flag = !blinkDischa_flag;
    }

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

    if (brinDots.isReady()) {
      save_flag = false;
    }

    if (save_flag) { // если сохраняем то отобразить точки
      bitWrite (outArr [0], 2, 1); // точка для часов на 2 сегменте
      bitWrite (outArr [1], 2, 1); // точка для часов на 2 сегменте
      bitWrite (outArr [2], 2, 1); // точка для часов на 2 сегменте
      bitWrite (outArr [3], 2, 1); // точка для часов на 2 сегменте
    } else {
      bitWrite (outArr [0], 2, 0); // точка для часов на 2 сегменте
      bitWrite (outArr [1], 2, 0); // точка для часов на 2 сегменте
      bitWrite (outArr [2], 2, 0); // точка для часов на 2 сегменте
      bitWrite (outArr [3], 2, 0); // точка для часов на 2 сегменте
    }
    getDigits(outArr , light1, light2, blinkDischa_flag);
    /*for (int n = 0; n < 4; n++) {
      digitalWrite(RCLK, LOW);
      shiftOut(SER, SRCLK, MSBFIRST, outArr[n]);
      digitalWrite(RCLK, HIGH);


      if ((light1 && blinkDischa_flag) && (light2 && blinkDischa_flag)) { // моргание индикаторов
        digitalWrite(anodPins[n], HIGH); // Подали напряжение на анод - все индикаторы загорелись
      } else if ((light1 && blinkDischa_flag) || (light2 && blinkDischa_flag)) {
        if (light1) {
          if (n == 0 || n == 1) {
            digitalWrite(anodPins[0], HIGH); // Подали напряжение на анод - все индикаторы загорелись
            digitalWrite(anodPins[1], HIGH); // Подали напряжение на анод - все индикаторы загорелись
          } else {
            digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
          }
        }
        if (light2) {
          if (n == 2 || n == 3) {
            digitalWrite(anodPins[2], HIGH); // Подали напряжение на анод - все индикаторы загорелись
            digitalWrite(anodPins[3], HIGH); // Подали напряжение на анод - все индикаторы загорелись
          } else {
            digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
          }
        }
      } else {
        digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
      }
      //    digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
      delay(5); // Дали паузу глазу воспринять
      digitalWrite(anodPins[n], HIGH); // Сняли напряжение с анода, чтобы переключение катодов не вызывали мерцание
      }
    
  }
}

*/


void getDigits (uint8_t outArr[4], bool lt1, bool lt2, bool blin) { // функция вывода
  for (int n = 0; n < 4; n++) {
    digitalWrite(RCLK, LOW);
    shiftOut(SER, SRCLK, MSBFIRST, outArr[n]);
    digitalWrite(RCLK, HIGH);

    // МИГАНИЕ
    if ((lt1 && blin) && (lt2 && blin)) { // моргание индикаторов
      digitalWrite(anodPins[n], HIGH); // Подали напряжение на анод - все индикаторы загорелись
    } else if ((lt1 && blin) || (lt2 && blin)) {
      if (lt1) {
        if (n == 0 || n == 1) {
          digitalWrite(anodPins[0], HIGH); // Подали напряжение на анод - все индикаторы загорелись
          digitalWrite(anodPins[1], HIGH); // Подали напряжение на анод - все индикаторы загорелись
        } else {
          digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
        }
      }
      if (lt2) {
        if (n == 2 || n == 3) {
          digitalWrite(anodPins[2], HIGH); // Подали напряжение на анод - все индикаторы загорелись
          digitalWrite(anodPins[3], HIGH); // Подали напряжение на анод - все индикаторы загорелись
        } else {
          digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
        }
      }
    } else {
      digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
    }
    //    digitalWrite(anodPins[n], LOW); // Подали напряжение на анод - все индикаторы загорелись
    
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


          if (brinDots.isReady()) {
            save_flag = false;
          }

          if (save_flag) { // если сохраняем то отобразить точки
           bitWrite (outArr [0], 2, 1); // точка для часов на 2 сегменте
          bitWrite (outArr [1], 2, 1); // точка для часов на 2 сегменте
           bitWrite (outArr [2], 2, 1); // точка для часов на 2 сегменте
           bitWrite (outArr [3], 2, 1); // точка для часов на 2 сегменте
         } else {
           bitWrite (outArr [0], 2, 0); // точка для часов на 2 сегменте
           bitWrite (outArr [1], 2, 0); // точка для часов на 2 сегменте
           bitWrite (outArr [2], 2, 0); // точка для часов на 2 сегменте
           bitWrite (outArr [3], 2, 0); // точка для часов на 2 сегменте
         }
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
