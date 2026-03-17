# Glossary – Snake Game

| Term | Definition |
|------|-----------|
| **Apple / Food** | A single red circle placed randomly on the board that the snake must eat to grow and score points. |
| **Board** | The 30 × 28 grid of cells on which the game takes place. Each cell is 20 × 20 pixels. |
| **Cell** | The smallest unit of the board. Each segment of a snake occupies exactly one cell. |
| **Collision** | An event in which the snake's head occupies an illegal cell: a wall cell, one of its own body cells, or (in multiplayer) any cell of the opponent's body. |
| **Dead Snake** | A snake whose head has collided with a wall, its own body, or another snake. The game ends when any snake dies. |
| **Difficulty** | One of three speed settings that control how fast the game timer fires: Slug (200 ms), Worm (120 ms), Python (70 ms). |
| **Direction** | One of four cardinal directions (Up, Down, Left, Right) that the snake moves each tick. The snake cannot reverse 180° in a single step. |
| **Draw** | A multiplayer outcome where both snakes die on the same timer tick (e.g., head-to-head collision). |
| **Food** | See *Apple*. |
| **Game Loop** | A repeating QTimer signal (`timeout`) that drives snake movement, collision detection, and screen repaints at a fixed interval. |
| **Game Over** | The state reached when at least one snake has died. The game-over overlay is displayed with scores and options to restart or return to the menu. |
| **GameMode** | Either *SinglePlayer* (one snake, one player) or *MultiPlayer* (two snakes, two players on the same keyboard). |
| **GameWidget** | The Qt widget responsible for rendering the board, snakes, and food; owning the game loop timer; and processing keyboard input. |
| **GrowPending** | An internal flag on the Snake class that tells `move()` to keep the tail in place on the next tick, effectively growing the snake by one cell. |
| **Head** | The first element of the snake's body list; the leading cell that moves first and collides with obstacles. |
| **Head-to-Head Collision** | A multiplayer event where both snakes' heads occupy the same cell after a tick. Both snakes are declared dead, resulting in a Draw. |
| **High Score** | Not persisted across sessions in this version; the score is shown on the Game-Over overlay and cleared on restart. |
| **MainWindow** | The top-level QMainWindow that hosts the toolbar, the MenuScreen, and the GameWidget inside a QStackedWidget. |
| **MenuScreen** | The start-screen widget where the player selects Game Mode and Difficulty before starting a game. |
| **Multi-Player** | A two-player game mode where Player 1 uses WASD and Player 2 uses the Arrow keys. Both snakes share the same board. |
| **Paused** | A game state in which the timer is stopped and a "PAUSED" overlay is shown. No input is processed except the resume key. |
| **Pending Direction** | The direction recorded from the most recent key press, applied to the snake at the start of the next timer tick to prevent double-moves within a single tick. |
| **Python (mode)** | The hardest difficulty level. The game timer fires every 70 ms (~14 ticks/second). |
| **QTimer** | Qt's built-in timer class used as the game loop. When it fires `timeout()`, `GameWidget::onTimerTick()` is called. |
| **Score** | An integer counter incremented by 10 each time the snake eats an apple. Displayed in the score bar during play and on the Game-Over overlay. |
| **Score Bar** | A 40-pixel-tall header strip at the top of the GameWidget that shows the current score, snake length, difficulty, and control hints. |
| **Self-Collision** | An event where the snake's head occupies a cell already occupied by one of its own body segments. |
| **Segment** | One cell-sized unit of a snake's body. The snake gains a segment each time it eats an apple. |
| **Single Player** | A one-player game mode where the player controls a single snake using WASD or Arrow keys. |
| **Slug (mode)** | The easiest difficulty level. The game timer fires every 200 ms (5 ticks/second). |
| **Snake** | The player-controlled entity composed of a linked list of cell positions. The head leads, and the tail follows automatically on each tick. |
| **Spawn** | The act of placing the apple (or initialising the snake) at a valid random position on the board. The algorithm avoids all currently occupied cells. |
| **Tail** | The last element of the snake's body list. It is removed each tick (unless `GrowPending` is set), giving the illusion of movement. |
| **Tick** | One execution of the game loop, triggered by the QTimer timeout signal. Each tick moves the snake, checks collisions, and repaints the board. |
| **Wall** | The boundary of the board. If the snake's head moves outside the range [0, BOARD_WIDTH) × [0, BOARD_HEIGHT) it is considered a wall collision. |
| **Winner** | In Multi-Player mode, the snake that is still alive when the other snake dies. |
| **Worm (mode)** | The medium difficulty level. The game timer fires every 120 ms (~8 ticks/second). |
