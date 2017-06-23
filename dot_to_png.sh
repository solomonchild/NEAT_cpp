for i in `ls *dot`; do dot -Tpng $i -o "${i%%.*}.png"; done
