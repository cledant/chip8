# chip8_emu

Yet another chip8 emulator  

![screenshot](./screenshots/c8_logo.png?raw=true)  
(Chip8 emulator logo rom from Garstyciuks)  

## Features

`chip8_emu` can run Chip8 / Superchip8 roms.  
It also features a Cosmac VIP compatibility mode.  
Background and sprites colors can be customized.  
Buzzer frequency can also be set.  

## Compiling

Make sure you have `chip8_emu` dependencies by running `git submodule init && git submodule update`.  
You may compile `chip8_emu` binary by running `mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make -j8`.  

## Usage

`./chip8_emu [-b BACKGROUND_COLOR][-c CYCLE_PER_FRAME][-h | --help][-i SILENT_COLOR]`  
`[-k | --keys][-n][-m][-p SPRITE_COLOR][-r][-s SCALE][-t ROME_TYPE][-u BUZZER_TONE]`  
`[-z BUZZER_ACTIVE_COLOR][-A][-B][-W] ROM_FILEPATH`  

-b: Background RGB color in hex format. Default is 0x222222  
-c: Cycles per frames (Emulation speed). Default is 30.  
-h | --help: Display help.  
-i: Silent RGB color in hex format. Default is 0x000000  
-k | --keys: Display key mapping.  
-n: Will not save flag registers at exit. Only for SUPERCHIP8.  
-m: Mute buzzer sound.  
-p: Sprite RGB color in hex format. Default is 0x00bb00  
-r: Reset flag registers. Only for SUPERCHIP8 Roms.  
-s: Resolution scaling from original resolution. Default is 8.  
-t: Rom Type can be CHIP8_COSMAC_VIP, CHIP8, SUPERCHIP8. Default is CHIP8.  
-u: Buzzer tone in Hz. Default is 2000 Hz  
-z: Buzzer RGB color in hex format. Default is 0x555555  
-A: Warn when instruction are not even aligned.  
-B: Activate SUPERCHIP8 quirk on BNNN instrution.  
-W: Change sprite drawing behaviour from clipping to wrapping