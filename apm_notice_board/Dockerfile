FROM ubuntu:latest

LABEL name="Sean"
LABEL email="qtt1536@gmail.com"
LABEL version="1.0"
LABEL description="Web otice board using APM"

# 도커 이미지를 생성할 때는 사용자가 입력할 수 없기 때문에 사용자의 입력 없이 넘어가기 위해 설정
# ARG는 docker build시에만 적용되는 변수이며, ENV는 Docker container 내부의 환경변수로 설정된다.
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Seoul

RUN apt install -y software-properties-common

RUN add-apt-repository ppa:ondrej/php

RUN apt update && apt upgrade -y && apt full-upgrade -y && apt autoremove -y

RUN apt install -y net-tools build-essential make gcc git curl wget unzip \
gdb vim zsh ssh netcat python2 python3 python2-dev python3-dev python3-pip apache2 libapache2-mod-fcgid libapache2-mod-proxy-uwsgi \
libapache2-mod-xforward openssl php phpmyadmin php-gd libapache2-mod-php php-mysql php-fpm mysql-server mysql-client 

RUN apt-get install -y php-{bz2,imagick,imap,intl,gd,mbstring,pspell,curl,mb,mbstring,mysql,readline,xml,xmlrpc,zip}
# RUN apt-get install php-{bz2, intl, gd, mb, mbstring, mysql, zip}

# apache2 ServerName 지정
RUN echo "ServerName localhost" >> /etc/apache2/apache2.conf

# mysql home directory 지정
RUN usermod -d /var/lib/mysql mysql

# Dockerfile이 위치한 디렉토리에 있는 파일들
# docker image 내의 /app 디렉토리에 복사
ADD . /app

RUN cp /app/* /var/www/html/

RUN service apache2 start

RUN service apache2 stop


# mysql 보안 강화 스크립트 실행
# RUN mysql_secure_installation

# 우분투 저장소에서 지원하는 php Extension 목록 확인 명령어
# apt list php-* php7.4-*

EXPOSE 80