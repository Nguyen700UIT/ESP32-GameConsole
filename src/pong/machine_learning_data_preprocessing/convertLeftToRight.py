import pandas
import sys

SCREEN_WIDTH = 320
SCREEN_HEIGHT = 200
MAX_BALL_SPEED = 6.0
MIN_BALL_SPEED = 2.0
PADDLE_HEIGHT = 30

df = pandas.read_csv(sys.argv[1])

df['ballX'] = SCREEN_WIDTH - df['ballX']
df['vx'] = -df['vx']
df['distance'] = (((SCREEN_WIDTH - 10) - df['ballX']) ** 2 + ((df['paddleY'] + PADDLE_HEIGHT / 2) - df['ballY']) ** 2) ** 0.5

df.to_csv(sys.argv[2], index=False)
