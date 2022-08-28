<?php
    require_once($_SERVER['DOCUMENT_ROOT'].'/top.php');
    include_once($_SERVER['DOCUMENT_ROOT'].'/header.php');
?>
    로그아웃 되었습니다.
    <br />
<?php
    header("Location: ".$url['root']."index.php");
    exit();
?>
<?php
    include_once ($_SERVER['DOCUMENT_ROOT'].'/footer.php');
?>
