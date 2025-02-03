# Pega a lista de processadores -----------------------------------------------

set   fileID    [open "proc_list.txt" r]
gets  $fileID   linha
set   proc_list [split $linha " "]
close $fileID

# Loop nos processadores ------------------------------------------------------

set nfacs [gtkwave::getNumFacs]

foreach proc $proc_list {

# Separador de processadores --------------------------------------------------

gtkwave::/Edit/Insert_Comment "### Proc. $proc"

# Separador de Sinais ---------------------------------------------------------

gtkwave::/Edit/Insert_Comment {Sinais *************}

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

# insere req_in
for {set i 0} {$i < $nfacs } {incr i} {
    set f_req_in [gtkwave::getFacName $i]

    set  proc_id [string first $proc $f_req_in]
    set  index   [string first id.req_in $f_req_in]
    if {$proc_id != -1 && $index != -1} break
}

# insere out_en
for {set i 0} {$i < $nfacs } {incr i} {
    set f_out_en [gtkwave::getFacName $i]

    set  proc_id [string first $proc $f_out_en]
    set  index   [string first id.out_en $f_out_en]
    if {$proc_id != -1 && $index != -1} break
}

set filter [list $clk $rst $f_req_in $f_out_en]
gtkwave::addSignalsFromList $filter

# Separador de I/O ------------------------------------------------------------

gtkwave::/Edit/Insert_Comment {I/O ****************}

# Sinais de entrada -----------------------------------------------------------

}

# Visualizacao ----------------------------------------------------------------

gtkwave::/Time/Zoom/Zoom_Best_Fit
gtkwave::/View/Left_Justified_Signals