mov edx, 3
mov [edx], 3
add [edx], 7
mov eax, 0
ldl ecx, 1
ldh ecx, 4
sys 15
stop