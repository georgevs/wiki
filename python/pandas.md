# Pandas tutorial
https://pandas.pydata.org/docs/  
https://colab.research.google.com/github/google/eng-edu/blob/main/ml/cc/exercises/pandas_dataframe_ultraquick_tutorial.ipynb  

### Model
- DataFrame - two-dimensional (index X xolumns) tabular data
- Series - one-dimensional ndarray with axis labels
- Axis - dimension 0=index, 1=columns
- Shape - a tuple of number of values in each axis

### Dependncies
```python
import numpy as np
import pandas as pd
```

### Load data
https://pandas.pydata.org/docs/reference/io.html  
```python
df = pd.read_csv(url or file_path, names=columns)
df = pd.DataFrame(data=rows, index=index or None, columns)

# print schema
df.info()

# stats
stats_df = df.describe()
```

### Select data
Basics:
- `df.iloc` uses integer-location indexing (`0,1,2...`)
- `df.loc`uses name-location indexing (`'col0', 'col1',...`)
- a row (`df.iloc[i]`) or a column (`df['col']`) are both a series
- a series has index axis only
- a data frame has both index and column axis
- index axis is first, column axis is second
- column axis is optional and defaults to all columns if not specified
- axis index can be a VALUE, a LIST of values, a SPAN of values, a SELECTOR list, or a function
- a function index accepts the context (data frame or series) and returns a VALUE/LIST/SPAN/SELECTOR 
- a SELECTOR list has the same size as the axis, and True for all target elements
- a selector is typicaly aquired through an OPERATOR
- an OPERATOR is a function in the context of a data frame or series over an axis which returns a SELECTOR 

Select a series
```python
# select a row
xs = df.iloc[i]

# select a column
ys = df['col']
```

Select a subset
```python
# by value
x = xs.iloc[i]
x = ys.loc['col']
x = df.iloc[i,c]
x = df.loc[i,'col']

# by list
xs = xs.iloc[[i0,i1,ik]]
xs = df.loc[i,['col0','col1','coll']]
ys = ys.loc[[c0,c1,ck]]
ys = ys.loc[['col0','col1','colk']]
ys = df.loc[i0:ik,'col']
df = df.iloc[[i0,i1,ik],[c0,c1,ck]]
df = ys.loc[[i0,i1,ik],['col0','col1','colk']]

# by span
xs = xs.iloc[i0:ik]
ys = ys.loc['col0':'colk']
df = df.iloc[i0:ik, c0:ck]
df = df.loc[i0:ik, 'col0':'colk']

# by selector
xs = xs.iloc[[True,False,True]]
xs = xs.iloc[xs.eq(x)]
ys = ys.loc[[True,False,True]]
df = df.iloc[[True,False,True],[True,False,True]]
df = ys.loc[[True,False,True],[True,False,True]]

# by function
fni = lambda xs: i or [i0,i1,ik] or span(i0,ik) or [True,False,True]
fnci = lambda xs: c or [c0,c1,ck] or span(c0,ck) or [True,False,True]
fnc = lambda xs: 'col' or ['col0','col1','colk'] or span('col0','colk') or [True,False,True]
xs = xs.iloc[fni]
ys = ys.iloc[fnci]
ys = ys.loc[fnc]
df = df.iloc[fni,fnci]
df = df.loc[fni,fnc]
```