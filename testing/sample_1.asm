cad1 equ 5
mov efx, 0
mov fx, 5
mov edx, ds
mov [edx], efx
mov eax, 1
ldl ecx, 1
ldh ecx, 4
sys 2
stop