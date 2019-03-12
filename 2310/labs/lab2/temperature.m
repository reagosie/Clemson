comment(`Reagan Leonard, Lab 2, CPSC 2310-004')

                                        comment(`this program converts Celsius to Fahrenheit')
bipush(-40)                             comment(`push -40 onto the stack')

istore_1                                comment(`store the counter that was at top of stack into')
                                        comment(`local register 1 for safe keeping, this pops it at')
                                        comment(`the same time.')

label(loop)                             comment(`beginning of the loop')

iconst_1
invokevirtual(1)                        comment(`print loop counter')

bipush(9)
iload_1
imul
bipush(5)
idiv
bipush(32)
iadd
istore_2

iconst_2
invokevirtual(2)                        comment(`print result of conversion and a new line')

iinc(1, 10)                             comment(`increment counter by 10')

iload_1
bipush(120)
isub

ifle(loop)

return
