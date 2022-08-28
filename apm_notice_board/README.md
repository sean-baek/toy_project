# Apache, php, Mysql로 웹 게시판 만들기

---

# issue

- list.php 페이지와 블럭 수 표시가 원하는대로 안된다.
- write.php와 view.php에 css 내용 적용이 안된다.
- 파일 업로드 기능을 추가해야 한다.
- 아직 뼈대만 만들었기 때문에 전체적으로 디자인 수정이 필요하다.

---

# DB 구성
```sql
create database member;
```

```sql
use member;
```

```sql
create table signup_info (
	member_idx bigint(20) NOT NULL AUTO_INCREMENT,
	username VARCHAR(30) NOT NULL,
	id INT(11) NOT NULL,
	pw VARCHAR(50) NOT NULL,
	pw_ck VARCHAR(50) NOT NULL,
	primary key(member_idx)
);
```

```sql
create table bbs (
	doc_idx bigint(20) NOT NULL AUTO_INCREMENT,
	member_idx bigint(20) NOT NULL,
	writer varchar(40) NOT NULL,
  subject varchar(60) NULL,
  content text NULL,
  reg_date int(10) NULL,
	views int(10) NULL default 0,
) default character set utf8 collate utf8_general_ci;
```

```sql
alter table bbs MODIFY column subject varchar(60) character set utf8 collate utf8_general_ci;
alter table bbs modify column content text character set utf8 collate utf8_general_ci;
```

---

# useful info
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

> CSS class 정의할 때
```css
p a {}            --><P>태그안의 모든 <A>태그

.mem a {}      --> mem이라는 class안의 모든 <A>태그

P .mem {}      --> <P>태그안의 모든 mem이라는 class를 가진 태그들

.ttt tr.www td {}       --> ttt라는 테이블안의 www라는 tr안의 td
```

> <table> 태그에서 칸과 칸 사이에 공백을 없앨 때 css 속성
```html
border-collapse: collapse;
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
---

# 참고 url
>
[mysqli_fetch_array](https://m.blog.naver.com/diceworld/220295811114)

- <a> 태그로 버튼 모양처럼 만들기
<https://ojji.wayful.com/2013/12/HTML-set-Two-Parallel-DIVs-columns.html>
<http://www.webpaper.kr/show/120&page=1>

[php server 변수](https://unabated.tistory.com/entry/PHP-SERVER-함수)

[php if문으로 html 조건 실행](https://araikuma.tistory.com/154)

[table 태그로 테이블 생성 예시](https://www.codingfactory.net/10232)

[css 파일 수정 후 적용이 안될 때](https://ho-ding.tistory.com/19)

[CSS 문법](https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=special3&logNo=140125803945)

[ceil() 함수](https://m.blog.naver.com/diceworld/220232696704)

[mysql 컬럼 타입](https://dwbutter.com/entry/MySQL-데이터베이스-컬럼의-데이터타입)

[alter로 auto_increment 초기화](https://m.blog.naver.com/dldudcks1779/222006115309)

[<li> 태그 등을 가로로 배열하기](https://blog.outsider.ne.kr/111)

[<input> 태그 무조건 입력받도록 하고 포커싱하기](https://gogogameboy.tistory.com/99)

[<input> 태그 크기 조절](https://dasima.xyz/html-input-text-size/)

[<table> 태그 layout 콘텐츠 내용에 따라 셀 너비 지정하기](https://devjhs.tistory.com/642)

[<div> 영역 밖과 안쪽에 여백주기](https://ojji.wayful.com/2013/12/HTML-DIV-to-Space-Inside-and-Outside-div-block.html)

[<div> 태그 마스터](https://lifeblog.co.kr/1648/)

---