rm -rf ./dot/
mkdir dot/
for i in `ls *.dot`; do dot -Tpng $i -o dot/"${i%%.*}.png"; done
rm *.dot
