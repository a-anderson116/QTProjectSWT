# Sequence Diagrams – Snake Game

All diagrams are written in PlantUML notation and as ASCII art for readability without a renderer.

---

## SD-01  Starting a Game (Single Player)

```
Player          MainWindow        MenuScreen        GameWidget         Snake / Food
  |                  |                 |                 |                   |
  |-- click START -->|                 |                 |                   |
  |                  |--onStartGame()->|                 |                   |
  |                  |                 |                 |                   |
  |                  |  showGame()     |                 |                   |
  |                  |---startGame()-->|                 |                   |
  |                  |                 | new Snake(…)    |                   |
  |                  |                 |---------------->|                   |
  |                  |                 | new Snake(…)    |                   |
  |                  |                 |                 |-- reset() ------->|
  |                  |                 | food.randomize()|                   |
  |                  |                 | timer.start()   |                   |
  |                  |                 |                 |                   |
  |                  |                 | update()        |                   |
  |<================ repaint ==========|                 |                   |
```

---

## SD-02  One Game Tick (Normal Move – Single Player)

```
QTimer           GameWidget              Snake               Food
  |                   |                    |                   |
  |-- timeout() ----->|                    |                   |
  |                   | applyDirection()   |                   |
  |                   |------------------->|                   |
  |                   | move()             |                   |
  |                   |------------------->|                   |
  |                   |<-- true (ok) ------|                   |
  |                   | checkSelfCollision |                   |
  |                   |------------------->|                   |
  |                   |<-- false (no hit)--|                   |
  |                   | head == food.pos?  |                   |
  |                   |--------------------|------------------>|
  |                   |    YES             |                   |
  |                   | scheduleGrow()     |                   |
  |                   |------------------->|                   |
  |                   | addScore(10)       |                   |
  |                   |------------------->|                   |
  |                   | food.randomize()   |                   |
  |                   |--------------------|------------------>|
  |                   | update()           |                   |
  |<================ repaint ==============|                   |
```

---

## SD-03  Collision Detection and Game Over

```
QTimer           GameWidget              Snake          MainWindow
  |                   |                    |                 |
  |-- timeout() ----->|                    |                 |
  |                   | snake.move()       |                 |
  |                   |------------------->|                 |
  |                   |<-- false (wall) ---|                 |
  |                   | OR checkSelfCollision() → true       |
  |                   | handleDeath()      |                 |
  |                   | timer.stop()       |                 |
  |                   | m_gameOver = true  |                 |
  |                   | emit gameOver(...) |                 |
  |                   |---------------------------------->   |
  |                   |                    |    onGameOver() |
  |                   | update()           |    update info  |
  |<================ repaint (overlay) ====|                 |
```

---

## SD-04  Multiplayer – Simultaneous Collision Check

```
QTimer       GameWidget         Snake1              Snake2
  |               |                |                   |
  |-- timeout() ->|                |                   |
  |               | snake1.move()  |                   |
  |               |--------------->|                   |
  |               |<-- true -------|                   |
  |               | snake2.move()  |                   |
  |               |----------------|------------------>|
  |               |<-- true -------|-------------------|
  |               | s1Dead = snake1.checkSelfCollision()|
  |               | s2Dead = snake2.checkSelfCollision()|
  |               |                                    |
  |               | snake2.containsPoint(snake1.head)? |
  |               | → s1Dead = true (if yes)           |
  |               |                                    |
  |               | snake1.containsPoint(snake2.head)? |
  |               | → s2Dead = true (if yes)           |
  |               |                                    |
  |               | handleDeath(s1Dead, s2Dead)        |
  |               | determine winner / draw            |
  |               | emit gameOver(...)                 |
```

---

## SD-05  Pause / Resume

```
Player       GameWidget          QTimer         MainWindow
  |               |                  |                |
  |-- key P ----->|                  |                |
  |               | togglePause()    |                |
  |               | m_paused=true    |                |
  |               | timer.stop() --->|                |
  |               | emit pauseStateChanged(true)      |
  |               |---------------------------------->|
  |               |                  |  "▶ Resume"   |
  |               | update()         |                |
  |<============= repaint (PAUSED overlay) ==========>|
  |               |                  |                |
  |-- key P ----->|                  |                |
  |               | m_paused=false   |                |
  |               | timer.start() -->|                |
  |               | emit pauseStateChanged(false)     |
  |               |---------------------------------->|
  |               |                  |  "⏸ Pause"   |
```

---

## SD-06  Restart After Game Over

```
Player          GameWidget         MainWindow
  |                  |                  |
  |-- key R -------->|                  |
  |                  | emit restartRequested()         |
  |                  |-------------------------------->|
  |                  |                  onRestartRequested()
  |                  |<-- startGame(lastMode, lastDiff)|
  |                  | reset Snake(s)   |              |
  |                  | spawnFood()      |              |
  |                  | timer.start()    |              |
  |<================ repaint ==========>|              |
```
