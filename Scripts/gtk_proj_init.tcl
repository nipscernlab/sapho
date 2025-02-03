# Pega a lista de processadores -----------------------------------------------

set   fileID    [open "proc_list.txt" r]
gets  $fileID   linha
set   proc_list [split $linha " "]
close $fileID

# Loop nos processadores ------------------------------------------------------

set nfacs [gtkwave::getNumFacs]

foreach proc $proc_list {

# Separador de processadores --------------------------------------------------

gtkwave::/Edit/Insert_Comment "###### $proc"

# Insere sinais ---------------------------------------------------------------

# insere clock
for {set i 0} {$i < $nfacs } {incr i} {
    set clk [gtkwave::getFacName $i]

    set  proc_id [string first $proc $clk]
    set  index   [string first core.clk $clk]
    if {$proc_id != -1 && $index != -1} break
}

# insere reset
for {set i 0} {$i < $nfacs } {incr i} {
    set rst [gtkwave::getFacName $i]

    set  proc_id [string first $proc $rst]
    set  index   [string first core.rst $rst]
    if {$proc_id != -1 && $index != -1} break
}

# insere interrupcao
for {set i 0} {$i < $nfacs } {incr i} {
    set itr [gtkwave::getFacName $i]

    set  proc_id [string first $proc $itr]
    set  index   [string first core.itr $itr]
    if {$proc_id != -1 && $index != -1} break
}

set filter [list $clk $rst $itr]
gtkwave::addSignalsFromList $filter

# Separador de I/O ------------------------------------------------------------

gtkwave::/Edit/Insert_Comment {I/O ****************}

# Sinais de entrada -----------------------------------------------------------

set j 0
set req_in [list]
set entrada [list]
for {set i 0} {$i < $nfacs } {incr i} {
    set facname [gtkwave::getFacName $i]

    set proc_id [string first $proc $facname]
    set index [string first "$proc.req_in_sim_" $facname]
    if {$proc_id != -1 && $index != -1} {
        lappend req_in $facname
        incr j
    }

    set index [string first "$proc.in_sim_" $facname]
    if {$proc_id != -1 && $index != -1} {
        lappend entrada $facname
    }
}

for {set i 0} {$i < $j } {incr i} {
    set filter [list [lindex $req_in $i]]
    gtkwave::addSignalsFromList $filter
    gtkwave::highlightSignalsFromList $filter
    gtkwave::/Edit/Color_Format/Yellow
    set nome [list req_in $i]
    gtkwave::/Edit/Alias_Highlighted_Trace $nome

    set filter [list [lindex $entrada $i]]
    gtkwave::addSignalsFromList $filter
    gtkwave::highlightSignalsFromList $filter
    gtkwave::/Edit/Data_Format/Signed_Decimal
    gtkwave::/Edit/Color_Format/Yellow
    set nome [list Entrada $i]
    gtkwave::/Edit/Alias_Highlighted_Trace $nome
}

# Sinais de saida -------------------------------------------------------------

set j 0
set out_en [list]
set saida [list]
for {set i 0} {$i < $nfacs } {incr i} {
    set facname [gtkwave::getFacName $i]

    set proc_id [string first $proc $facname]
    set index [string first "$proc.out_en_sim_" $facname]
    if {$proc_id != -1 && $index != -1} {
        lappend out_en $facname
        incr j
    }

    set index [string first "$proc.out_sig" $facname]
    if {$proc_id != -1 && $index != -1} {
        lappend saida $facname
    }
}

for {set i 0} {$i < $j } {incr i} {
    set filter [list [lindex $out_en $i]]
    gtkwave::addSignalsFromList $filter
    gtkwave::highlightSignalsFromList $filter
    gtkwave::/Edit/Color_Format/Yellow
    set nome [list out_en $i]
    gtkwave::/Edit/Alias_Highlighted_Trace $nome

    set filter [list [lindex $saida $i]]
    gtkwave::addSignalsFromList $filter
    gtkwave::highlightSignalsFromList $filter
    gtkwave::/Edit/Data_Format/Signed_Decimal
    gtkwave::/Edit/Color_Format/Yellow
    set nome [list Saída $i]
    gtkwave::/Edit/Alias_Highlighted_Trace $nome
}

}

# Visualizacao ----------------------------------------------------------------

gtkwave::/Time/Zoom/Zoom_Best_Fit
gtkwave::/View/Left_Justified_Signals