# bugs

# todo

## system

- [ ] configurable "meters per unit"
- [ ] new ppu dimensions. need mpu first. Right now its 1:1

| Resolution | PPU   | Player      | Screen (W) | Screen (H) | Ratio |
| ---------- | ----- | ----------- | ---------- | ---------- | ----- |
| 1280x720   | 10.67 | 4u (44px)   | 120u       | 67.5u      | 1.777 |
| 1920x1080  | 16    | 4u (64px)   | 120u       | 67.5u      | 1.777 |
| 2560x1440  | 21.3  | 4u (85.3px) | 120u       | 67.5u      | 1.777 |
| 3840x2160  | 32    | 4u (128px)  | 120u       | 67.5u      | 1.777 |
| 7680x4320  | 64    | 4u (256px)  | 120u       | 67.5u      | 1.777 |

- this was a good execise to wrap my head around, however, it did take awhile
  to fully understand and what I ended up with may not be as useful as I wanted.
  we shall see!

## resources V

- [ ] integrate sounds

## assets III

- [ ] asset packer
- [ ] save and load assets
  - font atlas
  - sprites
- [ ] loaded into resource manager

## assets IV

- [ ] asset packer utility
  - game loads the pack.
  - utilty bundles the package

## waves IV

- [ ] when a wave enters, movement to/from each extent with everyone on screen
  - for example: 5 enemies at the top move left to right, keeping everyone always on screen
  - space invaders style
- [ ] enemies move until furthest enemy is within X of the edge
  - if 1 outside enemy dies, then the group moves slightly more (etc...)
  - when column 0 is empty, then column 1 moves to the extent
  - when column 0-3 is empty, column 4 moves to the extent, etc...

## levels II

- [x] 10 waves per level
  - sequenced or seeded?
- [ ] 10 levels in the game

## levels III

- [ ] every 10th level is a boss(?)

## camera III (juice)

- [ ] small shake when player is hit
- [ ] even smaller shake when enemy dies?

## camera IV

- [x] lerp camera when moving to wave direction
- [ ] small buffer outside of camera
  - will move with player but still constrained
  - player is constrained to the same bounds but camera will extend
    an additional bit (more than player but not much more.)
- [ ] slightly move camera when moving
  - slight "tilt"

## player III

- [ ] refine movement; make the drift feel less slippery

## backgroud III

- [ ] on a boss level shows a planet
  - somewhere...maybe it moves?

## background IV

- [ ] smooth transitions between waves when switching directions

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
  - start: pause

## gameplay

- [ ] save progress
  - furthest level achieved and date achieved
- [ ] chapter select
  - start of any level, granted the previous was completed

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

## core IV

- [x] texture
  - use SDL_Texture

## core V

- [x] sprites

## core VI

- [x] args

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
  - int64_t i64
  - float f32
  - double f64

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

## waves II

- [x] as closer enemies are killed, move a further enemy to the vacan spot
- [x] wave done when all enemies are dead
- [x] player moved to 0,0 then camera pans, then wave starts
  - is this responsibility of the level?

## waves III

as an enemy dies:

- [x] if the immediate one behind dies, move it forward
- [x] if the immediate one behind dies, look for closest based on that position.

## collisions I

- [x] enemy bullets reduce player health
- [x] player bullets reduce enemy health
- [x] player/enemy collisions destroy both

## enemies II (heavy)

- [x] fires big bullets every 1600-2000ms
- [x] red square

## enemies III

- [x] aim has a margin of error

## main menu

- [x] play
- [x] quit
- [x] mouse

## pause

- [x] continue
- [x] menu
- [x] quit

## game over

- [x] appears when player health is zero
- [x] back to main menu
- [x] quit

## levels I

- levels control enemy waves and iterations
- [x] levels start waves when starting
- [x] new level starts when one ends

## resources IV

- [x] integrate images

## assets I

- [x] asset manager
- [x] handling assets with multiple ppu's

## assets II

- [x] Loading assets with multiple ppu's
  - when an entity requests its texture, it needs to know which version to request?
  - or does the asset library need refactored to return the asset for the current ppu?
    - Sprites have a pointer reference to the Texture. This would need refactoring to fetch
      the texture at each draw
  - how will each entity know what the ppu is?
- [x] Implemented by removing the concern from the entity. the asset mangaer will load
      assets based upon the system config.

## background I

- [x] stars moving slowly in direction of wave

## background II

- [x] slight parallax with camera movement

## background IIa

- [x] Update graphics
  - colors:
    - white (#ffffff)
    - yellow (#e3e679)
    - red (#bf0600)
    - blue (#005a7e)
  - brush: stamp sparkles
  - near: 200px
  - mid: 100px
  - far: 45px

# bugs

- [x] start a level, pause and go back to menu
  - game does not properly reset.
  - camera focus is still set
  - player is not at the center
- [x] game over screen allows you to pause
