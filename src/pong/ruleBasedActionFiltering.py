import pandas
import sys

SCREEN_WIDTH = 320
SCREEN_HEIGHT = 200
THRESHOLD = 2
df = pandas.read_csv(sys.argv[1])

#find row in deltaY collumn where higher than THRESHOLD meaning ball is higher so action must be up
df.loc[df['deltaY'] > THRESHOLD, 'action'] = 0

#find row in deltaY collumn where less than THRESHOLD meaning ball is lower so action must be down
df.loc[df['deltaY'] < -THRESHOLD, 'action'] = 1

#delta in [-THRESHOLD; THRESHOLD] so can stay 
df.loc[df['deltaY'].abs() <= 2, 'action'] = 2

df.to_csv(sys.argv[2], index=False)