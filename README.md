# cs4850-lab-states

## Instructions

In this lab you'll use finite state machines to create behavior in games.



### Part 1 - Setup

Look over the code for the assignment.  There is a class
`StateComponent` that implements a component with the basics of a
state machine.  There are two nested classes in StateComponent: State
and Transition, which represent those in the state machine.  The state
machine has a current state, on which it calls `update` each update.
For all the transitions going out of the current state, it calls
`shouldTrigger` to see if that transition should trigger and the state
machine should transition to another state.

The state and transtion subclasses themselves are in the
`StatesAndTransitions.*` files.

In the adventure game, the enemy has been written to use a state
machine, so that in addition to patrolling it will chase the player if
the player gets closeby. The state machine is set up like this:

<img src=README-statemachine1.png width=250/>



### Part 2 - Implementing a State

The enemy isn't moving around yet. Implement `PatrolState::update` so
that the enemy in adventure patrols back and forth near its starting
location.

When complete, the enemy in adventure will patrol, but not chase the player.



### Part 3 - Implementing a Transition

Now the enemy patrols, but it doesn't chase the player. Implement `ObjectProximityTransition::shouldTrigger`.

When complete, the enemy in adventure will patrol, chase the player, and return to its starting location.



### Part 4 - Adding Another Enemy Behavior

Now that the enemy in the adventure game is working, let's update the
enemy in the jump game.  We can update the enemy to patrol in a
triangle using the states and transitions already implemented if we
set up a starte machine like this:

<img src=README-statemachine2.png width=250/>

Update `JmpEnemy::JmpEnemy` to set up a StateComponent that moves the
enemy in a triangle (it doesn't have to chase the player).



### Part 5 - Extensions (CS5850 only)

Choose and implement **at least one** of the following extensions (you can do more if you like):
- Update the jump enemy's state machine so that it will dash briefly toward the player if nearby, then continue in its triangle pattern (using only the states/transitions from the base assignment).
- Add another state and a second enemy to both games that uses it.
- Add another transition and a second enemy to both games that uses it.
- Change the enemy's color dependinng on which state it's in.
- Implement a behavior tree and use it to control an enemy.
- (If you have an idea for another extension check with me before implementing it.)



### Part 6 - Meta

Fill out `meta.txt` with your answers to the questions about the assignment in the file (including the extension(s) you implemented).



## Rubric

3 pt: Part 2 State implemented

3 pt: Part 3 Transition implemented

3 pt: Part 4 Enemy in jump game uses state machine

1 pt: Part 5 extension complete **(CS5850 only)**

1 pt: Part 6 meta answers complete

+0.5 pt: implementing one additional extension beyond requirements

~~-X: general assignment guidelines~~
# game-engines-states
# game-engines-states
