

#include <GyverBME280.h> // Подключение библиотеки
GyverBME280 bmp;     

// Можно ли использовать 1 таймер для 2 разныx функций ????

//#include <microWire.h> // не работает :(
//#include <buildTime.h> // дата и время компиляции уже подключена в microDS3231.h
#include "microDS3231.h"
#include <GyverButton.h>
#include <GyverTimer.h>

MicroDS3231 rtc;
GButton butLeft(2, HIGH_PULL);
GButton butOK(3, HIGH_PULL);
GButton butRight(4, HIGH_PULL);

GTimer secondTim(MS, 1000); // а может можно 1???
GTimer alarmTimer(MS, 1000);
GTimer blinking(MS, 500); // мигание сегментов
GTimer holdTimer (MS, 100); // для удержания
GTimer getClockTimer (MS, 60000UL); // опрос времени с датчика каждую минуту // секунду норм будет????

GTimer brinDots (MS);
GTimer goAlarmTimer(MS); // таймер будильника
GTimer blinkSegOut(MS); // время перестования маргания сегментами

//GTimer alarmChackTimer (MS, 60000); // ??? не помню что это

#define SER 10
#define RCLK 11
#define SRCLK 12
#define _ 0b00000000
#define _C 0b11000011
#define _L 0b01000011
#define _o 0b00011011
#define _c 0b00010011
#define _A 0b11111001
#define _r 0b00010001
#define _S 0b11011010
#define _t 0b01010011
#define _P 0b11110001
#define _i 0b00000001
#define _e 0b11110011
#define _E 0b11010011

#define BUZZER_PIN  5       // Пин для баззера (если нужен)

int anodPins[4] = {A0, A1, A2, A3};

//bool oneShow_flag = true; // ?? нужен в основе???

//bool blinkDischa_flag;

bool changeRank;
bool setTime_flag; // настройки времени

bool stopRankSele_flag; // разрешение на выбор разряда
bool rankSelected_flag; // разрешение на смену сегмента

bool blinkHouSeg;
bool blinkMinutSeg;

int hou = 0; // время отображения
int minut = 0;
int secon = 0;

int curr_hou = 0; // время текущее
int curr_minut = 0;
int curr_secon = 0;

int last_hou; // заморозка времени
int last_minut;

// test
int menuItems = 2; // количество менфемых функций

int caunterMenu = 0; // для перелистывания меню индикатора

bool save_flag; // для загарания точек при сохранении

bool blinkDischa_flag = true; // маргание индикаторов

bool clickOK_flag = false; // для нескольких нажатий ОК

// alarm
int alarmArr[10][2] { // массив для будильников
  {5, 0},
  {10, 40},
  {19, 30},
};

int alarmCaunter;

bool alarmSetNew_flag; // флаг для перелистывания установленных будильников

int iAlarm = 3; // количество будильников не вычитая 0

bool alarmChack; // проверяем будильник каждую минуту
bool goAlarm_falg; // для срабатывания будильника

// stopwatch
int8_t hours_idle = 0;   // Часы секундомера
int8_t minutes_idle = 0; // Минуты секундомера

bool startWatch = false; // для остановки секундомера
bool stopWatchCau = false; // для отсёта секунд из stapTime

// timer
int8_t hours_timer = 0;   // Часы на обратный отсчет
int8_t minutes_timer = 0; // Минуты на обратный отсчет


bool goTimer = true; // при срабатывании звенеть 1 раз
bool timerReset_falg = false; // таймер сброшен человеком

bool timerStart = false; // остановка таймера
//bool timerReady = true; // отображать таймер
bool timerReady = false; // отображать таймер

float valSeaLevelPressure;

static float pres; // давление

void setup() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(anodPins[i], HIGH);
    pinMode(anodPins[i], OUTPUT);
  }
  pinMode(SER, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);

  butLeft.setDebounce(20); // дребизг
  butLeft.setClickTimeout(350); // нажатие
  butLeft.setTimeout(1000); // задержка

  butOK.setDebounce(20); // дребизг
  butOK.setClickTimeout(200); // нажатие !!!!
  butOK.setTimeout(1000); // задержка

  butRight.setDebounce(20); // дребизг
  butRight.setClickTimeout(350); // нажатие
  butRight.setTimeout(1000); // задержка

  brinDots.setTimeout(500); // гарят все кнопки при сохранении или горят??? 
  goAlarmTimer.setTimeout(120000); // 2 минуты

  blinkSegOut.setTimeout(2000);
   
  Serial.begin(9600);

//  rtc.setTime(BUILD_SEC + 5, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR); // устанвока времени вручную

  pinMode(BUZZER_PIN, OUTPUT); // зуммер выход

  bmp.setHumOversampling(MODULE_DISABLE);           // Отключаем неиспользуемый модуль измерения влажности - экономим энергию
  bmp.setStandbyTime(STANDBY_500MS); // STANDBY_500MS STANDBY_250MS STANDBY_125MS
  bmp.begin();   
  delay(1000);
  
  valSeaLevelPressure = pressureToMmHg(bmp.readPressure());
  pres = pressureToMmHg(bmp.readPressure()); // получаю давление в мм рт. столба
  
//  curr_hou = BUILD_HOUR; // установить время компиляции
//  curr_minut = BUILD_MIN;
//  curr_secon = BUILD_SEC;

  //  Serial.println((String) BUILD_HOUR +  BUILD_MIN);
  //  Serial.println(minut);
  //  Serial.println(secon);
}

void loop() {

//  while(true) {
//    getMeteorData();
//  }
//  return;


 /* static uint32_t timerS1 = 0;
  static uint32_t timerS2 = 0;
  while(true) {
    if (millis() - timerS1 >= 1000) {
      timerS1 = millis();
      Serial.println("Прошла секунда");
    }

    if (millis() - timerS2 >= 1000) {
      timerS2 = millis();
      Serial.println("Прошла ещё таже секунда");
    }

    
//    if (alarmChack) { // также каждую минуту должна быть проверка на будильник
//      alarmChack = false;
//      for (int j = 0; j < iAlarm; j++) { // проходимся по всем будильникам
//        if (alarmArr[j][0] == curr_hou && alarmArr[j][1] == curr_minut) { // если время будильника и текущее совпало, то
//          goAlarm_falg = true;
//          goAlarm (j);
//        }
//      }
//    }
  }*/

/*  static bool clickOK_flag = false; // для нескольких нажатий ОК
  
//  goAlarm (1); // проверка срабатывания будильника

//    printNumb(56,false, 78, false); // проверка индикатора
//    save_flag = true;
//    Serial.println((String) digitalRead(2) + digitalRead(3) + digitalRead(4)); // проверка кнопок
//    butOK.tick();

//    if (butOK.isClick()) {
//      Serial.println("Click");
//    }
//    if (butOK.isSingle()) {
//      Serial.println("isSingle");
//    }
//    if (butOK.isDouble()) {
//      Serial.println("isDouble");
//    }
//
//    printNumb(44, false, 66, false); // отобразить данные на индикатор
//      printByte(_C, _L, _o, _c); // часы
//    return; */

  static bool ok = false; 
  static bool seticng_falg = false;

  static long int tim = 0;

  static uint32_t secondStopTimer = 0; // суекндомер пока так
  static uint32_t secondtimer = 0; // суекндомер пока так
  
  butOK.tick();
 
  // функции в фоне
  stapTime(); // время должно быть всегда

//  if (millis() - tim >= 15000) {
//    tim = millis();
//    alarmChack = true;
//    Serial.print("alarmChack");
//    Serial.println(alarmChack);
//  }


  
  if (alarmChack) { // также каждую минуту должна быть проверка на будильник
    alarmChack = false;
//    Serial.println("Все установленыые будильники: ");
    for (int j = 0; j < iAlarm; j++) { // проходимся по всем будильникам
//      Serial.println((String)alarmArr[j][0]+ ":" + alarmArr[j][1]);
      if ((alarmArr[j][0] == curr_hou) && (alarmArr[j][1] == curr_minut)) { // если время будильника и текущее совпало, то
//        Serial.println("Пора вставать!!!");
//        Serial.print("alarmChack");
//        Serial.println(alarmChack);
        goAlarm_falg = true; // для чего
        goAlarmTimer.start(); // презапустить таймер сброса
        butOK.resetStates(); // хочу как-то сбросить кнопку
        goAlarm (j);
      }
    }
  }
  
  if (startWatch) { // секундомер
    if (caunterMenu == 2 && ok == true) {
      save_flag = false; // точки гаснут
    }
     
    // секундомер будет работать до 90 минут
    if (millis() - secondStopTimer >= 1000) { // пока минуты это секунды, а часы - минуты
     secondStopTimer = millis();
      if (++minutes_idle > 59) {          // инкремент минут и проверка на переполнение
        minutes_idle = 0;                 // переполнение минут
        hours_idle++;                     // инкремент часа
        if (hours_idle > 90) {
          hours_idle = 90; // жду перезагрузки...
        }
      }
    }
    
  } else { // остановить секундомер
    if (caunterMenu == 2 && ok == true) { // если секундомр и мы в секундомере, то точки нужны
      save_flag = true; // секундомер на паузе, все точки гарят
    } 
//      else {
//        save_flag = false;
//      } // Надо думать как граматно это сделать пока не знаю, точки при сейве не гаряд или горят \_\
    
   }

  if (timerStart) { // таймер
    if (timerReady) { // если мы в таймере и он работает, то гасим точки
      save_flag = false; // точки гаснут
    }
    if (millis() - secondtimer >= 1000) {
      secondtimer = millis();
      if (--minutes_timer < 0) {   // уменьшаем минуты и сравниваем, меньше нуля - идем дальше
        if (hours_timer-- > 0) {   // Уменьшаем еще и часы и сравниваем, больше нуля - идем дальше
          minutes_timer = 59;      // Переполняем часы
        } else {                  // если время закончилось
            hours_timer = 0;
            minutes_timer = 0;
            if (goTimer && !timerReset_falg) { // запустить 1 раз и это не рестарт
              do { // подастся сигнал там 3 - 10 всякое разное
                goTimer = false;
                timerReset_falg = false;
                
                butOK.tick();
                tone(BUZZER_PIN, 2500); 
                displayClock(hours_timer, true, minutes_timer, true); // отобразить данные на индикатор
              } while (!butOK.isDouble());
              noTone(BUZZER_PIN);
              butOK.resetStates(); // сбросить кнопку
            }
          }
        }
      }
    } else {
      if (timerReady) { // если таймер работает и пауза
        save_flag = true; // точки загараются
      }
    }
  
  
  if (seticng_falg == false) { // если не выбран пункт
    butLeft.tick();
    butRight.tick();
    
    if (butLeft.isClick()) {
      caunterMenu = constrain(caunterMenu - 1, 0, 4 - 1);
//      Serial.println(caunterMenu);
    }

    if (butRight.isClick()) {
      caunterMenu = constrain(caunterMenu + 1, 0, 4 - 1);
//      Serial.println(caunterMenu);
    }
    
    if (butOK.isSingle()) { // если выбрали пункт меню
      seticng_falg = true; // вышел из пунка меню
      ok = true;
//      Serial.println("Выбран пункт меню");
      butOK.resetStates(); // хочу как-то сбросить кнопку

      if (caunterMenu == 3) { // таймер
//        save_flag = false;
        timerReady = true; // вернуть его к жизни
        
      }
    }
  } 
  
  if (ok) { // если выбран пункт меню
    if (caunterMenu == 0 && setTime_flag == false) { // часы и я не в настройках
//      if (getClockTimer.isReady()) { // каждую минуту шлю запрос о времени в датчик
//        hou = rtc.getHours(); // время из датчика
//        minut = rtc.getMinutes(); 
//      }
      hou = curr_hou; // время из функции stapTime
      minut = curr_minut; 
    } else if (caunterMenu == 1 && setTime_flag == false) { // будильник
      butLeft.tick();
      butRight.tick();
      alarmClock();
    } else if (caunterMenu == 2 && setTime_flag == false) { // секундомер
      clickOK_flag = true; // настройки мне не нужны
      hou = hours_idle;
      minut = minutes_idle;
      stopWatch();
    } else if (caunterMenu == 3 && setTime_flag == false) {
//      clickOK_flag = true; // ПОКА вырублю настройки
      timer();
    }

   if (!clickOK_flag) { // я не в настройках
      if (butOK.isSingle()) { // зайти в настроки 
        clickOK_flag = true; // чтоб больше здесь не срывалось
        
//        Serial.println("Захожу в настройки времени");
        
        if (caunterMenu == 0) { // изменяем время
          last_hou = curr_hou; // отобразить текущее время
          last_minut = curr_minut;
        } else if (caunterMenu == 1) { // если будильник
          alarmSetNew_flag = true; // заблокировать выбор установленных будильников
          last_hou = curr_hou; // отобразить текущее время
          last_minut = curr_minut;
        } else if (caunterMenu == 2) { // секундомер
          hou = hours_idle;
          minut = minutes_idle; // ????
        } else if (caunterMenu == 3) { // таймер
          last_hou = 0;
          last_minut = 0;
          timerReady = false; // выйти из таймера
        }
        hou = last_hou; // сразу устанавливаем нужное время
        minut = last_minut;
        
        changeRank = false; // первая пара разрядов
        blinking.start(); // обновить таймер для маргния (сразу погасить сегмент)
        blinkDischa_flag = true; // для маргания сегментами
        setTime_flag = true;
     }
   }

   displayClock(hou, blinkHouSeg, minut, blinkMinutSeg); // отобразить данные на индикатор

  } else { // если не выбрано
    switch (caunterMenu) { // здесь будут все возможные пункты меню
      case 0:
        printByte(_C, _L, _o, _c, false); // часы
      break;
      case 1:
        printByte(_A, _L, _A, _r, false); // будильник
      break;
      case 2:
        printByte(_S, _t, _o, _P, false); // секундомер
      break;
      case 3:
        printByte(_t, _i, _e, _r, false); // таймер
      break;
    }
  }

  if (setTime_flag) { // в настройках времени
    butLeft.tick();
    butRight.tick();
    setTimes();
    if (butOK.isDouble()) { // выхожу из настроек
      clickOK_flag = false; // для кнопки ОК, чтоб можно было зайти ещё раз потом
      setTime_flag = false;
  
      if (caunterMenu == 0) { // установка часов
      } else if (caunterMenu == 1) { // если будильник
        alarmSetNew_flag = false; // разблокировать выбор установленных будильников
      } else if (caunterMenu == 2) { // если рестартнуть секундомер
//        startWatch = false;
//        minutes_idle = 0;           
//        hours_idle = 0;  
      } else if (caunterMenu == 3) { //таймер
        // ?????? \-|-/
        minutes_timer = 0;           
        hours_timer = 0;
        timerReady = true;
        save_flag = false; // точки гаснут
        timerStart = true;
      }
      // мигнуть всеми сегментами
      blinkHouSeg = true;
      blinkMinutSeg = true;
      delay(100);
      blinkHouSeg = false;
      blinkMinutSeg = false;
    }
  } else { // если не в настройках
    if (butOK.isDouble()) { // выйти из пункта меню
      seticng_falg = false; // вышел из пунка меню
      ok = false;
//      Serial.println("Выход из выбора");
      if (caunterMenu == 2) { // секундомер
        save_flag = false;
        clickOK_flag = false;
      } else if (caunterMenu == 3) { // таймер
//        timerReady = false; // таймер останавливается
        save_flag = false;
        timerReady = false;
        clickOK_flag = false; // можно усанавливать время
      }
    }
  }




  

/*
  if (caunterMenu == 1) {
      alarmClock();
    } else if (caunterMenu == 2) { // У ЧАСТОВ НЕ РАБОТАЕТ УСТАНОВКА ВРЕМЕНИ !!!!!
      if (setTime_flag == false) { // отображать время если я не в настройках
        minut = curr_minut; // время из функции stapTime
        hou = curr_hou;
      }
    } else if (caunterMenu == 3) { // секундомер
      hou = hours_idle;
      minut = minutes_idle;
      clickOK_flag == true; // запретить использовать кнопку входа в настройки
      stopWatch();
    } else if (caunterMenu == 4) { // таймер
  //    clickOK_flag == true; // запретить использовать кнопку входа в настройки
  //    hou = hours_timer;
  //    minut = minutes_timer;
      timer();
    }

  butLeft.tick();
  butOK.tick();
  butRight.tick();
  
  stapTime(); // время должно быть всегда
  //  if (butOne.isClick()) {
  //    constrain(caunter++; 0, menuItems - 1);
  //    Serial.prin("caunter");
  //    Serial.println(caunter);
  //  }

// также каждую минуту должна быть проверка на будильник
  if (alarmChack) { // лучше в stapTime это делать !!!
    alarmChack = false;
    for (int j = 0; j < iAlarm; j++) { // проходимся по всем будильникам
      if (alarmArr[j][0] == curr_hou && alarmArr[j][1] == curr_minut) { // если время будильника и текущее совпало, то
        goAlarm_falg = true;
        goAlarm (j);
      }
    }
  }

  if (caunterMenu == 1) {
    alarmClock();
  } else if (caunterMenu == 2) { // У ЧАСТОВ НЕ РАБОТАЕТ УСТАНОВКА ВРЕМЕНИ !!!!!
    if (setTime_flag == false) { // отображать время если я не в настройках
      minut = curr_minut; // время из функции stapTime
      hou = curr_hou;
    }
  } else if (caunterMenu == 3) { // секундомер
    hou = hours_idle;
    minut = minutes_idle;
    clickOK_flag == true; // запретить использовать кнопку входа в настройки
    stopWatch();
  } else if (caunterMenu == 4) { // таймер
//    clickOK_flag == true; // запретить использовать кнопку входа в настройки
//    hou = hours_timer;
//    minut = minutes_timer;
    timer();
  }
  
  /*if (!clickOK_flag) { // я не в настройках v1
    if (butOK.isClick()) { // зайти в настроки 
      clickOK_flag = true; // чтоб больше здесь не срывалось
      
      setTime_flag = true;
      changeRank = true; // первая пара разрядов
  
      if (caunter == 1) { // если будильник
        alarmSetNew_flag = true; // заблокировать выбор установленных будильников
        last_hou = curr_hou; // отобразить текущее время
        last_minut = curr_minut;
      } else if (caunter == 2) { // это можно поменять ...
        //      last_hou = hou; // время захода в настройки
        //      last_minut = minut;
      }
  
      hou = last_hou; // сразу устанавливаем нужное время
      minut = last_minut;
  
      // надо поставить в деффолд какие-то переменные ???
      stopRankSele_flag = false;
      rankSelected_flag = false;
    }
  }*/
/*
  if (clickOK_flag == false) { // я не в настройках v2
    if (butOK.isClick()) {
      clickOK_flag = true;
      if (caunterMenu == 1) { // если будильник
        alarmSetNew_flag = true; // заблокировать выбор установленных будильников
        last_hou = curr_hou; // отобразить текущее время
        last_minut = curr_minut;
      } else if (caunterMenu == 2) {
        last_hou = curr_hou; // отобразить текущее время
        last_minut = curr_minut;
      } else if (caunterMenu == 4) { // таймер
        last_hou = 0;
        last_minut = 0;
        timerReady = false; // выйти из таймера
      }
      hou = last_hou; // сразу устанавливаем нужное время
      minut = last_minut;
      
      changeRank = true; // первая пара разрядов
      blinking.start(); // обновить таймер для маргния (сразу погасить сегмент)
      blinkDischa_flag = true; // для маргания сегментами
      setTime_flag = true;
    }
  }
  
  if (butOK.isDouble()) { // выхожу из настроек

    clickOK_flag = false; // для кнопки ОК, чтоб можно было зайти ещё раз потом
    setTime_flag = false;
    //setTime_flag = !setTime_flag; // если true, зайти в меню

    if (caunterMenu == 1) { // если будильник
      alarmSetNew_flag = false; // разблокировать выбор установленных будильников
    } else if (caunterMenu == 3) { // если рестартнуть секундомер
      startWatch = false;
      minutes_idle = 0;           
      hours_idle = 0;  
    } else if (caunterMenu == 4) { //таймер
      timerReady = true;
      minutes_timer = 0;           
      hours_timer = 0;
      save_flag = false; // точки гаснут
      timerStart = true;
    }

//    rankSelected_flag = false; // убираем выбор сегментов
//    stopRankSele_flag = false;

    // мигнуть всеми сегментами
    blinkHouSeg = true;
    blinkMinutSeg = true;
    delay(100);
    blinkHouSeg = false;
    blinkMinutSeg = false;

    /* с 2 кнопками код
    //    if (setTime_flag) { // захожу в настройки
    //      changeRank = true; // первая пара разрядов
    //
    //      if (caunter == 1) { // если будильник
    //        alarmSetNew_flag = true; // заблокировать выбор установленных будильников
    //        last_hou = curr_hou;
    //        last_minut = curr_minut;
    //      } else if (caunter == 2) { // это можно поменять ...
    //        last_hou = hou; // время захода в настройки
    //        last_minut = minut;
    //      }
    //
    //      hou = last_hou; // сразу устанавливаем нужное время
    //      minut = last_minut;
    //
    //      // надо поставить в деффолд какие-то переменные ???
    //      stopRankSele_flag = false;
    //      rankSelected_flag = false;
    //    } else { // если изменений нет, то мне надо вернуть не время захоа в настройки, а текущее время
    //
    //      if (caunter == 1) { // если будильник
    //        alarmSetNew_flag = false; // разблокировать выбор установленных будильников
    //      }
    //
    //      rankSelected_flag = false; // убираем выбор сегментов
    //      stopRankSele_flag = false;
    //
    //      // мигнуть всеми сегментами
    //      blinkHouSeg = true;
    //      blinkMinutSeg = true;
    //      delay(100);
    //      blinkHouSeg = false;
    //      blinkMinutSeg = false;
    //    }*/
  }
/*
  if (setTime_flag) { // в настройках времени
    setTimes();
  } else { // не в настройках
    //    minut = curr_minut; // время из функции stapTime
    //    hou = curr_hou;
    //    Serial.println(curr_hou);
  }

//  printNumb(hou, blinkHouSeg, minut, blinkMinutSeg); // отобразить данные на индикатор
}*/


void setTimes () { // настройка времени
  static bool clickButSetTime = false;
  static bool butClickLR = false;
  
  if (butOK.isClick()) {       // короткое нажатие, смена пары разряда
    changeRank = !changeRank;  // инверт флага
    blinking.start(); // обновить таймер для маргния (сразу погасить сегмент)
    blinkDischa_flag = true; // чтоб начинать с гашения
    clickButSetTime = false; // возобновить марагание
    last_minut = minut;
    last_hou = hou;
    butClickLR = false; // при нажатии кнопок сбрасывать марагание
  }

  if (blinkSegOut.isReady()) { // если определённое время не надисаем ничего, то начать моргать
    if (butClickLR == true) {
//      Serial.println("Марагие через 2s надо");
      blinking.start(); // обновить таймер для маргния (сразу погасить сегмент)
      blinkDischa_flag = true;
      clickButSetTime = false; 
    }
  }
  
  if (!clickButSetTime) { // если мы меняем
    if (changeRank) {
      minut = last_minut;
      blinkHouSeg = true; // моргаем часами
      blinkMinutSeg = false;
    } else {
      hou = last_hou;
      blinkHouSeg = false; // моргаем минутами
      blinkMinutSeg = true;
    }
  }
//  holdTimer

  if (butLeft.isClick() || (butLeft.isHold() && holdTimer.isReady())) {  // Короткое нажатие или удержание
//    Serial.println("Уменьшаем время");
    butClickLR = true;
    blinkSegOut.start(); // скипнуть маргание
    clickButSetTime = true;
    blinkHouSeg = false;
    blinkMinutSeg = false;
    if (changeRank) {                    // Если ввод часов
      if (caunterMenu == 3) { // для таймера
        if (--hou < 0) {
          hou = 90;                  
        }
      } else {
        if (--hou < 0) {             // Уменьшаем и сравниваем // что значит -- value ???
          hou = 23;                  // Если меньше 0 - переполняем
        }
      }
    } else {                              // Если ввод минут
      if (--minut < 0) {           // Уменьшаем и сравниваем
        minut = 59;                // Если меньше 0 - переполняем
      }
    }
    
  }

  if (butRight.isClick() || (butRight.isHold() && holdTimer.isReady())) { // Аналогично для другой кнопки
//    Serial.println("Увеличиваем время");
    butClickLR = true;
    blinkSegOut.start(); // скипнуть маргание
    clickButSetTime = true;
    blinkHouSeg = false;
    blinkMinutSeg = false;
    if (changeRank) {
      if (caunterMenu == 3) { // для таймера
        if (++hou > 90) {
          hou = 0;
        }
      } else {
        if (++hou > 23) {
          hou = 0;
        }
      }
    } else {
      if (++minut > 59) {
        minut = 0;
      }
    }
  }

  if (butOK.isHolded()) { // СОХРАНИТЬ ИЗМЕНЕНИЯ
    last_hou = hou; // сохранить изменения
    last_minut = minut;
    if (caunterMenu == 0) { // время
//      last_minut = minut; // сохранить изменения
//      last_hou = hou;
      curr_hou = last_hou; // применить изменения
      curr_minut = last_minut;
      rtc.setTime(0, last_minut, last_hou, BUILD_DAY, BUILD_MONTH, BUILD_YEAR); // загрузить в модуль
//      Serial.print ("Новое время: ");
//      Serial.println (curr_hou);
    } else if (caunterMenu == 1) { // для будильника
      alarmArr[iAlarm][0] = hou; // сохранить в нувую ячейку
      alarmArr[iAlarm][1] = minut;
      // мне надо добавлять это в новую ячейку массива
//      Serial.print("Колличество будильников: ");
//      Serial.println(iAlarm + 1);
      iAlarm++;
      alarmSetNew_flag = false; // разрешаю показ будильников
      alarmCaunter = iAlarm - 1; // показать только что установленне время
    }  else if (caunterMenu == 2) { // секундомера не надо 
//      last_minut = minut; // сохранить изменения
//      last_hou = hou;
//      curr_hou = last_hou; // применить изменения
//      curr_minut = last_minut;
//      Serial.print ("Новое время: ");
//      Serial.println (curr_hou);
    } else if (caunterMenu == 3) { // таймер
      hours_timer = last_hou;
      minutes_timer = last_minut;
      timerReady = true; // сразу начать работу
      timerStart = true; // паузы нету!!!!
      
      timerReset_falg = false; // рестартуть флаг перезапуска
      goTimer = true; // переменная чтоб 1 раз звенел при рабатывании
    }

    clickOK_flag = false; // для кнопки ОК, чтоб можно было зайти ещё раз потом
    setTime_flag = false; // выход в "главное меню"

    blinkHouSeg = false; // отрубаю марагие
    blinkMinutSeg = false;
    save_flag = true; // точки загараются
    brinDots.start(); // перезапускаю таймер для точек
    butOK.resetStates(); // чтоб заново не зайти в настройку
    return; // выйти из setTime
  }

  /*if (!stopRankSele_flag) { // если пара сегментов ещё не выбрана
    if (butOK.isClick()) {
      stopRankSele_flag = true; // ну чтоб не использовать перемену сегмнта кнопку
      rankSelected_flag = true; // пара сегментов выбрана

      blinkHouSeg = false;  // убрать мигание
      blinkMinutSeg = false;
      hou = last_hou; // и вывести время начала настройки
      minut = last_minut;
      // если я хочу использовать её ещё как и настройка будильника, то
      //надо в начале отображать 00:00, а потом уже то, что там стоит
    }
  }*/
  
  /*  if (rankSelected_flag == false) { // ЭТОТ КОД МОЖНО ПЕРЕНСТИ ТУДА ...>
      if (butTwo.isHolded()) { // если мы в выборе задержим 2 кнопту, то значит выйти с изменениями
        // СОХРАНИТЬ ЗИМЕНЕНИЯ
        curr_hou = hou;
        curr_minut = minut;
  
        Serial.print ("Новое время: ");
        Serial.println (curr_hou);
  
        setTime_flag = false; // выход в "главное меню"
  
        blinkHouSeg = true;
        blinkMinutSeg = true;
        delay(100);
        blinkHouSeg = false;
        blinkMinutSeg = false;
        return; // выйти из setTime
      }
    } */
  
  /*if (rankSelected_flag) { // изменяем время выбранной пары разрядов
    stopRankSele_flag = true; // ну чтоб не использовать перемену сегмнта кнопку

    if (butRight.isHolded()) { // СОХРАНИТЬ и выйти в выбор пары разрядов
      rankSelected_flag = false; // разрешить выбирать какой разряд менять
      stopRankSele_flag = false; // чтоб не менялись сегменты ещё, разрешить выбрать разряд

      last_hou = hou; // сихранить изменения в переменные
      last_minut = minut;
    }
    if (changeRank) { // меняем часы
      if (butRight.isClick()) { // не работает, как так????
        hou++;
        //        Serial.println("Хочу увеличить время");
      }
      if (butLeft.isClick()) {
        hou--;
        //        Serial.println("Хочу уменьшить время");
      }
      if (hou >= 24) {
        hou = 0;
      } else if (hou < 0) {
        hou = 23;
      }
    } else { // меняем минуты
      if (butRight.isClick()) { // не работает, как так????
        minut++;
        //    Serial.println("Хочу увеличить время");
      }
      if (butLeft.isClick()) {
        minut--;
        //        Serial.println("Хочу уменьшить время");
      }
      if (minut >= 60) {
        minut = 0;
      } else if (minut < 0) {
        minut = 59;
      }
    }
    
  } else { // выбор какую пару разрядов меняем
  
    if (butRight.isHolded()) { // СОХРАНИТЬ ИЗМЕНЕНИЯ
      if (caunter == 1) { // для будильника
        alarmArr[iAlarm][0] = hou; // сохранить в нувую ячейку
        alarmArr[iAlarm][1] = minut;
        // мне надо добавлять это в новую ячейку массива
        Serial.print("Колличество будильников: ");
        Serial.println(iAlarm + 1);
        iAlarm++;
        alarmSetNew_flag = false; // разрешаю показ будильников
        alarmCaunter = iAlarm - 1; // показать только что установленне время
      } else { // сохранение реального времени
        curr_hou = hou;
        curr_minut = minut;

        Serial.print ("Новое время: ");
        Serial.println (curr_hou);
      }

      clickOK_flag = false; // для кнопки ОК, чтоб можно было зайти ещё раз потом
      setTime_flag = false; // выход в "главное меню"

      blinkHouSeg = false; // отрубаю марагие
      blinkMinutSeg = false;
      save_flag = true; // точки загараются
      brinDots.start(); // перезапускаю таймер для точек
      return; // выйти из setTime
    }
    
    
    if (butLeft.isClick()) {
      changeRank = !changeRank; // смена выбора пары сегментов
    }
    
    if (changeRank) {
      minut = last_minut;
      blinkHouSeg = true; // моргаем часами
      blinkMinutSeg = false;
    } else {
      hou = last_hou;
      blinkHouSeg = false; // моргаем минутами
      blinkMinutSeg = true;
    }

    /*  if (blinking.isReady()) {
            blinkDischa_flag = !blinkDischa_flag; // мигалка
          }
          if (changeRank) { // часы
            minut = last_minut;
            if (blinkDischa_flag) {
             hou = 99;
            } else {
              // если я устанавливаю время, то оно потом сбросится снова на время захода
              hou = last_hou;
            }
            /* мигалка с двумя таймерами
            if (blinkingTimerLOW.isReady()) {
              hou = -1-1;
              blinkingTimerHIGH.start();
            }
            if (blinkingTimerHIGH.isReady()) {
              hou = last_hou; // прошлому значению он будет сбрасывать постоянно блин
              blinkingTimerLOW.start();
            }
      
          }  else { // минуты
            // Как сделать маргание сразу после перехода в сегмент?
            hou = last_hou;
            if (blinkDischa_flag) {
              minut = 99;
            } else {
              minut = last_minut;
            }
      мигалка
            if (blinkingTimerLOW.isReady()) {
              minut = -1-1; // аааа как это сделать, чтоб не задевать числаа ааааа
              blinkingTimerHIGH.start();
            }
            if (blinkingTimerHIGH.isReady()) {
              minut = last_minut; // прошлому значению
              blinkingTimerLOW.start();
            }
    
  }*/
   
   /* else {
        // Как сделать маргание сразу после перехода в сегмент?
        hou = last_hou;
        if (blinkingTimerLOW.isReady()) {
          minut = 99;
          blinkingTimerHIGH.start();
        }
        if (blinkingTimerHIGH.isReady()) {
         minut = last_minut; // прошлому значению
          blinkingTimerLOW.start();
        }

      // должно запоминатся значение
      if (blinkingTimerLOW.isReady()) {
        hou = 99;
      }
    
      if (blinkingTimerHIGH.isReady()) {
        hou = 10; // прошлому значению
      }

      if (but_1) {
        if (разряд1) {
           // тут такой реверс, выбора изменения разряда
           // изменять hou
        } else {
          // изменять minut
        }
      }*/
  
}

void stapTime() { // счётчик времени
  static bool oneCompil = true; // сработает 1 раз при перезагрузки
  // надо заменить на получения с датчика просто и всё
  if (getClockTimer.isReady() || oneCompil) { // каждую минуту шлю запрос о времени в датчик
    oneCompil = false;
    curr_hou = rtc.getHours(); // время из датчика
    curr_minut = rtc.getMinutes();
    alarmChack = true; // прошла минута, проверить будильник
  }
//  if (alarmChackTimer.isReady()) {
//    alarmChack = true; // прошла минута, проверить будильник
//  }
//  if (secondTim.isReady()) {
//    curr_secon++; 
//    if (curr_secon >= 60) {
//      curr_secon = 0;
//      curr_minut++;
//      // переменная чтоб чекнуть будтльники
//      alarmChack = true; // прошла минута, проверить будильник
//    }
//    if (curr_minut >= 60) {
//      curr_minut = 0;
//      curr_hou++;
//    }
//    if (curr_hou >= 24) {
//      curr_hou = 0;
//    }
//  }
}

void alarmClock () { // будильник
//  static int alarmCaunter;
//  static int alarmRemoteCunter = 11; // счётчик для удаления будильника

  if (!alarmSetNew_flag) { // если идёт просмотр будильников
    hou = alarmArr[alarmCaunter][0]; // выставляем время последнего будильника или не последнего, а самого ближнего к срабатыванию, так можно ????
    minut = alarmArr[alarmCaunter][1];

    if (butRight.isClick()) { // просмотр будильников
      alarmCaunter++;
      if (alarmCaunter >= iAlarm - 1) {
        alarmCaunter = iAlarm - 1;
      }
//      Serial.println(alarmCaunter);
    }

    if (butLeft.isClick()) {
      alarmCaunter--;
      if (alarmCaunter <= 0) {
        alarmCaunter = 0;
      }
//      Serial.println(alarmCaunter);
    }

    //  if (butOne.isHolded()) { // удаление элемента
    //    alarm[AlarmCaunter] делит короч;
    //  }
  
    //  if (butTwo.isHolded()) { // создать новый будильник
    //    setTime_flag = true;
    //  }
  }
}

void goAlarm (int itemsAlarm) { // будильник сработал
  blinking.start(); // обновить таймер для маргния (сразу погасить сегмент)
  blinkDischa_flag = true; // чтоб начинать с гашения
  while(true) {
    stapTime(); // время должно быть всегда
    butOK.tick();
    displayClock(alarmArr[itemsAlarm][0], true, alarmArr[itemsAlarm][1], true);
    // может быть надо будет всё выклчать ...
    
    if (!blinkDischa_flag) { // звучание зуммара должно быть одинаково с частотой моргания сегментов
      tone(5, 700);
    } else {
      noTone(5);
    }
    
    if (goAlarmTimer.isReady() || butOK.isDouble()) {
      // goAlarm_falg = false; // убрать будильник
      noTone(5);
      butOK.resetStates(); // хочу как-то сбросить кнопку

      blinking.start(); // обновить таймер для маргния (сразу погасить сегмент)
      blinkDischa_flag = true; // чтоб начинать с гашения
      
      alarmChack = false; // ?????
      return;
    }
  }
  
}

void stopWatch(void) { // секундомер
//  static uint32_t secondTimer = 0;
//  static bool startWatch = false;
  
//  if (butOK.isClick()) {
//    startWatch = !startWatch;
//  }
  if (butOK.isSingle()) {
    startWatch = !startWatch;
    butOK.resetStates(); // хочу как-то сбросить кнопку
  }

  // это должно быть в фоне...
//  if (startWatch) { // запустить секундомер 
//    save_flag = false; // точки загараются
//     if (millis() - secondTimer >= 1000) { // пока минуты это секунды, а часы - минуты
//      secondTimer = millis();
//      if (++minutes_idle > 59) {          // инкремент минут и проверка на переполнение
//        minutes_idle = 0;                 // переполнение минут
//        hours_idle++;                     // инкремент часа
//      }
//    }
//  } else { // остановить секундомер
//    save_flag = true; // секундомер на паузе, все точки гарят
////    if (minutes_idle == 0 && hours_idle == 0) { // если мы только начали, то точки не нужны
////      save_flag = false; // точки гаснут
////    } else {
////      save_flag = true; // точки загараются
////    }
//  }

  if (butOK.isHolded()) { // скипнуть секундомер
    startWatch = false;
    minutes_idle = 0;           
    hours_idle = 0;
    butOK.resetStates(); // хочу как-то сбросить кнопку  
  }
  
  
//  if (alarmChack) { // если прошла минута
//    if (++minutes_idle > 59) {          // инкремент минут и проверка на переполнение
//      minutes_idle = 0;                 // переполнение минут
//      hours_idle++;                     // инкремент часа
//    }
//  }
}

void timer(void) { // таймер
//  static uint32_t secondTimer = 0;
//  static bool timerFinish = false; // если таймер вышел или перезагрузка
//  static bool timerStart = false;

  if (timerReady) { // если отображаем таймер
//    hou = hours_timer;
//    minut = minutes_timer;
    
    if (butOK.isSingle()) { // стоп/старт
      timerStart = !timerStart;
      butOK.resetStates(); // сбросить кнопку
    }

    if (butOK.isHolded()) { // сбросить таймер
      hours_timer = 0;
      minutes_timer = 0;
//      timerStart = false;
      timerReset_falg = true; // таймер сброшен человеком
      butOK.resetStates(); // хочу как-то сбросить кнопку  
    }
    
    if (hours_timer == 0 && minutes_timer == 0) { // если всё по нулям, ставин новый таймер
      clickOK_flag = false; // можно усанавливать время
      timerReady = false;
      save_flag = false; // точки выкл
    } else {
      clickOK_flag = true; // пока таймер идёт нельзя зайти в установку времени
    }

    /*if (timerStart) {
//      if (!brinDots.isReady()) { // надо чтоб не моргал!!!
//        save_flag = false;
//      } else {
//        save_flag = true;
//      }
      save_flag = false; // точки гаснут
      if (millis() - secondTimer >= 1000) {
        secondTimer = millis();
        if (--minutes_timer < 0) {   // уменьшаем минуты и сравниваем, меньше нуля - идем дальше
          if (hours_timer-- > 0) {   // Уменьшаем еще и часы и сравниваем, больше нуля - идем дальше
            minutes_timer = 59;      // Переполняем часы
          } else {                  // если время закончилось
            hours_timer = 0;
            minutes_timer = 0;
            // подастся сигнал там 3 - 10 всякое разное
          }
        }
      }
    } 
    else {
      if (timerReady) { // если таймер работает и пауза
        save_flag = true; // точки загараются
      }
    }*/

    hou = hours_timer;
    minut = minutes_timer;
  }

    



    
//    if (timerFinish) { // если таймер вышел или перезагрузка
////      clickOK_flag = false; // можно войти в настройки    
//      save_flag = false; // точки гаснут
//    } else { // если таймер идёт, то мы можем его останавливать
//      hou = hours_timer; // выводить время
//      minut = minutes_timer;
//      if (butOK.isClick()) {
//        timerStart = !timerStart;
//      }
//    }
//    if (timerStart) {
//      save_flag = false; // точки гаснут
//      if (millis() - secondTimer >= 1000) {
//        secondTimer = millis();
//        if (--minutes_timer < 0) {   // уменьшаем минуты и сравниваем, меньше нуля - идем дальше
//          if (hours_timer-- > 0) {   // Уменьшаем еще и часы и сравниваем, больше нуля - идем дальше
//            minutes_timer = 59;      // Переполняем часы
//          } else {                  // если время закончилось
//  //           timerFinish = true;
//             hours_timer = 0;
//             minutes_timer = 0;
               /* disp.point(0);          // выкл точку
                disp.displayByte(_empty, _E, _n, _d); // вывод надписи
              for (uint8_t i = 0; i < 3; i++) {     // цикл с подачей сигналов
                tone(BUZZER_PIN, BUZZER_TONE);
                delay(BUZZER_DUR);
                noTone(BUZZER_PIN);
                delay(BUZZER_DUR);
              }
              do {
                big.tick();               // Опрашивать кнопку
              } while (!big.isClick());   // пока ее нажмут
              do {
                big.tick();               // Опрашивать кнопку
              } while (!big.isClick());   // пока ее нажмут
              return; */                   // По нажатию вернуться назад
//            }
//          }
//        } 
    //    hou = hours_timer; // выводить время
    //    minut = minutes_timer;
//      } else { // если таймер остановлен
//        save_flag = true; // точки загараются
//      }
//  }


}


void getMeteorData (void) { // барометр, высотометр, термометр
//  displayClock(99, false, 12, false);
  printByte(_, _E, _r, _r, true);
/* Старое  
 
  static long int timerPres = 0;
//  static float pres;
  static float altitud;
  
  butOK.tick();
//   int presStart;
//   int lastNumberPres;
  static bool one;
  
  static int newValue;

  static int cauntMeteo = 2; 

  static int Lastaltitud;
  
  if (butOK.isClick()) {
    Serial.println("Значениея обновлены");
    valSeaLevelPressure = pres;
  }
///*
  if (millis() - timerPres >= 500) {
    timerPres = millis();
    pres = pressureToMmHg(bmp.readPressure()); // получаю давление в мм рт. столба

//    Serial.print("Temperature: ");
//    Serial.print(bmp.readTemperature());        // Выводим темперутуру в [*C]
//    Serial.println(" *C");
    
//    Serial.println("----");

    Serial.print(pres);     // Выводим давление в мм рт. столба
    Serial.println(" mm Hg");

//    newValue = pres * 10;
//    Serial.print("Последняя цыфра (1 вычесление): ");
//    Serial.println(newValue);


//    Serial.print("Последняя цыфра ввода: "); // дробная часть
//    Serial.println(fracInt); // дробная часть

    
//    newValue = newValue % 10; // последняя цыфра
//    Serial.print("Последняя цыфра: ");
//    Serial.println(newValue);
      
    if (!one) {
      one = true;
      valSeaLevelPressure = pres;
    }
//    altitud = pressureToAltitude(pres, valSeaLevelPressure);
//    altitud = pow(pres / valSeaLevelPressure, 0.1914);
    altitud = 44330 * ( 1 - pow(pres / valSeaLevelPressure, 0.1914));

    
    
//    if (altitud < 0) {
//      altitud = 0;
//    }
//    Serial.print("altitud: ");
//    Serial.println(altitud);     // Выводим давление в мм рт. столба
//
//    Lastaltitud = altitud * 10;
//    Serial.print("Lastaltitud1: ");
//    Serial.println(Lastaltitud);
//    Lastaltitud = Lastaltitud % 10;
//    
//    Serial.print("Lastaltitud2: ");
//    Serial.println(Lastaltitud);
//    
//    Serial.print((int)altitud);     // Выводим давление в мм рт. столба
//    Serial.println(" m");
    
  }
//  printNumb((int)altitud, false, Lastaltitud, false);
  printFloatNumb(altitud, 3); // отображаем давление, температуру

//  printFloatNumb
//  printNumb(presStart, false, lastNumberPres, false); // отобразить данные на индикатор
//  static float numbers[] = {25.54, 25.03, 5.05, 5.63, -25.05, -25.63, -3.05, -3.79, -0.11, 0.02}; // проверка температуры i = 7
//  static float numbers[] = {765.30, 765.30, 765.31, 765.32, 765.33, 765.34, 765.35, 765.36, 765.37, 765.38, 765.39, 765.40}; // i = 11 проверка давления
//  static float numbers[] = {765.40, 765.39, 765.40, 765.39}; // проверка давления
//  static float numbers[] = {0.4}; // проверка высоты
  static int i = 0;
  if (millis() - timerPres >= 2000) {
    timerPres = millis();
    i++;
    if (i > 12) i = 0;
//    Serial.println("-------------------------------------------------------");
//    Serial.print("numbers[]: ");
//    Serial.println(numbers[i]);
  }
  
//  printFloatNumb(-0.03, 3);
//  printFloatNumb(-3.05, 1); // ошибка
//  printFloatNumb(32.3243, 3); 
  
*/
  
}
