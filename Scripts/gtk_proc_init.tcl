# funcoes de controle ---------------------------------------------------------

# pega o primeiro Fac com o padrao de texto dado
proc getFac {padrao} {
    for {set i 0} {$i < [gtkwave::getNumFacs]} {incr i} {
        set facname [gtkwave::getFacName $i]
        if {[string first $padrao $facname] != -1} {return $facname}
    }
    return ""
}

# lista todas as Facs com o mesmo padrao de texto dado
proc listFac {padrao} {
    set lista [list]
    for {set i 0} {$i < [gtkwave::getNumFacs]} {incr i} {
        set facname [gtkwave::getFacName $i]
        if {[string first $padrao $facname] != -1} {lappend lista $facname}
    }
    return $lista
}

# adiciona uma Fac
proc addFac {facname dataFormat color alias tradutor filter} {
    gtkwave::addSignalsFromList $facname
    gtkwave::/Edit/Data_Format/$dataFormat
    if {![string equal $tradutor ""]} {gtkwave::installFileFilter [gtkwave::setCurrentTranslateFile $tradutor]}
    if {![string equal $filter   ""]} {gtkwave::installProcFilter [gtkwave::setCurrentTranslateProc $filter  ]}
    gtkwave::/Edit/Color_Format/$color
    gtkwave::/Edit/Alias_Highlighted_Trace $alias
}

# adiciona uma variavel de acordo com o tipo de dado
proc addVar {padrao tipo dataFormat filter} {
    set var_typ [listFac $padrao]

    puts "Info: found [llength $var_typ] $tipo type variables."
    
    for {set i 0} {$i < [llength $var_typ] } {incr i} {
        set facname [lindex $var_typ $i]
        regexp {_f_(.*?)_v_} $facname -> funcao
        regexp {_v_(.*?)_e_} $facname -> var
        if {[string compare $funcao global]!=0} {append funcao "()"}
        addFac $facname $dataFormat "Orange" "$tipo $var in $funcao" "" $filter
    }
}

# Pega infos do processador ---------------------------------------------------

puts "Info: running standard GTKWave configuration..."

set    fileID [open "tcl_infos.txt" r]
gets  $fileID tmp_dir
gets  $fileID bin_dir
close $fileID

# Insere sinais basicos -------------------------------------------------------

set clk      [getFac "core.clk" ]
set rst      [getFac "core.rst" ]

gtkwave::addSignalsFromList [list $clk $rst]

# Separador de I/O ------------------------------------------------------------

gtkwave::/Edit/Insert_Comment {I/O ****************}

# Sinais de entrada -----------------------------------------------------------

set req_in  [listFac "req_in_sim"]
set entrada [listFac "in_sim"    ]

puts "Info: found [llength $req_in] input ports in use."

for {set i 0} {$i < [llength $req_in] } {incr i} {

    puts "Info: adding signals for input port $i."

    addFac [list [lindex $req_in  $i]] "Binary"         "Yellow" "req_in $i" "" ""
    addFac [list [lindex $entrada $i]] "Signed_Decimal" "Yellow" "input  $i" "" ""
}

# Sinais de saida -------------------------------------------------------------

set out_en [listFac "out_en_sim"]
set saida  [listFac "out_sig"   ]

puts "Info: found [llength $out_en] output ports in use."

for {set i 0} {$i < [llength $out_en] } {incr i} {

    puts "Info: adding signals for output port $i."

    addFac [list [lindex $out_en $i]] "Binary"         "Yellow" "out_en $i" "" ""
    addFac [list [lindex $saida  $i]] "Signed_Decimal" "Yellow" "output $i" "" ""
}

# Separador de Instrucoes -----------------------------------------------------

gtkwave::/Edit/Insert_Comment {Instructions *******}

puts "Info: adding Assembly and C± instructions."

# Assembly --------------------------------------------------------------------

addFac [getFac "proc.valr2"] "Decimal" "Indigo" "Assembly" "$tmp_dir/trad_opcode.txt" ""

# C+- -------------------------------------------------------------------------

addFac [getFac "proc.linetabs"] "Signed_Decimal" "Violet" "C+-" "$tmp_dir/trad_cmm.txt" ""

# Separador de Variaveis ------------------------------------------------------

gtkwave::/Edit/Insert_Comment {Variables **********}

puts "Info: adding variables..."

# Variavel tipo int -----------------------------------------------------------

addVar "proc.me1" "int" "Signed_Decimal" ""

# Variavel tipo float ---------------------------------------------------------

addVar "proc.me2" "float" "Binary" "$bin_dir/float2gtkw.exe"

# Variavel tipo comp ----------------------------------------------------------

addVar "proc.comp_me3" "comp" "Binary" "$bin_dir/comp2gtkw.exe"

# Array tipo int --------------------------------------------------------------

array set grupos_int {}

# pega todos os arryas tipo comp e separa em grupos
for {set i 0} {$i < [gtkwave::getNumFacs]} {incr i} {
    set facname [gtkwave::getFacName $i]

    if {[string match "*arr_me1*" $facname] &&
        [regexp {^(.*?)(\d{4})\[\d+:\d+\]$} $facname -> base _]} {
        
        lappend grupos_int($base) $facname
    }
}

# adiciona cada array encontrado
foreach base_int [lsort [array names grupos_int]] {
    
    # adiciona array no gtkwave e da highlight
    gtkwave::addSignalsFromList $grupos_int($base_int)

    # tradutor
    gtkwave::/Edit/Data_Format/Signed_Decimal

    # pega funcao e nome do array
    regexp {_f_(.*?)_v_} $base_int -> funcao
    regexp {_v_(.*?)_e_} $base_int -> var

    # cria grupo no gtkwave e tira o highlight
    gtkwave::/Edit/Create_Group "int $var in $funcao"
    gtkwave::/Edit/Toggle_Group_Open|Close
    gtkwave::/Edit/UnHighlight_All

    # muda o nome de cada indice do array
    set i 0
    foreach sinal $grupos_int($base_int) {
        gtkwave::highlightSignalsFromList $sinal
        gtkwave::/Edit/Alias_Highlighted_Trace "$var $i"
        incr i
    }
}

# Array tipo float ------------------------------------------------------------

array set grupos_float {}

# pega todos os arryas tipo float e separa em grupos
for {set i 0} {$i < [gtkwave::getNumFacs]} {incr i} {
    set facname [gtkwave::getFacName $i]

    if {[string match "*arr_me2*" $facname] &&
        [regexp {^(.*?)(\d{4})\[\d+:\d+\]$} $facname -> base _]} {

        lappend grupos_float($base) $facname
    }
}

# adiciona cada array encontrado
foreach base_float [lsort [array names grupos_float]] {

    # adiciona array no gtkwave e da highlight
    gtkwave::addSignalsFromList $grupos_float($base_float)

    # tradutor
    gtkwave::/Edit/Data_Format/Binary
    gtkwave::installProcFilter [gtkwave::setCurrentTranslateProc $bin_dir/float2gtkw.exe]

    # pega funcao e nome do array
    regexp {_f_(.*?)_v_} $base_float -> funcao
    regexp {_v_(.*?)_e_} $base_float -> var

    # cria grupo no gtkwave e tira o highlight
    gtkwave::/Edit/Create_Group "float $var in $funcao"
    gtkwave::/Edit/Toggle_Group_Open|Close
    gtkwave::/Edit/UnHighlight_All

    # muda o nome de cada indice do array
    set i 0
    foreach sinal $grupos_float($base_float) {
        gtkwave::highlightSignalsFromList $sinal
        gtkwave::/Edit/Alias_Highlighted_Trace "$var $i"
        incr i
    }
}

# Array tipo comp -------------------------------------------------------------

array set grupos_comp {}

# pega todos os arryas tipo comp e separa em grupos_comp
for {set i 0} {$i < [gtkwave::getNumFacs]} {incr i} {
    set facname [gtkwave::getFacName $i]

    if {[string match "*comp_arr_me3*" $facname] &&
        [regexp {^(.*?)(\d{4})\[\d+:\d+\]$} $facname -> base_comp _]} {
        
        lappend grupos_comp($base_comp) $facname
    }
}

# adiciona cada array encontrado
foreach base_comp [lsort [array names grupos_comp]] {
    
    # adiciona array no gtkwave e da highlight
    gtkwave::addSignalsFromList $grupos_comp($base_comp)

    # tradutor
    gtkwave::/Edit/Data_Format/Binary
    gtkwave::installProcFilter [gtkwave::setCurrentTranslateProc $bin_dir/comp2gtkw.exe]

    # pega funcao e nome do array
    regexp {_f_(.*?)_v_} $base_comp -> funcao
    regexp {_v_(.*?)_e_} $base_comp -> var

    # cria grupo no gtkwave e tira o highlight
    gtkwave::/Edit/Create_Group "comp $var in $funcao"
    gtkwave::/Edit/Toggle_Group_Open|Close
    gtkwave::/Edit/UnHighlight_All

    # muda o nome de cada indice do array
    set i 0
    foreach sinal $grupos_comp($base_comp) {
        gtkwave::highlightSignalsFromList $sinal
        gtkwave::/Edit/Alias_Highlighted_Trace "$var $i"
        incr i
    }
}

# Visualizacao ----------------------------------------------------------------

gtkwave::/Time/Zoom/Zoom_Best_Fit
gtkwave::/View/Left_Justified_Signals