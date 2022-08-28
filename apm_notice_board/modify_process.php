<?php
    require_once($_SERVER['DOCUMENT_ROOT']."/top.php");
?>

<?php
    $sql = "update bbs set subject='$subject', content='$content' where doc_idx=$doc_idx";
    $query_result = $mysqli->query($sql);

    if(!$query_result)
    {
        $_SESSION['modify_status'] = 'n';
    }
    else
    {
        $_SESSION['modify_status'] = 'y';
    }

    $mysqli->close();

    header("Location: ".$url['root'].'modify_done.php');
    exit();
?>

<?php  
    include_once($_SERVER['DOCUMENT_ROOT']."/footer.php");
?>