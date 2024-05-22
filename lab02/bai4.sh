echo "Nhap vao chuoi can tim:"
read givenString
for file in $(ls *.txt) 
do
    if grep -l "IT007 rat vui" $file 
    then more $file
    fi
done
exit 0
