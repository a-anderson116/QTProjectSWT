# Class Diagram – Snake Game

```
┌─────────────────────────────────────────────────────────────────────────────────┐
│                              Qt Application Layer                               │
└─────────────────────────────────────────────────────────────────────────────────┘

┌──────────────────────────────────┐
│         <<enum>>                 │
│           Direction              │
├──────────────────────────────────┤
│ + Up                             │
│ + Down                           │
│ + Left                           │
│ + Right                          │
│ + None                           │
├──────────────────────────────────┤
│ + areOpposite(a, b) : bool       │  ← free function
└──────────────────────────────────┘

┌──────────────────────────────────┐
│         <<enum>>                 │
│           GameMode               │
├──────────────────────────────────┤
│ + SinglePlayer                   │
│ + MultiPlayer                    │
└──────────────────────────────────┘

┌──────────────────────────────────┐
│         <<enum>>                 │
│          Difficulty              │
├──────────────────────────────────┤
│ + Slug   (200 ms/tick)           │
│ + Worm   (120 ms/tick)           │
│ + Python  (70 ms/tick)           │
└──────────────────────────────────┘


┌────────────────────────────────────────────────┐
│                    Snake                        │
├────────────────────────────────────────────────┤
│ - m_body      : QList<QPoint>                  │
│ - m_direction : Direction                      │
│ - m_growPending : bool                         │
│ - m_score     : int                            │
├────────────────────────────────────────────────┤
│ + Snake(startPos, startDir, length=3)          │
│ + reset(startPos, startDir, length=3) : void   │
│ + move(boardW, boardH) : bool                  │
│ + scheduleGrow()       : void                  │
│ + setDirection(dir)    : void                  │
│ + getDirection()       : Direction             │
│ + getHead()            : QPoint                │
│ + getBody()            : const QList<QPoint>&  │
│ + checkSelfCollision() : bool                  │
│ + containsPoint(p)     : bool                  │
│ + getScore()           : int                   │
│ + getLength()          : int                   │
│ + addScore(pts)        : void                  │
└────────────────────────────────────────────────┘


┌────────────────────────────────────────────────┐
│                    Food                         │
├────────────────────────────────────────────────┤
│ - m_position  : QPoint                         │
├────────────────────────────────────────────────┤
│ + Food()                                        │
│ + randomize(boardW, boardH, occupied) : void   │
│ + getPosition() : QPoint                       │
└────────────────────────────────────────────────┘


┌─────────────────────────────────────────────────────────────────────┐
│                         GameWidget   [QWidget]                       │
├─────────────────────────────────────────────────────────────────────┤
│ <<constants>>                                                        │
│ + CELL_SIZE    : int = 20                                            │
│ + BOARD_WIDTH  : int = 30                                            │
│ + BOARD_HEIGHT : int = 28                                            │
│ + SCORE_BAR    : int = 40                                            │
├─────────────────────────────────────────────────────────────────────┤
│ - m_snake1      : Snake*                                             │
│ - m_snake2      : Snake*                                             │
│ - m_food        : Food                                               │
│ - m_timer       : QTimer                                             │
│ - m_gameMode    : GameMode                                           │
│ - m_difficulty  : Difficulty                                         │
│ - m_gameRunning : bool                                               │
│ - m_paused      : bool                                               │
│ - m_gameOver    : bool                                               │
│ - m_winner      : QString                                            │
│ - m_pendingDir1 : Direction                                          │
│ - m_pendingDir2 : Direction                                          │
├─────────────────────────────────────────────────────────────────────┤
│ + startGame(mode, difficulty) : void                                 │
│ + togglePause()               : void                                 │
│ + isPaused()                  : bool                                 │
│ + isRunning()                 : bool                                 │
│ + sizeHint()                  : QSize                                │
│ <<signal>> gameOver(s1, s2, winner, mode)                            │
│ <<signal>> restartRequested()                                        │
│ <<signal>> menuRequested()                                           │
│ <<signal>> pauseStateChanged(paused)                                 │
│ <<protected>> paintEvent(event)    : void                            │
│ <<protected>> keyPressEvent(event) : void                            │
│ <<slot>>    onTimerTick()          : void                            │
│ - checkFoodCollisions() : void                                       │
│ - handleDeath(s1, s2)   : void                                       │
│ - spawnFood()           : void                                       │
│ - allOccupied()         : QList<QPoint>                              │
│ - timerInterval()       : int                                        │
│ - drawBackground(p)     : void                                       │
│ - drawFood(p)           : void                                       │
│ - drawSnake(p, s, body, head) : void                                 │
│ - drawScoreBar(p)       : void                                       │
│ - drawPauseOverlay(p)   : void                                       │
│ - drawGameOverOverlay(p): void                                       │
└─────────────────────────────────────────────────────────────────────┘
           │ 1                             │ 1
           │ owns                          │ owns
           ▼                              ▼
        Snake (m_snake1)            Snake (m_snake2, nullable)
           │                              │
           │ aggregates                   │
           ▼
        Food  (m_food)


┌─────────────────────────────────────────────────────────────────────┐
│                       MenuScreen   [QWidget]                         │
├─────────────────────────────────────────────────────────────────────┤
│ - m_modeGroup  : QButtonGroup*                                       │
│ - m_diffGroup  : QButtonGroup*                                       │
│ - m_singleBtn  : QRadioButton*                                       │
│ - m_multiBtn   : QRadioButton*                                       │
│ - m_slugBtn    : QRadioButton*                                       │
│ - m_wormBtn    : QRadioButton*                                       │
│ - m_pythonBtn  : QRadioButton*                                       │
│ - m_startBtn   : QPushButton*                                        │
├─────────────────────────────────────────────────────────────────────┤
│ + MenuScreen(parent)                                                 │
│ <<signal>> startGame(mode, difficulty)                               │
│ <<slot>>   onStartClicked()                                          │
│ - buildUI() : void                                                   │
└─────────────────────────────────────────────────────────────────────┘


┌─────────────────────────────────────────────────────────────────────┐
│                      MainWindow   [QMainWindow]                      │
├─────────────────────────────────────────────────────────────────────┤
│ - m_stack      : QStackedWidget*                                     │
│ - m_menuScreen : MenuScreen*                                         │
│ - m_gameWidget : GameWidget*                                         │
│ - m_pauseBtn   : QPushButton*                                        │
│ - m_menuBtn    : QPushButton*                                        │
│ - m_infoLabel  : QLabel*                                             │
│ - m_lastMode   : GameMode                                            │
│ - m_lastDiff   : Difficulty                                          │
├─────────────────────────────────────────────────────────────────────┤
│ + MainWindow(parent)                                                 │
│ <<slot>> onStartGame(mode, diff)                                     │
│ <<slot>> onGameOver(s1, s2, winner, mode)                            │
│ <<slot>> onRestartRequested()                                        │
│ <<slot>> onMenuRequested()                                           │
│ <<slot>> onPauseStateChanged(paused)                                 │
│ - buildUI()   : void                                                 │
│ - showMenu()  : void                                                 │
│ - showGame()  : void                                                 │
└─────────────────────────────────────────────────────────────────────┘
         │ owns                  │ owns
         ▼                      ▼
    MenuScreen            GameWidget


┌──────────────────────────────────────────────────────────────────────────────┐
│                         Relationships Summary                                 │
├──────────────────────────────────────────────────────────────────────────────┤
│ MainWindow   --owns--> MenuScreen, GameWidget (via QStackedWidget)           │
│ GameWidget   --owns--> Snake (×1 or ×2), Food (×1)                          │
│ Snake        --uses--> Direction (enum)                                      │
│ GameWidget   --uses--> GameMode, Difficulty (enums)                          │
│ MenuScreen   --signal--> MainWindow.onStartGame()                            │
│ GameWidget   --signal--> MainWindow.onGameOver / onRestart / onMenu / onPause│
│ QTimer       --signal--> GameWidget.onTimerTick()                            │
└──────────────────────────────────────────────────────────────────────────────┘
```
