//#include <GyverTM1637.h>
#include <GyverButton.h>

/*
   
   Таймер и секундомер...
 
   Шахматные часы для контроля рабочего времени
   Два потока времени - обратный отсчет рабочего времени и прямой счет перерывов
   Встроенные оповещения о длительном перерыве (если необходимо)
   Проверка присутствия на рабочем месте (если необходимо)
*/

/*---------------------------------------Настройки----------------------------------------------*/
#define BIG_BTN     3       // Пин для большой кнопки
#define LEFT_BTN    2       // Пин для левой кнопки управления
#define RIGHT_BTN   4       // Пин для правой кнопки управления
#define BUZZER_PIN  5       // Пин для баззера (если нужен)
#define BUZZER_TONE 2500    // Частота баззера в герцах
#define BUZZER_DUR  300     // Длинна сигнала баззера в мс
#define SER 10
#define RCLK 11
#define SRCLK 12

// ?????
#define CHECK_ENABLE   0    // Включить проверку наличия на рабочем месте (0 - выкл, 1 - вкл)

#define CHECK_TIMEOUT 15    // Таймаут на нажатие кнопки при проверке в секундах

#define CHECK_PERIOD 2      // Постоянный период проверки в минутах
#define CHECK_PERIOD_RAND 0 // Генерировать опрос со случайным периодом  (0 - выкл, 1 - вкл)

#define CHECK_PERIOD_MIN 1  // Мин период опроса в минутах
#define CHECK_PERIOD_MAX 15 // Макс период опроса в минутах

#define IDLE_ALERT 0        // Делать оповещения о долгом перерыве 
#define IDLE_ALERT_PERIOD 5 // Период оповещений в минутах
/*-----------------------------------------------------------------------------------------------*/

GButton big(BIG_BTN, HIGH_PULL);
GButton left(LEFT_BTN, HIGH_PULL);
GButton right(RIGHT_BTN, HIGH_PULL);
//GyverTM1637 disp(CLK, DIO);

int hours_need = 0;   // Ввод часов
int8_t hours_left = 0;   // Часы на обратный отсчет
int8_t hours_idle = 0;   // Часы безделья
int minutes_need = 0; // Ввод минут
int8_t minutes_left = 0; // Минуты на обратный отсчет
int8_t minutes_idle = 0; // Минуты безделья

int anodPins[4] = {A0, A1, A2, A3};

#if (CHECK_PERIOD_RAND == true) // ????
uint32_t checkPeriod = random(CHECK_PERIOD_MIN * 60000UL, CHECK_PERIOD_MAX * 60000UL);  // Если рандом период - получаем этот случайный период
#else
uint32_t checkPeriod = CHECK_PERIOD * 60000UL;                                          // А если постоянный - просто взять из константы
#endif

void setup() {
  pinMode(SER, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(SRCLK, OUTPUT);

  Serial.begin(9600);
  
  for (int i = 0; i < 4; i++) {
    digitalWrite(anodPins[i], HIGH);
    pinMode(anodPins[i], OUTPUT);
  }
//  disp.clear();           // Очистить дисп
//  disp.brightness(7);     // Макс яркость (0-7)
//  disp.point(0);          // Выкл точки

  big.setTimeout(1500);       // Таймаут длительного удержания красной кнопки
  left.setStepTimeout(120);   // Таймаут инкремента при удержании других кнопок
  right.setStepTimeout(120);

  pinMode(BUZZER_PIN, OUTPUT);  // Пин баззера как выход
}

void loop() {

//  printNumb(15, false, 4, false);
//  return;

  static bool insertHours = true; // выбор разряда

  big.tick();   // опрос кнопок
  left.tick();
  right.tick();

  if (big.isClick()) {           // короткое нажатие
    insertHours = !insertHours;  // инверт флага
  }

//  if (big.isHolded()) { // длинное нажатие
//    workCycle();        // переход в рабочий цикл
//  }

  if (left.isClick() or left.isStep()) {  // Короткое нажатие или удержание
    if (insertHours) {                    // Если ввод часов
      if (--hours_need < 0) {             // Уменьшаем и сравниваем // что значит -- value ???
        hours_need = 23;                  // Если меньше 0 - переполняем
      }
    } else {                              // Если ввод минут
      if (--minutes_need < 0) {           // Уменьшаем и сравниваем
        minutes_need = 59;                // Если меньше 0 - переполняем
      }
    }
  }

  if (right.isClick() or right.isStep()) { // Аналогично для другой кнопки
    if (insertHours) {
      if (++hours_need > 23) {
        hours_need = 0;
      }
    } else {
      if (++minutes_need > 59) {
        minutes_need = 0;
      }
    }
  }

  printNumb(hours_need, false, minutes_need, false);

// Круто реализованно!!!!
//  if (millis() - pointTimer >= 600) {   // мигание точкой каждые 600 мс
//    pointTimer = millis();
//    pointState = !pointState;
//    disp.point(pointState);
//  }
}

void workCycle(void) { // таймер
  static uint32_t minsTimer = 0;
  static uint32_t checkTimer = 0;
  static bool checkFlag = false;
  static bool buzzerEnable = false; // переменные и флаги

  hours_idle = 0;                   // передаем нужные значения и обнуляем перерывы
  minutes_idle = 0;
  minutes_left = minutes_need - 1;
  hours_left = hours_need;

  tone(BUZZER_PIN, BUZZER_TONE, BUZZER_DUR); // подаем сигнал
//  disp.point(1);                             // вкл точку ????

  minsTimer = millis();
  checkTimer = millis();                     // обновляем таймеры

  while (true) {    // бесконечный цикл
    big.tick();     // опрос кнопки

    if (big.isClick()) {    // короткое нажатие
      if (checkFlag) {      // Если флаг установлен
        checkFlag = false;  // сброс флага
      } else {              // Если флага нет
        idleCycle();        // перейти в режим счета перерыва
      }
    }

    if (big.isHolded()) {   // Длительное нажатие на клаву
      tone(BUZZER_PIN, BUZZER_TONE, BUZZER_DUR); // подать сигнал и вернуться назад
      return;
    }

    if (millis() - minsTimer >= 60000) {          // минутный таймер
      minsTimer = millis();
      if (--minutes_left < 0) {   // уменьшаем минуты и сравниваем, меньше нуля - идем дальше
        if (hours_left-- > 0) {   // Уменьшаем еще и часы и сравниваем, больше нуля - идем дальше
          minutes_left = 59;      // Переполняем часы
        } else {                  // если время закончилось
//          disp.point(0);          // выкл точку
//          disp.displayByte(_empty, _E, _n, _d); // вывод надписи
          for (uint8_t i = 0; i < 3; i++) {     // цикл с подачей сигналов
            tone(BUZZER_PIN, BUZZER_TONE);
            delay(BUZZER_DUR);
            noTone(BUZZER_PIN);
            delay(BUZZER_DUR);
          }
          do {
            big.tick();               // Опрашивать кнопку
          } while (!big.isClick());   // пока ее нажмут
//          disp.point(1);              // как нажали - вкл точку
//          disp.displayClockTwist(hours_idle, minutes_idle, 50); // вывести время безделья
          do {
            big.tick();               // Опрашивать кнопку
          } while (!big.isClick());   // пока ее нажмут
          return;                     // По нажатию вернуться назад
        }
      }
    }

  
  #if (CHECK_ENABLE == true)            // Если проверка включена
      if (millis() - checkTimer >= checkPeriod) { // Таймер опроса
        checkTimer = millis();
        checkFlag = true;
        buzzerEnable = true;
  #if (CHECK_PERIOD_RAND == true) // понятие не имею как это работает
        checkPeriod = random(CHECK_PERIOD_MIN * 60000UL, CHECK_PERIOD_MAX * 60000UL); // получение случайного периода
  #endif
        tone(BUZZER_PIN, BUZZER_TONE, BUZZER_DUR);    // Подать первый сигнал
      }
  
      if (buzzerEnable and millis() - checkTimer >= BUZZER_DUR * 2UL) { // После окончания подать еще один сигнал по таймеру
        tone(BUZZER_PIN, BUZZER_TONE, BUZZER_DUR);
        buzzerEnable = false;                                           // Выкл баззер
      }
  
      if (checkFlag and millis() - checkTimer >= CHECK_TIMEOUT * 1000UL) {  // Таймаут нажатия на кнопку
        checkFlag = false;
  //      idleCycle();                                                        // автоматом идем в режим безделья
      }
  #endif

    printNumb(hours_left, false, minutes_left, false);                         // Вывод часов и минут обратного отсчета
  }
}

void idleCycle(void) { // секундомер
  static uint32_t pointTimer = 0;
  static uint32_t minsTimer = 0;
  static uint32_t alertTimer = 0;
  static bool pointState = false;
  static bool buzzerEnable = false;                   // переменные

//  disp.point(0);                                      // выкл точку
//  disp.displayByte(_1, _d, _L, _E);                   // вывод надписи
  tone(BUZZER_PIN, BUZZER_TONE);                      // подача сигнала
  delay(BUZZER_DUR);  
  noTone(BUZZER_PIN);
  delay(BUZZER_DUR);

  pointTimer = millis();                               // апдейт таймеров
  minsTimer = millis();
  alertTimer = millis();

  while (true) {   // бесконечный цикл
    big.tick();     // опрос кнопки

    if (big.isClick()) {    // нажатие на кнопку
      tone(BUZZER_PIN, BUZZER_TONE, BUZZER_DUR); // подать сигнал
//      disp.point(1);        // вкл точку
      return;               // вернуться назад
    }

    if (millis() - minsTimer >= 60000) {  // минутный таймер
      minsTimer = millis(); 
      if (++minutes_idle > 59) {          // инкремент минут и проверка на переполнение
        minutes_idle = 0;                 // переполнение минут
        hours_idle++;                     // инкремент часа
      }
    }

//    if (millis() - pointTimer >= 1500) {  // таймер мигания точек
//      pointTimer = millis();
//      pointState = !pointState; 
//      disp.point(pointState);
//    }

//#if (IDLE_ALERT == true)    // если включено напоминание о длительном безделье
//    if (millis() - alertTimer >= IDLE_ALERT_PERIOD * 60000UL) {  // таймер опроса безделья
//      alertTimer = millis();
//      buzzerEnable = true;  
//      tone(BUZZER_PIN, BUZZER_TONE, BUZZER_DUR);                  // подать первый сигнал
//    }
//
//    if (buzzerEnable and millis() - alertTimer >= BUZZER_DUR * 2UL) { // Таймер для второго сигнала без delay
//      tone(BUZZER_PIN, BUZZER_TONE, BUZZER_DUR);                      // Подача второго сигнала
//      buzzerEnable = false;
//    }
//#endif

//    disp.displayClock(hours_idle, minutes_idle);                      // вывод часов и минут безделья в реальном времени
    printNumb(hours_idle, false, minutes_idle, false); 
  }
}
