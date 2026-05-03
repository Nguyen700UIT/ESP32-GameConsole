import pandas
import sys

#argv[1] = file name

SCREEN_WIDTH = 320
SCREEN_HEIGHT = 200
MAX_BALL_SPEED = 6.0
MIN_BALL_SPEED = 2.0
df = pandas.read_csv(sys.argv[1])

#filter X and Y
df = df[(df['ballX'] >= 0) & (df['ballX'] <= SCREEN_WIDTH)]
df = df[(df['ballY'] >= 0) & (df['ballY'] <= SCREEN_HEIGHT)]
df = df[(df['paddleY'] >= 0) & (df['paddleY'] <= SCREEN_HEIGHT)]

#filter speed

df = df[(df['vx'].abs() >= MIN_BALL_SPEED) & (df['vx'].abs() <= MAX_BALL_SPEED)]
df = df[(df['vy'].abs() >= MIN_BALL_SPEED) & (df['vy'].abs() <= MAX_BALL_SPEED)]

#deltaY negative when paddle is higher than ball so must move down but instead move up so clear
invalid_up = (df['deltaY'] < -30) & (df['action'] == 0) #boolean mask 
#deltaY positive when paddle is lower than ball so must move up but instead move down so clear   
invalid_down = (df['deltaY'] > 30) & (df['action'] == 1)

df_clean = df[~(invalid_down | invalid_up)] #remove invalid

df_clean.to_csv(sys.argv[2], index=False)