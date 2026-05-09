import pandas 
import sys
from sklearn.model_selection import train_test_split

df = pandas.read_csv(sys.argv[1])

x = df.drop('action', axis=1)
y = df['action']
#Train 80%, (Test = 10%, Validation = 10%), random_state help split evenly
x_Train, x_Temp, y_Train, y_Temp = train_test_split(x, y, test_size=0.2, random_state=42, shuffle=True)

x_Val, x_Test, y_Val, y_Test = train_test_split(x_Temp, y_Temp, test_size=0.5, random_state=42, shuffle=True)

train_df = x_Train.copy()
train_df['action'] = y_Train

val_df = x_Val.copy()
val_df['action'] = y_Val

test_df = x_Test.copy()
test_df['action'] = y_Test



train_df.to_csv("train_data_new.csv", index=False)
val_df.to_csv("validation_data_new.csv", index=False)
test_df.to_csv("test_data_new.csv", index=False)