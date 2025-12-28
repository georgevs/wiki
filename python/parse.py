import collections
import enum
import json
import sys


Kind = enum.Enum('Kind', 'EOF SYMBOL NAME NUMBER')
Token = collections.namedtuple('Token', 'kind value')
Tokens = collections.namedtuple('Tokens', 'head tail')
class LexError(Exception): pass
class ParseError(Exception): pass


def main(args):
  if args.data_file_path:
    with open(args.data_file_path, 'r') as data_file:
      value = parse_data_file(data_file)
  else:
      value = parse_data_file(sys.stdin)

  if args.format == 'json':
    json.dump(value, fp=sys.stdout, indent=2)
  else:
    print(value)

def parse_data_file(data_file):
    lines = iter(data_file.readline, '')
    return parse_data(lines)


def parse_data(lines):
  tokens = lex(lines)
  tokens = Tokens(next(tokens), tokens)

  value, tokens = parse_value(tokens)
  if tokens.head.kind != Kind.EOF:
    raise ParseError() 
  
  return value


def next_pos(tokens):
  token = next(tokens.tail, None) or Token(Kind.EOF, None)
  return Tokens(token, tokens.tail)


def parse_value(tokens):
  if tokens.head == Token(Kind.SYMBOL, '{'):
    return parse_object_value(tokens)
  
  elif tokens.head == Token(Kind.SYMBOL, '['):
    return parse_array_value(tokens)
  
  elif tokens.head.kind == Kind.NUMBER:
    result = tokens.head.value
    tokens = next_pos(tokens)
    return (result, tokens)
  
  else:
    raise ParseError()
  

def parse_object_value(tokens):
  assert tokens.head == Token(Kind.SYMBOL, '{')
  
  result = {}
  tokens = next_pos(tokens)

  if tokens.head == Token(Kind.SYMBOL, '}'):
    tokens = next_pos(tokens)
    return (result, tokens)
  
  else:
    while True:
      if tokens.head.kind == Kind.NAME:
        name = tokens.head.value
        tokens = next_pos(tokens)
        if tokens.head == Token(Kind.SYMBOL, '='): 
          tokens = next_pos(tokens)
          value, tokens = parse_value(tokens)
          result[name] = value
          if tokens.head == Token(Kind.SYMBOL, ','):
            tokens = next_pos(tokens)
            continue
          if tokens.head == Token(Kind.SYMBOL, '}'):
            tokens = next_pos(tokens)
            return (result, tokens)
          else:
            raise ParseError()
        else:
          raise ParseError()
      
      else:
        raise ParseError()


def parse_array_value(tokens):
  assert tokens.head == Token(Kind.SYMBOL, '[')

  result = []
  tokens = next_pos(tokens)

  if tokens.head == Token(Kind.SYMBOL, ']'):
    tokens = next_pos(tokens)
    return (result, tokens) 
  
  else:
    while True:
      value, tokens = parse_value(tokens)
      result.append(value)
      if tokens.head == Token(Kind.SYMBOL, ','):
        tokens = next_pos(tokens)
        continue
      if tokens.head == Token(Kind.SYMBOL, ']'):
        tokens = next_pos(tokens)
        return (result, tokens)
      else:
        raise ParseError()


def lex(lines):
  CRLF = '\r\n'
  SYMBOLS = '[]{}=,'
  DIGITS = '0123456789'
  SIGNS = '+-'
  SIGNDIGITS = SIGNS + DIGITS
  LETTERS = 'abcdefghijklmnopqrstuvwxyz'
  NAMES = '_' + LETTERS + LETTERS.upper() + DIGITS

  line = next(lines)
  line_no = 1

  while line:
    line = line.lstrip(' \t')

    if line[0] in CRLF:
      line = next(lines)
      line_no += 1

    elif line[0] in SYMBOLS:
      yield Token(Kind.SYMBOL, line[0])
      line = line[1:]

    elif line[0] in SIGNDIGITS:
      i = 0
      if line[i] in SIGNS: i += 1
      while line[i] in DIGITS: i += 1
      if line[i] == '.': i += 1
      while line[i] in DIGITS: i += 1
      if line[i] in 'eE': i += 1
      if line[i] in SIGNS: i += 1
      while line[i] in DIGITS: i += 1
      float(line[:i])
      yield Token(Kind.NUMBER, line[:i])
      line = line[i:]

    elif line[0] in NAMES:
      i = 1
      while line[i] in NAMES: i += 1
      yield Token(Kind.NAME, line[:i])
      line = line[i:]

    else:
      raise LexError()


if __name__ == '__main__':
  import argparse
  parser = argparse.ArgumentParser()
  parser.add_argument('data_file_path', type=str, nargs='?')
  parser.add_argument('--format', choices='json')
  args = parser.parse_args()
  main(args)
