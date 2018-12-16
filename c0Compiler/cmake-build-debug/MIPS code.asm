.data
a: .space 4

.text
#const define:  a
#load 1 to s1
add $s1, $0, $0
ori $s1, $s1, 1
add $t0, $0, $s1
sw $t0, a
#end const define:  a

end:
