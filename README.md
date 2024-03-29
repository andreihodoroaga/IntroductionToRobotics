# IntroductionToRobotics (2023-2024)
This repository contains the assignments for the "Introduction to Robotics" course, taken in my third year at the Faculty of Mathematics and Informatics of the University of Bucharest.

## Table of contents
- [Homework 1](#homework-1)
- [Homework 2](#homework-2)
- [Homework 3](#homework-3)
- [Homework 4](#homework-4)
- [Homework 5](#homework-5)
- [Homework 6](#homework-6)
- [Homework 7](#homework-7)

## Homework 1
### Requirement
Control each channel (red, green, blue) in an RGB LED using individual potentiometers.

### File
[Homework 1 file](homework1/homework1.ino)


<details><summary><h3>Photo</h3></summary>

![Homework 1 photo](homework1/photo.jpg)

</details>

### Video
https://www.youtube.com/watch?v=MONQZTE_G2A

## Homework 2
### Requirement
Simulate an elevator using at least 3 buttons and 3 LEDs corresponding to the floors. <br>
Pressing a button should move the elevator to the associated floor. <br>
While the elevator is moving between floors, there should be an additional LED signaling this.

### File
[Homework 2 file](homework2/homework2.ino)

<details><summary><h3>Photo</h3></summary>

![Homework 2 photo](homework2/photo.jpg)

</details>

### Video
https://www.youtube.com/watch?v=TdfMBzYgRU8

## Homework 3
### Requirement
Control a 7-segment display using a joystick.<br>
There should be a blinking segment to indicate the current selected segment.<br>
Upon pressing on the joystick, the segment should switch its state (from ON to OFF or from OFF to ON).<br>
The display should reset by long pressing the joystick.

### File
[Homework 3 file](homework3/homework3.ino)

<details><summary><h3>Photo</h3></summary>

![Homework 3 photo](homework3/photo.jpg)

</details>

### Video
https://www.youtube.com/watch?v=aXNt2QwVl8E

## Homework 4
### Requirement
Implement a timer using a 4-digit 7-segment segment display and three buttons. <br>
There should be one that button has start/pause functionality, one lap button, and one reset button. <br>
When the reset button is first pressed, it should reset the timer to 000.0 and toggle "lap viewing mode". <br>
In lap viewing mode, pressing the lap button should cycle through the last 4 saved laps. Pressing the reset button again should reset the timer completely.

### File
[Homework 4 file](homework4/homework4.ino)

<details><summary><h3>Photo</h3></summary>

![Homework 4 photo](homework4/photo.jpg)

</details>

### Video
https://www.youtube.com/watch?v=Ycb3rkpeRDs

## Homework 5
### Requirement
Develop an intelligent environmental monitoring system featuring an ultrasonic sensor and an LDR sensor. <br>
Implement an interactive menu to access various functionalities. <br>
Enable users to establish alert thresholds for sensors, such as proximity warnings from the ultrasonic sensor or low light levels detected by the LDR sensor.<br>
Upon triggering an alert, signify the event by having an RGB LED turn red; otherwise, maintain a green color or any user-selected color from the menu. Additionally, include options for adjusting the sensor sampling rate and viewing real-time as well as the last 10 recorded sensor readings.

### File
[Homework 5 file](homework5/homework5.ino)

<details><summary><h3>Photo</h3></summary>

![Homework 5 photo](homework5/photo.jpg)

</details>

### Video
https://www.youtube.com/watch?v=MPQRv6WlfSU

## Homework 6
### Requirement
Develop a Bomberman-like game using an 8x8 LED matrix. Implement various features such as a menu, animations, etc.

### File
[Homework 6 file](homework6/homework6.ino)

<details><summary><h3>Photo</h3></summary>

![Homework 6 photo](homework6/photo.jpg)

</details>

### Video
https://www.youtube.com/watch?v=tHLFfbs_FkI

## Homework 7
### Requirement
Implement an LCD menu for the game developed in [homework 6](#homework-6). Implement an About section with information such as game name, author, and github link. <br>
There should also be a settings section, where the user can change the LCD or matrix brightness. The LCD should display relevant information during the game and an appropriate message upon ending.

### File
[Homework 7 file](homework7/homework7.ino)

<details><summary><h3>Photo</h3></summary>

![Homework 7 photo](homework7/photo.jpg)

</details>

### Video
https://youtu.be/bRdJDQXD-AQ
