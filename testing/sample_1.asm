mov edx, ds
mov ecx, ds
add ecx, 8
mov [ecx], 0
mov w[ecx], 7
mov [edx], w[ecx]
mov eax, 1
ldl ecx, 1
ldh ecx, 4
sys 2
stop
