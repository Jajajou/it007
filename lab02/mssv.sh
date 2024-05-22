#!/bin/sh
echo -n "Nhap vao ten cua ban: "
read yourname
echo -n "Nhap vao ma so sinh vien cua ban: "
read mssv
while [ $mssv != "22520805" ]
do
    echo -n "Ma so sinh vien cua ban khong chinh xac, xin nhap lai: "
    read mssv
done
echo "Ten ban la: $yourname, ma so sinh vien cua ban la: $mssv"
exit 0
