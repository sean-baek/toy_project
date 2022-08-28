<?php
    require_once($_SERVER['DOCUMENT_ROOT']."/top.php");
    include_once($_SERVER['DOCUMENT_ROOT']."/header.php");

    $writing_status = $_SESSION['writing_status'];
    if($writing_status == 'y')
    {
        echo "글이 저장되었습니다.";
        header("Location: http://".$_SERVER['HTTP_HOST'].'/list.php');
        exit();
    }
    else
    {
        echo "저장에 실패했습니다";
        header("Location: http://".$_SERVER['HTTP_HOST'].'/write.php');
        exit();
    }
?>

<?php
    include $_SERVER['DOCUMENT_ROOT'].'/footer.php';
?>

