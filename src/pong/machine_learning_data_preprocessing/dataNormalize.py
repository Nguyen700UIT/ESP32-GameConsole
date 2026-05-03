import pandas 
import sys
from sklearn.preprocessing import MinMaxScaler

df = pandas.read_csv(sys.argv[1])

#action output so no scale
feature_to_scale = ['ballX', 'ballY', 'vy', 'vx', 'paddleY', 'deltaY']

scaler = MinMaxScaler()
#transform to 0 to 1 data 
df[feature_to_scale] = scaler.fit_transform(df[feature_to_scale])

for i, col in enumerate(feature_to_scale):
    # scaler.data_min_[i] print min of col i
    # scaler.data_max_[i] print max of col i
    print(f"Cột {col}: Min = {scaler.data_min_[i]:.2f}, Max = {scaler.data_max_[i]:.2f}")

df.to_csv(sys.argv[2], index=False)