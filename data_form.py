# 495 assets in 466 days actually
import glob
import pandas as pd
import numpy as np

# initialize the data frame with all the dates
# left_most = pd.read_csv('500/A.csv')["Date"]

# specify the path to look for all csv files
path = r'500'  # 'Data' is the name of the folder holding files in the same dir as this code file
sp500 = glob.glob(path + "/*.csv")
merged = []  # put the date column in first
total_volume = np.array([0.0] * 466)  # initialize empty column to hold total volume

# read in the closing prices of each asset each day
for filename in sp500:
    if filename != '500/_Tickers.csv' and filename != '500/_SPX.csv':
        new_close = pd.read_csv(filename)["Adj Close"]
        this_volume = np.array(pd.read_csv(filename)["Volume"])
        # print(len(this_volume))
        if len(this_volume) == 466:
            total_volume += this_volume  # not an optimal way
        # read in the name of the stock and put as the column name of this column of close prices
        name = filename.replace('.csv', '')
        name = name.replace('500/', '')
        new_close.rename(name, inplace=True)  # rename the column with our vanilla name just got
        merged.append(new_close)  # put them in the same list, same as merged = [merged, new_close]
total_volume = np.delete(total_volume, 0)  # delete the volume of the first day as no one cares, we start from day 2
total_volume = np.append(total_volume, 0.0)  # add a trivial 0 to the tail so we can merge
total_volume = pd.Series(total_volume)
total_volume.rename("Total Volume", inplace=True)

merged.append(total_volume)
data = pd.concat(merged, axis=1)  # axis = 1 means we are merging columns instead of rows
data.fillna(0, inplace=True)

# print(data.columns)
print(data)
# print(data.loc[1, :])
# test = np.array(data.loc[1, :])
# print(test[1:])

data.to_csv('new_500.csv', sep=' ', header=False, index=False)
