
#include "constants.h"
#include "pong.h"
#include "sounds/hit.h"
#include "sounds/loss.h"
#include "sounds/wall.h"
#include "sounds/winner.h"

Ball ball;
int paddleOffset = 20;
Scoreboard scoreboard;
Paddle paddleLeft, paddleRight;
char scoreboardText[200] = "";
Line lineTop, lineBottom, lineMiddle;

void initialize();
void update();
void draw();

int main() {
	initialize();

	while(1) {
		update();
		draw();
		display();
	}
	return 0;
}


void initialize() {
	// setup initalizers
	initializeScreen();
	initializePad();
	initializeDebugFont();
	
	// define the background colour
	setBackgroundColor(createColor(20, 20, 20));
	
	// define the 'playing field'
	lineTop = createLine(createColor(255, 255, 255), 0, 0, SCREEN_WIDTH, 0);
	lineBottom = createLine(createColor(255, 255, 255), 0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, SCREEN_HEIGHT - 20);
	lineMiddle = createLine(createColor(120, 120, 120), SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);
	
	// define the Paddle objects
	paddleLeft = createPaddle(paddleOffset);
	paddleRight = createPaddle(SCREEN_WIDTH - paddleOffset);
	
	// define the ball
	ball = createBall();
	
	// define the scoreboard
	scoreboard = createScoreboard();

    // init sound files
    audioInit();
    // loads music files into ram
    audioTransferVagToSPU(&hit, hit_size, SPU_0CH);
    audioTransferVagToSPU(&loss, loss_size, SPU_1CH);
    audioTransferVagToSPU(&wall, wall_size, SPU_2CH);
    audioTransferVagToSPU(&winner, winner_size, SPU_3CH);
}


void update() {
	padUpdate();
	
	if (padCheck(Pad1Up)) {
		paddleLeft = movePaddle(paddleLeft, -4);
	}
	
	if (padCheck(Pad1Down)) {
		paddleLeft = movePaddle(paddleLeft, 4);
	}
	
    if (padCheck(Pad2Up)) {
        paddleRight = movePaddle(paddleRight, -4);
    }

    if (padCheck(Pad2Down)) {
        paddleRight = movePaddle(paddleRight, 4);
    }
	
	if (padCheck(Pad1Start) || padCheck(Pad2Start)) {
		ball = kickBall(ball);
	}

    if (padCheck(Pad1Select) || padCheck(Pad2Select)) {
        ball = resetBall(ball);
    }
	
	// move the ball
	ball = moveBall(ball, paddleLeft, paddleRight);
	
	// update the scoreboard
	sprintf(scoreboardText, "%d:%d", scoreboard.scoreLeft, scoreboard.scoreRight);

    // check winning condition
    if((scoreboard.scoreLeft >= 11 && scoreboard.scoreLeft - scoreboard.scoreRight >= 2) || (scoreboard.scoreRight >= 11 && scoreboard.scoreRight - scoreboard.scoreLeft >= 2)) {
        audioPlay(SPU_3CH);
        // freezes screen for winning theme
        VSync(160);
        initialize();
    }
}

void draw() {
	// draw the 'playing field'
	drawLine(lineTop);
	drawLine(lineBottom);
	drawLine(lineMiddle);
	
	// draw the 'paddle' objects
	drawPaddle(paddleLeft);
	drawPaddle(paddleRight);
	
	// draw the 'ball' object
	drawBall(ball);
	
	// draw the scoreboard
	FntPrint(scoreboardText);
}
