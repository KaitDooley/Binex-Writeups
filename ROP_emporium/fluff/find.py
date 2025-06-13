with open('fluff', 'rb') as f:
  s = f.read()

for i in b'flag.txt':
  print(hex(s.find(i)))
