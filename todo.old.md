### more rail thoughts
- add patters:
  - drop-down, reverse direction, increase speed (space invaders style)
  - sine-wave/circles l->r
  - repeat options
    - bonus enemies (like space invaders) that just do a fly by once

- consistent start/end position? Yes?
- rate/speed from start to end? Consistent? Stops at end until enemies are 0?
  - rail speed based on difficulty
  - enemy speed based on difficulty
- do enemies die at the end or "bounce back"?
- is each level scripted or randomized?
  - can we get good randomness with only 4 sides?

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

## Chores
- window size out of pre-processor

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
- parallaxing to where the enemies are...

## bullets fade as their health drops
- can't use a linear fade.
- need to still see the bullets until the very end
- final fade after health = 0?

## Original plan
- enemies come from a specific direction in waves.
- some waves could be multiple directions as difficulty increase
