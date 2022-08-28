<?php
    require_once($_SERVER['DOCUMENT_ROOT'].'/top.php');
?>

<?php
    $_SESSION['is_signin'] =  'n';
    $_SESSION['user_id'] = '';
    $_SESSION['member_idx'] = '';

    unset($_SESSION['user_id']);
    unset($_SESSION['member_idx']);

    header("Location:".$url['root'].'logout_done.php');
exit();
?>