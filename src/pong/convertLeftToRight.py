import pandas
import sys

SCREEN_WIDTH = 320
SCREEN_HEIGHT = 200
MAX_BALL_SPEED = 6.0
MIN_BALL_SPEED = 2.0

df = pandas.read_csv(sys.argv[1])

df['ballX'] = SCREEN_WIDTH - df['ballX']
df['vx'] = -df['vx']

df.to_csv(sys.argv[2], index=False)