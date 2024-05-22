#!/bin/sh
echo -n "Enter password: "
read try
while [ "$try" != "secret" ]; do 
    echo "Sorry, try again"
    read try
done 
exit