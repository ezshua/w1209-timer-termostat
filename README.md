# w1209-timer-termostat
Расширение функционала альтернативной прошивки с добавлением функции таймера. 

Устройство работает как термостат до истечения интервала таймера, после чего реле отключается.

Время таймера вводится аналогично целевой температуре, после нее. Т.е.
нажатие SET переводит в режим ввода целевой температуры, следующее нажатие переводит в режим ввода 
целевого интервала, следующее нажатие сохраняет оба значения (целевых температуры и времени).
Время представлено десятичной формой, где целая часть значения это часы (0-23), 
а дробная представляет минуты (0-59). Максимальное значения таймера - 23 часа 59 минут.
Значения отображаются ввиде двух поддиапазонов от "0.00" до "9.59" и от "10.0" до "23.5". В обоих
поддиапазонах значения меняются с шагом в 1 минуту, но во втором поддиапазоне единицы минут 
не отображаются.
Установка интервала таймера в "0.00" отключает его работу, устройство переходит в режим термостата
и отображает время, прошедшее с момента включения питания.

Оставшийся функционал аналогичен оригинальной и альтернативной прошивкам.

# w1209-firmware
The functional equivalent to the original firmware of "Digital Thermostat Module Model XH-W1209".

The F.A.Q. page is available at https://github.com/mister-grumbler/w1209-firmware/wiki/FAQ

Look at the list of issues to have an idea of what needs to be done for the initial release.
https://github.com/mister-grumbler/w1209-firmware/issues
