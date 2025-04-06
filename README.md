# mymelodisc
ECE243 Project @ University of Toronto

This project is part of our **Computer Systems** course at the University of Toronto, where we explored the integration of FPGA hardware and software systems. The project was developed using a DE1-SoC board and a **NiosV/ RISC-V** processor, coded entirely in **C** and **Arduino API**. For this project, we designed and implemented a fully functional karaoke machine that incorporates real-time processing for audio, video, and lighting effects.

## Design Overview

This project implements a karaoke machine using two FPGA boards. The first allows users to select and play songs, display lyrics and background animations on a Video Graphics Array (VGA) monitor, and sync LED strips to the song's bpm. 

The design choice to use 2 FPGA boards was due to many processor cycles spent on displaying VGA and the song audio, which resulted in a slightly choppy song audio. Hence, the other audio output (microphone) was implemented with another FPGA board.

## Features
- **Song Selection**: Enable song selection between five available songs using *keyboard interrupt*.
- **Lyrics and Background**: Displays customized lyrics and background animations on the VGA using an *interval timer interrupt*.
- **LED Lights**: Used an Arduino Nano to control LED strips. The Arduino connects to the FPGA using GPIO pins, allowing them to work together to change colors in sync with the song’s tempo and vibe.
- **Microphone Audio**: Captures the user’s voice through a microphone and outputs it to a secondary speaker.
- **Song Control**: Pause or exit the song via the spacebar or escape key. Note that this feature only works with the VGA displays and light strips and not the background audio.
  
## Future Enhancements
- Add functionality to allow the pause and escape controls to also stop background music playback.
- Add more song options.
- Implement more complex lighting effects.
- Improve background audio processing and integration.

## Demos

Demo of Pause/Escape feature with the VGA & lights. Lights turn stay on when paused and turn off when escaped. 
https://youtube.com/shorts/BSLlZOSv11Q?si=GVUTnL2XxmN_baAR
