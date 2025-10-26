mov efx, 0
mov fh, 5
mov fl, 2
mov edx, ds
mov [edx], efx
mov eax, 1
ldl ecx, 1
ldh ecx, 4
sys 2
stop