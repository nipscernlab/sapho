
// ----------  Empacota numero ----------

@float_pack LOAD float_aux3
PLD 0
SEQU
JZ floatL1else
LOAD float_zero
RETURN
JMP floatL1end
@floatL1else LOAD 0
SET float_aux4
LOAD float_aux3
PLD nbexpp1
SSHL
SET float_aux3
@floatL2 LOAD 0
PLD float_aux3
SLES
JZ floatL2end
LOAD float_aux3
PLD 1
SSHL
SET float_aux3
LOAD 1
PLD float_aux4
SADD
SET float_aux4
JMP floatL2
@floatL2end LOAD float_aux1
PLD float_nbits
SSHL
PLD float_aux2
PLD float_aux4
MULT -1
SADD
PLD nbmantp1
SSHL
PLD 1
SSHR
SADD
PLD float_aux3
PLD nbexpp1
SSHR
SADD
@floatL1end RETURN

// ---------- Separa expoente ----------

@get_exp SET float_aux2
PLD 1
SSHL
PLD nbmantp1
SSRS
SET float_aux2
RETURN

// ---------- Equaliza expoente de 2 numeros ----------

@denorm SETP float_aux3
SET float_aux1
PLD float_nbits
SSHR
SET denormsig_a
LOAD float_aux3
PLD float_nbits
SSHR
SET denormsig_b
LOAD float_aux1
CALL get_exp
SET denormexp_a
LOAD float_aux3
CALL get_exp
SET denormexp_b
LOAD float_aux1
PLD nbexpp1
SSHL
PLD nbexpp1
SSHR
SET float_aux1
LOAD float_aux3
PLD nbexpp1
SSHL
PLD nbexpp1
SSHR
SET float_aux3
LOAD denormexp_b
PLD denormexp_a
MULT -1
SADD
SET float_aux4
LOAD denormexp_a
PLD denormexp_b
SLES
JZ floatL4else
LOAD float_aux1
PLD float_aux4
SSHR
SET float_aux1
LOAD denormexp_b
SET float_aux2
JMP floatL4end
@floatL4else LOAD float_aux3
PLD float_aux4
MULT -1
SSHR
SET float_aux3
LOAD denormexp_a
SET float_aux2
@floatL4end LOAD denormsig_a
JZ floatL5else
LOAD float_aux1
MULT -1
SET float_aux1
@floatL5else LOAD denormsig_b
JZ floatL6else
LOAD float_aux3
MULT -1
SET float_aux3
@floatL6else RETURN