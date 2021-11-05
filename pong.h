#pragma once

typedef struct {
	int x, y, height;
	Line line;
} Paddle;

typedef struct {
	int size, x, y, isMoving, speedX, speedY;
	Box box;
} Ball;

typedef struct {
	int scoreLeft, scoreRight;
} Scoreboard;

Scoreboard scoreboard;

Scoreboard createScoreboard() {
	Scoreboard scoreboard;
	scoreboard.scoreLeft = 0;
	scoreboard.scoreRight = 0;
	return scoreboard;
}

Ball createBall() {
	Ball ball;
    ball.size = 3;
    ball.x = SCREEN_WIDTH / 2 - ball.size / 2;
    ball.y = SCREEN_HEIGHT / 2 - ball.size / 2;
    ball.box = createBox(createColor(0, 255, 0), ball.x, ball.y, ball.x + ball.size, ball.y + ball.size);
	ball.isMoving = 0;
    ball.speedX = 0;
    ball.speedY = 0;
    return ball;
}

Ball accelerateBall(Ball ball) {
	if(ball.speedX < 0) ball.speedX -= rand() % 2;
	else ball.speedX += rand() % 3;
	if(ball.speedY < 0) ball.speedY -= rand() % 2;
	else ball.speedY += rand() % 2;
	return ball;
}

Ball resetBall(Ball ball) {
	ball = createBall();
	return ball;
}

Ball moveBall(Ball ball, Paddle paddleLeft, Paddle paddleRight) {
	if(!ball.isMoving) return ball;
	ball.x += ball.speedX;
	ball.y += ball.speedY;
	ball.box = moveBox(ball.box, ball.x, ball.y);

	if(ball.x < paddleLeft.x && ball.x > 10 && ball.y > paddleLeft.y && ball.y < paddleLeft.y + paddleLeft.height) {
		ball.speedX = ball.speedX *= -1;
		ball.x = paddleLeft.x;
		ball = accelerateBall(ball);
        audioPlay(SPU_0CH);
	}

	if(ball.x > paddleRight.x && ball.x < SCREEN_WIDTH - 10 && ball.y > paddleRight.y && ball.y < paddleRight.y + paddleRight.height) {
		ball.speedX = ball.speedX *= -1;
		ball.x = paddleRight.x;
		ball = accelerateBall(ball);
        audioPlay(SPU_0CH);
	}

	if(ball.y > SCREEN_HEIGHT - 20) {
		ball.speedY = ball.speedY *= -1;
		ball.y = SCREEN_HEIGHT - 20;
        audioPlay(SPU_2CH);
	}

	if(ball.y < 1) {
		ball.speedY = ball.speedY *= -1;
		ball.y = 1;
        audioPlay(SPU_2CH);
	}

	if(ball.x < -5) {
		ball = resetBall(ball);
		scoreboard.scoreRight ++;
        audioPlay(SPU_1CH);
	}

	if(ball.x > SCREEN_WIDTH + 5) {
		ball = resetBall(ball);
		scoreboard.scoreLeft ++;
        audioPlay(SPU_1CH);
	}

	return ball;
}

Ball kickBall(Ball ball) {
	if(ball.isMoving) return ball;
    while(ball.speedX == 0) {
        ball.speedX = 1 + rand() % 2 - rand() % 4;
    }
	ball.speedY = 1 + rand() % 2 - rand() % 2;
	ball.isMoving = 1;
	return ball;
}

void drawBall(Ball ball) {
	drawBox(ball.box);
}

Paddle createPaddle(int x) {
	Paddle paddle;

	paddle.x = x;
	paddle.y = SCREEN_HEIGHT / 2 - 20;
	paddle.height = 40;
	paddle.line = createLine(createColor(255, 255, 255), paddle.x, paddle.y, paddle.x, paddle.y + paddle.height);

	return paddle;
}

Paddle movePaddle(Paddle paddle, int delta_y) {
	if(delta_y < 0 && paddle.y + delta_y <= 4) {
		paddle.y = 0;
		return paddle;
	}

	if(delta_y > 0 && paddle.y + paddle.height + delta_y > SCREEN_HEIGHT - 24) {
		paddle.y = SCREEN_HEIGHT - 20 - paddle.height;
		return paddle;
	}

	paddle.y += delta_y;
	paddle.line = moveLine(paddle.line, paddle.x, paddle.y, paddle.x, paddle.y + paddle.height);
	return paddle;
}

void drawPaddle(Paddle paddle) {
	drawLine(paddle.line);
}