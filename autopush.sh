commit_msg=`date`


git add .

git status

#д��sleep 1s ��Ϊ�˽���������¿���

sleep 2s

echo "####### ����ļ� #######"

git commit -m "$commit_msg"

echo "####### commit #######"

sleep 2s

echo "####### ��ʼ���� #######"


git push -u origin main

sleep 1s

#QuectelWB

echo "####### ���ͳɹ� #######"
