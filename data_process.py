import pandas as pd
import os
from os import path

DATA_DIR = "../Data"

def read_data():
    print("Retrieving data ...")
    global DATA_DIR
    data = pd.DataFrame()
    volume = pd.DataFrame()
    for root, dirs, files in os.walk(DATA_DIR):
        for file in files:
            if not file.endswith(".csv") or file.startswith("_"):
                continue
            name = file.split('.')[0]
            cur_df = pd.read_csv(path.join(DATA_DIR, file), parse_dates=["Date"])
            cur_df.set_index("Date", inplace=True)
            data = data.join(cur_df[["Adj Close"]].rename(columns={"Adj Close": name}), how="outer")
            volume = volume.join(cur_df[["Volume"]].rename(columns={"Volume": name}), how="outer")
    return data, volume

if __name__ == '__main__':
    data, volume = read_data()
    data.to_csv("data.csv")
    volume.to_csv("volume.csv")
