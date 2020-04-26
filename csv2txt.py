import pandas as pd

def process_data():
    df = pd.read_csv("data.csv", index_col=0)
    df.fillna(0, inplace=True)
    df.drop([df.index[-1]], axis=0, inplace=True)
    print("data shape:", df.shape)
    df.to_csv("data.txt", index=False, header=False, sep=' ')

def process_volume():
    df = pd.read_csv("volume.csv", index_col=0)
    df.fillna(0, inplace=True)
    df = df.sum(axis=1)
    df.drop([df.index[0]], axis=0, inplace=True)
    print("volume shape:", df.shape)
    df.to_csv("volume.txt", index=False, header=False)


if __name__ == '__main__':
    process_data()
    process_volume()
