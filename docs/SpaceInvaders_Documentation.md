# Space Invaders — Project Documentation

**Course:** Software Patterns and Teams
**Platform:** Qt 6 / C++ (Windows)
**Date:** 2026-03-25

---

## Table of Contents

1. [Use Cases](#1-use-cases)
2. [Sequence Diagrams](#2-sequence-diagrams)
3. [Class Diagram](#3-class-diagram)
4. [Glossary](#4-glossary)
5. [Test Cases](#5-test-cases)

---

## 1. Use Cases

### UC-01 — Start New Game

| Field        | Detail |
|--------------|--------|
| **ID**       | UC-01 |
| **Name**     | Start New Game |
| **Actor**    | Player |
| **Precondition** | Application is running; Main Menu is displayed |
| **Main Flow** | 1. Player clicks **PLAY** (or presses SPACE on the menu). <br>2. System initialises Wave 1 with 55 invaders (5 rows × 11 columns), 4 bunkers, Player ship with 3 lives, and score = 0. <br>3. System starts the game loop timer (60 fps). <br>4. Game screen is displayed; the player can now interact. |
| **Alternate Flow** | — |
| **Postcondition** | Game is in PLAYING state; invaders begin marching. |

---

### UC-02 — Move Player Ship

| Field        | Detail |
|--------------|--------|
| **ID**       | UC-02 |
| **Name**     | Move Player Ship |
| **Actor**    | Player |
| **Precondition** | Game is in PLAYING state |
| **Main Flow** | 1. Player holds **← / A** key. <br>2. System moves the ship 5 px left each tick; movement stops at left boundary. <br>3. Player holds **→ / D** key. <br>4. System moves the ship 5 px right each tick; movement stops at right boundary. |
| **Alternate Flow** | If the ship is already at a boundary, no movement occurs. |
| **Postcondition** | Player ship position is updated on screen. |

---

### UC-03 — Fire Player Bullet

| Field        | Detail |
|--------------|--------|
| **ID**       | UC-03 |
| **Name**     | Fire Player Bullet |
| **Actor**    | Player |
| **Precondition** | Game is PLAYING; no player bullet is currently active on screen |
| **Main Flow** | 1. Player presses **SPACE**. <br>2. System spawns a bullet at the centre-top of the player ship with upward velocity (-9 px/tick). <br>3. Bullet moves each tick until it hits an invader, bunker, or the top boundary. |
| **Alternate Flow** | If a player bullet is already on screen, pressing SPACE has no effect (one-bullet-at-a-time rule). |
| **Postcondition** | A white bullet travels upward from the player ship. |

---

### UC-04 — Destroy Invader

| Field        | Detail |
|--------------|--------|
| **ID**       | UC-04 |
| **Name**     | Destroy Invader |
| **Actor**    | Player (indirect — via fired bullet) |
| **Precondition** | A player bullet is active; at least one invader is alive |
| **Main Flow** | 1. Collision detection detects bullet rect intersects an invader rect. <br>2. Invader is marked dead; bullet is deactivated. <br>3. Score is incremented by the invader's point value (10 / 20 / 30 pts). <br>4. An explosion effect is rendered at the invader's position. <br>5. Invader step speed is recalculated for the remaining count. |
| **Alternate Flow** | If the bullet misses all invaders it continues until it exits the play area. |
| **Postcondition** | Invader is removed from the grid; score updated; speed potentially increased. |

---

### UC-05 — Invader Reaches Ground (Player Loses)

| Field        | Detail |
|--------------|--------|
| **ID**       | UC-05 |
| **Name**     | Invader Reaches Ground |
| **Actor**    | System (Invader AI) |
| **Precondition** | At least one invader is alive; game is PLAYING |
| **Main Flow** | 1. Each tick the system checks if any invader's bottom edge has crossed the ground line. <br>2. If so, the GAME OVER overlay is immediately displayed. <br>3. The game loop is stopped; final score is emitted to MainWindow. |
| **Alternate Flow** | — |
| **Postcondition** | Game transitions to GAME_OVER state. |

---

### UC-06 — Player Loses a Life

| Field        | Detail |
|--------------|--------|
| **ID**       | UC-06 |
| **Name**     | Player Loses a Life |
| **Actor**    | System (Invader bullet) |
| **Precondition** | An invader bullet is active; player has ≥ 1 life |
| **Main Flow** | 1. Invader bullet rect intersects the player rect. <br>2. Bullet deactivated; life counter decremented. <br>3. Explosion effect shown at player position. <br>4a. If lives > 0: player ship is re-centred; game continues. <br>4b. If lives == 0: UC-05 game-over flow is triggered. |
| **Alternate Flow** | — |
| **Postcondition** | Either the game continues with one fewer life, or GAME_OVER is triggered. |

---

### UC-07 — Destroy Bunker Section

| Field        | Detail |
|--------------|--------|
| **ID**       | UC-07 |
| **Name**     | Destroy Bunker Section |
| **Actor**    | Player or Invader (via bullets) |
| **Precondition** | A bullet (player or invader) hits a live bunker cell |
| **Main Flow** | 1. Bullet rect is tested against each live 5×5 cell of the bunker. <br>2. Overlapping cells are destroyed; adjacent cells have a 50% chance of also being destroyed (chunky erosion). <br>3. Bullet is deactivated. |
| **Alternate Flow** | If the bullet does not overlap any live cell, it passes through. |
| **Postcondition** | One or more bunker cells are removed; bunker appearance is updated. |

---

### UC-08 — Mystery Ship Appears

| Field        | Detail |
|--------------|--------|
| **ID**       | UC-08 |
| **Name**     | Mystery Ship Appears |
| **Actor**    | System (timed event) |
| **Precondition** | Game is PLAYING; mystery ship is inactive; countdown timer has elapsed |
| **Main Flow** | 1. Timer fires (900–1 500 ticks, random). <br>2. Mystery ship spawns off-screen edge, travels horizontally at speed 3 px/tick. <br>3. If the player shoots it, a random bonus (50–300 pts) is awarded and a bonus text floats up. <br>4. If the ship crosses the opposite edge without being shot, it deactivates silently. |
| **Alternate Flow** | — |
| **Postcondition** | Mystery ship is either destroyed (bonus score) or has exited. A new countdown begins. |

---

### UC-09 — Clear All Invaders (Level Complete)

| Field        | Detail |
|--------------|--------|
| **ID**       | UC-09 |
| **Name**     | Clear All Invaders |
| **Actor**    | Player |
| **Precondition** | All invaders have been destroyed |
| **Main Flow** | 1. System detects alive count == 0. <br>2. Level-complete overlay is shown for ~3 seconds. <br>3. Wave counter increments. <br>4. New invader grid is initialised (starting slightly lower, slightly faster). <br>5. Bunkers are fully restored. <br>6. Game resumes. |
| **Alternate Flow** | — |
| **Postcondition** | Next wave begins with increased difficulty. |

---

### UC-10 — View / Record High Score

| Field        | Detail |
|--------------|--------|
| **ID**       | UC-10 |
| **Name**     | View / Record High Score |
| **Actor**    | Player |
| **Precondition** | Game Over has occurred with a score that qualifies for the top-10 list |
| **Main Flow** | 1. System determines the score qualifies. <br>2. Name-entry overlay appears; player types their name (max 12 chars) and presses ENTER. <br>3. Score and name are saved to the top-10 list (persistent file). <br>4. Player can open **HIGH SCORES** from the main menu to review the list. |
| **Alternate Flow** | If score does not qualify, the game returns to the main menu without prompting. |
| **Postcondition** | Score list is updated and persisted; player can view rankings. |

---

## 2. Sequence Diagrams

### SD-01 — Player Fires and Kills an Invader

```
Player         GameWidget          Invader         HighScore
  |                  |                 |                |
  |---SPACE key----->|                 |                |
  |                  |--new Bullet()-->|                |
  |                  |                 |                |
  |  [each 16ms]     |                 |                |
  |                  |--bullet.move()  |                |
  |                  |--checkCollisions()              |
  |                  |    intersects? YES              |
  |                  |--inv.kill()---->|                |
  |                  |--bullet.deactivate()            |
  |                  |--player.addScore(pts)           |
  |                  |--add Explosion effect           |
  |                  |--updateDifficulty()             |
  |                  |--update() (repaint)             |
```

---

### SD-02 — Game Over Flow

```
Invader       GameWidget         MainWindow        HighScore
  |                |                  |                 |
  | [reaches       |                  |                 |
  |  GROUND_Y]     |                  |                 |
  |                |--checkWinLose()  |                 |
  |                |  gameOver=true   |                 |
  |                |  timer.stop()    |                 |
  |                |--emit gameOver(score)              |
  |                |                  |                 |
  |                |             onGameOver(score)      |
  |                |                  |--isHighScore()?->|
  |                |                  |<-- YES ---------|
  |                |--beginNameEntry(prompt)            |
  |                |  [player types name, ENTER]        |
  |                |--emit nameConfirmed(name)          |
  |                |             onNameConfirmed(name)  |
  |                |                  |--addScore(name,score)->|
  |                |                  |                 |--save()
```

---

### SD-03 — Wave Cleared → Next Wave

```
GameWidget                   MainWindow
    |                              |
    | aliveCount() == 0            |
    | levelComplete = true         |
    | levelTimer = 180 ticks       |
    |                              |
    | [180 ticks later]            |
    | wave++                       |
    | buildBunkers()  (restore)    |
    | initWave()      (new grid)   |
    | levelComplete = false        |
    | [game loop resumes normally] |
```

---

### SD-04 — Mystery Ship Spawn and Destruction

```
GameWidget (timer)           MysteryShip          Player
    |                            |                    |
    | mysteryTimer-- == 0        |                    |
    |--mystery.spawn(boardW,y)-->|                    |
    |                            |                    |
    | [each tick]                |                    |
    |--mystery.move(boardW)----->|                    |
    |                            |                    |
    | [player shoots]            |                    |
    |                            |<---SPACE------------|
    | bullet.rect intersects mystery.rect             |
    |--mystery.deactivate()----->|                    |
    | pts = mystery.pointValue() |                    |
    | player.addScore(pts)       |                    |
    | bonusText "+pts" floats    |                    |
```

---

## 3. Class Diagram

```
┌─────────────────────────────────────────────────────────────────────────┐
│                          MainWindow                                      │
│  - m_stack : QStackedWidget                                              │
│  - m_menu  : MenuScreen*                                                 │
│  - m_game  : GameWidget*                                                 │
│  - m_highScore : HighScore                                               │
│  + onPlayRequested()                                                     │
│  + onGameOver(score)                                                     │
│  + onNameConfirmed(name)                                                 │
└────────────────────────┬───────────────────────────┬────────────────────┘
                         │ owns                       │ owns
               ┌─────────▼──────────┐    ┌───────────▼──────────────────┐
               │   MenuScreen       │    │          GameWidget           │
               │  - m_blinkTimer    │    │  - m_player   : Player        │
               │  + paintEvent()    │    │  - m_invaders : QVector<Inv.> │
               │  + keyPressEvent() │    │  - m_bullets  : QVector<Bul.> │
               │  signals:          │    │  - m_bunkers  : QVector<Bunk> │
               │    playRequested() │    │  - m_mystery  : MysteryShip   │
               │    highScores..()  │    │  - m_highScore: HighScore     │
               └────────────────────┘    │  + startGame()                │
                                         │  + togglePause()              │
                                         │  + onTick()  [slot]           │
                                         │  - stepInvaders()             │
                                         │  - invaderShoot()             │
                                         │  - checkCollisions()          │
                                         │  - checkWinLose()             │
                                         │  signals:                     │
                                         │    gameOver(score)            │
                                         │    nameConfirmed(name)        │
                                         └──────────────┬────────────────┘
                                                        │ uses
          ┌─────────────────────────────────────────────┼──────────────────────┐
          │                         │                   │                      │
 ┌────────▼────────┐   ┌────────────▼──────┐  ┌────────▼────────┐  ┌─────────▼──────┐
 │    Player       │   │    Invader        │  │    Bullet       │  │    Bunker      │
 │ + WIDTH  = 52   │   │ + WIDTH  = 36     │  │ + WIDTH  = 3    │  │ + CELL  = 5    │
 │ + HEIGHT = 24   │   │ + HEIGHT = 26     │  │ + HEIGHT = 14   │  │ + COLS  = 16   │
 │ - m_x, m_y      │   │ - m_col, m_row    │  │ - m_vy (±vel)   │  │ + ROWS  = 10   │
 │ - m_lives       │   │ - m_type          │  │ - m_owner       │  │ - m_cells[10][16]│
 │ - m_score       │   │   (Small/Med/Lge) │  │   (Player/Inv.) │  │ + hit(rect)    │
 │ + moveLeft()    │   │ - m_alive         │  │ + move()        │  │ + isCellAlive()│
 │ + moveRight()   │   │ - m_frame (0/1)   │  │ + rect()        │  └────────────────┘
 │ + addScore(pts) │   │ + kill()          │  │ + isOffScreen() │
 │ + loseLife()    │   │ + pointValue()    │  │ + deactivate()  │
 └─────────────────┘   └───────────────────┘  └─────────────────┘

 ┌──────────────────────┐     ┌──────────────────────┐     ┌───────────────────────┐
 │    MysteryShip       │     │     HighScore         │     │  HighScoreDialog       │
 │ + WIDTH  = 60        │     │ + MAX_ENTRIES = 10    │     │ (QDialog)             │
 │ + HEIGHT = 22        │     │ - m_entries[]         │     │ + HighScoreDialog(hs) │
 │ + SPEED  = 3         │     │ + load()              │     └───────────────────────┘
 │ - m_active           │     │ + save()              │
 │ + spawn(boardW, y)   │     │ + isHighScore(score)  │
 │ + move(boardW)       │     │ + addScore(name,score)│
 │ + pointValue()       │     └──────────────────────┘
 └──────────────────────┘

Enumerations
────────────
  InvaderType { Large, Medium, Small }
  BulletOwner { Player, Invader }

Plain structs
─────────────
  ScoreEntry  { name:QString, score:int }
  Explosion   { x,y,timer,size : int }
  BonusText   { x,y,timer:int, text:QString }
```

---

## 4. Glossary

| Term | Definition |
|------|-----------|
| **Bunker** | One of four destructible green barriers positioned between the player and the invaders. Each bunker is an arch-shaped grid of 5×5 px cells that erode when hit by bullets. |
| **Collision Detection** | The process of checking whether two game-object rectangles (QRect) overlap each tick of the game loop. |
| **Difficulty Scaling** | The dynamic adjustment of the invader step interval and shooting interval in proportion to the number of remaining invaders. As fewer invaders remain, they move and shoot faster. |
| **Drop** | The vertical downward movement (18 px) made by all invaders when the rightmost or leftmost live invader reaches the screen boundary. |
| **Explosion** | A short-lived visual effect rendered at the position of a destroyed invader, player, or mystery ship. Represented by the `Explosion` struct with a countdown timer. |
| **Frame (Invader)** | A binary animation flag (0 or 1) that toggles on each lateral step, giving each invader two-frame walking animation. |
| **Game Loop** | A `QTimer` firing every 16 ms (~60 fps). Each tick processes input, updates positions, checks collisions, and triggers a repaint. |
| **Grid** | The 5-row × 11-column formation of 55 invaders that advances left and right, dropping each time it reaches a screen edge. |
| **Ground Line** | A horizontal green line near the bottom of the play area. If any invader crosses it the game ends immediately. |
| **High Score** | A sorted list of up to 10 player-name/score pairs persisted to a text file in the OS application-data folder. |
| **Invader Step** | One lateral movement of the entire invader grid (±8 px). Steps occur at a variable interval that decreases as the grid shrinks. |
| **Level Complete** | A 3-second overlay shown when all invaders are cleared. The wave counter then increments and a new grid is initialised. |
| **Lives** | The number of times the player ship can be hit before Game Over. Starts at 3; shown as small ship icons in the score bar. |
| **Mystery Ship (UFO)** | A red bonus craft that flies across the top of the screen at irregular intervals. Destroying it earns a random bonus of 50–300 points. |
| **Pixel Art** | 8×8 bit patterns rendered as scaled solid rectangles (pixel size = 3 or 4) to replicate the look of classic arcade sprites. |
| **Player** | The horizontally-moving spacecraft controlled by the user, occupying the bottom of the play area. |
| **Point Value** | The score awarded for each invader type: Large = 10 pts, Medium = 20 pts, Small = 30 pts. |
| **Repaint / `update()`** | Qt's mechanism for scheduling a `paintEvent`. Called once per tick to redraw the game canvas. |
| **Score Bar** | A 50 px header strip at the top of the window showing current score, wave number, and remaining lives. |
| **Sprite** | The visual representation of a game object drawn using the `drawSprite8` helper which interprets an 8-row `uint8_t` bit-pattern. |
| **Step Interval** | The number of game ticks between successive grid steps. Ranges from 55 (full grid) down to 2 (final invader). |
| **Wave** | A complete set of 55 invaders. Each wave starts with a slightly lower initial grid position and a slightly higher base speed than the last. |

---

## 5. Test Cases

### TC-01 — Application Launches to Menu

| Field | Detail |
|-------|--------|
| **ID** | TC-01 |
| **Description** | Verify the application opens and the Main Menu is displayed. |
| **Precondition** | Compiled binary is available. |
| **Steps** | 1. Launch `SpaceInvaders.exe`. |
| **Expected** | Main Menu is displayed with "SPACE INVADERS" title, PLAY / HIGH SCORES / QUIT buttons, blinking "PRESS SPACE TO PLAY" prompt, score guide showing all invader types. |
| **Pass Criteria** | All UI elements visible; no crash on startup. |

---

### TC-02 — Start Game from Menu (PLAY button)

| Field | Detail |
|-------|--------|
| **ID** | TC-02 |
| **Description** | Verify clicking PLAY transitions to the game. |
| **Steps** | 1. Launch app. 2. Click **PLAY**. |
| **Expected** | Game screen appears. Score = 000000, Wave = 1, Lives = 3 (three ship icons). 55 invaders in 5×11 grid. 4 bunkers visible. Player ship centred at bottom. |
| **Pass Criteria** | Invaders are marching; no crash. |

---

### TC-03 — Player Ship Moves Left and Right

| Field | Detail |
|-------|--------|
| **ID** | TC-03 |
| **Description** | Verify keyboard input moves the player ship. |
| **Steps** | 1. Start game. 2. Hold **← (Left Arrow)**. 3. Release; hold **→ (Right Arrow)**. |
| **Expected** | Ship moves left then right; stops at left boundary (x = 10) and right boundary (x = BOARD_W − WIDTH − 10). |
| **Pass Criteria** | Ship does not exit play area. |

---

### TC-04 — Player Fires One Bullet at a Time

| Field | Detail |
|-------|--------|
| **ID** | TC-04 |
| **Description** | Verify only one player bullet can be on screen at once. |
| **Steps** | 1. Start game. 2. Press **SPACE** once; observe bullet. 3. Press **SPACE** again before bullet exits. |
| **Expected** | Only one white bullet visible on screen at a time. Second press is ignored while first bullet is active. |
| **Pass Criteria** | No duplicate player bullets. |

---

### TC-05 — Player Bullet Destroys Invader

| Field | Detail |
|-------|--------|
| **ID** | TC-05 |
| **Description** | Verify a bullet correctly removes an invader and awards points. |
| **Steps** | 1. Start game. 2. Move ship under a bottom-row invader. 3. Fire (SPACE). |
| **Expected** | Bullet travels upward, intersects the invader; invader disappears; explosion effect renders; score increases by 10. |
| **Pass Criteria** | Score = 10 after first hit on a Large invader. |

---

### TC-06 — Invader Scoring Tiers

| Field | Detail |
|-------|--------|
| **ID** | TC-06 |
| **Description** | Verify score increments match invader type (10 / 20 / 30 pts). |
| **Steps** | 1. Start game. 2. Destroy a bottom-row (Large) invader → score should be 10. 3. Destroy a middle-row (Medium) → score should be 30. 4. Destroy a top-row (Small) → score should be 60. |
| **Expected** | Score totals 10, 30, 60 after the respective kills. |
| **Pass Criteria** | Scores match expected values. |

---

### TC-07 — Invader Speed Increases as Grid Shrinks

| Field | Detail |
|-------|--------|
| **ID** | TC-07 |
| **Description** | Verify invaders accelerate as their number decreases. |
| **Steps** | 1. Start game. Observe invader movement pace. 2. Destroy 50 invaders. Observe movement pace. |
| **Expected** | With 5 invaders remaining, the grid steps noticeably faster than at the start. |
| **Pass Criteria** | Visible increase in speed; game is harder with fewer invaders. |

---

### TC-08 — Invader Bullet Hits Player (Loses Life)

| Field | Detail |
|-------|--------|
| **ID** | TC-08 |
| **Description** | Verify an invader bullet reduces player lives. |
| **Steps** | 1. Start game and do not move. 2. Wait until an invader bullet hits the ship. |
| **Expected** | Life counter decrements from 3 to 2; explosion effect at ship position; ship re-centres. |
| **Pass Criteria** | Lives = 2 after first hit; game continues. |

---

### TC-09 — Player Dies (Three Hits)

| Field | Detail |
|-------|--------|
| **ID** | TC-09 |
| **Description** | Verify Game Over triggers when all lives are lost. |
| **Steps** | 1. Start game. 2. Allow three invader bullets to hit the player. |
| **Expected** | On the third hit, GAME OVER overlay appears. Game loop stops. |
| **Pass Criteria** | "GAME OVER" shown; no further input is accepted. |

---

### TC-10 — Bunker Erosion by Player Bullet

| Field | Detail |
|-------|--------|
| **ID** | TC-10 |
| **Description** | Verify bunkers are destroyed cell-by-cell by player bullets. |
| **Steps** | 1. Start game. 2. Fire several bullets into the top of a bunker. |
| **Expected** | Cells at the bullet impact point are removed; surrounding cells have a chance to also erode (chunky look). |
| **Pass Criteria** | Visible hole appears in the bunker; game does not crash. |

---

### TC-11 — Bunker Erosion by Invader Bullet

| Field | Detail |
|-------|--------|
| **ID** | TC-11 |
| **Description** | Verify invader bullets also damage bunkers. |
| **Steps** | 1. Position player behind a bunker. 2. Wait for invader bullets to land on the bunker. |
| **Expected** | Cells erode from the top of the bunker. Bullet stops upon hitting a live cell. |
| **Pass Criteria** | Bunker erodes from above; player is temporarily shielded. |

---

### TC-12 — Mystery Ship Appears and Crosses Screen

| Field | Detail |
|-------|--------|
| **ID** | TC-12 |
| **Description** | Verify the mystery ship spawns and traverses the screen. |
| **Steps** | 1. Start game. 2. Wait ~15–25 seconds without shooting the mystery ship. |
| **Expected** | A red UFO sprite appears from one edge, moves horizontally, and disappears off the opposite edge without scoring. |
| **Pass Criteria** | Mystery ship appears and exits silently; game state unchanged. |

---

### TC-13 — Shoot Mystery Ship for Bonus

| Field | Detail |
|-------|--------|
| **ID** | TC-13 |
| **Description** | Verify shooting the mystery ship awards a bonus score. |
| **Steps** | 1. Start game. 2. Wait for mystery ship. 3. Move under it and fire. |
| **Expected** | Mystery ship is destroyed; floating bonus text ("+50" to "+300") appears; score increases accordingly. |
| **Pass Criteria** | Score increases by a value in {50, 100, 150, 200, 300}. |

---

### TC-14 — Pause and Resume

| Field | Detail |
|-------|--------|
| **ID** | TC-14 |
| **Description** | Verify the game pauses and resumes correctly. |
| **Steps** | 1. Start game. 2. Press **P**. 3. Observe game state. 4. Press **P** again. |
| **Expected** | On first press: PAUSED overlay appears; invaders and bullets freeze. On second press: game resumes from where it stopped. |
| **Pass Criteria** | No objects move while paused; game continues correctly on resume. |

---

### TC-15 — Wave Complete Transitions to Next Wave

| Field | Detail |
|-------|--------|
| **ID** | TC-15 |
| **Description** | Verify clearing all invaders advances to Wave 2. |
| **Steps** | 1. Start game. 2. Destroy all 55 invaders. |
| **Expected** | "WAVE 1 CLEARED!" overlay appears for ~3 s; then Wave 2 begins with a new grid, restored bunkers, and wave counter = 2. |
| **Pass Criteria** | Wave counter increments; new invader grid is present; bunkers are intact. |

---

### TC-16 — Invader Reaches Ground — Instant Game Over

| Field | Detail |
|-------|--------|
| **ID** | TC-16 |
| **Description** | Verify game over when invaders reach the ground line. |
| **Steps** | 1. Start game. 2. Do not destroy any invaders; wait for the grid to descend to the ground. |
| **Expected** | When any invader's bottom edge crosses GROUND_Y, GAME OVER overlay appears immediately. |
| **Pass Criteria** | Game ends regardless of player lives remaining. |

---

### TC-17 — High Score Recorded After Qualifying Game

| Field | Detail |
|-------|--------|
| **ID** | TC-17 |
| **Description** | Verify a high score is saved when the player achieves a qualifying score. |
| **Steps** | 1. Play and achieve a score > 0. 2. Allow game to end. 3. Enter name "TESTER" at prompt. 4. Return to menu. 5. Click HIGH SCORES. |
| **Expected** | "TESTER" appears in the high scores list with the correct score value. |
| **Pass Criteria** | Entry is persistent; visible in dialog. |

---

### TC-18 — High Scores Not Prompted for Zero Score

| Field | Detail |
|-------|--------|
| **ID** | TC-18 |
| **Description** | Verify name-entry is skipped if score is 0 or below the top-10 threshold. |
| **Steps** | 1. Start game. 2. Immediately allow the ship to be destroyed three times without firing. Score = 0. |
| **Expected** | GAME OVER overlay shows score 000000; no name-entry prompt is shown. |
| **Pass Criteria** | Name-entry overlay does not appear. |

---

### TC-19 — High Scores Dialog Shows All Entries

| Field | Detail |
|-------|--------|
| **ID** | TC-19 |
| **Description** | Verify the High Scores dialog displays up to 10 entries in descending order. |
| **Steps** | 1. Submit 10+ different scores via successive games. 2. Click HIGH SCORES from menu. |
| **Expected** | Dialog lists at most 10 entries; entries are sorted highest score first; lowest score is replaced when a new higher score is submitted. |
| **Pass Criteria** | List is sorted; max 10 entries; lowest entry replaced correctly. |

---

### TC-20 — Return to Menu from Game

| Field | Detail |
|-------|--------|
| **ID** | TC-20 |
| **Description** | Verify the MENU button returns to the main menu. |
| **Steps** | 1. Start game. 2. Click the **MENU** button in the toolbar. |
| **Expected** | Main Menu screen is displayed; PAUSE button is disabled; game state is discarded. |
| **Pass Criteria** | Menu is shown with no lingering game state. |

---

*End of Documentation*
