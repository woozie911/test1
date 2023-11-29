# 使用官方的 Ubuntu 镜像作为基础
FROM ubuntu

# 设置工作目录
WORKDIR /test

# 复制当前目录的内容到工作目录
COPY . /test

# 安装一些依赖
# 更换为国内阿里云的软件源
RUN sed -i 's/http:\/\/archive.ubuntu.com\/ubuntu\//http:\/\/mirrors.aliyun.com\/ubuntu\//g' /etc/apt/sources.list
RUN apt update
RUN apt install -y vim
RUN apt install -y g++
RUN apt install -y make
    # 其他依赖和配置

# 3设置默认命令
# CMD ["g++ mytest.cpp -o mytest && ./mytest"]

