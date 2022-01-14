# Waternet for Game Boy and Game Boy Color.
Waternet is a puzzle game written for the original Gameboy and Gameboy color using the gbdk sdk. 

## Graphics 
Graphics (tiles), i created in [Game Boy Tile Designer](http://www.devrs.com/gb/hmgd/gbtd.html) and the titlescreen graphic is based on a modified title screen image from my waternet game i made for [gp2x](https://www.willemssoft.be/index.php?main=5&sub=6&action=productdetails&id=218) and [windows](https://www.willemssoft.be/index.php?main=46&sub=7&action=productdetails&id=220). I designed the title screen in the [Gimp Image Editor](https://www.gimp.org/) to just have the word waternet, the waterdrop and the menu box to fit it all on the Game Boy screen. Afterwards i used [Game Boy Png Converter](https://github.com/gingemonster/GameBoyPngConverter) to convert the titlescreen to an optimized tilemap and tiles. 

## Music
Music was made using [Online Sequencer](https://onlinesequencer.net/), i created single channel, non mixed music files and later converted this music to an array storing the frequencies of notes to be used on the gameboy. I got [this idea](https://www.gamedeveloper.com/programming/making-a-game-boy-game-in-2017-a-quot-sheep-it-up-quot-post-mortem-part-2-2-) from the [sheep it up](https://gamejolt.com/games/sheepitup/267335) game developed by Dr. Ludos. He stored the frequencies to be used for the music notes in a first array, containing the registers for the gameboy for channel 2. Later he referenced this array in another array to create the music itself with these notes. So i used the same system and all i had todo was convert the music from onlinesequencer i made to such array (by hand) as well.

You can find the music files i used for the game below:

* Game Music: https://onlinesequencer.net/2485064
* Title Music: https://onlinesequencer.net/2484977
* Level Done Tune: https://onlinesequencer.net/2484974

## Testing on Real Game Boy's
To test my rom on real Game Boy hardware i used a flash cartridge named EZ-Flash Jr. It allowed me to put my compiled rom on a sd card and the load that rom from the gameboy itself. I'm guessing these flash cardridges flash a certain chip on the fly or so when you select it or some other way, but it allowed me to test my game's on a real Game Boy and Game Boy Color. I highly advise anyone making Game Boy games to do something similar as initially everything looked fine on an emulator on pc but as soon as i tested on the real hardware i noticed some tiles were not really visible and i had to adapt the tiles i initially created.

## Cartridge Graphics
To create the cartridge graphics i used a [template](https://www.deviantart.com/armando92/art/Label-Template-for-Gameboy-Gameboy-Color-Carts-418758475) made by armando92 to start with in Gimp and then added different pictures i found on [cleanpng](https://www.cleanpng.com) to create the Cartridge Label and the logo i created using a [free logo creator](https://www.flamingtext.com/) site. You can see the end result below. 

* Game Boy (Color) - 4.2 x 3.7 cm.
![Waternet Cartridge Image](/cartridge_labels/waternet.gbc.png)

## Credits
Waternet game concept is based on the Net and Netslide game from the [Simon Tatham's Portable Puzzle Collection](https://www.chiark.greenend.org.uk/~sgtatham/puzzles/), it's my own implementation of these 2 game concepts

### Tutorials used
* GamingMonsters "How to create a gameboy game" video tutorials on [youtube](https://www.youtube.com/watch?v=HIsWR_jLdwo&list=PLeEj4c2zF7PaFv5MPYhNAkBGrkx4iPGJo&index=1)

### Code Credits
* Fading function for Game Boy Color is based on ZGB's implementation - https://github.com/Zal0/ZGB
* Music [implementation](https://www.gamedeveloper.com/programming/making-a-game-boy-game-in-2017-a-quot-sheep-it-up-quot-post-mortem-part-2-2-) based on system used in the [sheep it up](https://gamejolt.com/games/sheepitup/267335) gameboy game made by Dr. Ludos

### Cartridge Graphcis used
* [Game Boy Cartridge Template](https://www.deviantart.com/armando92/art/Label-Template-for-Gameboy-Gameboy-Color-Carts-418758475) - armando92
* [Water droplets](https://www.cleanpng.com/png-circle-graphic-design-brand-pattern-cartoon-fine-d-440349/) - Baure
* [Plumber Guy](https://www.cleanpng.com/png-commercial-cleaning-window-cleaner-maid-service-ja-4614244/) - Endara
* [Plastic Pipes](https://www.cleanpng.com/png-piping-and-plumbing-fitting-plastic-pipework-chlor-5780276/) - Jaumo

### Tools used:
* GBDK 2020 - https://github.com/Zal0/gbdk-2020/ 
* Visual Studio Code - https://code.visualstudio.com/
* BGB - https://bgb.bircd.org/
* Emulicious - https://emulicious.net/
* Game Boy Tile Designer - http://www.devrs.com/gb/hmgd/gbtd.html
* Game Boy Map Builder - http://www.devrs.com/gb/hmgd/gbmb.html
* Game Boy Png Converter - https://github.com/gingemonster/GameBoyPngConverter
* Gimp Image Editor - https://www.gimp.org/
* Online Sequencer - https://onlinesequencer.net/
* Music notes to Game Boy Frequencies chart - http://www.devrs.com/gb/files/sndtab.html
