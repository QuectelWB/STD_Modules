t_msg=`date`


git add .

git status

#д��sleep 1s ��Ϊ�˽���������¿���

sleep 10s

echo "####### ����ļ� #######"

git commit -m "$commit_msg"

echo "####### commit #######"

sleep 10s

echo "####### ��ʼ���� #######"


git push -u origin main


echo "####### ���ͳɹ� #######"
