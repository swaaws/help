mkdir ~/temp
cd ~/temp 

git clone https://github.com/${$1} .


$ rm -rf .git


$ git init
$ git add .
$ git commit -m "Removed history, due to sensitive data"


$ git remote add origin github.com:${$1}.git
$ git push -u --force origin main