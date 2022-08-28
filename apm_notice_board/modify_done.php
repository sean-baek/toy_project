<?php
    require_once($_SERVER['DOCUMENT_ROOT']."/top.php");
    include_once($_SERVER['DOCUMENT_ROOT']."/header.php");
?>

<?php
    $modify_status  = $_SESSION['modify_status'];
    if($modify_status == 'y')
        echo "글이 수정되었습니다.";
    else
        echo "수정 실패했습니다.";
?>

<?php  
    include_once($_SERVER['DOCUMENT_ROOT']."/footer.php");
?>