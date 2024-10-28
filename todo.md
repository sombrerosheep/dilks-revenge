# todo

## core IV
- [ ] texture
  - use SDL_Texture
  - Do I really need an abstraction for this?

## core V
- [ ] configurable "meters per unit"
- [ ] new ppu dimensions. need mpu first. Right now its 1:1
  | Resolution | PPU    | Player     | Screen (W) | Screen (H) | Ratio |
  | ---------- | ------ | ---------- | ---------- | ---------- | ----- |
  | 1280x720   | 10.67  | 4u (44px)  | 120u       | 67.5u      | 1.777 |
  | 1920x1080  | 16     | 4u (64px)  | 120u       | 67.5u      | 1.777 |
  | 3840x2160  | 32     | 4u (128px) | 120u       | 67.5u      | 1.777 |
  | 7680x4320  | 64     | 4u (256px) | 120u       | 67.5u      | 1.777 |

## resources IV
- [ ] integrate images

## resources V
- [ ] integrate sounds

## assets I
- [ ] asset packer
- [ ] save and load assets
  - font atlas
  - sprites
- [ ] loaded into resource manager

## assets II
- [ ] asset packer utility
  - game loads the pack.
  - utilty bundles the package

## waves II
- [x] as closer enemies are killed, move a further enemy to the vacan spot
- [x] wave done when all enemies are dead
- [x] player moved to 0,0 then camera pans, then wave starts
  - is this responsibility of the level?

## waves III
- [ ] when a wave enters, movement to/from each extent with everyone on screen
  - 5 enemies at the top move left to right, keeping everyone always on screen
  - if 1 outside enemy dies, then the group moves slightly more (etc...)

## levels I
- [ ] 10 waves per level
  - sequenced or seeded?
- [ ] 10 levels in the game

## levels II
- [ ] every 10th wave is a boss

## camera II
- [x] lerp camera when moving to wave direction
- [ ] small buffer outside of camera
  - will move with player but still constrained
- [ ] slightly move camera when moving
  - slight "tilt"

## player III
- [ ] refine movement; make the drift feel less slippery

## camera III (juice)
- [ ] small shake when player is hit 
- [ ] even smaller shake when enemy dies? 

## background I
- [ ] stars moving slowly in direction of wave

## background II
- [ ] slight parallax with camera movement 

## backgroud III
- [ ] on a boss level shows a planet
  - somewhere...maybe it moves?

## background IV
- [ ] smooth transitions between waves when switching directions 

## main menu
- [ ] play
- [ ] quit

## game over
- [ ] appears when player health is zero
- [ ] back to main menu
- [ ] quit

## bosses I
- [ ] boss scene
- [ ] make a big square appear

## bosses II: components
- [ ] warpy/relay -> spawns enemies every x.
  - max spawned enemies?
  - destorying the component stops spawning
- [ ] light turrets
  - fire rate Xms
- [ ] heavy turrets
  - fire rater Xms
- [ ] power station
  - if within X will shock you every Xms
  - otherwise, no damage. can be destroyed.
  - does it stun you?
- [ ] engines
  - no damage but can be destroyed.

## bosses III: big ship
- [ ] always traveling up direction
- [ ] appear for Xms in each wave direction
- [ ] top shows rear of ship
- [ ] left shows right side of ship
- [ ] right shows left side of ship
- [ ] bottom shows front of ship

## bosses IIIa: big ship
- [ ] components
  - [ ] turrets
  - [ ] engines

## bosses IV: space station
- [ ] similar to big ship
- [ ] moves in a direction (up->down, left->right etc)
  - simulates that you're in a world moving around
- [ ] destructible sections: can shoot off a section of the  

## bosses IVa: space station
- [ ] components
  - [ ] turrets
  - [ ] power stations

## bosses V: elites
- [ ] tougher ships
- [ ] lighter + heavier projectiles

## bosses Va: elites
- [ ] includes waves of regular enemies between waves of the elites

## bosses VI: moon base
- [ ] moves in a direction (up->down, left->right etc)
  - simulates that you're in a world moving around
- [ ] lots of turrets
- [ ] lots of power stations

## difficulty
- [ ] easy/med/hard  
- [ ] fix menu to select difficulty 
- [ ] enemy damage increases  
- [ ] margin of error decreases  

## gamepad I
- [ ] implement gamepad  
  - l-stick: move
  - r-stick: aim
  - r-rigger: shoot
  - start:  pause

## gameplay
- [ ] save progress 
	-  furthest level achieved and date achieved
- [ ] chapter select 
	-  start of any level, granted the previous was completed

# Thoughts and Ideas

## Ships/Upgrades
- do you get a ship upgrade ever?
- do you pick different ships?
- do ships have stats?
- do you get a new ship offer at each planet?
- do you get a new ship offer at certain planet?
- do you get a new upgrade/weapon at each planet?
- do you get a new upgrade/weapon at each planet?

# new enemy: drones
- swarms of 5 (or so)
- buzzing in circles as if they were bugs
- hard to hit but weak attack and defense

# done

## camera I
- [x] implement camera 
- [x] camera movement commands 
  - [x] to center
  - [x] to left
  - [x] to right
  - [x] to top
  - [x] to bottom

## core I
- [x] game clock
  - need ms [uint] per frame
  - need s [float] per frame

## core II
- [x] scale
  - size of ship and enemies and other entities by "meters"
  - all speeds by m/sec

## core III
- [x] game memory
  - layout
  - only one up-front allocation

## enemies I (light)
- [x] fires small bullets every 1000-1200ms 
- [x] blue square

## entities I
- [x] entity manager
- [x] projectiles as entities
- [x] player as entity

## fonts I
- [x] load and use font
  - store as a single texture atlas

## misc
- [x] const pointer parameters and when necessary
- [x] implement GetBounds for all entities
- [x] implement DREV_DRAW_BB for all entities
- [x] SDL implementaitons of stdlib things
- [x] primitive types names?
  - typedefs for stdint -> my-types
  - uint32_t u32
  - int64_t  i64
  - float    f32
  - double   f64

## player I
- [x] movement
- [x] small drift after moving?
- [x] player aim with mouse
- [x] player constrained by camera

## player II
- [x] player has health
- [x] player shoot with LMB 
- [x] player shoots bullets 

## resources I
- [x] resource manager
  - global singleton for entities to access as needed
  - keeps the entity interface clean to abstract later
- [x] integrate camera

## resources II
- [x] integrate input/controller

## resources III
- [x] integrate fonts

## waves I
- [x] select a waves direction (up/down/left/righ)
- [x] get group of enemies (Sequenced or seeded?)
- [x] enter as a group (offscreen) stopping at a specific place on-screen

## collisions I
- [x] enemy bullets reduce player health
- [x] player bullets reduce enemy health
- [x] player/enemy collisions destroy both

## enemies II (heavy)
- [x] fires big bullets every 1600-2000ms 
- [x] red square 

## enemies III
- [x] aim has a margin of error  
