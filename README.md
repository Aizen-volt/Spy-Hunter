# Spy Hunter

The game goal is to drive down road and destroy other (enemy) vehicles. Project is based on the Atari 2600 version. More detailed information is available on the [Wikipedia](https://en.wikipedia.org/wiki/Spy_Hunter). The example of the gameplay is available on [Youtube](https://www.youtube.com/watch?v=DLyJS8mlCLY).



## Library
Project uses [SDL2 (2.0.7)](https://github.com/libsdl-org/SDL) library for both displaying graphics and calculating increment of time allowing to track the flow.
## Functionalities
- Displaying elapsed time and score during gameplay
- Saving and restoring game state. Saved games are identified by the time of save. When loading, the list of saved games is displayed
- Pausing the game
- Enemy cars that attack and overrun player
- Non-enemy cars that when destroyed halt the score counter for a while
- Player can shoot both enemy and non-enemy cars, resulting in different score outcomes
- Player can force off the road both enemy and non-enemy cars, resulting in different score outcomes
- In the beginning player has unlimited amount of cars. After that, number of cars is limited and the new ones are obtained by achieving score thresholds
- When the number of available cars is zero, game is over
- Power-ups - weapons appear on the road and when collected they temporarily (limited ammo) change the way shooting works (greater range, increased damage etc.)
- Storing and displaying best results list. Menu allows to display list sorted by points or by time
## Gameplay

<p align="center">
  <img src="exemplary-gameplay.png" alt="Exemplary gameplay" />
</p>
