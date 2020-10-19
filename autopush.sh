t_msg=`date`


git add .

git status

#写个sleep 1s 是为了解决并发导致卡壳

sleep 10s

echo "####### 添加文件 #######"

git commit -m "$commit_msg"

echo "####### commit #######"

sleep 10s

echo "####### 开始推送 #######"


git push -u origin main


echo "####### 推送成功 #######"
