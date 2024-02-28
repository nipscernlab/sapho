

// ---------- Funcao multiplica ----------

@float_mult SETP multb
SET multa
LOAD nbmant
PLD float_nbits
PLD 1
SADD
PLD 1
SSHR
MULT -1
SADD
SET float_aux4
LOAD multa
PLD nbexpp1
SSHL
PLD nbexpp1
SSHR
PLD float_aux4
SSHR
SET float_aux1
LOAD multb
PLD nbexpp1
SSHL
PLD nbexpp1
SSHR
PLD float_aux4
PLD 1
SADD
SSHR
SET float_aux2
LOAD float_aux1
PLD float_aux2
SMLT
SET float_aux3
LOAD multa
PLD float_nbits
SSHR
PLD multb
PLD float_nbits
SSHR
SADD
SET float_aux1
LOAD multa
CALL get_exp
PLD float_aux4
SADD
PLD multb
CALL get_exp
SADD
PLD float_aux4
SADD
PLD 1
SADD
PLD nbexp
SADD
SET float_aux2
LOAD float_aux3
PLD nbexp
SSHR
SET float_aux3
CALL float_pack
RETURN