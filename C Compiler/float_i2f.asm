

// ---------- Funcao int2float ----------

@int2float SET int2floata
PLD float_nbits
SSHR
SET float_aux1
LOAD 0
SET float_aux2
LOAD float_aux1
JZ floatL8else
LOAD int2floata
MULT -1
SET int2floata
@floatL8else LOAD int2floata
SET float_aux3
CALL float_pack
RETURN