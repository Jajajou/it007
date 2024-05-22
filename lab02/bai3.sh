foo=1
sum=0
echo -n "Nhap vao mot so n: "
read numN

while [ $numN -lt 10 ] 
do
    echo -n "Xin hay nhap lai: "
    read numN
done

while [ $foo -le $numN ] 
do
    sum=$(($sum+$foo))
    foo=$(($foo+1))
done
echo "Tong tu 1 den $numN la: $sum"
exit 0