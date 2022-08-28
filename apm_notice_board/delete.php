<?php
    require_once($_SERVER['DOCUMENT_ROOT'].'/top.php');
?>

<?php
    $sql = "delete from bbs where doc_idx = $doc_idx";
    $query_result = $mysqli->query($sql);

    if(!$query_result)
        $_SESSION['delete_status'] = 'n';
    else
    {
        $_SESSION['delete_status'] = 'y';
        $sql = "set @count=0;";
        $query_result = $mysqli->query($sql) or die($mysqli->error());

        $sql = "update bbs set bbs.doc_idx = @count:=@count+1;";
        $query_result = $mysqli->query($sql) or die($mysqli->error());

        $sql = "alter table bbs auto_increment=1;";
        $query_result = $mysqli->query($sql) or die($mysqli->error());
        
        //$mysqli->query($sql);
    }
        

    $mysqli->close();

    header("Location: ".$url['root'].'delete_done.php');
    exit();
?>

