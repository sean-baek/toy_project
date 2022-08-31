<?php
    require_once($_SERVER['DOCUMENT_ROOT'].'/top.php');
?>

<?php
    $reg_date = time();
    $member_idx = $_SESSION['member_idx'];

    if($_SESSION['is_signin'] == 'y')
    {
        $member_idx = $_SESSION['member_idx'];
        $writer = $_SESSION['user_id'];
    }
    else
    {
        $member_idx = 0;
        $writer = "Unknown";
    }

    $sql = "insert into bbs (member_idx, writer, subject, content, reg_date) values ('$member_idx', '$writer' ,'$subject', '$content', '$reg_date')";

    $query_result = $mysqli->query($sql);

    if($query_result == false)
    {
        $_SESSION['writing_status'] = 'n';
    }
    else
    {
        $_SESSION['writing_status'] = 'y';
    }

    $mysqli->close();
    header("Location: ".$url['root']."write_done.php");
    exit();
?>
