

// ---------- Funcao divide ----------

@float_div SETP divb
SET diva
PLD nbexpp1
SSHL
PLD 1
SSHR
SET float_aux1
LOAD divb
PLD nbexpp1
SSHL
PLD nbexp
PLD nbexp
SADD
SSHR
SET float_aux2
LOAD float_aux1
PLD float_aux2
SDIV
SET float_aux3
LOAD diva
PLD float_nbits
SSHR
PLD divb
PLD float_nbits
SSHR
SADD
SET float_aux1
LOAD diva
CALL get_exp
PLD divb
CALL get_exp
MULT -1
SADD
PLD nbexp
MULT -1
SADD
PLD 1
SADD
PLD nbexp
MULT -1
SADD
SET float_aux2
CALL float_pack
RETURN