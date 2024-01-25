# Pandas tutorial
https://pandas.pydata.org/docs/  
https://colab.research.google.com/github/google/eng-edu/blob/main/ml/cc/exercises/pandas_dataframe_ultraquick_tutorial.ipynb  

### Model
- DataFrame - a 2-axis grid of values
- Series - a 1-axis list of values
- Axis - 0=rows direction, 1=columns direction
- Shape - a tuple of number of values in each axis

### Dependncies
```python
import numpy as np
import pandas as pd
```

### Load data
```python
df = pd.read_csv('titanic.csv')
df.loc[df['Cabin']=='C123']   # rows with Cabin=C123
df.loc[[0,1,2]]               # rows with values 0,1,2
df.iloc[[0,1,2]]              # first three rows

df = pd.read_csv('titanic.csv', index_col="Cabin") 
df.loc['C123']                # rows with Cabin=C123
df.loc[['C123','C85']]        # rows with Cabin=C123 or C85
df.iloc[[0,1,2]]              # first three rows (no matter the index)
```

### Generate random data
```python
columns = ['X','Y']
num_samples = 100
data = np.random.randint(low=1, high=101, size=(num_samples, len(columns)))
df = pd.DataFrame(data=data, columns=columns)
```

### Shuffle, sort
```python
df = df.reindex(np.random.permutation(df.index))

df = df.sort_values(by='Age')
df = df.sort_values(by=['Pclass', 'Age'], ascending=False)
```

### Statistics
```python
# (<df>|<xs>)
#   [.groupby(<col>|<cols>)]
#   [[<col>|<cols>]]
#   .describe/count/mean/median/min/max/value_counts()
x = df['X'].mean()
x = df.groupby(['X','Y'])['Z'].mean()
xs = df[['X','Y']].mean()
xs = df[['X','Y']].groupby('X').mean()
xs = df[['X','Y']].value_counts()

# (<df>|<xs>).agg({<col>:<stats>})
df = df.agg({ 'X': ['min','max'], 'Y': ['min','mean','skew']})

# correlation
df.corr()           
```

### Column operations
```python
df['Z'] = df['X'] + df['Y']
df['Z'] = (df['X'] > df['Y']).astype(float)
df['Z'] = (df['X'] > df['Y']).astype(float)

df.loc[df['X']>25,'Y'] = value
```

### Subsets
```python
# df[<col>|<cols>|<range>|<cond>]
df = df[df['X'] > df['Y']]        # rows where X value is greater than Y value
df = df[df['X'].notna()]          # X value is non empty
df = df[df['X'].isin([1,2,3])]    # rows where X value is 1,2,or 3
df = df[(df['X']==1) | (df['X']==2) | (df['X']==3)]    # rows where X value is 1,2,or 3

# df.loc[ <row>|<rows>|<range>|<cond>, <col>|<cols> ]
df = df.loc[df['X'] > 42, ['X']]
xs = df.loc[df['X'] > 42, 'X']

# df.iloc[ <row>|<rows>|<range>, <icol>|<icols>|<crange> ]
df = df.iloc[0:4,0:2]       # rows 0..3 and colimns 0..1
df = df.loc[0:4,['X','Y']]  # rows 0..3 and colimns 0..1


df = df[['X','Y']]                 # subset of columns
df = df.iloc[[0,2,3,4]]            # subset of rows
df = df.iloc[[0,2,3,4],['X','Y']]  # subset of rows and columns
xs = df['X']                       # a column
x  = df['X'][0]                    # a value

df = df.head(3)          # first three rowa
df = df[0:3]             # first three rows
df = df.iloc[0:3]        # first three rows
df = df.iloc[[0,4,4,2]]  # rows 0,4,4,2
df = df[['X','Y']]       # columns X, Y
xs = df['X']             # X values
x  = df['X'][0]          # X[0] value
x  = df.iloc[y,x]        # value
x  = df.loc[y,'X']       # value
```

### Pivot
```python
df = df.pivot(columns='X',values='Z')
df = df.pivot(columns=['X','Y'],values='Z')
df = df.pivot_table(values='value', index='location', columns='parameter', aggfunc='mean', margins='All')
```
