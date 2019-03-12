comment(`Reagan Leonard')
comment(`CPSC 2310-002')
comment(`Programming Assignment 1')
comment(`2-15-19')
comment(`This program is a binary calculator. It translates binary numbers to decimal numbers.')

word(N, 1101)     comment(`The binary number we want to convert to decimal')
word(decimal, 0)
word(expresult, 1)
word(zero, 0)
word(one, 1)
word(two, 2)
word(ten, 10)
word(single, 0)
word(temp, 0)
word(divtemp, 0)
word(multemp, 0)

label(start)

load(N)
store(temp)

label(loop)

  load(temp)
  div(ten)
  store(divtemp)
  load(divtemp)
  mul(ten)
  store(multemp)
  load(temp)
  sub(multemp)
  store(single)

  load(expresult)
  mul(single)
  store(single)

  load(decimal)
  add(single)
  store(decimal)

  load(temp)
  div(ten)
  store(temp)

  load(expresult)
  mul(two)
  store(expresult)

  load(temp)
  bgt0(loop)

  print(decimal)

halt
end(start)
