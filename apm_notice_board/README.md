# Web notice board using Apache, php, Mysql

---

# issue

- list.php 페이지와 블럭 수 표시가 원하는대로 안된다.
- write.php와 view.php에 css 내용 적용이 안된다.
- 파일 업로드 기능을 추가해야 한다.
- 아직 뼈대만 만들었기 때문에 전체적으로 디자인 수정이 필요하다.

---

# build the image using Dockerfile
```
docker build --tag apm_notice_board:1.0 ./
```

---

# pull the docker image
- If you pull and run the image, all the settings below are complete.

```bash
docker pull xeanbaek/apm_notice_board:2.0
```

---

# when build using dockerfile, run a command in a new container
```bash
docker run -it --name ubuntu-apm-notice-board -p 8080:80 apm_notice_board:1.0 /bin/bash
```

# when pull docker image, run a command in a new container
```bash
docker run -it --name apm-notice-board -p 8080:80 xeanbaek/apm_notice_board:2.0 /bin/bash
```
```bash
service apache2 restart
```
```bash
service mysql restart
```

## [optional] Enabling the systemctl command
```bash
docker run --privileged -it --name ubuntu-apm-notice-board -p 8080:80 xeanbaek/apm_notice_board:2.0 /bin/bash
```

---

# run a command in a running container
```bash
docker ps
```

```bash
docker exec -it <Container ID> <command>
ex) docker exec -it 286a6c299290 /bin/bash
```

---
# Setting when building using Dockerfile

## 0. install PHP packages
```bash
apt-get install -y software-properties-common
```
```bash
add-apt-repository ppa:ondrej/php
```
```bash
apt-get install -y php-{bz2,imagick,imap,intl,gd,mbstring,pspell,curl,mbstring,mysql,readline,xml,xmlrpc,zip}
```

---

## 1. Modify /etc/php/[php version]/apache2/php.ini
```bash
date.timezone = Asia/Seoul

# php 버전 정보 숨기기
expose_php = Off

# post 방식으로 넘겨질 최대 데이터 사이즈
# upload_max_filesize와 인코더(encoder)를 포함한 mine 헤더 양식을 더한 모든 다른 필드 길이 합계
post_max_size = 500M

# 파일 업로드 허용 여부
file_uploads = On

# 최대 업로드 파일 사이즈(업로드 하는 모든 파일의 크기를 더한 값)
upload_max_filesize = 200M

# 최대 실행 시간. 파일 사이즈가 클수록 시간을 늘려줘야 한다.
max_execution_time = 30

# 스크립트 페이지로 넘어가기 전에 php 엔진이 데이터를 업로더 받는 시간
max_input_time = 3600

# 개발 환경을 위해 오류 출력
display_errors = On

# <?php 이렇게 쓰지 않고 <? 써도 알아먹게 설정
short_open_tag On

# 메모리 크기
memory_limit = 512M
```

---

## Mysql Change password for root user

### 2-1. start Mysql
```bash
service mysql start
```

### 2-2. Login Mysql
```bash
mysql
```

### 2-3. Type the command
```bash
# after php 8.0
-> ALTER user 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '변경 비밀번호';

# before php 8.0
-> UPDATE mysql.user SET authentication_string=password(“변경할 비밀번호”) WHERE user=’root’

# Save
flush privileges;
```

### 2-4. restart Mysql 
```bash
service mysql restart
```

---

## Create Db and Table

### 3-1. Login Mysql
```bash
mysql -uroot -p
```

### 3-2. Create DB
```sql
create database member default character set utf8;
```

### 3-3. use DB
```sql
use member;
```

### 3-4. Create signup_info table
```sql
create table signup_info (
	member_idx bigint(20) NOT NULL AUTO_INCREMENT primary key,
	username VARCHAR(30) NOT NULL,
	id INT(11) NOT NULL,
	pw VARCHAR(50) NOT NULL,
	pw_ck VARCHAR(50) NOT NULL
);
```

### 3-5. Create bbs table
```sql
create table bbs (
	doc_idx bigint(20) NOT NULL AUTO_INCREMENT primary key,
	member_idx bigint(20) NOT NULL,
	writer varchar(40) NOT NULL,
  	subject varchar(60) NULL,
  	content text NULL,
  	reg_date int(10) NULL,
	views int(10) NULL default 0
) default character set utf8 collate utf8_general_ci;
```

### Optional
```sql
alter table bbs MODIFY column subject varchar(60) character set utf8 collate utf8_general_ci;
alter table bbs modify column content text character set utf8 collate utf8_general_ci;
```

---

## 4. restart apache2
```bash
service apache2 restart
```

---

# phpmyadmin 설치
```bash
apt install -y phpmyadmin
```

# phpmyadmin 설정
- /etc/apache2/apache2.conf 파일 맨 밑에 아래 내용 기재 후 apache2 재시작

```bash
/etc/apache2/apache2.conf
```
```bash
#Include phpMyAdmin
Include /etc/phpmyadmin/apache.conf
```
```bash
service apache2 restart
```

---

# useful info

## minimal Ubuntu packages when running apache, php, mysql environments
```bash
apt install -y apache2 mysql-server php libapache2-mod-php php-mysql
```

## Apache

> hide apache version
```bash
/etc/httpd/conf/httpd.conf

```
```bash
ServerTokens Prod
```

## PHP

> php의 $_SERVER 변수
```php
$_SERVER['DOCUMENT_ROOT'] = 현재 사이트가 위치한 서버상의 위치 = webappinclude
$_SERVER['HTTP_ACCEPT_ENCODING'] = 인코딩 방식 = gzip, deflate
$_SERVER['HTTP_ACCEPT_LANGUAGE'] = 언어 = ko
$_SERVER['HTTP_USER_AGENT'] = 사이트 접속한 사용자 환경 = Mozilla4.0(compatible; MSIE 6.0; Windows NT 5.1; Q312461; .NET CLR 1.0.3705
$_SERVER['REMOTE_ADDR'] = 사이트 접속한 사용자 IP = xxx.xxx.xxx.xxx
$_SERVER['SCRIPT_FILENAME'] = 실행되고 있는 위치와 파일명 = webappincludeindex.php
$_SERVER['SERVER_NAME'] = 사이트 도메인 =WWW.X2CHI.COM
$_SERVER['SERVER_PORT'] = 사이트가 사용하는 포트 = 80
$_SERVER['SERVER_SOFTWARE'] = 서버의 소프트웨어 환경 = Apache1.3.23 (Unix) PHP4.1.2 mod_fastcgi2.2.10 mod_throttle3.1.2 mod_ssl2.8.6 OpenSSL0.9.6c
$_SERVER['GATEWAY_INTERFACE'] = cGI 정보 = CGI1.1
$_SERVER['SERVER_PROTOCOL'] = 사용된 서버 프로토콜 = HTTP1.1
$_SERVER['REQUEST_URI'] = 현재페이지의 주소에서 도메인 제외 =  index.phpuser=&name=
$_SERVER['PHP_SELF'] = 현재페이지의 주소에서 도메인과 넘겨지는 값 제외 = index.php
$_SERVER['APPL_PHYSICAL_PATH'] = 현재페이지의 실제 파일 주소 = Dwebapp
```

## Mysql

> mysql 테이블 생성 시 제약 조건들
```bash
- NOT NULL : 해당 필드는 NULL 값을 저장할 수 없게 됩니다.
- UNIQUE : 해당 필드는 서로 다른 값을 가져야만 합니다.
- PRIMARY KEY : 해당 필드가 NOT NULL과 UNIQUE 제약 조건의 특징을 모두 가지게 됩니다.
- FOREIGN KEY : 하나의 테이블을 다른 테이블에 의존하게 만듭니다.
- DEFAULT : 해당 필드의 기본값을 설정합니다.
```

> mysql 컬럼 타입 변경하고자 할 때
```php
alter table [테이블명] modify [컬럼명] [타입];
```

> auto_increment 값 초기화
```sql
alter table [테이블명] auto_increment=[변경할 숫자];
```

> auto_increment 재정렬 후 올바른 순서의 값 설정
```sql
set @count = 0;
update [테이블명] set [테이블명].[컬럼명] = @count:=@count+1;
alter table [테이블명] auto_increment=1;
```

> 특정 테이블의 auto_increment 값 확인
```sql
show table status where name = '[테이블명]';
```

> list 내의 글들을 가장 오래전에 쓴 것부터 보이게 할 때
```sql
select * from bbs where 1 order by doc_idx limit 0, 5;
```
> list 내의 글들을 가장 최근에 쓴 것부터 보이게 할 때
```sql
select * from bbs where 1 order by doc_idx desc limit 0, 5;
```

> innodb_buffer_pool_size 조회
```sql
show global variables like '%innodb_buffer_pool_size%';
```

## HTML + CSS

> CSS class 정의할 때
```css
p a {}            --><P>태그안의 모든 <A>태그

.mem a {}      --> mem이라는 class안의 모든 <A>태그

P .mem {}      --> <P>태그안의 모든 mem이라는 class를 가진 태그들

.ttt tr.www td {}       --> ttt라는 테이블안의 www라는 tr안의 td
```

> table 태그에서 칸과 칸 사이에 공백을 없앨 때 css 속성
```html
border-collapse: collapse;
```

---

# 참고 url

## programming

[mysqli_fetch_array](https://m.blog.naver.com/diceworld/220295811114)

- a 태그로 버튼 모양처럼 만들기
<https://ojji.wayful.com/2013/12/HTML-set-Two-Parallel-DIVs-columns.html>
<http://www.webpaper.kr/show/120&page=1>

[php server 배열](https://unabated.tistory.com/entry/PHP-SERVER-함수)

[php if문으로 html 조건 실행](https://araikuma.tistory.com/154)

[table 태그로 테이블 생성 예시](https://www.codingfactory.net/10232)

[css 파일 수정 후 적용이 안될 때](https://ho-ding.tistory.com/19)

[CSS 문법](https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=special3&logNo=140125803945)

[ceil() 함수](https://m.blog.naver.com/diceworld/220232696704)

[mysql 컬럼 타입](https://dwbutter.com/entry/MySQL-데이터베이스-컬럼의-데이터타입)

[alter로 auto_increment 초기화](https://m.blog.naver.com/dldudcks1779/222006115309)

[li 태그 등을 가로로 배열하기](https://blog.outsider.ne.kr/111)

[input 태그 무조건 입력받도록 하고 포커싱하기](https://gogogameboy.tistory.com/99)

[input 태그 크기 조절](https://dasima.xyz/html-input-text-size/)

[table 태그 layout 콘텐츠 내용에 따라 셀 너비 지정하기](https://devjhs.tistory.com/642)

[div 태그 영역 밖과 안쪽에 여백주기](https://ojji.wayful.com/2013/12/HTML-DIV-to-Space-Inside-and-Outside-div-block.html)

[div 태그 마스터](https://lifeblog.co.kr/1648/)

## setting

### dockerfile 레이어 관련하여 효율적으로 작성하기
<https://junhyunny.github.io/information/docker/docker-file-build-and-image-layer/>
<https://jonnung.dev/docker/2020/04/08/optimizing-docker-images/>

### ubuntu에 apm 환경 구축 시 최소 권장 패키지들
<https://wnw1005.tistory.com/441>

### docker ubuntu APM 환경 이미지 만들기 참고 url
<https://hoing.io/archives/7367#APM_-_Apache_PHP_MySQL>
<https://falsy.me/docker-1-docker로-ubuntu-apache-php-환경의-이미지-만들기/>
[docker APM 웹문서 경로 동기화](https://trytoso.tistory.com/1554)
[Apache2 + php 웹 개발환경을 dockerfile로 직접 빌드하여 도커 컨테이너 이미지 생성 및 배포하기](https://blog.naver.com/PostView.naver?blogId=love_tolty&logNo=222473245364)
[ubuntu APM 자동 설치](https://woochan-autobiography.tistory.com/655)

### APM 환경 설정
[ubuntu php.ini 파일 수정](https://webdir.tistory.com/221)


### 에러났을 때 참고
[mysql 실행시 디렉토리 관련 오류](https://stackoverflow.com/questions/62987154/mysql-wont-start-error-su-warning-cannot-change-directory-to-nonexistent)
[dockerfile 빌드 시 apt 관련 오류](https://technote.kr/372)
<https://zetawiki.com/wiki/우분투_apache2:_Could_not_reliably_determine_the_server's_fully_qualified_domain_name>
[Access denied for user 'root'@'172.17.0.1' (using password: YES) 오류 났을  때](https://csksoft.tistory.com/69)
[auto_increment 사용할 때 주의할 점](https://blog.daum.net/question0921/539)
[ubuntu php 파일 실행이 안될 때](https://chojpsh1.tistory.com/94)

---
