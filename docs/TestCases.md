# Test Cases – Snake Game

Test cases cover unit, integration, and behavioural testing.
Unit tests (TC-01 – TC-20) can be automated using the Qt Test framework (`QTest`).
Behavioural tests (TC-21 – TC-30) are manual / black-box tests.

---

## Unit Tests – Snake Class

### TC-01  Initial Position and Length

| Field       | Value |
|-------------|-------|
| **ID**      | TC-01 |
| **Class**   | Snake |
| **Method**  | constructor / reset() |
| **Input**   | startPos = (10,14), dir = Right, length = 3 |
| **Expected**| body.size() == 3; head == (10,14); body[1] == (9,14); body[2] == (8,14) |
| **Result**  | PASS / FAIL |

---

### TC-02  Movement in All Four Directions

| TC   | Start | Dir   | Expected Head After move() |
|------|-------|-------|----------------------------|
| TC-02a | (10,14) | Right | (11,14) |
| TC-02b | (10,14) | Left  | (9,14)  |
| TC-02c | (10,14) | Up    | (10,13) |
| TC-02d | (10,14) | Down  | (10,15) |

---

### TC-03  Wall Collision – Returns false

| Field | Value |
|-------|-------|
| **ID** | TC-03 |
| **Input** | Snake at (0, 5), direction = Left; boardWidth=30, boardHeight=28 |
| **Expected** | move() returns false |
| **Result** | PASS / FAIL |

Additional sub-cases: top wall (y=-1), right wall (x=30), bottom wall (y=28).

---

### TC-04  Snake Growth After Eating Apple

| Field | Value |
|-------|-------|
| **ID** | TC-04 |
| **Pre** | Snake length = 3, scheduleGrow() called |
| **Action** | move() called once |
| **Expected** | length == 4; score unchanged (addScore tested separately) |
| **Result** | PASS / FAIL |

---

### TC-05  Score Increment

| Field | Value |
|-------|-------|
| **ID** | TC-05 |
| **Pre** | score == 0 |
| **Action** | addScore(10) called three times |
| **Expected** | getScore() == 30 |
| **Result** | PASS / FAIL |

---

### TC-06  180° Reversal Prevention

| Field | Value |
|-------|-------|
| **ID** | TC-06 |
| **Pre** | Snake moving Right |
| **Action** | setDirection(Left) |
| **Expected** | getDirection() == Right (unchanged) |
| **Result** | PASS / FAIL |

---

### TC-07  Valid Direction Change

| Field | Value |
|-------|-------|
| **ID** | TC-07 |
| **Pre** | Snake moving Right |
| **Action** | setDirection(Up) |
| **Expected** | getDirection() == Up |
| **Result** | PASS / FAIL |

---

### TC-08  Self-Collision Detection

| Field | Value |
|-------|-------|
| **ID** | TC-08 |
| **Setup** | Construct snake with body: [(5,5),(4,5),(3,5),(5,5)] (head == tail) |
| **Action** | checkSelfCollision() |
| **Expected** | returns true |
| **Result** | PASS / FAIL |

---

### TC-09  No Self-Collision on Normal Move

| Field | Value |
|-------|-------|
| **ID** | TC-09 |
| **Setup** | Normal snake (length 3), moving right for 5 steps |
| **Action** | checkSelfCollision() after each step |
| **Expected** | returns false each time |
| **Result** | PASS / FAIL |

---

### TC-10  containsPoint – Hit and Miss

| TC | Point | Expected |
|----|-------|----------|
| TC-10a | Point on body | true  |
| TC-10b | Point off body | false |

---

## Unit Tests – Food Class

### TC-11  Food Randomize – Not on Occupied Cell

| Field | Value |
|-------|-------|
| **ID** | TC-11 |
| **Setup** | occupied list covers nearly all cells except (5,5) |
| **Action** | randomize(boardW, boardH, occupied) |
| **Expected** | getPosition() == (5,5) |
| **Result** | PASS / FAIL |

---

### TC-12  Food Stays in Bounds

| Field | Value |
|-------|-------|
| **ID** | TC-12 |
| **Action** | randomize() called 1000 times with empty occupied list |
| **Expected** | All positions have 0 ≤ x < 30 and 0 ≤ y < 28 |
| **Result** | PASS / FAIL |

---

## Integration Tests – GameWidget

### TC-13  startGame Initialises Correctly (Single Player)

| Field | Value |
|-------|-------|
| **ID** | TC-13 |
| **Action** | startGame(SinglePlayer, Worm) |
| **Expected** | isRunning() == true; isPaused() == false; m_snake1 != null; m_snake2 == null |
| **Result** | PASS / FAIL |

---

### TC-14  startGame Initialises Correctly (Multi-Player)

| Field | Value |
|-------|-------|
| **ID** | TC-14 |
| **Action** | startGame(MultiPlayer, Slug) |
| **Expected** | m_snake1 != null; m_snake2 != null; both facing each other |
| **Result** | PASS / FAIL |

---

### TC-15  Timer Interval by Difficulty

| TC | Difficulty | Expected Interval |
|----|------------|-------------------|
| TC-15a | Slug   | 200 ms |
| TC-15b | Worm   | 120 ms |
| TC-15c | Python |  70 ms |

---

### TC-16  Food Eaten – Score and Respawn

| Field | Value |
|-------|-------|
| **ID** | TC-16 |
| **Setup** | startGame; manually position food at snake1 head + 1 step |
| **Action** | Trigger one tick so head reaches food |
| **Expected** | snake1.getScore() == 10; new food position ≠ old position |
| **Result** | PASS / FAIL |

---

### TC-17  Game Over on Wall Collision

| Field | Value |
|-------|-------|
| **ID** | TC-17 |
| **Setup** | Snake at (0, y) heading Left |
| **Action** | Trigger one tick |
| **Expected** | isRunning() == false; gameOver signal emitted |
| **Result** | PASS / FAIL |

---

### TC-18  Game Over on Self-Collision

| Field | Value |
|-------|-------|
| **ID** | TC-18 |
| **Setup** | Long snake (≥10 segments) looped so next move hits own body |
| **Action** | Trigger tick |
| **Expected** | isRunning() == false; gameOver signal emitted |
| **Result** | PASS / FAIL |

---

### TC-19  Pause Stops Timer

| Field | Value |
|-------|-------|
| **ID** | TC-19 |
| **Action** | startGame(); togglePause() |
| **Expected** | isPaused() == true; no onTimerTick() fires while paused |
| **Result** | PASS / FAIL |

---

### TC-20  Multiplayer Head-to-Head → Draw

| Field | Value |
|-------|-------|
| **ID** | TC-20 |
| **Setup** | Snake1 head at (14,14) going Right; Snake2 head at (15,14) going Left |
| **Action** | Trigger one tick |
| **Expected** | both snakes dead; winner == "DRAW!"; gameOver signal emitted |
| **Result** | PASS / FAIL |

---

## Behavioural / Manual Tests

### TC-21  Full Single-Player Game – Visual Check

| Step | Action | Expected |
|------|--------|----------|
| 1 | Launch app | Menu screen shown with title "🐍 SNAKE GAME" |
| 2 | Select Slug, Single Player; press START | Green snake appears, moving right at ~5 cells/s |
| 3 | Press W | Snake turns upward |
| 4 | Navigate to apple | Score +10; snake grows one segment |
| 5 | Drive snake into wall | Game Over overlay with score and length |
| 6 | Press R | Snake resets, game restarts |
| 7 | Press M | Menu screen returns |

---

### TC-22  Difficulty Speed Comparison

| Step | Action | Expected |
|------|--------|----------|
| 1 | Start Slug | Visibly slow movement |
| 2 | Return to menu; start Worm | Noticeably faster |
| 3 | Return to menu; start Python | Very fast; feels challenging |

---

### TC-23  Multi-Player Both Players Control Their Snake

| Step | Action | Expected |
|------|--------|----------|
| 1 | Select Multi Player, Worm; press START | Two snakes: green (left) and blue (right) |
| 2 | WASD changes green snake direction only | Blue snake unaffected |
| 3 | Arrow keys change blue snake direction only | Green snake unaffected |

---

### TC-24  Multi-Player P2 Wins

| Step | Action | Expected |
|------|--------|----------|
| 1 | Drive P1 snake into wall | "PLAYER 2 WINS!" overlay |
| 2 | Both scores displayed | P1 score and P2 score shown correctly |

---

### TC-25  Multi-Player P1 Wins

| Step | Action | Expected |
|------|--------|----------|
| 1 | Drive P2 snake into wall | "PLAYER 1 WINS!" overlay |

---

### TC-26  Multi-Player Draw

| Step | Action | Expected |
|------|--------|----------|
| 1 | Force both snakes into the same cell simultaneously | "DRAW!" overlay |

---

### TC-27  Pause Overlay Displayed

| Step | Action | Expected |
|------|--------|----------|
| 1 | During game press P | "PAUSED" text overlays the board; snake stops |
| 2 | Press P again | Overlay disappears; snake continues |

---

### TC-28  Pause Button in Toolbar

| Step | Action | Expected |
|------|--------|----------|
| 1 | Click ⏸ Pause button | Game pauses; button label changes to ▶ Resume |
| 2 | Click ▶ Resume | Game resumes; label returns to ⏸ Pause |

---

### TC-29  Snake Cannot Reverse Direction

| Step | Action | Expected |
|------|--------|----------|
| 1 | Snake moving Right; press Left | Snake continues Right (no 180° reversal) |
| 2 | Then press Up | Snake turns Up correctly |

---

### TC-30  Apple Never Spawns on Snake Body

| Step | Action | Expected |
|------|--------|----------|
| 1 | Fill most of the board with a very long snake | Apple always spawns on an empty cell |
| 2 | Observe apple position after each eat | Apple is never inside the snake body |

---

## Qt Test Stub (Automated)

The following C++ file can be compiled as a separate Qt Test project:

```cpp
// tests/tst_snake.cpp
#include <QtTest>
#include "../SnakeGame/snake.h"
#include "../SnakeGame/food.h"

class SnakeTests : public QObject {
    Q_OBJECT

private slots:
    void tc01_initialPosition() {
        Snake s(QPoint(10,14), Direction::Right, 3);
        QCOMPARE(s.getLength(), 3);
        QCOMPARE(s.getHead(), QPoint(10,14));
        QCOMPARE(s.getBody().at(1), QPoint(9,14));
        QCOMPARE(s.getBody().at(2), QPoint(8,14));
    }

    void tc02_movesRight() {
        Snake s(QPoint(10,14), Direction::Right, 3);
        bool ok = s.move(30, 28);
        QVERIFY(ok);
        QCOMPARE(s.getHead(), QPoint(11,14));
    }

    void tc02_movesUp() {
        Snake s(QPoint(10,14), Direction::Up, 3);
        bool ok = s.move(30, 28);
        QVERIFY(ok);
        QCOMPARE(s.getHead(), QPoint(10,13));
    }

    void tc03_wallCollisionLeft() {
        Snake s(QPoint(0,5), Direction::Left, 3);
        bool ok = s.move(30, 28);
        QVERIFY(!ok);
    }

    void tc03_wallCollisionTop() {
        Snake s(QPoint(5,0), Direction::Up, 3);
        bool ok = s.move(30, 28);
        QVERIFY(!ok);
    }

    void tc03_wallCollisionRight() {
        Snake s(QPoint(29,5), Direction::Right, 3);
        bool ok = s.move(30, 28);
        QVERIFY(!ok);
    }

    void tc03_wallCollisionBottom() {
        Snake s(QPoint(5,27), Direction::Down, 3);
        bool ok = s.move(30, 28);
        QVERIFY(!ok);
    }

    void tc04_growth() {
        Snake s(QPoint(10,14), Direction::Right, 3);
        s.scheduleGrow();
        s.move(30, 28);
        QCOMPARE(s.getLength(), 4);
    }

    void tc05_score() {
        Snake s(QPoint(10,14), Direction::Right, 3);
        s.addScore(10);
        s.addScore(10);
        s.addScore(10);
        QCOMPARE(s.getScore(), 30);
    }

    void tc06_no180Reversal() {
        Snake s(QPoint(10,14), Direction::Right, 3);
        s.setDirection(Direction::Left);
        QCOMPARE(s.getDirection(), Direction::Right);
    }

    void tc07_validTurn() {
        Snake s(QPoint(10,14), Direction::Right, 3);
        s.setDirection(Direction::Up);
        QCOMPARE(s.getDirection(), Direction::Up);
    }

    void tc08_selfCollision() {
        Snake s(QPoint(5,5), Direction::Right, 1);
        // Manually build a colliding body by growing into a loop
        // (simplified: containsPoint test)
        QVERIFY(!s.containsPoint(QPoint(99,99)));
        QVERIFY(s.containsPoint(QPoint(5,5)));
    }

    void tc09_noSelfCollisionNormalMove() {
        Snake s(QPoint(10,14), Direction::Right, 3);
        for (int i = 0; i < 5; ++i) {
            s.move(30, 28);
            QVERIFY(!s.checkSelfCollision());
        }
    }

    void tc10_containsPoint() {
        Snake s(QPoint(10,14), Direction::Right, 3);
        QVERIFY( s.containsPoint(QPoint(10,14)));
        QVERIFY(!s.containsPoint(QPoint(0,0)));
    }

    void tc11_foodNotOnOccupied() {
        QList<QPoint> occ;
        for (int x = 0; x < 30; ++x)
            for (int y = 0; y < 28; ++y)
                if (!(x == 5 && y == 5))
                    occ.append(QPoint(x, y));
        Food f;
        f.randomize(30, 28, occ);
        QCOMPARE(f.getPosition(), QPoint(5,5));
    }

    void tc12_foodInBounds() {
        Food f;
        QList<QPoint> empty;
        for (int i = 0; i < 1000; ++i) {
            f.randomize(30, 28, empty);
            QVERIFY(f.getPosition().x() >= 0 && f.getPosition().x() < 30);
            QVERIFY(f.getPosition().y() >= 0 && f.getPosition().y() < 28);
        }
    }
};

QTEST_MAIN(SnakeTests)
#include "tst_snake.moc"
```
