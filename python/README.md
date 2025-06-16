# Python

[Monadic error handling](https://www.youtube.com/watch?v=J-HWmoTKhC8)  
[Structural pattern matching](https://earthly.dev/blog/structural-pattern-matching-python/)  
[Project layout](./project-layout.md)  
[Setup emvironment](./setup-environment.md)  

## Setup environment
Install Python 3
```bash
sudo apt install python3 python3-pip
export PATH="$PATH:/home/$USER/.local/bin"
```
Install Jupyter notebook
```bash
pip3 install notebook
jupyter notebook --ip 0.0.0.0 --port 8888 --no-browser
```
Install python auto format
```bash
pip3 install autopep8
```
Autopep8 cli
```bash
autopep8 --max-line-length 100 --indent-size 2 --in-place file.py
```

## Install dependencies and run apps
```bash
pip3 install -r requirements.txt
PYTHONPATH=path/to/lib python3 path/to/src/app.py x y z
```

## Reflection
```python
type(x) -> <class X>
```

## Variable args
```python
fn = lambda *args: args
fn(x,...) -> (x,...)

fn = lambda **kwargs: kwargs
fn(x=y,...) -> {'x':y,...}
```

## For-loop idioms
```python
for i in range(n): ...
for i in range(b,e,k): ...
for x in itx: ...
for i,x in enumerate(itx): ...
```

## Range vs. slice
- `slice(b,e,k)` returns an object 
- `range(b,e,k)` returns an iterator seeded by a slice prms
```python
list(range(10))[slice(0,5,2)]   # [0,2,4]
```

## Iterators, generators, generator expressions, and itertools
```python
def generator(seed=0):
  x = seed
  while x < seed+10:
    yield x
    x += 1

print(*generator())

fn = lambda x: x*2
print(*(fn(x) for x in generator()))
print(*map(fn, generator()))

pfn = lambda x: x%2==0
print(*(x for x in generator() if pfn(x)))
print(*filter(pfn, generator()))

mfn = lambda x: x*2 if x%2==0 else None
print(*(y for x in generator() if (y := mfn(x)) is not None))

mfn = lambda x: [x*2] if x%2==0 else []
flatten = itertools.chain.from_iterable
print(*flatten(map(mfn, generator())))
```

## Recursive Iterator
```python
class Trie:
  def __init__(self):
    self.root = [[None]*26, False]
  def __str__(self):
    return str([x for x in self])
  def __iter__(self):
    def dfs(u, x):
      if u[1]:
        yield x
      for i, u in enumerate(u[0]):
        if u:
          yield from dfs(u, x+chr(i+ord('a')))
    return dfs(self.root, '')
```

## Comprehensions
```python
list(fn(x) for x in itx if p(x))            [fn(x) for x in itx if p(x)]
tuple(fn(x) for x in itx if p(x))           (fn(x) for x in itx if p(x))
set(fn(x) for x in itx if p(x))             {fn(x) for x in itx if p(x)}
dict((key(x),fn(x)) for x in itx if p(x))   {key(x):fn(x) for x in itx if p(x)}
```

## Regular expressions
```python
(parse = re.compile(r).search)(s) -> None | m
m.group(0) -> s           -- matched substring
m.group(i) -> s           -- matched group i
m.groups() -> (s,...)     -- matched groups
```

## Random
```python
random.randint(l, r) -> n                   l <= n <= r
random.choice('abcd') -> 'a' | 'b' | ...
random.choices('abcd', k=k) -> ['a',...]

string.ascii_letters
string.digits
string.ascii_uppercase
string.ascii_lowercase
```

## Functional
```python
map(fn= x->y, itx) -> ity
filter(p= x->t, itx) -> itx
(fold = functools.reduce)(fn= (y,x)->y1, itx, y0) -> y1
(flatten = itertools.chain.from_iterable)(itxs) -> itx
(flatmap = lambda fn,xs: flatten(map(fn,xs)))(fn= x->[y], itx) -> ity
(slice = itertools.islice)(itx,b,e,k) -> itx
(pair = itertools.pairwise)(itx) -> it (xi,xi+1)
(group = itertools.groupby)(sorted(itx), key=x->y) -> it (y,itx)
(scan = itertools.accumulate)(itx, func= (a,x)->a1, initial=None) -> ita

(pipe = lambda *fns: reduce(lambda g,f: lambda x: f(g(x)), fns))(fn,...) -> fn

list(flatmap(lambda x:[10*x] if x%2==0 else [], range(10))) -> [0,20,40,60,80]
pipe(lambda x:10*x, str, lambda x:'a'+x)(1) -> 'a10'
```

## List / Sequence
```python
xs[i]
xs[b:e:k]
xs[b:e] = xs1
del xs[b:e]
n * [None]

x in xs
x not in xs

len(xs) -> n
min/max(xs) -> x

xs.sort(key= x->y, reverse= t)
sorted(itx, key= x->y, reverse= t) -> xs1

xs.reverse()
reversed(xs) -> itx

xs.index(x,b,e) -> i  ValueError
next((x for x in xs if fn(x)), default) -> x
next(find(pfn, xs), default) -> x
xs.count(x) -> n

xs.append(x)      xs[n:n] = [x]
xs.extend(itx)    xs[n:] = itx, xs += itx
xs.insert(i,x)    xs[i:i] = [x]
xs.pop(i)         xs[i:i+1] = []
xs.remove(x)      del xs[s.index(x)]   ValueError
xs.copy() -> xs1  xs1 = xs[:]
xs.clear()        del xs[:]
```

## String
```python
str(x) -> s       x.__str__() -> s

f'...{expr:n}...' -> s
'...{i}...'.format(x,...) -> s
'...{x}'.format_map({x:y,...}) -> s

s.center(n,ch) -> s1
s.l/rjust(n,ch) -> s1
s.l/r/strip(chs) -> s1

len(s) -> n

s.index(u,b,e) -> i
s.count(u,b,e) -> n
s.r/find(u,b,e) -> i

s.encode('utf8') -> bs
bs.decode('utf8') -> s

chr(s) -> n
ord(n) -> s

s.removeprefix/suffix(u) -> s1
s.starts/endswith(u,b,e) -> t

s.replace(u1,u2,n)

s.r/split(u) -> [s1,...]
s.splitlines(keeprn= t) -> [s1,...]
s.r/partition(u) -> (s1,u,s2)
s.join(itx) -> s1

s.upper/lower() -> s1
s.swapcase/title/capitalize() -> s1
```

## Bytes, bytearray, memoryview
```python
byte(src, encoding= 'utf8')         -- immutable
bytearray(src, encoding= 'utf8')    -- mutable     

fromhex(s) -> bs
bs.hex() -> s

s.encode('utf8') -> bs
bs.decode('utf8') -> s

base64.urlsafe_b64decode(s).decode('utf8')

memoryview(bs) -> xs
```

## Set
```python
set(itx) -> s

list(s) -> [k]

x in s
x not in s

s.isdisjoint(u) -> t
s.issubset(u) -> t                  s <= u,   s < u
s.issuperset(u) -> t                s >= u,   s > u
s.union(u,...) -> s1                s | u | ...
s.intersection(u,...) ->s1          s & u & ...
s.difference(u,...) -> s1           s - u - ...
s.symmetric_difference(u)           s ^ u
s.update(u,...)                     s |= u | ...
s.symmetric_difference_update(u)    s ^= u

s.copy() -> s1
s.add(x)
s.remove(x)     # KeyError
s.discard(x)
s.pop()
s.clear()
```

## Dictionary
```python
dict( it ('x',y) )
dict(x=y,...)

len(d) -> n

list(d) -> [x,...]
d.keys() -> it x
d.items() -> it (x,y)
d.values() -> it y

d[x] -> y                # KeyError
d[x] = y
del d[x]                 # KeyError
d.pop(x, y0) -> y|y0
d.update(d2) -> d
d.update(x=y,...) -> d
d.update(it ('x',y)) -> d

d.setdefault(x,y0) -> y  # mutates
d.get(x,y0) -> y|y0
```
## Paths
```python
os.path.join('dir1/dir2', 'dir3', f'{file}.{ext}') -> 'dir1/dir2/dir3/file.ext'
os.path.split('dir1/dir2/dir3/file.ext') -> ('dir1/dir2/dir3', 'file.ext')
os.path.dirname('dir1/dir2/dir3/file.ext') -> 'dir1/dir2/dir3'
os.path.basename('dir1/dir2/dir3/file.ext') -> 'file.ext'
os.path.splitext('dir1/dir2/dir3/file.ext') -> ('dir1/dir2/dir3/file', '.ext')
os.path.relpath('dir1/dir2/dir3/file.ext', 'dir1/dir2') -> 'dir3/file.ext'
os.path.commonpath(['dir1/dir2/dirA/dirB/file.ext', 'dir1/dir2/dirC/dirD/file.ext']) -> 'dir1/dir2
```
### Scan folder recursively
```python
def scandir(folder_path):
  folder_paths = [folder_path]
  while folder_paths:
    folder_path = folder_paths.pop()
    with os.scandir(folder_path) as entries:
      for entry in entries:
        if entry.is_dir():
          folder_paths.append(entry.path)
        else:
          yield entry.path 
```
### File or Stdin
```python
import logging
import sys

logger = logging.getLogger(__name__)

def main(config):
  fp = open(config.file_path, 'rt') if config.file_path else sys.stdin
  fpo = open(config.out_file_path, 'rt') if config.out_file_path else sys.stdout
  exit_code = transform(fp, fpo)
  return exit_code

def transform(fp, fpo):
  logging.debug('tranform')
  lines = iter(fp.readline, '')
  lines = map(str.upper, lines)
  for line in lines: print(line, file=fpo)
  return 0

if __name__ == '__main__':
  import argparse
  parser = argparse.ArgumentParser()
  parser.add_argument('file_path', type=str, nargs='?')
  parser.add_argument('--out', dest='out_file_path', type=str)
  parser.add_argument('-v', '--verbose', action='count', default=0)
  config = parser.parse_args()
  level = (
    logging.DEBUG if config.verbose >= 2 else
    logging.INFO if config.verbose >= 1 else
    logging.WARNING
  )
  logging.basicConfig(level=level, format='%(message)s')
  sys.exit(main(config))
```
### Parse Xml and Text
```python
import collections
import re
import xml.etree.ElementTree

Point = collections.namedtuple('Point', 'x y')

def load_points_xml(fp):
  root = xml.etree.ElementTree.parse(fp).getroot()
  parse_point = make_parse_node(Point._fields, Point)
  return map(parse_point, root.findall('points/point'))

def make_parse_node(fields, ctor=lambda *xs: xs):
  return lambda node: ctor(*[node.find(field).text for field in fields])

def load_points_txt(fp):
  lines = iter(fp.readline, '')
  parse_point = make_parse_pattern(r'point (\d+) (\d+)', Point)
  return filter(bool, map(parse_point, lines))

def make_parse_pattern(pattern, ctor=lambda *xs: True):
  search_pattern = re.compile(pattern).search
  return lambda line: (match := search_pattern(line)) and ctor(*match.groups())
```
