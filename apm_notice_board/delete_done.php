<?php
    require_once($_SERVER['DOCUMENT_ROOT'].'/top.php');
    include_once($_SERVER['DOCUMENT_ROOT'].'/header.php');
?>


<?php
    $delete_status = $_SESSION['delete_status'];
    if($delete_status == 'y')
        echo "글이 삭제되었습니다.";
    else
        echo "글을 삭제하지 못했습니다."
?>

<?php
        include_once($_SERVER['DOCUMENT_ROOT']."/footer.php");
?>