# Embedded Media Center – LPC1768 (MCB1700)

This project is a fully integrated **embedded Media Center** built for the LPC1768 microcontroller on the MCB1700 development board.  
It includes four interactive modules — **MP3 Player, Photo Gallery, Snake Game, and Poker Game** — all accessible through a user-friendly menu controlled via the onboard joystick and displayed on the GLCD.

The system demonstrates real-time processing, USB audio streaming, bitmap rendering, game logic, interrupt-driven tasks, and modular embedded system design.

---

## 🚀 Features

### **Main Menu**
- Central navigation hub
- Joystick-based input
- Clean UI with smooth transitions
- Loads each module independently

### **Photo Gallery**
- Displays RGB565 bitmap images stored as C arrays  
- LEFT/RIGHT joystick navigation  
- Fast rendering and clean screen transitions  

### **Snake Game**
- Real-time grid movement  
- Collision detection (walls + self)  
- Random food placement  
- Game Over screen with score + high score  
- Fully joystick-controlled  

### **Poker Game**
- Full deck generation and shuffling  
- Evaluates all major poker hands  
- Betting and chip system  
- Tested in desktop C environment before hardware adaptation  

### **MP3 Player**
- USB audio streaming using `USBAudio` framework  
- Timer0 interrupt outputs samples to DAC at 32 kHz  
- Potentiometer controls volume in real time  
- SELECT button safely exits the module  
- Custom LCD audio interface graphics  

---

## 🧱 System Architecture

The Media Center follows a **modular design**, with each feature implemented as an isolated component.  
Core shared elements include:

- GLCD driver  
- Joystick input functions  
- Framebuffer and screen-clearing routines  
- Random number generator  
- USB audio stack  

The **Menu module** acts as the controller, resetting the display and launching each component cleanly.

---

## 🔧 Hardware Requirements

- **MCB1700 Development Board**  
- LPC1768 Microcontroller  
- 3.2” GLCD  
- Onboard joystick  
- USB connection for MP3 streaming  
- Onboard DAC + Speaker  
- Potentiometer (for volume control)

---

## 📁 Repository Structure

