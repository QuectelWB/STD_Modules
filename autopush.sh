commit_msg=`date`


git add .

git status

sleep 2s

echo "####### git add done #######"

git commit -m "$commit_msg"

echo "####### commit start######"

sleep 2s

echo "####### commit done, start git push #######"


git push -u origin 1127

sleep 1s

#QuectelWB

echo "####### git push done #######"
