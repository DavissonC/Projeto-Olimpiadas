set terminal qt size 1000,800
set multiplot layout 2,1 title "Evolucao de Atletas: Primeiras 10 Edicoes" font ",16"

set key top left
set grid y

set style data histograms
set style fill solid 0.8 border -1
set boxwidth 1.5 relative

set xtics rotate by 0
set ylabel "Qtd de Atletas"

set title "Jogos de Verao" font ",13"
plot "verao.dat" using 2:xtic(1) lc rgb "#fcbd00" title "Atletas Verao"

set title "Jogos de Inverno" font ",13"
set xlabel "Ano da Edicao"
plot "inverno.dat" using 2:xtic(1) lc rgb "#90eeff" title "Atletas Inverno"

unset multiplot
