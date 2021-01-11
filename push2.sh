/usr/bin/expect << EOF

spawn git push -u origin Q 

set timeout 3

expect "Username for 'https://github.com':"

send "QuectelWB\r\n"

set timeout 3

expect "Password for 'https://QuectelWB@github.com':"

send "wangbees09\r\n"

set timeout 8

expect "Username for 'https://gitee.com':"

send "MrVW\r\n"

set timeout 5

expect "Password for 'https://MrVW@gitee.com':"

send "wangbees09\r\n"

expect eof


EOF

#QuectelWB

echo "####### git push done #######"

exit
