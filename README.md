# Star
git clone --recurse-submodules https://github.com/lovethiscode/kingkong.git


git submodule update --remote --merge


git push --recurse-submodules=check

androiod:
使用NDK r16 .
cmake 有个bug list append ' ' 空字符串不生效