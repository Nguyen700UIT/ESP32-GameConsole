import pandas
import sys
                                                              
df_human = pandas.read_csv(sys.argv[1])
df_bot = pandas.read_csv(sys.argv[2])
#axis = 0 add row; axis = 1 add col; ignore_index=True reindex 
df_final = df_final = pandas.concat([df_human, df_bot], axis=0, ignore_index=True)
#frac = 1 keep all data
df_final = df_final.sample(frac=1).reset_index(drop=True)

df_final.to_csv(sys.argv[3], index=False)