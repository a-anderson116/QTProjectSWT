# Use Cases – Snake Game

---

## Actors

| Actor    | Description                                                  |
|----------|--------------------------------------------------------------|
| Player 1 | Primary player; always present in both game modes            |
| Player 2 | Second player; active only in Multi-Player mode              |
| System   | The Qt application (timer, renderer, collision engine)       |

---

## UC-01  Launch Game

**Actor:** Player 1
**Precondition:** Application is installed and running.
**Main Flow:**
1. Player starts the application.
2. System displays the Main Menu (mode, difficulty, controls).
3. Player selects Single Player or Multi Player.
4. Player selects difficulty: Slug / Worm / Python.
5. Player presses **START GAME**.
6. System initialises the board, spawns the snake(s) and one apple, starts the game loop.

**Postcondition:** Game is running.

---

## UC-02  Move Snake (Single Player)

**Actor:** Player 1
**Precondition:** Game is in the Running state.
**Main Flow:**
1. Player presses W / A / S / D **or** Arrow key.
2. System records the new desired direction (ignoring 180° reversals).
3. On the next timer tick, system moves the snake one cell in that direction.
4. System repaints the board.

**Alternative:** Player presses the same direction key that is already active → no change.

---

## UC-03  Move Snakes (Multi-Player)

**Actor:** Player 1, Player 2
**Precondition:** Multi-Player game is running.
**Main Flow:**
1. Player 1 presses W / A / S / D to steer Snake 1.
2. Player 2 presses Arrow keys to steer Snake 2.
3. System processes both direction inputs independently.
4. On each timer tick, system moves both snakes simultaneously.

---

## UC-04  Eat an Apple

**Actor:** System (triggered by snake movement)
**Precondition:** Game is running; at least one apple is present on the board.
**Main Flow:**
1. Snake's head moves onto the cell containing the apple.
2. System increments the snake's score by 10 points.
3. System schedules the snake to grow by one segment on the next tick.
4. System spawns a new apple at a random unoccupied cell.
5. System updates the score display.

---

## UC-05  Pause / Resume Game

**Actor:** Player 1
**Precondition:** Game is in the Running state.
**Main Flow:**
1. Player presses **P** or **ESC**, or clicks the ⏸ Pause button.
2. System stops the game-loop timer.
3. System renders a "PAUSED" overlay on screen.
4. Player presses **P** / **ESC** again, or clicks **▶ Resume**.
5. System restarts the timer and removes the overlay.

**Postcondition:** Game resumes from where it was paused.

---

## UC-06  Detect Collision and End Game

**Actor:** System
**Precondition:** Game is running.
**Main Flow:**
1. On each timer tick the system moves the snake(s).
2. System checks for:
   - **Wall collision** – head is outside the board boundaries.
   - **Self-collision** – head occupies any body segment.
   - **Inter-snake collision** (Multi-Player) – head of one snake occupies any segment of the other snake.
3. If a collision is detected the system stops the timer.
4. System determines winner/loser (Multi-Player) or shows "GAME OVER" (Single Player).
5. System renders the Game-Over overlay with final scores.
6. System emits the `gameOver` signal.

**Extension (head-to-head):** Both snakes collide simultaneously → "DRAW!" is shown.

---

## UC-07  Restart Game

**Actor:** Player 1
**Precondition:** Game-Over overlay is visible.
**Main Flow:**
1. Player presses **R**.
2. System resets both snakes to their initial positions, clears scores, spawns a new apple.
3. System restarts the timer using the same mode and difficulty as the previous game.
4. System repaints the board.

---

## UC-08  Return to Main Menu

**Actor:** Player 1
**Precondition:** Game is running, paused, or game-over.
**Main Flow:**
1. Player presses **M** (during game-over overlay) or clicks ◀ Menu button.
2. System stops the timer.
3. System switches the stacked widget to the Main Menu screen.
4. System enables difficulty and mode controls.

---

## UC-09  Select Difficulty

**Actor:** Player 1
**Precondition:** Main Menu is visible; no game is in progress.
**Main Flow:**
1. Player selects one radio button: **Slug** (200 ms/tick), **Worm** (120 ms/tick), or **Python** (70 ms/tick).
2. System records the chosen difficulty.
3. When game starts (UC-01), system uses this interval for the QTimer.

---

## UC-10  Win (Multi-Player)

**Actor:** System
**Precondition:** Multi-Player game is running.
**Main Flow:**
1. One snake dies (UC-06).
2. The surviving snake is declared winner.
3. System displays winner name and both scores on the overlay.
4. Players may restart (UC-07) or return to menu (UC-08).
