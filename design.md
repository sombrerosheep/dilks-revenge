
## Plot
Aliens captured your kids. You took a ship and are headed after them.

## Gameplay
- [x] Player will survive a number of waves of enemies while traveling to a planet.
  - [x] gameplay to/from each planet is a "level" #levelsI
  - [x] a wave is not complete until all enemies are killed #wavesI
- [x] Once you reach a planet:
  - [x] boss fight (planetary defences) #levelsII
  - you'll find your family is not there (your princess is in another castle)
  - [x] How many planets do you travel before you get to them?
    - [x] To start; 100 waves. Every 10 is a new planet ##levelsI
    - Reach the 10th planet (100 waves) and you get your family back
- [x] Each set of waves is a group of enemies in a specific direction (up/down/left/right) #wavesI
- [x] game camera will pan to the direction the wave is coming from. #cameraI
  - [x] player always at 0,0 to start waves. #wavesI
  - or can parallax be used?
  - gives breathing for the player.
  - allows them to creep closer (urgency)
- [x] game background parallax's stars in the direction of the wave #backgroundI
  - [x] animation to go from one direction to another #backgroundIV

## Player

### Movement

- [x] player moves freely constrained only by the camera bounds #playerI
- [x] player aim uses mouse position (left mouse-button to shoot) #playerII
  - [x] controller uses right joystick (right trigger to shoot) #gamepadI
- [x] Can there be som ebit of extra buffer of the camera that eases in and out? #cameraII
- [x] as a new level starts, the player is gently moved #wavesI
  - [x] center opposite edge of the wave's direction #wavesI
    - [x] plus some buffer; not flush against the opposite edge #wavesII

### Behavior

- player must dogdge enemy projectiles and enemies
- [x] player health is reduces by projectile contact #collisionsI
- [x] player is dead when colliding with enemy #collisionsI

## Enemies

### standard wave
- [x] start with 2 types of enemies, light and heavy
  - [x] light shoots often with less damage #enemiesI
  - [x] heavy shoots less-often with more damage #enemiesII

### boss
- [x] a boss fight at each "planet" #bossesI #levelsII

#### types
- [x] big ships #bossesIII
  - [x] consumes entire side of play area #bossesIII
  - [x] destroy ship components turrets, power stations, engines, etc.. #bossesIIIa
  - [x] appears in different wave directions: bottom = front of ship, left = righ-side #bossesIII
- [x] space stations #bossesIV
  - [x] similar to big-ships #bossesIV
  - [x] destrucible components (turrets, etc..) #bossesIVa
  - [x] destructible sections (like components but could be destroyed before components) #bossesIV
- [x] smaller waves of elite forces #bossesV
  - [x] faster, more rapid waves #bossesVa
- [x] moon bases #bossesVI
  - [x] base moves in a direction simulating a full world #bossesVI

#### components
- [x] warps/relays that spawn regular enemies. #bossesII
  - [x] must destroy the spawn-component #bossesII
  - launch bays of ships/stations
  - special static enemy for other boss fights
- [x] turrets #bossesII
- [x] power stations #bossesII
- [x] engines #bossesII

### Movement

- [x] each wave drops in enemies #wavesI
- [x] move in from off-screen until they reach a set point #wavesI
- [x] once in position move left to right until they're all dead #wavesII

## questions
- Free/collect allies (ala Jackal)
- Free allies as other ships to help?
- Do power station components damage or stun you?
- are there pickups?
- are there upgrades after each level?
  - This may make a new game+?
- does player regenerate health between waves?
- does player regenerate health between levels?
- what does simultaneous directions look/play like?

## later questions
- Once you reach the end do you fly 100 levels back?
  - is this new-game+?
  - what makes the next 100 levels worth playing again?
  - are bosses a partially rebuilt version of what they were?
    - meaning most difficult is the last boss. but in this case it'd have less time to rebuild, so it'd be easier? and the first few bosses would be more difficult.
