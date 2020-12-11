#!/bin/bash

commit_msg=`date`


git add .

git status

sleep 2s

echo "####### git add done #######"

git commit -m "$commit_msg"

echo "####### commit start######"

sleep 2s

echo "####### commit done, start git push #######"

/usr/bin/expect << EOF


spawn git push -u origin ch-zh

set timeout 3

expect "Username for 'https://github.com':"

send "QuectelWB\r\n"

set timeout 10

expect "Password for 'https://QuectelWB@github.com':"

send "wangbees09\r\n"

expect eof

EOF

#QuectelWB

echo "####### git push done #######"

exit
