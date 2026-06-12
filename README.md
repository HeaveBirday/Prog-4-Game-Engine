# Tron BattleTanks

## About the Game

Tron BattleTanks is a project based on the arcade game Tron 1982. The original Tron game features 4 different mini games, whilst this Project focuses only on the Battle Tanks mini game.
The player controls a tank shooting down enemy tanks while progressing through different levels. The game has a Single Player mode, a Coop mode and a Versus mode. 
In Coop mode there are two players working together. In Versus mode, the second player takes control over an Enemy Tank, and can shoot the Player Tank down.
The player starts with 3 extra lives, so 4 in total. The player can achieve highscores that are saved between sessions. Only the top 10 highscores overall get saved


## Controls

### Keyboard

W - Move Up
A - Move Left
S - Move Down
D - Move Right
Z - Rotate Turret Left
X - Rotate Turret Right
Space - Shoot
F1 - Skip Level
F2 - Mute Sound

### Controller

D-Pad - Movement
X - Rotate Turret Left
B - Rotate Turret Right
A - Shoot

## Engine Features

The project started from the Minigin template and was expanded throughout the semester.
Some of the systems I added are:

- Component system
- Scene management
- Collision system
- Input system with keyboard and controller support
- Event system
- Sound system
- Game state management
- Highscore saving/loading
- UpdateLoop

## Design Choices

### Component Pattern

I used a component oriented architecture because it allows me to build GameObjects out of smaller reusable pieces instead of creating large inheritance trees.
Examples include:

- TankComponent
- ShooterComponent
- CollisionComponent
- VelocityComponent

### Command Pattern

Input is handled through commands such as MoveCommand and ShootBulletCommand.
This makes it easy to support both keyboard and controller input without duplicating gameplay code.

### State Pattern

I use different states for:

- Main Menu
- Single Player
- Co-op
- Versus
- Game Over

This keeps the logic for each screen separated.

### Event System

I implemented an EventManager so objects can communicate without directly knowing about each other.
For example, enemy destruction and collision events are sent through the event system

### Threading

The sound system runs on a separate thread so playing sounds does not block the main game loop.
There are multiple allocated tracks, so they dont interfere with each other.


## Highscores

At the end of the game the player can enter a 3 letter name similar to old arcade games, by iterating through all letters.
Highscores are saved to a file and loaded again when the game starts.


## Git Repository

<https://github.com/HeaveBirday/Prog-4-Game-Engine>

## Credits

### Alessandro Manzini

Helped me understand and implement the CollisionComponent system. Also helped out and gave feedback on bullet bouncing calculations.

### Rani Nagels

Helped me a lot throughout the project, especially with understanding CMake, splitting the project into a game and engine project, and general development advice.

### Nils Hammond

Provided feedback and guidance for the InputManager implementation.

## Author

Georgi Georgiev
DAE Game Dev
Programming 4 Exam Project