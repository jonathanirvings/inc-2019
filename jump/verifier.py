import re
import string
import sys


class VerifierReader(object):
  def __init__(self, text):
    self.text = text
    self.position = 0

  def HasNext(self):
    return self.position < len(self.text)

  def Read(self, target):
    actual = self.text[self.position : self.position + len(target)]
    assert actual == target, "Could not read '{0}'".format(target)
    self.position += len(target)

  def ReadSpace(self):
    self.Read(' ')

  def ReadEndl(self):
    self.Read('\n')

  def ReadEof(self):
    assert self.position == len(self.text), 'Found junk at the end of the file'

  def ReadChar(self):
    assert self.position < len(self.text), 'Unexpected EOF'
    char = self.text[self.position]
    self.position += 1
    return char

  def ReadLine(self, valid_characters):
    old_position = self.position
    while True:
      assert self.position < len(self.text), 'Unexpected EOF'
      c = self.text[self.position]
      self.position += 1
      if c == '\n':
        break
      assert c in valid_characters
    assert self.position - old_position > 1, 'Nothing before next newline'
    return self.text[old_position:self.position - 1]

  def ReadRegex(self, target_regex):
    match = re.compile(target_regex).match(self.text, self.position)
    assert match is not None, 'Could not match /%s/' % target_regex
    self.position = match.end()
    return match.group(0)

  def ReadList(self, n, read_fn, *args):
    ls = []
    for i in range(n):
      if i > 0:
        self.Read(' ')
      ls.append(read_fn())
    self.ReadEndl()
    return ls

  def ReadInt(self, inclusive_min, inclusive_max):
    value = int(self.ReadRegex(r'0|(-?[1-9][0-9]*)'))
    assert inclusive_min <= value <= inclusive_max, (
        'Failed on %d <= %d <= %d' % (inclusive_min, value, inclusive_max))
    return value

  def ReadIntList(self, n, inclusive_min, inclusive_max):
    return self.ReadList(n, lambda: self.ReadInt(inclusive_min, inclusive_max))

  def ReadString(self, valid_characters=string.ascii_letters + string.digits,
                 inc_min_len=1, inc_max_len=10**6):
    old_position = self.position
    while True:
      assert self.position < len(self.text), 'Unexpected EOF'
      c = self.text[self.position]
      if c not in valid_characters:
        break
      self.position += 1
    st = self.text[old_position:self.position]
    assert inc_min_len <= len(st) <= inc_max_len, (
        'Invalid string length: %d' % len(st))
    return st

  def ReadStringList(self, n,
                     valid_characters=string.ascii_letters + string.digits,
                     inc_min_len=1, inc_max_len=10**6):
    return self.ReadList(
        n,
        lambda: self.ReadString(valid_characters, inc_min_len, inc_max_len))


def main(argv):
  vr = VerifierReader(sys.stdin.read())

  N = vr.ReadInt(1, 1000000000)
  vr.ReadSpace()
  
  M = vr.ReadInt(0, 50000)
  vr.ReadSpace()
  
  Q = vr.ReadInt(1, 50000)
  vr.ReadEndl()
  
  P = vr.ReadIntList(M, 1, N)
  before = 0
  
  stones = {}
  
  for p_i in P:
    assert p_i > before, ('P should be in ascending order')
    before = p_i
    
    stones[p_i - 1] = 0
    
  third_type = 0
  for i in range(Q):
    type = vr.ReadString()
    vr.ReadSpace()
    
    assert type == 'add' or type == 'rem' or type == 'go', ('Invalid query type')
    
    if (type == 'go'):
      third_type = third_type + 1;
      
      p, q = vr.ReadIntList(2, 1, N)
      
      assert p != q, 'P and Q must be different'
      
      assert stones.get(p - 1, 1) == 1 and stones.get(q - 1, 1) == 1, ('There must be a stone on both P and Q')
    else:
      p = vr.ReadInt(1, N)
      vr.ReadEndl()
      
      if (type == 'add'):
        assert stones.get(p - 1, 1) == 0, 'There must be no stone on P'
        
        stones[p - 1] = 1
      else:
        assert stones.get(p - 1, 1) == 1, 'There must be a stone on P'
        
        stones[p - 1] = 0
      
  assert third_type > 0, ('There must be at least 1 query type three')
  
  vr.ReadEof()


if __name__ == '__main__':
  sys.exit(main(sys.argv))
