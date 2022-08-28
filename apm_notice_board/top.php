<?php
    session_start();

    $p = array();
    $path['root'] = $_SERVER['DOCUMENT_ROOT'].'/';

    $url = array();
    $url['root'] = "http://".$_SERVER['HTTP_HOST'].'/';

    require_once($path['root'].'config.php');

    header('Content-Type: text/html; charset=utf-8'); // utf-8인코딩

    mysqli_report(MYSQLI_REPORT_ERROR | MYSQLI_REPORT_STRICT);
    $mysqli = new mysqli($DB['host'], $DB['id'], $DB['pw'], $DB['db']);
    if($mysqli->connect_errno)
    {
        throw new RuntimeException('mysqli connection error: '.$mysqli->connect_error);
    }
    $mysqli->set_charset("utf8");


    // POST 또는 GET 방식으로 전달된 내용을 각 배열의 인덱스 이름대로 변수가 생성된다.
    extract($_POST, EXTR_SKIP);
    extract($_GET, EXTR_SKIP);
?>

<h1><a href="index.php">Web Hacking victim Site</a></h1>
<br>