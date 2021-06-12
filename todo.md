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

## Enemy
* Types - list of enemy types to deploy?
* bullets die when enemy expires
  * when should they expire?
  * shared list of bullets?

## Collisions
* detection
* resolution

## General
* stretchy buffer for collections

## Chores
* window size out of pre-processor