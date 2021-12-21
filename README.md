# Doomed in Led's Labyrinth <br> An labyrinth matrix game on Arduino

## Description (lore) 

Led is a crazy magician that threw you in his magic labyrinth, in which he has a lot of his minions that are trying to mercilessly kill you.  
Led made the labyrinth such that whenever you enter a new room, he actually teleports you to a random room in the labyrinth, far away from an exit!  
You have to kill a lot of his minions and to make him teleport you through a lot of rooms so that his magic energy runs out and eventually get to an exit when he's too weakened and can't keep you away anymore.  
Be aware each health point matters, since there is a lot to overcome!


## Application functionality  

You can interact with the application with the joystick and optionally use the extra button added for better user experience.  

### Intro screen  

In the introduction screen you are shown the title of the game.  
You can skip it by pressing the button or wait the short timer it has!  

### The menu  

The menu can be scrolled with the joystick movements of up and down (on y axis).  
There are 4 main buttons in the menu:  

* New Game   - starts a new game
* Highscores - shows top 3 highscores
* Settings   - game settings configuration
* About      - prints more information about the game (creator and github)  

The settings also has multiple tabs that can be scrolled through:

* Name          - set the player name
* Difficulty    - set the difficulty
* Contrast      - set the lcd display contrast
* Brightness    - set the lcd display brightness
* Map Bright.   - set the map brightness (matrix)  

## Gameplay  

Once you start the game the theme song will start playing, until the game finishes.  

#### LCD display

On the lcd display in gameplay you will be shown your name, and the current health points you have.  
Also you will be displayed the current score, and number of enemies left to kill (alternating at 2 seconds) and when an enemy is the current room, his current health points.  
You also have other 3 game screens depending of the game state:

* Game over screen
* Game won screen
* New highscore screen

#### Matrix 

Once you start a new game you can use the joystick to move in the 4 direction and to attack use the button, it will create a 3 blocks horizontal <a href="https://cdn.discordapp.com/attachments/902874706854682637/922806548399554561/unknown.png" target="_blank">attack</a> relative to player's direction.  
A room always has a <a href="https://cdn.discordapp.com/attachments/902874706854682637/922804320074870824/unknown.png" target="_blank">2 blocks(leds) tick wall</a>, and openings in the wall that represent <a href="https://cdn.discordapp.com/attachments/902874706854682637/922804847487643688/unknown.png" target="_blank">passages</a> to other rooms, if you step "outside" the map through that passage, you will enter the next room.  
<a href="https://cdn.discordapp.com/attachments/902874706854682637/922805706149736458/unknown.png" target="_blank">Enemies</a> are spawned with a specific chance, depending on difficulty, you can kill it or just go right past it. They have a timed <a href="https://cdn.discordapp.com/attachments/902874706854682637/922805495687946240/unknown.png" target="_blank">attack</a> that attacks all around them, dealing damage to anyone in the proximity.  
Once you kill all the enemies (enemies left counter reaches 0) an <a href="https://cdn.discordapp.com/attachments/902874706854682637/922807822436143114/unknown.png" target="_blank">exit</a> will spawn randomly and once you go through it, you get away and "win" the game.  
If you die along the game or if you exit successfuly, the game ends having your screen updated correspondingly, and if you have have a new highscore you will be moved to that screen after, else to the main menu.  

## Hardware

The hardware is a little bit more complex, but we have some main parts:  

![Arduino_image](https://cdn.discordapp.com/attachments/902874706854682637/922812799170785280/unknown.png)  

1. Joystick - used for movement/scrolling
2. LCD Display - displays menu
3. 8x8 Matrix - displays game
4. Button   - same button as joystick button (better UX)
5. Potentiometer - Controlling buzzer intensity
6. MAX7219 driver - For easier matrix led control
7. Buzzer   - theme song  

Items used overall:  

* 1 lcd display
* 1 8x8 matrix
* 1 MAX7219 driver
* 1 joystick
* 1 button
* 1 buzzer
* 1 potentiometer
* 2 capacitators
* 1 ceramic capacitator
* 2 330 resistors (for lcd backlight control + the one capacitator)
* 1 10k resistor (MAX7219 ISET pin)
* a bunch of cables and arduino ports (pin ports can be found in [Config.ino](Game/Config.ino))








