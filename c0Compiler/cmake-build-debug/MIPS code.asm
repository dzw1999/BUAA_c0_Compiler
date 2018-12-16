.data

.text

j main

f1:
#return: 1
#load 1 to s1
add $s1, $0, $0
ori $s1, $s1, 1
add $v0, $0, $s1
addi $sp, $s0, 0
jr $ra
#end return: 1
#return: 
addi $sp, $s0, 0
jr $ra
#end return: 

main:
add $s0, $0, $sp
la $ra, end
sw $s0, ($sp)
addiu $sp, $sp, -4
sw $ra, ($sp)
addiu $sp, $sp, -4
#return: 
addi $sp, $s0, 0
jr $ra
#end return: 

end:
