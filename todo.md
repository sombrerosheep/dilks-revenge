## rail controller
* Config
  * Should this be set per rail or across all rails? (or both?)
    * some of the options below only make sense if at the manager level not the rail level
  * Placement strategy
    * Random - each rail gets an enemy at random times
    * Random_Spread - random rail gets enemy at random timing
    * static - add enemy at set interval
    * static_spread - add enemy to random rail at set interval
  * Population control
    * Min - each rail has min number of enemies
    * Min_Spread - min num of active enemies across all rails

### more rail thoughts
* rails move towards the player?
* add patters:
  * drop-down, reverse direction, increase speed (space invaders style)
  * sine-wave/circles l->r
  * repeat options
    * bonus enemies (like space invaders) that just do a fly by once

## rail names
- are these necessary anymore?
- if the manager is handling them why would we ever need to retrieve one and by name?

## why play?
- is there score?
- is score based on survival time?

## difficulty
- is there a selectable difficulty?
- What does this influence?
  - player bullet affectiveness?
  - number of enemies?
  - rater of enemies?
  - attach/defence of enemies

## Enemy
* Types - list of enemy types to deploy?

## Collisions
* detection
* resolution

## Chores
* window size out of pre-processor

## particles
- for all the collisions

## random darting enemy (like galaga)
- per rail_manager?
- do we want this to happen multiple times?

## left aim
- does the ship "need time to turn"?
- could add difficulty or be a punishment?

## What does the shield really do?
- does it deplete?
- how does it charge?

## space background
- parallaxing?
- parallaxing to where the enemies are?

## resolve valgrind detected leaks

## bullets fade as their health drops
- can't use a linear fade.
- need to still see the bullets until the very end
- final fade after health = 0?

# what this is

## Original plan
* enemies come from a specific direction in waves.
* some waves could be multiple directions as difficulty increase


