#python -m SimpleHTTPServer 80 &
#python -m http.server 80 &


#echo $0
#echo $1
echo \$2: $2
#echo $#

#$1$2

port=""

if [ -z $2 ]; then
	port=80
else
	port=$2
fi

#echo port is $port

if [ -z $1 ]; then

#	echo 1
	python3 -m http.server $port &
else
#	echo 2
	python2 -m SimpleHTTPServer $port &
fi



