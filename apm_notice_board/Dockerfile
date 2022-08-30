FROM ubuntu:latest

ADD . /app

LABEL name="Sean"
LABEL email="qtt1536@gmail.com"
LABEL version="1.0"
LABEL description="Web otice board using APM"

# 도커 이미지를 생성할 때는 사용자가 입력할 수 없기 때문에 사용자의 입력 없이 넘어가기 위해 설정
# ARG는 docker build시에만 적용되는 변수이며, ENV는 Docker container 내부의 환경변수로 설정된다.
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Seoul

RUN apt update && apt upgrade -y && apt full-upgrade -y && apt autoremove -y

RUN apt install -y net-tools build-essential make gcc git curl wget \
gdb vim zsh ssh netcat apache2 libapache2-mod-fcgid libapache2-mod-proxy-uwsgi unzip python2 python3 python3-pip \
libapache2-mod-xforward openssl python2-dev python3-dev php php-gd libapache2-mod-php php-mysql php-fpm mysql-server mysql-client 

RUN apt install -y php-{bz2,imagick,imap,intl,gd,mbstring,pspell,curl,readline,xml,xmlrpc,zip}

RUN service apache2 start

RUN service apache2 stop

# mysql 보안 강화 스크립트 실행
# RUN mysql_secure_installation

# 우분투 저장소에서 지원하는 php Extension 목록 확인 명령어
# apt list php-* php7.4-*

EXPOSE 80