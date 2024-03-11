

// ---------- Funcao soma ----------

@float_add LOAD float_aux1
PLD float_aux3
SADD
SET float_aux3
PLD float_nbits
SSHR
SET float_aux1
LOAD float_aux2
PLD 1
SADD
SET float_aux2
LOAD float_aux1
JZ floatL7else
LOAD float_aux3
MULT -1
SET float_aux3
@floatL7else LOAD float_aux3
PLD 1
SSHR
SET float_aux3
CALL float_pack
RETURN