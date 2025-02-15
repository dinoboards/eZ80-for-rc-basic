

## Some of the stack frame structures:

## FOR LOOPS

FOR var = expr TO expr [STEP expr]

STACK FRAME ON EXIT

| byte offset | Contents                   | size
|-------------|----------------------------|-------|
| 23          | X X TY (OPER, LIMIT TYPE)  | 3 BYTES
| 21          | X H  L (LIMIT HIGH)        | 3 BYTES
| 18          | X H' L' (LIMIT LOW)        | 3 BYTES
| 15          | X OP TY (OPER, STEP TYPE)  | 3 BYTES
| 12          | X H  L (STEP HIGH)         | 3 BYTES
| 09          | X H' L' (STEP LOW)         | 3 BYTES
| 06          | IY - TEXT POINTER          | 3 BYTES
| 03          | LOOP VAR                   | 3 BYTES
| 00          | RETURN ADDR - FORCHK       | 3 BYTES

## PROC Arguments

### Numeric Arg:

| byte offset | Contents                         | size
|-------------|----------------------------------|-------|
| 09          | VAR PTR                          | 3 bytes |
| 06          | xx H L  (number high 16 bits)    | 3 bytes |
| 03          | xx H'L' (number low 16 bits)     | 3 bytes |
| 00          | xx B C  (B=VAR TYPE, C=EXPONENT) | 3 bytes |



## LOCCHK

I think this is whats pushed, for each numeric arg onto the stack for each param to be saved.

| byte offset | Contents                         | size
|-------------|----------------------------------|---------|
| 18          | xx H L  (number high 16 bits)    | 3 bytes |
| 09          | xx H'L' (number low 16 bits)     | 3 bytes |
| 06          | xx B C  (B=VAR TYPE, C=EXPONENT) | 3 bytes |
| 03          | VAR PTR                          | 3 bytes |
| 00          | LOCCHK                           | 3 bytes |


