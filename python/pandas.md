# Pandas tutorial
https://pandas.pydata.org/docs/  
https://colab.research.google.com/github/google/eng-edu/blob/main/ml/cc/exercises/pandas_dataframe_ultraquick_tutorial.ipynb  

### Model
- DataFrame - two-dimensional (index X columns) tabular data
- Series - one-dimensional `ndarray` with axis labels
- Axis - dimension 0=INDEX, 1=COLUMN
- Shape - a tuple of number of values in each axis

Legend:
- `df` represents a DataFrame
- `rec` represents a Series over the COLUMN axis (aka RECORD). Index axis is COLUMN names
- `vals` represents a Series over the INDEX axis (aka COLUMN). Index axis is ROW id
- `xs` represents any Series of values (no semantics or axes implied)

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

df.hist(column='X')
```
### Generate data
```python
# generate Series
rec = pd.Series(data={'a':10, 'b':20, 'c':30}, index=['a','b','c']) 
vals = pd.Series(data=[10,20,30]) 

# generate random DataFrame
columns = ['a','b','c']
num_samples = 100
min_value, max_value = 1,10
data = np.random.randint(low=min_value, high=max_value+1, size=(num_samples, len(columns)))
pd.DataFrame(data=data, columns=columns)
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
- axis index can be a VALUE, a LIST of values, a SLICE of values, a SELECTOR list, or a function
- a function index accepts the context (data frame or series) and returns a VALUE/LIST/SLICE/SELECTOR 
- a SELECTOR list has the same size as the axis, and True for all target elements
- a selector is typicaly aquired through an OPERATOR
- an OPERATOR is a function in the context of a data frame or series over an axis which returns a SELECTOR 
- IMPORTANT: `iloc[i1:ik]` returns EXCLUSIVE `[i1..ik)`, and `iloc[i1:ik]` returns INCLUSIVE `[i1..ik]`



Select a SERIES or VALUE (also showing INVALID calls for completeness)
```python
# select a row
rec = df.iloc[i]

# select a column
vals = df['col']

# select a value
x = rec[i]
x = rec.iloc[i]
x = rec['col']
x = rec.loc['col']

x = vals[i]
x = vals.iloc[i]
x = vals['col']       # KeyError
x = vals.loc['col']   # KeyError

x = df.iloc[i,c]
x = df.iloc[i,'col']  # KeyError
x = df.loc[i,'col']
x = df.loc[i,c]       # KeyError
```

Select a SUBSET (only showing VALID calls for brevity)
```python
# by value
x = xs.iloc[i]
x = rec.loc['col']
x = df.iloc[i,c]
x = df.loc[i,'col']

# by list
rec = rec.iloc[[c0,c1,ck]]
rec = rec.loc[['col0','col1','colk']]
vals = vals.iloc[[i0,i1,ik]]
vals = vals.loc[[i0,i1,ik]]
rec = df.loc[i,['col0','col1','coll']]
rec = df.iloc[i,[i0,i1,ik]]
df = df.iloc[[i0,i1,ik],[c0,c1,ck]]
df = df.loc[[i0,i1,ik],['col0','col1','colk']]

# by slice
rec = rec.iloc[c1:ck]
rec = rec.loc['col1':'colk']
vals = vals.iloc[i1:ik]
vals = vals.loc[i1:ik]
df = df.iloc[i0:ik,c0]
df = df.loc[i0:ik,'col']
df = df.iloc[i0:ik,c0:ck]
df = df.loc[i0:ik,'col1':'colk']

# by selector
vals = vals.iloc[[True,False,True]]
vals = vals.loc[[True,False,True]]
vals = vals.loc[vals.eq(x)]
df = df.iloc[[True,False,True],[True,False,True]]

# by function
fni = lambda xs: i or [i0,i1,ik] or slice(i0,ik) or [True,False,True]
fnr = lambda rec: c or 'col' or \
                  [c0,c1,ck] or ['col0','col1','colk'] or \
                  slice(c0,ck) or slice('col0','colk') or \
                  [True,False,True]
xs = xs[fn]
xs = xs.loc[fn]
xs = xs.iloc[fn]
vals = vals.loc[fn]
rec = rec.loc[fnr]
df = df.iloc[fn,fn]
df = df.loc[fn,fnc]
```