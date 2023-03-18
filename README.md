# TermalCameraCorrection

Задача № 10, уровень сложности 2

Разработчик: *****

**Обработка изображения. Коррекция видео**

**Постановка задачи.** Дан файл данных от тепловизионной камеры («dump\_13122019\_145433.bin»). Формат файла: первые 32 байта – заголовок (служебная информация), далее – кадры 640\*512 16-ти разрядных значений интенсивности пикселей (всего 1000 кадров). Требуется преобразовать каждый кадр исходного видеоряда в черно-белое изображение (3-х канальное, 255 градаций серого) определив динамический диапазон исходных данных (предположительно – с помощью гистограммы интенсивности) и выполнив гамма-коррекцию (должен быть виден как фон (деревья), так и объекты (люди)). Все преобразования должны быть выполнены непосредственно (без использования спец. библиотек). Для отображения информации на экране использовать OpenCV.  Полученный видеоряд записать в видеофайл в формате «mp» (средствами OpenCV). 

**Требования:** 

С++, OpenCV (только для отображения информации)

Отчет с результатами решения, содержащий: 

1. Входные и выходные данные (переменные) программы;
1. Код программы на С++ с комментариями;
1. Алгоритм в виде блок схемы; 
1. Иллюстрация результатов работы программы видеофайлом в формате «mp».
1. Выводы. 

Также отчет по решению выслать *****

*Ориентировочное время для выполнения тестового задания для специалистов с различным опытом:*

|Наличие опыта специалиста|Ориентировочное время выполнения задачи, часы|
| :- | :- |
|Студент без опыта|24|
|Специалист с дипломом без опыта с минимальным опытом|16|
|Специалист с опытом работы более 2-3 лет|8|

*Исходные данные:*

Файл с данными, которые необходимо обработать находится в архиве dump\_13122019\_145433.zip.
