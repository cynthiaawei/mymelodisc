# mymelodisc
ECE243 Project @ University of Toronto

MyMelodisc is a project we developped for our **Computer Systems** course at the **University of Toronto**, where we explored the integration of FPGA hardware and software systems. The project was developed using a DE1-SoC board and a **NiosV/ RISC-V** processor, coded entirely in **C** and **Arduino API**. For this project, we designed and implemented a fully functional karaoke machine that incorporates real-time processing for audio, video, and lighting effects.

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

Demo of our friend using MyMelodisc for all 5 songs.
https://youtu.be/GxU2Z60IDpA?si=TU1yciVmzeLulIzQ

## Attribution Table

## 👥 Final Attribution Table

| **Task**                                | **Responsible**       | **Description**                                                                                                                                             |
|----------------------------------------|------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------|
| VGA conversions from PNG to arrays     | Cynthia                | Designed all PNG images by hand using canva, then used a Python program ([shared by a classmate]([https://github.com/Ken-2511/ECE243_ImageConverter]) to convert them to C arrays.             |
| Timer                                  | Cynthia                | Created an interrupt-driven timer that triggers at the BPM of the selected song.                                                                            |
| VGA                                    | Cynthia                | Displayed lyrics in the correct order, only when needed (e.g., at the end of a phrase), while the background syncs to the song's beat.                      |
| GPIO / Parallel Ports                  | Cynthia                | Used GPIO ports to send an enable signal from the keyboard interrupt to the LED lights.                                                                     |
| Arduino / LEDs                         | Cynthia                | Programmed LEDs to switch between two colors based on the BPM and theme of the song.                                                                        |
| Keyboard                               | Jenny                  | Developed an interrupt-driven keyboard program.                                                                                                              |
| Audio conversions from .mp3 to arrays  | Jenny                  | Converted `.mp3` files to `.wav` and then to C arrays using Linux tools and C programs. Played them through the DE1-SoC's audio port.                        |
| Partial Integration 1                  | Cynthia                | Integrated keyboard, mic, and VGA.                                                                                                                           |
| Partial Integration 2                  | Jenny                  | Integrated audio and VGA.                                                                                                                                    |
| Partial Integration 3                  | Cynthia                | Integrated VGA and LEDs.                                                                                                                                    |
| Full Integration                       | Jenny + Cynthia        | Complete integration of Audio, Keyboard, Microphone, VGA, LEDs, and FPGA system.                                                                            |
