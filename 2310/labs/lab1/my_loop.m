comment(`  Reagan Leonard, CPSC 231-004')

comment(`  code that adds all integers 1-10, starting from 1 and going to 10')

comment(`  code that implements the loop                       ')
comment(`    sum = 0;                                          ')
comment(`    for( i = 1; i <= 10; i++ ) {                       ')
comment(`       sum = sum + i;                                 ')
comment(`    }                                                 ')

   word(sum,0)
   word(i,0)
   word(zero,0)
   word(one,1)
   word(ten,10)

label(start)

   load(zero)  comment(`  sum = 0         ACC <- memory[zero]  ')
   store(sum)  comment(`                  memory[sum] <- ACC   ')
   load(one)   comment(`  i = 1                               ')
   store(i)

label(loop)

   load(i)     comment(`  branch to done if i <= 10             ')
   sub(ten)
   bgt0(done)

   load(sum)   comment(`  sum = sum + i                        ')
   add(i)
   store(sum)

   load(i)     comment(`  i = i + 1                            ')
   add(one)
   store(i)

   ba(loop)    comment(`  unconditionally go back to loop      ')

label(done)

   print(sum)
   halt

comment(`  start execution at label start  ')

   end(start)
