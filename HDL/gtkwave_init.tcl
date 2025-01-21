
# procura sinais alvo
set index -1
set nfacs [ gtkwave::getNumFacs ]
for {set i 0} {$i < $nfacs } {incr i} {
    set facname [gtkwave::getFacName $i]
    set index1 [string first proc.io_out $facname]
    set index2 [string first proc.out_en $facname]
    set index3 [string first pc.val $facname]
    
    if {$index1 != -1} {
    	set io_out $facname
	}

    if {$index2 != -1} {
    	set out_en $facname
	}

    if {$index3 != -1} {
    	set opcode $facname
	}
}

# adiciona os sinais no grafico
set filter [list clk $io_out $out_en $opcode]
gtkwave::addSignalsFromList $filter

# traducao de sinais
gtkwave::highlightSignalsFromList $io_out
gtkwave::/Edit/Data_Format/Signed_Decimal
gtkwave::/Edit/UnHighlight_All

set instructions [gtkwave::setCurrentTranslateFile trad_opcode.txt]
gtkwave::highlightSignalsFromList $opcode
gtkwave::/Edit/Data_Format/Decimal
gtkwave::installFileFilter $instructions
gtkwave::/Edit/Alias_Highlighted_Trace Instrucao
gtkwave::/Edit/UnHighlight_All

# estica as ondas na tela
gtkwave::/Time/Zoom/Zoom_Best_Fit