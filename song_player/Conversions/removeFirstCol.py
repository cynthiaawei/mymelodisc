import pandas as pd
readFile = pd.read_csv('log2.csv')

#delete the first column of row numbers 
col = readFile.columns[0]

# Delete first columny
readFile = readFile.drop([col], axis=1)

# new file
readFile.to_csv('log2csv.csv', index=False)

print("Finished removing first column")

# convert to .txt and format into array

